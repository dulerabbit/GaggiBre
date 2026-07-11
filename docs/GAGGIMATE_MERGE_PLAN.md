# GaggiBre ↔ Gaggimate Merge Plan

**Owner:** Cursor cloud agent (Dusan does not need to write code)  
**Updated:** 2026-07-10  
**Goal:** Make GaggiBre run on the **new Gaggimate stack** (EEZ UI + NanoPb protocol + modern controller) while keeping Waveshare 4.3, Voice, flasher, and branding.

WiFi is **deferred** (lowest priority). Do not block the merge on it.

This is a **phased reverse-port**, not a blind git merge.

---

## 0. Safety backups (already on GitHub)

| Backup | What it is |
|--------|------------|
| Tag `backup/pre-eez-migration-main-20260710` | Old GaggiBre `main` |
| Tag `backup/pre-eez-migration-ui-fixes-20260710` | UI stability tip (`db3c129`) |
| Branch `backup/gaggibre-squareline-stable-eb99` | **Daily-driver flash target** until the new stack is proven |

**Rule:** Keep making coffee from the backup branch until a phase is signed off on hardware.

**Matched flash rule:** Always flash **controller + display from the same generation**. Mixing old GaggiBre display with new upstream PCB (or reverse) breaks control.

---

## 1. Strategy change (important)

### Old idea (rejected as primary)
Cherry-pick ~50 upstream commits into current GaggiBre SquareLine/NimBLE tree. Too many conflicts; EEZ still requires a full UI rewrite anyway.

### New idea (primary path)
**Start from upstream Gaggimate `master`, then port GaggiBre DNA onto it.**

```
upstream/master  ──►  GaggiBre-next (EEZ + NanoPb + LittleFS)
                         │
                         ├─ add Waveshare drivers + board envs
                         ├─ rebuild 4.3 UI on EEZ
                         ├─ port Voice / Manual Brew / Profile Settings
                         ├─ port flasher + branding
                         └─ (later) WiFi polish
```

Old SquareLine GaggiBre stays frozen in backup tags forever.

Difficulty of Waveshare-on-EEZ redraw: **~7.5 / 10**. Still the hard part — but we inherit protocol, gear pump, EEZ menu/info, pressure fixes for free.

---

## 2. What must survive (GaggiBre DNA)

| Feature | Source paths (old tree) |
|---------|-------------------------|
| Waveshare 4.3 / 4.3C | `boards/WaveshareS3Touch43*`, `src/display/drivers/Waveshare43*`, `factory_16MB.csv` |
| Dual UI layouts | `src/display/ui/default/lvgl/screens43/` → **rebuild in EEZ** |
| Voice | `VoicePlugin.*` |
| Manual Brew / Profile Settings | SquareLine screens → **EEZ screens** |
| Volumetric weight fix | `BrewProcess.h` logic (merge with upstream negative-weight fix) |
| Flasher + CI | `scripts/flasher/`, `.github/workflows/build.yml` ws43 jobs |
| Branding | README, logo assets, license notes |

---

## 3. Phase order (WiFi last)

```
Phase 1  Scaffold GaggiBre-next from upstream/master          ✓
Phase 2  Bring non-UI DNA (boards, drivers, Voice, flasher)  ✓
Phase 3  LilyGo smoke: build + flash matched controller+display
Phase 4a Waveshare MVP: letterbox 480 EEZ on 800×480          ✓ (this branch)
Phase 4b Native EEZ 800×480 layouts (EEZ Studio / parallel tree)
Phase 5  Wire Profile Settings / Manual Brew into EEZ          ✓ Manual Brew MVP
Phase 6  CI + flasher for ws43 on new stack
Phase 7  Optional: WiFi coexistence / watchdogs / secure AP
```

### Phase 5 notes (Manual Brew MVP)
- `MODE_MANUAL` + NanoPb pressure pump control
- Hand-built 480×480 Manual Brew screen (letterboxes on Waveshare)
- Menu 4th slot via `secondaryAction` (Manual Brew / Grind / None)
- Same behavior on LilyGo + Waveshare builds
- Native 800×480 polish still Phase 4b

### Phase 1 — Scaffold (now)
- Branch from `upstream/master`
- Keep repo identity as GaggiBre (README/branding overlay)
- Confirm `display` + `controller` build
- Document restore path to backup SquareLine

### Phase 2 — Non-UI DNA
- Copy Waveshare board JSON + drivers + partition table
- Add `display-ws43` / `display-ws43c` envs (may not fully link until Phase 4)
- Port `VoicePlugin` behind flag
- Port flasher scripts (point at new artifacts when ready)

### Phase 3 — LilyGo acceptance
- Flash **new** controller + LilyGo display together
- Verify brew / steam / grind / profiles / menu / info
- Do **not** expect Waveshare yet

### Phase 4 — Waveshare on EEZ
- Extend EEZ project for 800×480 (or parallel `eez43` output)
- Port screens: Standby → Brew → Status → Menu/Info → Profile → Grind → Manual → Settings
- `DefaultUI` wide-display routing for EEZ screen IDs

### Phase 5 — Feature re-attach
- Gear → Profile Settings screen
- Manual brew parity with LilyGo concepts
- Branding on standby

### Phase 6 — Ship path
- CI builds ws43 + controller
- Flasher manifests prevent generation mixing
- README: “flash matched pair”

### Phase 7 — WiFi (deferred)
Only after Waveshare EEZ works. Coexistence, STA/network watchdogs, icon gating, etc.

---

## 4. What you do vs what the agent does

| You | Agent |
|-----|-------|
| Flash when given exact `pio` commands | All code, EEZ, merges, builds |
| Report: boots? brew works? which screen broken? | Fix and push |
| Keep backup branch for daily coffee | Maintain tags/branches/PRs |
| Never flash upstream PCB alone onto old display | Enforce matched pairs |

---

## 5. Definition of done

- [ ] Daily driver can move from backup SquareLine → GaggiBre-next
- [ ] LilyGo + Waveshare both on EEZ
- [ ] Profile Settings + Manual Brew restored
- [ ] Controller/display always matched in flasher
- [ ] Backup tags still restorable
- [ ] WiFi issues tracked separately (non-blocking)

---

## 6. Reference SHAs (2026-07-10)

| Ref | SHA |
|-----|-----|
| Old GaggiBre `main` | `4d28566` |
| UI stability backup | `db3c129` |
| Upstream `master` (scaffold base) | `0663391` |

Remotes: `origin` = GaggiBre, `upstream` = `jniebuhr/gaggimate`
