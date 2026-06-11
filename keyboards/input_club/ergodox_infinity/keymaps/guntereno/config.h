#pragma once

/* User Configuration
 * Modern QMK-compatible header for Ergodox layouts.
 * LCD and Infinity-specific options removed.
 */

/* ---------- Tapping / Mod-Tap Behaviour ---------- */

#define PERMISSIVE_HOLD
#define TAPPING_TERM 150

/* ---------- Split Keyboard Behaviour ---------- */

/* Use modern split transport mirroring:
 * - Mirrors layer state
 * - Mirrors LED state
 * - Mirrors mod state
 * - Replaces old SPLIT_LAYER_STATE_ENABLE / SPLIT_LED_STATE_ENABLE
 */
#define SPLIT_TRANSPORT_MIRROR

/* If your board uses EEPROM to determine handedness (Infinity did),
 * keep EE_HANDS. If you later move to a standard Ergodox EZ, you can
 * remove this and set handedness via config.
 */
#define EE_HANDS
