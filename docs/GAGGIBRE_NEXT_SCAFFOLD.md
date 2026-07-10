# GaggiBre-next scaffold status

This branch (`cursor/gaggimate-next-scaffold-eb99`) is **upstream Gaggimate master** plus GaggiBre DNA and a Waveshare EEZ MVP.

## What landed

### Phase 1–2 — scaffold
- Full upstream stack: EEZ UI, NanoPb protocol v3, LittleFS, gear pump, WiFi watchdogs
- Waveshare 4.3 / 4.3C board defs + drivers + `factory_16MB.csv`
- `display-ws43` / `display-ws43c` PlatformIO envs
- `AdaptiveBrewEngine` + `profile.adaptiveBrew` field (engine present; EEZ UI hook later)
- `VoicePlugin` sources (compile only with `-DGAGGIMATE_VOICE_ENABLED`)
- Web flasher assets + merge plan doc

### Phase 4 MVP — Waveshare boots EEZ (letterbox)
- LVGL keeps the upstream **480×480** EEZ coordinate space on Waveshare
- Flush path centers that UI on the **800×480** panel (160 px black bars each side)
- Touch X is remapped / clamped to the centered column
- Side bars cleared black once at init

This is **usable, not final**. Native wide EEZ layouts (using the full 800 px) still need EEZ Studio work.

## What is NOT done yet

- Native 800×480 EEZ page layouts (Option C in the merge plan)
- Adaptive pill / Profile Settings / Manual Brew on EEZ
- Matched flasher generation guards
- WiFi polish (explicitly deferred)

## How to try Waveshare on this branch

Flash a **matched pair** from this branch only:

```bash
pio run -e controller -t upload --upload-port <PCB_PORT>
pio run -e display-ws43 -t upload --upload-port COM11
```

Expect: round EEZ UI centered on the 4.3" panel with black side bars. Tap/swipe should hit the center column.

## Daily coffee

Until you sign off this stack, keep flashing: `backup/gaggibre-squareline-stable-eb99`

## Plan

See [docs/GAGGIMATE_MERGE_PLAN.md](docs/GAGGIMATE_MERGE_PLAN.md)
