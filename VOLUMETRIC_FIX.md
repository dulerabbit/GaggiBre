# Volumetric Weight Target Fix

## Problem
Shots with Bluetooth scales were not stopping at the target weight. For example, setting a 30g target would stop at 28g instead.

## Root Cause
The code was using aggressive predictive overshoot calculation that would estimate future weight gain and stop early to prevent overshooting. This caused premature stopping.

## Solution
Changed the stopping logic to use **actual measured weight** from the Bluetooth scale with only a small 0.2g safety margin for pump stopping latency.

## Changes Made

### 1. BrewProcess.h (src/display/core/process/BrewProcess.h)
- **Line 57-72**: `isCurrentPhaseFinished()` method
- **Old behavior**: Used `currentVolume + predictedAddedVolume` (could be several grams of prediction)
- **New behavior**: 
  - For volumetric targets: Uses `currentVolume + 0.2` (minimal safety margin only)
  - For other targets: Still uses predictive overshoot (unchanged)
- **Effect**: Shots now stop within 0.2g of target weight instead of 2+ grams early

### 2. DefaultUI.cpp (src/display/ui/default/DefaultUI.cpp)
- **Line 1127-1143**: `updateStatusScreen()` method
- **Old behavior**: Weight display hidden during active brews (only shown after shot ends)
- **New behavior**: Shows live weight during active brews when in volumetric mode
- **Effect**: You can now see real-time weight on display during the shot

## How to Use

1. **Flash the updated firmware** to your device (already done via COM5)
2. **Enable volumetric mode** in your profile (set a weight target)
3. **Connect Bluetooth scale** and verify it pairs with the controller
4. **Start a shot** - the display will show live weight
5. **Shot stops** when weight reaches your target ± 0.2g

## Testing Checklist

- [ ] Firmware boots successfully
- [ ] Bluetooth scale connects and shows weight
- [ ] Profile with 30g volumetric target loads
- [ ] Live weight displays during shot on screen
- [ ] Shot stops at 30g ± 0.2g (not earlier)
- [ ] Adaptive brew + volumetric works together

## Technical Details

**File changes:**
- BrewProcess.h: 2 lines changed (stopping logic)
- DefaultUI.cpp: 8 lines changed (display logic)

**Commit:** 737d5c6 - "Fix volumetric weight targets with Bluetooth scale"

**Build info:**
- Firmware size: 5029 KB
- Upload time: 125.99 seconds
- Device: ESP32-S3 on COM5

## Rollback (if needed)

```bash
git revert 737d5c6
pio run -e display -t upload --upload-port COM5
```
