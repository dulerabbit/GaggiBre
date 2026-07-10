# GaggiBre-next scaffold status

This branch (`cursor/gaggimate-next-scaffold-eb99`) is **upstream Gaggimate master** plus the first layer of GaggiBre DNA.

## What landed in Phase 1–2 (scaffold)

- Full upstream stack: EEZ UI, NanoPb protocol v3, LittleFS, gear pump, WiFi watchdogs
- Waveshare 4.3 / 4.3C board defs + drivers + `factory_16MB.csv`
- `display-ws43` / `display-ws43c` PlatformIO envs
- Wide-panel LVGL partial buffers (needed for 800×480)
- `AdaptiveBrewEngine` + `profile.adaptiveBrew` field (engine present; EEZ UI hook later)
- `VoicePlugin` sources (compile only with `-DGAGGIMATE_VOICE_ENABLED`)
- Web flasher assets + merge plan doc

## What is NOT done yet

- EEZ layouts for 800×480 (Waveshare still gets round EEZ UI until Phase 4)
- Adaptive pill / Profile Settings / Manual Brew on EEZ
- Matched flasher generation guards
- WiFi polish (explicitly deferred)

## Daily coffee

Keep flashing: `backup/gaggibre-squareline-stable-eb99`

## Plan

See [docs/GAGGIMATE_MERGE_PLAN.md](docs/GAGGIMATE_MERGE_PLAN.md)
