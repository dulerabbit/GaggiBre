#!/usr/bin/env python3
"""Validate Waveshare EEZ contracts without modifying generated files.

Checks:
  - Production display-ws43 stays on legacy eez + WideLayout + ManualBrewScreen
  - Experimental display-ws43-native / display-sim-ws43 use eez43
  - Generated eez43 geometry / actions / shared state contracts remain intact
"""

from __future__ import annotations

import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
WIDE = ROOT / "src/display/ui/default/eez43"
ROUND = ROOT / "src/display/ui/default/eez"


class ValidationError(RuntimeError):
    pass


def read(path: Path) -> str:
    try:
        return path.read_text(encoding="utf-8")
    except OSError as error:
        raise ValidationError(f"cannot read {path.relative_to(ROOT)}: {error}") from error


def require(condition: bool, message: str) -> None:
    if not condition:
        raise ValidationError(message)


def enum_entries(header: str) -> list[tuple[str, int]]:
    entries = re.findall(r"\b(SCREEN_ID_[A-Z0-9_]+)\s*=\s*(\d+)", header)
    return [(name, int(value)) for name, value in entries]


def function_body(source: str, name: str) -> str:
    match = re.search(rf"\bvoid\s+{re.escape(name)}\s*\(\s*\)\s*\{{", source)
    require(match is not None, f"missing generated function {name}")
    start = match.end()
    depth = 1
    index = start
    while index < len(source) and depth:
        depth += (source[index] == "{") - (source[index] == "}")
        index += 1
    require(depth == 0, f"unterminated generated function {name}")
    return source[start : index - 1]


def extern_actions(header: str) -> set[str]:
    return set(re.findall(r"\bextern\s+void\s+(action_[a-z0-9_]+)\s*\(", header))


def implemented_actions(source: str) -> set[str]:
    return set(re.findall(r"\bvoid\s+(action_[a-z0-9_]+)\s*\(", source))


def global_variables(header: str) -> list[tuple[str, int]]:
    return [
        (name, int(value))
        for name, value in re.findall(r"\b(FLOW_GLOBAL_VARIABLE_[A-Z0-9_]+)\s*=\s*(\d+)", header)
    ]


def env_block(platformio: str, name: str) -> str:
    match = re.search(rf"(?ms)^\[env:{re.escape(name)}\]\s*(.*?)(?=^\[|\Z)", platformio)
    require(match is not None, f"PlatformIO env:{name} is missing")
    return match.group(1)


