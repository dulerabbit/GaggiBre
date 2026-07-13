# GaggiBre eez43 visual & behavioral parity contract

This document is the **cutover gate** for moving production Waveshare (`display-ws43`) from the legacy stack to native `eez43`.

## Cutover gate (mandatory)

**Production stays on `display-ws43` (legacy `eez/` + `WideLayout` + procedural `ManualBrewScreen` + `eez/actions.cpp`) until every checkbox in this document is checked.**

Do **not**:

- Point `display-ws43` at `eez43`
- Define `GAGGIMATE_NATIVE_WS43_EEZ` on production envs
- Flash `display-ws43-native` as a daily driver

Experimental only:

| Env | UI tree | Role |
|-----|---------|------|
| `display-ws43` / `display-ws43c` | `eez/` + WideLayout + ManualBrewScreen | **Production** |
| `display-ws43-native` / `display-sim-ws43` | `eez43/` + `Actions.cpp` + `NativeManualBrew` | Experimental |

`GeneratedUI.h` selects `eez43` **only** when `GAGGIMATE_NATIVE_WS43_EEZ` is defined.

Shared non-generated helpers live under `src/display/ui/default/shared/` (`ProcessControl`, `ManualBrewSession`, `SecondaryActionIcons`) so both UIs can converge without editing generated trees.

---

## Current status (recovery)

- **Production** (`display-ws43` / `display-ws43c`) remains on the **legacy** stack after this recovery. Do not cut over.
- **Experimental** `display-ws43-native` is the compile-ready native path (`eez43` + `Actions.cpp` + `NativeManualBrew`). Flash only on a bench unit.
- Rollback alias: `display-ws43-legacy` currently extends production and is identical to it pre-cutover.
- **Cutover is blocked** until every checkbox in sections 1–6 is checked with device evidence (section 8).

Recent native increments (still experimental): rich `action_on_screen_load` click areas, brew cancel / active-brew pause via `ProcessControl::deactivateAndClear`, Manual Brew swipe → menu (EEZ gesture), Manual Brew exit cleanup on screen leave.

---

## Legend

- `[x]` Required behavior present and verified on the **production legacy** path (reference).
- `[ ]` Missing, incomplete, or unverified on **native eez43** — must pass before cutover.
- Notes call out native gaps relative to legacy.

---

## 1. Process control semantics

Reference: `Controller::{activate,deactivate,clear}`, legacy `eez/actions.cpp`, WideLayout pause, Manual Brew toggle/exit.

| ID | Behavior | Legacy | Native |
|----|----------|--------|--------|
| P1 | `activate()` clears prior process snapshot, tares, starts mode-appropriate process | [x] | [x] via controller |
| P2 | `deactivate()` ends current process, keeps `lastProcess` for status/clear | [x] | [x] |
| P3 | `clear()` discards `lastProcess` and fires `controller:brew:clear` for brew | [x] | [x] |
| P4 | Brew cancel / wide pause: **deactivate then clear** | [x] WideLayout + `action_on_brew_cancel` | [x] `Actions.cpp` cancel + active `MODE_BREW` toggle → `deactivateAndClear` |
| P5 | Simple process toggle (water/brew idle): toggle activate/deactivate, **no clear** | [x] | [x] water/idle brew → `ProcessControl::toggleActive`; steam exception kept |
| P6 | Legacy `action_on_brew_start` is **activate-only** (not toggle) on circular EEZ | [x] | N/A — native brew uses toggle / pause split |
| P7 | Leaving Manual Brew while active: deactivate + clear + restore profile backup | [x] ManualBrewScreen back | [x] `NativeManualBrew::onExit` via `handleScreenChange` + swipe/`action_on_menu_click` restore |
| P8 | Enter Manual Brew: backup selected profile, deactivate, `MODE_MANUAL`, pressure target `0` | [x] | [x] `ManualBrewSession::backupSelectedProfile` in `Actions.cpp` |
| P9 | Leave Manual Brew / switch to brew while manual: restore profile backup | [x] | [x] shared restore in `Actions.cpp` |

