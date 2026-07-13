# Regenerating EEZ UI sources

Run each command separately from the repository root:

```powershell
& "$env:LOCALAPPDATA\Programs\eezstudio\EEZ Studio.exe" --build-project "eez-ui\gaggimate.eez-project"
```

`gaggimate.eez-project` owns and generates `src/display/ui/default/eez`.
This is the **production** UI tree for LilyGo and Waveshare (`display` / `display-ws43`).

```powershell
& "$env:LOCALAPPDATA\Programs\eezstudio\EEZ Studio.exe" --build-project "eez-ui\gaggimate-ws43.eez-project"
```

`gaggimate-ws43.eez-project` owns and generates `src/display/ui/default/eez43`.
This is the **experimental** native 800×480 tree used only by
`display-ws43-native` and `display-sim-ws43`.

Treat both destination trees as generated output. Make UI changes in the matching
`.eez-project` source and regenerate only its destination; do not copy or hand-edit
generated files across the two boundaries.

Production Waveshare (`display-ws43`) continues to compile `eez/` + `WideLayout` +
procedural `ManualBrewScreen` until the checklist in
`docs/GAGGIBRE_EEZ43_PARITY.md` is complete.

Shared non-generated C++ under `src/display/ui/default/shared/` may be used by both
UI paths. Do not move behavior into generated `eez/` / `eez43/` trees.
