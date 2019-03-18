#include QMK_KEYBOARD_H

#include "version.h"

#define BASE 0
#define NUMPAD 1
#define FUNCTION 2
#define SPECIAL 3

enum custom_keycodes
{
  PLACEHOLDER = SAFE_RANGE, // can always be here
  VRSN
};

enum tapdance_definitions
{
  TD_ESC_CAPS = 0,
  TD_PAUSE_PRNTSCRN = 1,
  TD_SAFETY_RESET,
  TD_SAFETY_EEPROM
};

void dance_safety_reset(qk_tap_dance_state_t *state, void *user_data);
void dance_safety_eeprom(qk_tap_dance_state_t *state, void *user_data);

// Tap Dance Definitions
qk_tap_dance_action_t tap_dance_actions[] =
{
  [TD_ESC_CAPS]  = ACTION_TAP_DANCE_DOUBLE(KC_ESC, KC_CAPS),
  [TD_PAUSE_PRNTSCRN] = ACTION_TAP_DANCE_DOUBLE(KC_PSCREEN, KC_PAUSE),
  [TD_SAFETY_RESET] = ACTION_TAP_DANCE_FN(dance_safety_reset),
  [TD_SAFETY_EEPROM] = ACTION_TAP_DANCE_FN(dance_safety_eeprom)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] =
{
    [BASE] = LAYOUT_ergodox(
        // Left hand side
        KC_EQL,  KC_1,    KC_2,     KC_3,    KC_4,     KC_5, TD(TD_ESC_CAPS),
        KC_NUHS, KC_Q,    KC_W,     KC_E,    KC_R,     KC_T, KC_NUBS,
        KC_TAB,  KC_A,    KC_S,     KC_D,    KC_F,     KC_G,
        KC_LSPO, KC_Z,    KC_X,     KC_C,    KC_V,     KC_B, MO(SPECIAL),
        KC_LCTL, KC_LGUI, KC_LALT, KC_LEFT, KC_RIGHT,

                 KC_GRV, KC_HOME,
                         KC_END,
        KC_BSPC, KC_DEL, MO(FUNCTION),

        // Right hand side0
        TG(NUMPAD),   KC_6, KC_7,  KC_8,    KC_9,    KC_0,           KC_MINS,
        KC_LBRACKET,  KC_Y, KC_U,  KC_I,    KC_O,    KC_P,           KC_RBRACKET,
                      KC_H, KC_J,  KC_K,    KC_L,    KC_SCLN,        KC_QUOT,
        MO(SPECIAL),  KC_N, KC_M,  KC_COMM, KC_DOT,  KC_SLSH,        KC_RSPC,
                            KC_UP, KC_DOWN, KC_RALT, KC_APPLICATION, KC_RCTL,

        KC_PGUP,      TD(TD_PAUSE_PRNTSCRN),
        KC_PGDOWN,
        MO(FUNCTION), KC_ENT,                KC_SPC
    ),

    [NUMPAD] = LAYOUT_ergodox(
        // Left hand side
        KC_NO,    KC_NO,         KC_NO,          KC_NO,      KC_NO,       KC_NO, KC_TRNS,
        KC_NO,    KC_NO,         KC_NUHS,        LSFT(KC_4), LSFT(KC_3),  KC_NO, KC_NO,
        KC_TRNS,  KC_KP_SLASH,   KC_KP_ASTERISK, KC_KP_PLUS, KC_KP_MINUS, KC_NO,
        KC_TRNS,  KC_NO,         KC_NO,          KC_NO,      KC_NO,       KC_NO, KC_NO,
        KC_TRNS,  KC_TRNS,       KC_TRNS,        KC_NO,      KC_NO,

               KC_NO,     KC_TRNS,
                          KC_TRNS,
        KC_TRNS, KC_TRNS, KC_NO,

        // Right hand side0
        TG(NUMPAD), KC_NO, KC_NO,         KC_NUMLOCK, KC_KP_SLASH, KC_KP_ASTERISK, KC_KP_MINUS,
        KC_NO,      KC_NO, KC_NO,         KC_P7,      KC_P8,       KC_P9,          KC_KP_PLUS,
                    KC_NO, KC_NO,         KC_P4,      KC_P5,       KC_P6,          KC_KP_ENTER,
        KC_NO,      KC_NO, KC_CALCULATOR, KC_P1,      KC_P2,       KC_P3,          KC_TRNS,
                           KC_KP_ENTER,   KC_KP_0,    KC_KP_0,     KC_KP_DOT,      KC_TRNS,

        KC_TRNS, KC_KP_0,
        KC_TRNS,
        KC_TRNS, KC_TRNS,   KC_TRNS
    ),

    [FUNCTION] = LAYOUT_ergodox(
      // Left hand side
      KC_NO,    KC_F1,    KC_F2,    KC_F3,   KC_F4,    KC_F5,     KC_F11,
      KC_NO,    KC_NO,    KC_HOME,  KC_UP,   KC_END,   KC_PGUP,   KC_NO,
      KC_NO,    KC_NO,    KC_LEFT,  KC_DOWN, KC_RIGHT, KC_PGDOWN,
      KC_TRNS,  KC_NO,    KC_NO,    KC_NO,   KC_NO,    KC_NO,     KC_NO,
      KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_NO,   KC_NO,

             KC_NO, KC_NO,
                    KC_NO,
      KC_NO, KC_NO, KC_TRNS,

      // Right hand side0
      KC_F12,  KC_F6, KC_F7,         KC_F8,        KC_F9,        KC_F10, KC_NO,
      KC_NO,   KC_NO, KC_MS_WH_DOWN, KC_MS_UP,     KC_MS_WH_UP,  KC_NO,  KC_NO,
               KC_NO, KC_MS_LEFT,    KC_MS_DOWN,   KC_MS_RIGHT,  KC_NO,  KC_NO,
      KC_TRNS, KC_NO, KC_MS_ACCEL0,  KC_MS_ACCEL1, KC_MS_ACCEL2, KC_NO,  KC_TRNS,
               KC_NO, KC_NO,         KC_TRNS,      KC_TRNS,      KC_TRNS,


      KC_NO,    KC_NO,
      KC_NO,
      KC_TRNS, KC_MS_BTN2, KC_MS_BTN1
    ),


    [SPECIAL] = LAYOUT_ergodox(
        // Left hand side
        TD(TD_SAFETY_RESET),  KC_NO, KC_NO,               KC_NO,             KC_NO,               KC_NO, KC_NO,
        TD(TD_SAFETY_EEPROM), KC_NO, KC_NO,               KC_AUDIO_VOL_UP,   KC_MEDIA_PLAY_PAUSE, KC_NO, KC_NO,
        KC_NO,                KC_NO, KC_MEDIA_PREV_TRACK, KC_AUDIO_VOL_DOWN, KC_MEDIA_NEXT_TRACK, KC_NO,
        KC_NO,                KC_NO, KC_NO,               KC__MUTE,          VRSN,                KC_NO, KC_TRNS,
        KC_NO,                KC_NO, KC_NO,               KC_NO,             KC_NO,

                KC_INS, KC_SCROLLLOCK,
                        KC_NO,
        KC_NO,  KC_NO,  KC_NO,

        // Right hand side0
        KC_NO,   KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,   TD(TD_SAFETY_RESET),
        KC_NO,   KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,   TD(TD_SAFETY_EEPROM),
                 KC_NO, BL_TOGG, BL_DEC,  BL_INC,  KC_NO,   KC_NO,
        KC_TRNS, KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_TRNS,
                        KC_NO,   KC_NO,   KC_TRNS, KC_TRNS, KC_TRNS,

        KC_NO, KC_NO,
        KC_NO,
        KC_NO, KC_NO, KC_NO
    ),
};