Shared helper: `ProcessControl::{activate,deactivate,clear,deactivateAndClear,toggleActive}`.

---

## 2. Manual Brew — session, save, discard

Reference: `ManualBrewScreen`, `ManualBrewTimeline`, `ManualBrewSession`.

| ID | Behavior | Legacy | Native |
|----|----------|--------|--------|
| M1 | Profile backup on entry; restore on exit without save | [x] | [x] shared session helper |
| M2 | Invalidate backup after successful “save as profile” | [x] | [x] |
| M3 | Live pressure timeline recorded while shot in progress | [x] | [x] `ManualBrewSession::ShotState` |
| M4 | Trailing edge of active→idle shows “Save as profile?” when shot data exists | [x] | [x] |
| M5 | Save compresses timeline into pressure phases (`applyManualTimelineToProfile`) | [x] | [x] |
| M6 | Discard hides prompt and clears shot state without restoring profile | [x] | [x] |
| M7 | Pressure axis uses `min(settings.getPressureScaling(), 12)` | [x] | [x] (fixed; was hardcoded `12.0f`) |
| M8 | Edge snap (~10 px / 0.25 bar) so 0 and max are reachable | [x] | [x] shared `pressureFromZoneY` |
| M9 | Idle labels reset to placeholders when not active and prompt hidden | [x] | [x] (fixed) |
| M10 | Temp control via swipe / zone | [x] swipe-delta steps | [ ] Absolute Y→temp mapping differs from legacy swipe steps — feel/parity TBD |
| M11 | Flow zone must **not** call `setManualFlowTarget` (rewrites phases to flow target) | [x] no flow zone | [x] intentional no-op with code comment |
| M12 | Play/pause icon or label reflects active state | [x] img play/pause | [ ] Label PLAY/PAUSE present; icon swap vs legacy asset parity TBD |
| M13 | Chart: pressure + flow series, ~250 ms on wide | [x] | [x] |
| M14 | Swipe-up back corridor around play (±~1 cm) on wide | [x] | [ ] Generic swipe→menu wired on Manual Brew root (EEZ GESTURE); play corridor filter still missing |
| M15 | Save prompt styling (theme card, check / X) | [x] | [ ] EEZ prompt exists; visual polish vs procedural panel TBD |

---

## 3. Secondary action & menu icon

| ID | Behavior | Legacy | Native |
|----|----------|--------|--------|
| S1 | 4th menu icon: Manual Brew or Grind from `secondaryAction` | [x] | [x] |
| S2 | Manual Brew shows pressure icon (`img_manual_pressure_80x80`, blue recolor); Grind shows coffee bean | [x] | [x] `SecondaryActionIcons` from DefaultUI |
| S3 | `SECONDARY_ACTION_NONE` hides / ignores 4th action | [x] | [ ] Verify eez43 visibility flags match grindAvailable |
| S4 | Wide menu: 2×2 rectangular grid (not circular orbit) | [x] WideLayout + DefaultUI | [ ] Authored in eez43 — verify positions match WideLayout offsets |

---

## 4. WideLayout / shell (legacy-only runtime)

Native must **reproduce** these behaviors inside eez43 + DefaultUI, not by compiling `WideLayout.cpp`.

| ID | Behavior | Legacy | Native |
|----|----------|--------|--------|
| W1 | Screens are full 800×480; no letterbox side bars | [x] | [x] generated geometry (validator) |
| W2 | Full-height left temp / right pressure tick columns | [x] | [ ] eez43 dials differ; tick animation/morph TBD |
| W3 | Tick length short on menu/profile/info, long on brew-class; animated morph | [x] | [ ] DefaultUI circular tick anim still runs; wide tick morph not ported |
| W4 | Shared play control position `(kPlayX, kPlayY)` across brew/water/grind/manual | [x] | [ ] Verify eez43 control coords match |
| W5 | While brewing: hide EEZ start, show wide pause that deactivate+clears | [x] | [x] Generated pause / brew toggle uses `deactivateAndClear` (WideLayout clear semantics) |
| W6 | Duration label reparented/centered during brew | [x] | [ ] |
| W7 | Standalone ticks on Manual Brew | [x] | [ ] Manual Brew uses authored zones/chart, not WideLayout ticks |

