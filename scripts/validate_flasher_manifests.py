#!/usr/bin/env python3
"""Validate GaggiBre USB flasher manifests against catalog and optional out/ bins."""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path


def load_json(path: Path) -> dict:
    with path.open(encoding="utf-8") as f:
        return json.load(f)


def collect_manifest_paths(flasher_dir: Path) -> dict[str, Path]:
    manifests = {}
    for path in sorted(flasher_dir.glob("manifest-*.json")):
        if path.name == "manifest-catalog.json":
            continue
        manifests[path.name] = path
    return manifests


def validate(flasher_dir: Path, out_dir: Path | None, skip_missing_bins: set[str]) -> list[str]:
    errors: list[str] = []
    catalog_path = flasher_dir / "manifest-catalog.json"
    if not catalog_path.is_file():
        return [f"missing catalog: {catalog_path}"]

    catalog = load_json(catalog_path)
    targets = catalog.get("targets")
    if not isinstance(targets, list) or not targets:
        errors.append("manifest-catalog.json: targets must be a non-empty list")
        return errors

    manifest_files = collect_manifest_paths(flasher_dir)
    referenced: set[str] = set()
    all_parts: list[str] = []

    for i, target in enumerate(targets):
        if not isinstance(target, dict):
            errors.append(f"catalog.targets[{i}]: expected object")
            continue
        tid = target.get("id", f"#{i}")
        manifest_name = target.get("manifest")
        if not manifest_name:
            errors.append(f"catalog target {tid}: missing manifest")
            continue
        referenced.add(manifest_name)
        manifest_path = flasher_dir / manifest_name
        if not manifest_path.is_file():
            errors.append(f"catalog target {tid}: missing file {manifest_name}")
            continue

        data = load_json(manifest_path)
        builds = data.get("builds")
        if not isinstance(builds, list) or not builds:
            errors.append(f"{manifest_name}: builds must be a non-empty list")
            continue
        for bi, build in enumerate(builds):
            parts = build.get("parts") if isinstance(build, dict) else None
            if not isinstance(parts, list) or not parts:
                errors.append(f"{manifest_name}: builds[{bi}].parts must be a non-empty list")
                continue
            for pi, part in enumerate(parts):
                if not isinstance(part, dict) or "path" not in part:
                    errors.append(f"{manifest_name}: builds[{bi}].parts[{pi}] missing path")
                    continue
                part_path = part["path"]
                all_parts.append(part_path)
                if out_dir is not None and part_path not in skip_missing_bins:
                    bin_path = out_dir / part_path
                    if not bin_path.is_file():
                        errors.append(f"missing bin for {manifest_name}: {part_path}")

    for name, path in manifest_files.items():
        if name not in referenced:
            errors.append(f"orphan manifest not in catalog: {name}")
        # Ensure each standalone manifest is parseable even if not referenced.
        try:
            load_json(path)
        except json.JSONDecodeError as exc:
            errors.append(f"{name}: invalid JSON ({exc})")

    duplicates = sorted({p for p in all_parts if all_parts.count(p) > 1})
    if duplicates:
        errors.append("duplicate part paths across manifests: " + ", ".join(duplicates))

    index_html = flasher_dir / "index.html"
    if not index_html.is_file():
        errors.append("missing index.html in flasher dir")

    if out_dir is not None:
        for required in ("index.html", "manifest-catalog.json"):
            if not (out_dir / required).is_file():
                errors.append(f"out dir missing {required}")

    return errors


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--flasher-dir",
        type=Path,
        default=Path(__file__).resolve().parent / "flasher",
        help="Path to scripts/flasher",
    )
    parser.add_argument(
        "--out-dir",
        type=Path,
        default=None,
        help="If set, assert every manifest part path exists in this directory",
    )
    parser.add_argument(
        "--skip-missing-bins",
        default="",
        help="Comma-separated part basenames allowed to be absent from --out-dir",
    )
    args = parser.parse_args()

    skip = {s.strip() for s in args.skip_missing_bins.split(",") if s.strip()}
    errors = validate(args.flasher_dir, args.out_dir, skip)
    if errors:
        print("Flasher validation failed:", file=sys.stderr)
        for err in errors:
            print(f"  - {err}", file=sys.stderr)
        return 1

    print(f"OK: validated flasher manifests in {args.flasher_dir}")
    if args.out_dir:
        print(f"OK: all required bins present in {args.out_dir}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
