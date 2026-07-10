# GaggiBre-next — base material (flash this)

Branch: `cursor/gaggimate-next-scaffold-eb99`

This is the **matched stack for a controller already on new Gaggimate firmware** (NanoPb protocol). Do not flash old SquareLine display firmware against that PCB.

## What’s in this base
- Upstream Gaggimate: EEZ UI, NanoPb v3, LittleFS, modern controller protocol
- Waveshare 4.3 / 4.3C drivers + `display-ws43` / `display-ws43c` envs
- EEZ UI letterboxed on 800×480 (centered 480 UI + black side bars + touch remap)
- **No Adaptive Brew mod** (removed)

## Flash Waveshare display (controller already new)

From this branch, on the PC with the display on COM11:

```bash
git checkout cursor/gaggimate-next-scaffold-eb99
git pull
pio run -e display-ws43 -t upload --upload-port COM11
```

If you also need to re-flash the PCB from this same generation:

```bash
pio run -e controller -t upload --upload-port <PCB_PORT>
```

## What to expect
- Round EEZ UI centered on the 4.3" panel
- Black bars left/right
- Brew / menu / profile / grind should work with the new controller

## Not done yet (OK for first bring-up)
- Native full-width 800×480 EEZ layouts
- Profile Settings / Manual Brew / Voice on EEZ
- WiFi polish

## Rollback (old SquareLine daily driver)
Only if you flash **both** display and controller back to the old generation:

`backup/gaggibre-squareline-stable-eb99`  
(or Adaptive-removed SquareLine: `cursor/remove-adaptive-eb99`)