def validate() -> None:
    wide_header = read(WIDE / "screens.h")
    wide_screens = read(WIDE / "screens.c")
    wide_actions = read(WIDE / "actions.h")
    shared_actions = read(ROOT / "src/display/ui/default/Actions.cpp")
    legacy_actions = read(ROOT / "src/display/ui/default/eez/actions.cpp")
    default_ui = read(ROOT / "src/display/ui/default/DefaultUI.cpp")
    generated_ui = read(ROOT / "src/display/ui/default/GeneratedUI.h")
    platformio = read(ROOT / "platformio.ini")
    sdl_driver = read(ROOT / "sim/driver/SdlDriver.cpp")

    require(
        "defined(GAGGIMATE_NATIVE_WS43_EEZ)" in generated_ui,
        "GeneratedUI must gate eez43 on GAGGIMATE_NATIVE_WS43_EEZ only",
    )
    require(
        "defined(WS43C_BOARD)" not in generated_ui.split("#if", 1)[1].split("#else", 1)[0],
        "WS43C_BOARD alone must not select native eez43",
    )

    screens = enum_entries(wide_header)
    require(len(screens) == 12, f"expected 12 wide screens, found {len(screens)}")
    require([value for _, value in screens] == list(range(1, 13)), "wide screen IDs are not contiguous 1..12")
    require("_SCREEN_ID_LAST = 12" in wide_header, "wide final screen ID is not 12")

    for screen_id, value in screens:
        suffix = screen_id.removeprefix("SCREEN_ID_").lower()
        create = f"create_screen_{suffix}"
        delete = f"delete_screen_{suffix}"
        tick = f"tick_screen_{suffix}"
        body = function_body(wide_screens, create)
        object_name = suffix
        root_contract = re.compile(
            rf"lv_obj_t\s*\*obj\s*=\s*lv_obj_create\(0\);\s*"
            rf"objects\.{re.escape(object_name)}\s*=\s*obj;\s*"
            r"lv_obj_set_pos\(obj,\s*0,\s*0\);\s*"
            r"lv_obj_set_size\(obj,\s*800,\s*480\);",
            re.MULTILINE,
        )
        require(
            root_contract.search(body) is not None,
            f"{screen_id} root is not exactly (0,0) 800x480",
        )
        for function_name in (create, delete, tick):
            require(
                re.search(rf"^\s*{re.escape(function_name)},\s*$", wide_screens, re.MULTILINE) is not None,
                f"{screen_id} is unavailable through the generated navigation tables ({function_name})",
            )
        require(f"{screen_id} = {value}" in wide_header, f"lost navigation ID {screen_id}")

    require("tick_screen(screenId - 1);" in wide_screens, "screen-ID tick navigation is missing")
    require("create_screen_funcs[screenId - 1]();" in wide_screens, "screen-ID create navigation is missing")
    require("delete_screen_funcs[screenId - 1]();" in wide_screens, "screen-ID delete navigation is missing")

    wide_objects = set(re.findall(r"\blv_obj_t\s*\*([a-zA-Z0-9_]+);", wide_header))
    brew_objects = {"start_button", "pause_button"}
    require(brew_objects <= wide_objects, f"missing Brew play/pause objects: {sorted(brew_objects - wide_objects)}")
    for object_name in brew_objects:
        require(f"objects.{object_name} = obj;" in wide_screens, f"Brew object {object_name} is never created")

    manual_objects = {
        "manual_brew_screen",
        "manual_brew_chart",
        "manual_brew_elapsed",
        "manual_brew_flow",
        "manual_brew_pressure",
        "manual_brew_weight",
        "manual_brew_pressure_target",
        "manual_brew_temp_zone",
        "manual_brew_pressure_zone",
        "manual_brew_flow_zone",
        "manual_brew_play_pause",
        "manual_brew_play_pause_label",
        "manual_brew_save_prompt",
        "manual_brew_discard",
        "manual_brew_save",
    }
    require(manual_objects <= wide_objects, f"missing Manual Brew objects: {sorted(manual_objects - wide_objects)}")
    for object_name in manual_objects:
        require(f"objects.{object_name} = obj;" in wide_screens, f"Manual Brew object {object_name} is never created")

    required_actions = extern_actions(wide_actions)
    implemented = implemented_actions(shared_actions)
    require(required_actions <= implemented, f"shared Actions.cpp misses: {sorted(required_actions - implemented)}")
    require("ManualBrewScreen::backupProfile" in legacy_actions, "legacy actions lost Manual Brew profile backup")
    require("ManualBrewScreen::restoreProfile" in legacy_actions, "legacy actions lost Manual Brew profile restore")
    require(
        "action_on_brew_start(e);" in wide_screens or "action_on_simple_process_toggle(e);" in wide_screens,
        "Brew play/pause action is not wired",
    )
    require("action_on_manual_brew_toggle(e);" in wide_screens, "Manual Brew play/pause action is not wired")

    wide_themes = re.findall(r"\bTHEME_ID_[A-Z0-9_]+", wide_header)
    round_themes = re.findall(r"\bTHEME_ID_[A-Z0-9_]+", read(ROUND / "screens.h"))
    require(wide_themes == round_themes, "wide and round theme IDs differ")
    require(
        "change_color_theme(currentThemeMode);" in default_ui,
        "shared theme application is not wired",
    )

    wide_globals = global_variables(read(WIDE / "vars.h"))
    round_globals = global_variables(read(ROUND / "vars.h"))
    require(wide_globals == round_globals, "wide and round shared-state global-variable contracts differ")

    production = env_block(platformio, "display-ws43")
    for token in (
        "-DWS43C_BOARD",
        "-<display/ui/default/eez43/>",
        "-<display/ui/default/Actions.cpp>",
        "-<display/ui/default/NativeManualBrew.cpp>",
    ):
        require(token in production, f"production display-ws43 misses {token}")
    require("-DGAGGIMATE_NATIVE_WS43_EEZ" not in production, "production display-ws43 must not enable native EEZ")
    require(
        "-<display/ui/default/wide/WideLayout.cpp>" not in production,
        "production display-ws43 must compile WideLayout",
    )
    require(
        "-<display/ui/default/manual/ManualBrewScreen.cpp>" not in production,
        "production display-ws43 must compile ManualBrewScreen",
    )

    native = env_block(platformio, "display-ws43-native")
    for token in (
        "-DGAGGIMATE_NATIVE_WS43_EEZ=1",
        "-<display/ui/default/eez/>",
        "+<display/ui/default/eez43/>",
        "-<display/ui/default/wide/WideLayout.cpp>",
        "-<display/ui/default/manual/ManualBrewScreen.cpp>",
    ):
        require(token in native, f"display-ws43-native misses {token}")

    sim_env = env_block(platformio, "display-sim-ws43")
    for token in (
        "extends = env:display-sim",
        "-DGAGGIMATE_SIM_WS43=1",
        "-DGAGGIMATE_NATIVE_WS43_EEZ=1",
        "-<display/ui/default/eez/>",
        "+<display/ui/default/eez43/>",
        "-<display/ui/default/wide/WideLayout.cpp>",
        "-<display/ui/default/manual/ManualBrewScreen.cpp>",
    ):
        require(token in sim_env, f"display-sim-ws43 misses {token}")
    require(
        re.search(
            r"ui_tick\(\);\s*#if\s+!GAGGIMATE_HAS_NATIVE_MANUAL_BREW\s*"
            r"WideLayout::syncAfterEezTick\(\);\s*#endif\s*lv_task_handler\(\);",
            default_ui,
        )
        is not None,
        "native loop is not isolated from post-ui_tick WideLayout mutation",
    )
    require("#if defined(GAGGIMATE_SIM_WS43)" in sdl_driver, "SDL wide-resolution selection is missing")
    require(
        re.search(r"GAGGIMATE_SIM_WS43\).*?DISP_W\s*=\s*800;.*?DISP_H\s*=\s*480;", sdl_driver, re.DOTALL)
        is not None,
        "SDL wide resolution is not 800x480",
    )
    require(
        re.search(r"#else.*?DISP_W\s*=\s*480;.*?DISP_H\s*=\s*480;", sdl_driver, re.DOTALL) is not None,
        "SDL default resolution is not preserved at 480x480",
    )

    print("WS43 EEZ recovery/parity: PASS")
    print("  production display-ws43: legacy eez + WideLayout + ManualBrewScreen")
    print("  experimental display-ws43-native / display-sim-ws43: eez43")
    print(f"  eez43 geometry: 800x480 across {len(screens)} screens")
    print(f"  eez43 contracts: {len(required_actions)} actions, {len(wide_globals)} globals, {len(wide_themes)} themes")
    print("  GeneratedUI selects eez43 only under GAGGIMATE_NATIVE_WS43_EEZ")


if __name__ == "__main__":
    try:
        validate()
    except ValidationError as error:
        print(f"WS43 EEZ recovery/parity: FAIL: {error}", file=sys.stderr)
        raise SystemExit(1)