---

## 5. DefaultUI shared state & screens

| ID | Behavior | Legacy | Native |
|----|----------|--------|--------|
| D1 | Same FLOW global variable contract (`vars.h`) | [x] | [x] validator |
| D2 | Same theme IDs; `change_color_theme` wired | [x] | [x] |
| D3 | `heating_flash` / heating cue on UI flags | [x] | [ ] Visual heating cue on wide native gauges TBD |
| D4 | `showManualBrew()` → `SCREEN_ID_MANUAL_BREW_SCREEN` | [x] procedural overlay | [x] eez43 screen |
| D5 | Mode change / brew start / brew clear keep Manual on Manual screen | [x] | [x] |
| D6 | Brew start stays on brew screen (wide), not status dial | [x] | [x] |
| D7 | Ext click areas on key controls (menu icons, play, profile) | [x] rich list in eez/actions | [x] `Actions.cpp` matches legacy list for objects present in eez43 |
| D8 | Meter tick suppression + custom `action_on_meter_draw` pills/dots | [x] | [x] shared Actions path |
| D9 | Swipe up → menu; profile L/R swipes | [x] | [x] including Manual Brew root GESTURE → `action_on_screen_swipe` |
| D10 | Pressure scaling drives boiler max / fractions | [x] | [x] |

---

## 6. GaggiBre-only feature checklist

| Feature | Legacy | Native notes |
|---------|--------|--------------|
| Manual Brew as default secondary action | [x] | [x] entry path; session helper shared |
| Manual Brew pressure menu icon | [x] | [x] via `SecondaryActionIcons` |
| Gauge tick animations (short↔long) | [x] | [ ] Incomplete on native wide |
| Heating flash / heating color pulse | [x] | [ ] Incomplete |
| Play control placement & hit area | [x] | [ ] Verify coords + ext click |
| Touch zones (temp / pressure / play corridor) | [x] | [ ] Temp mapping + swipe corridor incomplete |
| No Adaptive Brew | [x] | [x] |

Pressure chase bar idea (not shipped): see [PRESSURE_CHASE_BAR.md](PRESSURE_CHASE_BAR.md).

---

## 7. Build / tree hygiene (always required)

| ID | Check | Status |
|----|-------|--------|
| B1 | `display-ws43` excludes `eez43`, `Actions.cpp`, `NativeManualBrew.cpp`; does **not** set `GAGGIMATE_NATIVE_WS43_EEZ` | [x] |
| B2 | `display-ws43-native` enables `GAGGIMATE_NATIVE_WS43_EEZ`, uses `eez43`, excludes WideLayout + ManualBrewScreen | [x] |
| B3 | `GeneratedUI.h` gates native only on `GAGGIMATE_NATIVE_WS43_EEZ` | [x] |
| B4 | `scripts/validate_ws43_eez.py` passes | required after every recovery change |
| B4b | `scripts/smoke_ws43_sim_contract.py` passes | required after every recovery change |
| B5 | Generated `eez/` and `eez43/` trees remain EEZ Studio output — no hand merges | [x] policy |
| B6 | Shared helpers under `src/display/ui/default/shared/` compile on both paths | [x] |
| B7 | `display-ws43-legacy` alias extends production (pre-cutover identical) | [x] |

---

## 8. Suggested verification order (before any production cutover)

1. Run `python scripts/validate_ws43_eez.py`
2. Run `python scripts/smoke_ws43_sim_contract.py`
3. Flash **production** `display-ws43` — confirm no regression (smoke Manual Brew, brew pause, menu icon)
4. Flash **experimental** `display-ws43-native` only on a bench unit
5. Walk sections 1–6 on device; check boxes only with evidence
6. Only then consider a separate change that retargets `display-ws43` (out of scope for recovery work)

---

## 9. Explicit non-goals for this recovery phase

- Switching production off the legacy path
- Editing generated `eez/` / `eez43/` sources by hand
- Uploading firmware as part of documentation/helper extraction
- Claiming native parity complete while unchecked items remain
