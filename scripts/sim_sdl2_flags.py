#!/usr/bin/env python3
"""Emit SDL2 cflags/libs for PlatformIO native sim builds.

Prefer `sdl2-config` (macOS Homebrew / Linux / MSYS2). On Windows, fall back to
SDL2DIR or common vcpkg / MSYS2 install roots when sdl2-config is missing.
Stdout is consumed by PlatformIO `!python ...` build_flags expansion.
"""

from __future__ import annotations

import os
import shutil
import subprocess
import sys
from pathlib import Path


def emit(flags: str) -> int:
    flags = " ".join(flags.split())
    if flags:
        sys.stdout.write(flags)
    return 0


def from_sdl2_config() -> str | None:
    exe = shutil.which("sdl2-config")
    if not exe:
        return None
    try:
        cflags = subprocess.check_output([exe, "--cflags"], text=True).strip()
        libs = subprocess.check_output([exe, "--libs"], text=True).strip()
    except (OSError, subprocess.CalledProcessError):
        return None
    return f"{cflags} {libs}"


def windows_fallback() -> str | None:
    roots: list[Path] = []
    sdl2dir = os.environ.get("SDL2DIR")
    if sdl2dir:
        roots.append(Path(sdl2dir))

    vcpkg = os.environ.get("VCPKG_ROOT")
    if vcpkg:
        roots.append(Path(vcpkg) / "installed" / "x64-windows")

    roots.extend(
        [
            Path(r"C:\msys64\mingw64"),
            Path(r"C:\msys64\ucrt64"),
            Path(r"C:\vcpkg\installed\x64-windows"),
        ]
    )

    for root in roots:
        include_sdl2 = root / "include" / "SDL2"
        include = root / "include"
        lib = root / "lib"
        if not lib.is_dir():
            continue
        if include_sdl2.is_dir():
            # Typical layout: include/SDL2/SDL.h and -I.../include so #include <SDL2/SDL.h> works.
            include_flag = f'-I"{include}"'
        elif (include / "SDL.h").is_file():
            include_flag = f'-I"{include}"'
        else:
            continue
        # Prefer import/static libs commonly named SDL2 / SDL2main.
        return f'{include_flag} -L"{lib}" -lSDL2main -lSDL2'
    return None


def main() -> int:
    flags = from_sdl2_config()
    if flags:
        return emit(flags)
    if os.name == "nt":
        flags = windows_fallback()
        if flags:
            return emit(flags)
        sys.stderr.write(
            "sim_sdl2_flags: SDL2 not found. Install via vcpkg or MSYS2 and set SDL2DIR, "
            "or put sdl2-config on PATH. See sim/README.md.\n"
        )
        return 1
    sys.stderr.write("sim_sdl2_flags: sdl2-config not found. Install SDL2 (e.g. brew install sdl2).\n")
    return 1


if __name__ == "__main__":
    raise SystemExit(main())