void output_version(void)
{
  SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
}

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  switch(id) {
  }
  return MACRO_NONE;
};

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
  switch (keycode)
  {
    case VRSN:
      if (record->event.pressed)
      {
        output_version();
      }
      return false;
  }

  return true;
}

// Runs constantly in the background, in a loop.
void matrix_scan_user(void)
{

};

// Runs whenever there is a layer state change.
uint32_t layer_state_set_user(uint32_t state)
{
  ergodox_board_led_off();
  ergodox_right_led_1_off();
  ergodox_right_led_2_off();
  ergodox_right_led_3_off();

  uint8_t layer = biton32(state);
  if(layer == NUMPAD)
  {
    ergodox_right_led_1_on();
  }

  return state;
};


// Tapdance Functions
bool process_safety_tapdance(qk_tap_dance_state_t *state)
{
  const int TAP_COUNT = 3;
  if (state->count >= TAP_COUNT)
  {
    reset_tap_dance(state);
    return true;
  }
  return false;
}

void dance_safety_reset(qk_tap_dance_state_t *state, void *user_data)
{
  if (process_safety_tapdance(state))
    reset_keyboard();
}

void dance_safety_eeprom(qk_tap_dance_state_t *state, void *user_data)
{
  if (process_safety_tapdance(state))
    eeconfig_init();
}
