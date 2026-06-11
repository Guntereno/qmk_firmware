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
 */
#define SPLIT_TRANSPORT_MIRROR

/* Uses EEPROM to determine handedness
 */
#define EE_HANDS
