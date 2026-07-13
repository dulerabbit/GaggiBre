# GaggiBre-next — base material (flash this)

Current bring-up branch with Manual Brew: `cursor/manual-brew-eez-eb99`  
(Scaffold base: `cursor/gaggimate-next-scaffold-eb99`)

This is the **matched stack for a controller already on new Gaggimate firmware** (NanoPb protocol). Do not flash old SquareLine display firmware against that PCB.

## What’s in this base
- Upstream Gaggimate: EEZ UI, NanoPb v3, LittleFS, modern controller protocol
- Waveshare 4.3 / 4.3C drivers + `display-ws43` / `display-ws43c` envs
- Production Waveshare UI: upstream `eez/` + runtime `WideLayout` + procedural Manual Brew
- Experimental native 800×480 EEZ lives under `display-ws43-native` / `eez43/` (not daily driver)
- Shared non-generated UI services under `src/display/ui/default/shared/` (process control, Manual Brew session, secondary-action icons) — used by both paths without cutting production over
- Manual Brew (menu 4th icon) on LilyGo + Waveshare
- **No Adaptive Brew mod** (removed)

## Flash Waveshare display (controller already new)

Web UI is **embedded in the firmware app image** (not LittleFS). If you skip the web
build, the AP page returns **Not found**. Always run `build_webui.sh` before
`pio run` when you need the web UI.

From this branch, on the PC with the display on COM11:

```bash
git checkout cursor/manual-brew-eez-eb99
git pull
./scripts/build_webui.sh
pio run -e display-ws43 -t upload --upload-port COM11
```

Then open `http://4.4.4.1/` on the phone (after joining `GaggiBre` with the AP password from the Info QR / serial log).

If you also need to re-flash the PCB from this same generation:

```bash
pio run -e controller -t upload --upload-port <PCB_PORT>
```

## What to expect
- Full-width rectangular EEZ UI on the 4.3" panel (no black side bars)
- Straight temp / pressure columns left and right; 4-icon menu in a 2×2 grid
- Brew / menu / profile / Manual Brew (or Grind) with the new controller
- Web UI at `http://4.4.4.1/` when connected to the AP

## Blank screen after first scaffold flash?
Fixed on scaffold: letterbox side-bar paint used width as `x_end` for
`esp_lcd_panel_draw_bitmap`, which can hang RGB bring-up. Pull latest and
re-flash `display-ws43`. Backlight is also forced on at panel init.

## AP asks for a password?
Upstream Gaggimate generates a WPA2 AP password on first boot (old GaggiBre was
open). Read it from the Info-screen WiFi QR or serial. Captive portal still
redirects to `http://4.4.4.1/` after you connect.

## Not done yet
- Voice on EEZ
- Open-AP option / WiFi polish
- Full native EEZ Studio 800×480 production cutover (see `docs/GAGGIBRE_EEZ43_PARITY.md`)

## Experimental native EEZ (do not flash as daily driver)
```bash
pio run -e display-ws43-native -t upload --upload-port COM11
```
Only after every item in `docs/GAGGIBRE_EEZ43_PARITY.md` passes.

## Rollback (old SquareLine daily driver)
Only if you flash **both** display and controller back to the old generation:

`backup/gaggibre-squareline-stable-eb99`  
(or Adaptive-removed SquareLine: `cursor/remove-adaptive-eb99`)
