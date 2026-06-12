#pragma once

/* User Configuration
 * Modern QMK-compatible header for Ergodox layouts.
 * LCD and Infinity-specific options removed.
 */

/* ---------- Tapping / Mod-Tap Behaviour ---------- */

#define PERMISSIVE_HOLD
#define TAPPING_TERM 150

/* ---------- Split Keyboard Behaviour ---------- */

#define SPLIT_TRANSPORT_MIRROR
#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_LED_STATE_ENABLE
#define SPLIT_ST7565_ENABLE

/* ---------- Uses EEPROM to determine handedness ---------- */

#define EE_HANDS


/* ---------- Allow naming of LED matrix modes for use on ST7565 ---------- */

#define LED_MATRIX_MODE_NAME_ENABLE
