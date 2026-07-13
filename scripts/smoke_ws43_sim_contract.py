#!/usr/bin/env python3
"""Smoke-gate for Waveshare native / simulator recovery contracts.

Validates (without flashing or building firmware):
  - production display-ws43 stays legacy
  - display-ws43-native / display-sim-ws43 stay experimental eez43
  - Actions.cpp implements every eez43 action
  - shared ManualBrewSession / ProcessControl / SecondaryActionIcons exist
  - parity doc exists
  - SDL wide 800x480 selection is present
"""

from __future__ import annotations

import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


class SmokeError(RuntimeError):
    pass


def read(path: Path) -> str:
    try:
        return path.read_text(encoding="utf-8")
    except OSError as error:
        raise SmokeError(f"cannot read {path.relative_to(ROOT)}: {error}") from error


def require(condition: bool, message: str) -> None:
    if not condition:
        raise SmokeError(message)


def env_block(platformio: str, name: str) -> str:
    match = re.search(rf"(?ms)^\[env:{re.escape(name)}\]\s*(.*?)(?=^\[|\Z)", platformio)
    require(match is not None, f"PlatformIO env:{name} is missing")
    return match.group(1)


def extern_actions(header: str) -> set[str]:
    return set(re.findall(r"\bextern\s+void\s+(action_[a-z0-9_]+)\s*\(", header))


def implemented_actions(source: str) -> set[str]:
    return set(re.findall(r"\bvoid\s+(action_[a-z0-9_]+)\s*\(", source))


def smoke() -> None:
    platformio = read(ROOT / "platformio.ini")
    actions = read(ROOT / "src/display/ui/default/Actions.cpp")
    eez43_actions = read(ROOT / "src/display/ui/default/eez43/actions.h")
    sdl_driver = read(ROOT / "sim/driver/SdlDriver.cpp")
    sim_readme = read(ROOT / "sim/README.md")

    production = env_block(platformio, "display-ws43")
    require("-DGAGGIMATE_NATIVE_WS43_EEZ" not in production, "production must stay legacy (no native flag)")
    require("-<display/ui/default/eez43/>" in production, "production must exclude eez43")
    require("-<display/ui/default/Actions.cpp>" in production, "production must exclude Actions.cpp")
    require("-<display/ui/default/NativeManualBrew.cpp>" in production, "production must exclude NativeManualBrew")

    native = env_block(platformio, "display-ws43-native")
    require("-DGAGGIMATE_NATIVE_WS43_EEZ=1" in native, "display-ws43-native must enable experimental eez43")
    require("+<display/ui/default/eez43/>" in native, "display-ws43-native must include eez43")

    sim = env_block(platformio, "display-sim-ws43")
    require("-DGAGGIMATE_NATIVE_WS43_EEZ=1" in sim, "display-sim-ws43 must enable experimental eez43")
    require("-DGAGGIMATE_SIM_WS43=1" in sim, "display-sim-ws43 must set SIM_WS43")

    required = extern_actions(eez43_actions)
    implemented = implemented_actions(actions)
    require(required <= implemented, f"Actions.cpp misses eez43 actions: {sorted(required - implemented)}")

    for rel in (
        "src/display/ui/default/shared/ManualBrewSession.h",
        "src/display/ui/default/shared/ManualBrewSession.cpp",
        "src/display/ui/default/shared/ProcessControl.h",
        "src/display/ui/default/shared/ProcessControl.cpp",
        "src/display/ui/default/shared/SecondaryActionIcons.h",
        "src/display/ui/default/shared/SecondaryActionIcons.cpp",
        "docs/GAGGIBRE_EEZ43_PARITY.md",
    ):
        require((ROOT / rel).is_file(), f"missing required path {rel}")

    require("#if defined(GAGGIMATE_SIM_WS43)" in sdl_driver, "SDL wide-resolution selection missing")
    require(
        re.search(r"GAGGIMATE_SIM_WS43\).*?DISP_W\s*=\s*800;.*?DISP_H\s*=\s*480;", sdl_driver, re.DOTALL)
        is not None,
        "SDL wide resolution is not 800x480",
    )
    require("vcpkg" in sim_readme.lower() or "msys2" in sim_readme.lower(), "sim/README.md should document Windows SDL2")

    # Process semantics smoke: brew cancel / active brew pause clear.
    require("deactivateAndClear" in actions, "Actions.cpp must use ProcessControl::deactivateAndClear")
    require(
        re.search(
            r"action_on_simple_process_toggle.*?MODE_BREW.*?deactivateAndClear",
            actions,
            re.DOTALL,
        )
        is not None,
        "active brew toggle should prefer deactivateAndClear (WideLayout pause parity)",
    )
    require("action_on_brew_cancel" in actions and "deactivateAndClear" in actions, "brew cancel must clear")

    print("WS43 sim/native smoke contract: PASS")
    print(f"  eez43 actions covered by Actions.cpp: {len(required)}")
    print("  production legacy / native+sim experimental: ok")
    print("  shared helpers + parity doc + SDL 800x480: ok")


if __name__ == "__main__":
    try:
        smoke()
    except SmokeError as error:
        print(f"WS43 sim/native smoke contract: FAIL: {error}", file=sys.stderr)
        raise SystemExit(1)
