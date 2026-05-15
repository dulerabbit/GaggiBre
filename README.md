# GaggiBre

<p align="center">
<img src="docs/assets/logo.png" alt="Logo" width="250px" />
<br />

[![CC BY-NC-SA 4.0][cc-by-nc-sa-shield]][cc-by-nc-sa]

</p>

**GaggiBre** is a community fork of [GaggiMate](https://github.com/jniebuhr/gaggimate) — the open-source smart controller for Gaggia espresso machines. It builds on everything GaggiMate provides and adds an **adaptive PI brew controller** that automatically adjusts pump pressure in real time based on live flow data, so you get a more consistent shot without manually tweaking profiles.

## What's Different from GaggiMate

GaggiBre is a feature branch, not a replacement. The hardware, PCB, and web flasher are identical. The differences are purely in firmware behaviour:

| Feature | GaggiMate | GaggiBre |
|---|---|---|
| Pressure profiling | ✅ Manual profiles | ✅ Manual profiles |
| Adaptive brew control | ❌ | ✅ Per-profile PI controller |
| Flow-based channeling detection | ❌ | ✅ Live annotation in shot log |
| Adaptive metadata in shot history | ❌ | ✅ Visible in Shot Analyzer |
| Toggle adaptive on/off per profile | ❌ | ✅ Long-press or UI toggle |

### Adaptive PI Brew Controller

When adaptive mode is enabled on a profile, the firmware runs a closed-loop PI controller that watches actual flow rate against the target and trims the pump output to compensate. If channeling is detected (a sudden flow spike mid-shot) it logs the event and annotates the shot chart so you can see exactly when it happened.

Adaptive mode is **per-profile** and **opt-in** — all existing profiles behave exactly as they do in stock GaggiMate unless you enable it.

## Hardware

GaggiBre runs on the same hardware as GaggiMate. You can buy a kit or build your own:

- **Kit**: [shop.gaggimate.eu](https://shop.gaggimate.eu/)
- **Docs & assembly**: [gaggimate.eu](https://gaggimate.eu/)
- **PCB files**: see the `pcb/` directory

## Flashing

### Web Flasher (easiest)

The latest stable release is available via the GitHub Pages web flasher — no tools required, just a USB cable and Chrome/Edge:

➡️ **[https://dulerabbit.github.io/GaggiBre/stable/](https://dulerabbit.github.io/GaggiBre/stable/)**

### Manual / PlatformIO

```bash
# Clone including submodules
git clone --recursive https://github.com/dulerabbit/GaggiBre.git
cd GaggiBre

# Build and flash (display variant)
pio run -e display -t upload --upload-port <PORT>

# Build and flash filesystem
pio run -e display -t uploadfs --upload-port <PORT>
```

## Features (inherited from GaggiMate)

- **Temperature Control** — boiler PID with configurable targets
- **Pressure Profiling** — multi-phase profiles with ramp, hold, and flow control
- **Brew Timer** — target duration with automatic stop
- **Steam & Hot Water** — dedicated modes for non-espresso tasks
- **Shot History** — full shot log with pressure, temperature, and flow charts
- **OTA Updates** — over-the-air firmware updates from the web UI
- **Web UI** — mobile-friendly interface served from the device itself
- **mDNS** — reach the device at `http://gaggimate.local` on your local network
- **Safety** — automatic shutoff on overtemp or watchdog timeout

## License

This work is licensed under [CC BY-NC-SA 4.0](http://creativecommons.org/licenses/by-nc-sa/4.0/). It is a derivative of [GaggiMate](https://github.com/jniebuhr/gaggimate) by Jan Niebuhr, used under the same license.

[cc-by-nc-sa]: http://creativecommons.org/licenses/by-nc-sa/4.0/
[cc-by-nc-sa-image]: https://licensebuttons.net/l/by-nc-sa/4.0/88x31.png
[cc-by-nc-sa-shield]: https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg?style=for-the-badge
