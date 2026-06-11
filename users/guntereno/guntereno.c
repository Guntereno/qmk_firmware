#include QMK_KEYBOARD_H
#include "version.h"

#define BASE 0
#define GAME 1
#define NUMPAD 2
#define FUNCTION 3
#define SPECIAL 4

enum custom_keycodes {
    PLACEHOLDER = SAFE_RANGE,  // can always be here
    CK_VRSN,
    CK_LSFT,
    CK_RSFT,
    CK_LCTR,
    CK_RCTR,
    CK_LFUN,
    CK_RFUN,
    CK_MOCK
};

enum tapdance_definitions { TD_ESC_CAPS, TD_PAUSE_PRNTSCRN, TD_SAFETY_RESET, TD_SAFETY_EEPROM };


static void dance_safety_reset(qk_tap_dance_state_t *state, void *user_data);
static void dance_safety_eeprom(qk_tap_dance_state_t *state, void *user_data);
static void dance_esc_caps_altf4(qk_tap_dance_state_t *state, void *user_data);

// Tap Dance Definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_ESC_CAPS]       = ACTION_TAP_DANCE_FN(dance_esc_caps_altf4),
    [TD_PAUSE_PRNTSCRN] = ACTION_TAP_DANCE_DOUBLE(KC_PSCREEN, KC_PAUSE),
    [TD_SAFETY_RESET]   = ACTION_TAP_DANCE_FN(dance_safety_reset),
    [TD_SAFETY_EEPROM]  = ACTION_TAP_DANCE_FN(dance_safety_eeprom),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] =
{
    // BASE is a standard QWERTY layout (subtly different from the EZ layout).
    // Main difference is the split cursor keys (which take some getting used to!)
    [BASE] = LAYOUT_ergodox(
        // Left hand side
        KC_EQL,   KC_1,    KC_2,    KC_3,    KC_4,     KC_5, TD(TD_ESC_CAPS),
        KC_NUHS,  KC_Q,    KC_W,    KC_E,    KC_R,     KC_T, KC_NUBS,
        KC_TAB,   KC_A,    KC_S,    KC_D,    KC_F,     KC_G,
        CK_LSFT,  KC_Z,    KC_X,    KC_C,    KC_V,     KC_B, MO(SPECIAL),
        CK_LCTR,  KC_LGUI, KC_LALT, KC_LEFT, KC_RIGHT,

                 KC_GRV, KC_HOME,
                         KC_END,
        KC_BSPC, KC_DEL, CK_LFUN,

        // Right hand side
        TG(NUMPAD),   KC_6, KC_7,  KC_8,    KC_9,    KC_0,           KC_MINS,
        KC_LBRACKET,  KC_Y, KC_U,  KC_I,    KC_O,    KC_P,           KC_RBRACKET,
                      KC_H, KC_J,  KC_K,    KC_L,    KC_SCLN,        KC_QUOT,
        MO(SPECIAL),  KC_N, KC_M,  KC_COMM, KC_DOT,  KC_SLSH,        CK_RSFT,
                            KC_UP, KC_DOWN, KC_RALT, KC_APPLICATION, CK_RCTR,

        KC_PGUP,      TD(TD_PAUSE_PRNTSCRN),
        KC_PGDOWN,
        CK_RFUN,      KC_ENT,                KC_SPC
    ),

    // GAME mode does nothign to the layout. It simply disables the mod tap feature
    // as it causes issues in certain games
    // (see handle_mod_tap)
    [GAME] = LAYOUT_ergodox(
        // Left hand side
        KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,

                 KC_TRNS, KC_TRNS,
                          KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS,

        // Right hand side
        KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                           KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,

        KC_TRNS, KC_TRNS,
        KC_TRNS,
        KC_TRNS, KC_TRNS,  KC_TRNS
    ),

    // NUMPAD adds a standard numpad to the right keyboard and has mathematical operators
    // at the home position of the left. There's also a handy calculator shortcut.
    [NUMPAD] = LAYOUT_ergodox(
        // Left hand side
        KC_NO,    KC_NO,         KC_NO,          KC_NO,      KC_NO,       KC_NO, KC_TRNS,
        KC_NO,    KC_NO,         KC_NUHS,        LSFT(KC_4), LSFT(KC_3),  KC_NO, KC_NO,
        KC_TRNS,  KC_KP_SLASH,   KC_KP_ASTERISK, KC_KP_PLUS, KC_KP_MINUS, KC_NO,
        KC_TRNS,  KC_NO,         KC_NO,          KC_NO,      KC_NO,       KC_NO, KC_NO,
        KC_TRNS,  KC_TRNS,       KC_TRNS,        KC_NO,      KC_NO,

                 KC_NO,   KC_TRNS,
                          KC_TRNS,
        KC_TRNS, KC_TRNS, KC_NO,

        // Right hand side
        TG(NUMPAD), KC_NO, KC_NO,         KC_NUMLOCK, KC_KP_SLASH, KC_KP_ASTERISK, KC_KP_MINUS,
        KC_NO,      KC_NO, KC_NO,         KC_P7,      KC_P8,       KC_P9,          KC_KP_PLUS,
                    KC_NO, KC_NO,         KC_P4,      KC_P5,       KC_P6,          KC_KP_ENTER,
        KC_NO,      KC_NO, KC_CALCULATOR, KC_P1,      KC_P2,       KC_P3,          KC_TRNS,
                           KC_KP_ENTER,   KC_KP_0,    KC_KP_0,     KC_KP_DOT,      KC_TRNS,

        KC_TRNS, KC_TRNS,
        KC_TRNS,
        KC_TRNS, KC_TRNS,   KC_TRNS
    ),

    // FUNCTION replaces the numbers with the function keys, has navigation keys around
    // the home position on the left side, with mouse controls on the right.
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

      // Right hand side
      KC_F12,  KC_F6, KC_F7,         KC_F8,        KC_F9,        KC_F10,  KC_NO,
      KC_NO,   KC_NO, KC_MS_WH_DOWN, KC_MS_UP,     KC_MS_WH_UP,  KC_NO,   KC_NO,
               KC_NO, KC_MS_LEFT,    KC_MS_DOWN,   KC_MS_RIGHT,  KC_NO,   KC_NO,
      KC_TRNS, KC_NO, KC_MS_ACCEL0,  KC_MS_ACCEL1, KC_MS_ACCEL2, KC_NO,   KC_TRNS,
               KC_NO, KC_NO,         KC_TRNS,      KC_TRNS,      KC_TRNS,


      KC_NO,    KC_NO,
      KC_NO,
      KC_TRNS, KC_MS_BTN2, KC_MS_BTN1
    ),

    // SPECIAL adds less commonly used functions. Media keys on the left home, backlight
    // controls on the right and safety (tap 3 times) reset and eeprom reset for each side
    // in the top corners.
    [SPECIAL] = LAYOUT_ergodox(
        // Left hand side
        TD(TD_SAFETY_RESET),  KC_NO,   KC_NO,               KC_NO,             KC_NO,               KC_NO,    KC_NO,
        TD(TD_SAFETY_EEPROM), KC_NO,   KC_NO,               KC_AUDIO_VOL_UP,   KC_MEDIA_PLAY_PAUSE, KC_NO,    KC_NO,
        KC_NO,                KC_NO,   KC_MEDIA_PREV_TRACK, KC_AUDIO_VOL_DOWN, KC_MEDIA_NEXT_TRACK, TG(GAME),
        KC_NO,                KC_NO,   KC_NO,               KC_MUTE,           CK_VRSN,             KC_NO,    KC_NO,
        KC_NO,                KC_NO,   KC_NO,               KC_NO,             KC_NO,

                KC_INS, KC_SCROLLLOCK,
                        KC_NO,
        KC_NO,  KC_NO,  KC_NO,

        // Right hand side
        KC_NO, KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,   TD(TD_SAFETY_RESET),
        KC_NO, KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,   TD(TD_SAFETY_EEPROM),
               KC_NO, BL_TOGG, BL_DEC,  BL_INC,  KC_NO,   KC_NO,
        KC_NO, KC_NO, CK_MOCK, KC_NO,   KC_NO,   KC_NO,   KC_TRNS,
                      KC_NO,   KC_NO,   KC_TRNS, KC_TRNS, KC_TRNS,

        KC_NO, KC_NO,
        KC_NO,
        KC_NO, KC_NO, KC_NO
    ),
};

void output_version(void) { SEND_STRING(QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION); }

void handle_mod_tap_pressed(uint16_t keycode) {
    switch (keycode) {
        case CK_LSFT: {
            register_code(KC_LSFT);
            return;
        }
        case CK_RSFT: {
            register_code(KC_RSFT);
            return;
        }
        case CK_LCTR: {
            register_code(KC_LCTL);
            return;
        }
        case CK_RCTR: {
            register_code(KC_RCTL);
            return;
        }
        case CK_LFUN:
        case CK_RFUN: {
            layer_on(FUNCTION);
            return;
        }
    }
}

void handle_mod_tap_released(uint16_t keycode) {
    switch (keycode) {
        case CK_LSFT: {
            unregister_code(KC_LSFT);
            return;
        }
        case CK_RSFT: {
            unregister_code(KC_RSFT);
            return;
        }
        case CK_LCTR: {
            unregister_code(KC_LCTL);
            return;
        }
        case CK_RCTR: {
            unregister_code(KC_RCTL);
            return;
        }
        case CK_LFUN:
        case CK_RFUN: {
            layer_off(FUNCTION);
            return;
        }
    }
}

void handle_mod_tap_tapped(uint16_t keycode) {
    switch (keycode) {
        case CK_LSFT: {
            send_string("(");
            return;
        }
        case CK_RSFT: {
            send_string(")");
            return;
        }
        case CK_LCTR: {
            send_string("{");
            return;
        }
        case CK_RCTR: {
            send_string("}");
            return;
        }
        case CK_LFUN: {
            send_string("[");
            return;
        }
        case CK_RFUN: {
            send_string("]");
            return;
        }
    }
}

void handle_mod_tap(uint16_t keycode, bool pressed, uint16_t *timer, void (*handle_pressed)(uint16_t keycode), void (*handle_released)(uint16_t keycode), void (*handle_tapped)(uint16_t keycode)) {
    if (pressed) {
        *timer = timer_read();
        handle_pressed(keycode);
        return;
    }

    handle_released(keycode);

    if(IS_LAYER_ON(GAME)) return;

    if (timer_elapsed(*timer) < TAPPING_TERM) {
        handle_tapped(keycode);
    }
}

#define MOD_TAP_HANDLER()                                                                                                               \
    ({                                                                                                                                  \
        static uint16_t timer;                                                                                                          \
        handle_mod_tap(keycode, record->event.pressed, &timer, handle_mod_tap_pressed, handle_mod_tap_released, handle_mod_tap_tapped); \
    })

void toggle_caps(void) {
    register_code(KC_CAPS);
    unregister_code(KC_CAPS);
}

static bool g_mock_text = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    if (g_mock_text) {
        if (record->event.pressed) {
            if ((keycode >= KC_A) && (keycode <= KC_Z)) {
                toggle_caps();
            }
        }
    }

    switch (keycode) {
        case CK_VRSN: {
            if (record->event.pressed) {
                output_version();
            }
            return false;
        }
        case CK_LSFT:
        case CK_RSFT:
        case CK_LCTR:
        case CK_RCTR:
        case CK_LFUN:
        case CK_RFUN: {
            MOD_TAP_HANDLER();
            return false;
        }
        case CK_MOCK: {
            if (record->event.pressed) {
                g_mock_text = !g_mock_text;
                if (g_mock_text) {
                    // Do an initial toggle, to ensure the first letter is lower case
                    toggle_caps();
                }
            }
            return false;
        }
    }

    return true;
}

void matrix_init_user(void) {
    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();
}

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {}

// Used to update the states of the LEDs (standard Ergodox)
static void update_leds(void) {
    led_t leds = host_keyboard_led_state();

    // Second LED has two meanings...
    if (IS_LAYER_ON(NUMPAD)) {
        ergodox_right_led_1_on();
        // ... in NUMPAD layer, it shows status of numlock
        if (leds.num_lock)
            ergodox_right_led_2_on();
        else
            ergodox_right_led_2_off();
    } else if (IS_LAYER_ON(GAME)) {
        ergodox_right_led_1_off();
        // ... otherwise it indicates the state of the GAME layer
        ergodox_right_led_2_on();
    }

    if (leds.caps_lock)
        ergodox_right_led_3_on();
    else
        ergodox_right_led_3_off();
}

// Runs when one of the LED states is set
void led_set_user(uint8_t usb_led) {
    update_leds();
};

// Runs whenever there is a layer state change.
uint32_t layer_state_set_user(uint32_t state) {
    update_leds();
    return state;
};

// Tapdance Functions
static bool process_safety_tapdance(qk_tap_dance_state_t *state) {
    const int TAP_COUNT = 3;
    if (state->count >= TAP_COUNT) {
        reset_tap_dance(state);
        return true;
    }
    return false;
}

static void dance_safety_reset(qk_tap_dance_state_t *state, void *user_data) {
    if (process_safety_tapdance(state)) reset_keyboard();
}

static void dance_safety_eeprom(qk_tap_dance_state_t *state, void *user_data) {
    if (process_safety_tapdance(state)) eeconfig_init();
}

static void dance_esc_caps_altf4(qk_tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            // 1 tap ESC
            tap_code(KC_ESC);
            break;
        case 2:
            // 2 taps toggles capslock
            tap_code(KC_CAPS);
            break;
        case 3:
            // 3 taps is alt-f4
            register_code(KC_LALT);
            tap_code(KC_F4);
            unregister_code(KC_LALT);
            break;
        default:
            // Do nothing
            break;
    }
}

#ifdef ST7565_ENABLE

static const char* get_layer_name(void) {
    const char* layer_name;
    if (IS_LAYER_ON(SPECIAL))
    {
        layer_name = "Special";
    }
    else if (IS_LAYER_ON(FUNCTION))
    {
        layer_name = "Function";
    }
    else if (IS_LAYER_ON(NUMPAD))
    {
        layer_name = "Numpad";
    }
    else if (IS_LAYER_ON(GAME))
    {
        layer_name = "Game";
    }
    else
    {
        layer_name = "Default";
    }

    return layer_name;
}

static void update_screen_color(void) {
    static const uint16_t lowest = UINT16_MAX / 8;
    static const uint16_t low = UINT16_MAX / 4;
    static const uint16_t normal = UINT16_MAX / 2;
    static const uint16_t full = UINT16_MAX;

    led_t leds = host_keyboard_led_state();
    uint16_t lightness = (leds.caps_lock) ? full : normal;

    uint16_t r, g, b;
    if (IS_LAYER_ON(SPECIAL))
    {
        r = lightness;
        g = lowest;
        b = lowest;
    }
    else if (IS_LAYER_ON(FUNCTION))
    {
        r = low;
        g = low;
        b = lightness;
    }
    else if (IS_LAYER_ON(NUMPAD))
    {
        r = 0;
        g = lightness;
        b = lightness;
    }
    else if (IS_LAYER_ON(GAME))
    {
        r = 0;
        g = lightness;
        b = 0;
    }
    else
    {
        lightness /= 2;
        r = lightness;
        g = lightness;
        b = lightness;
    }

    ergodox_infinity_lcd_color(r, g, b);
}

void st7565_on_user(void) {
    update_screen_color();
}

void st7565_off_user(void) {
    ergodox_infinity_lcd_color(0, 0, 0);
}

extern layer_state_t layer_state;

void st7565_task_user(void) {
    if(!st7565_is_on())
        return;

    if (is_keyboard_master()) {
        st7565_write_ln(get_layer_name(), false);

        st7565_set_cursor(0, 2);
        led_t leds = host_keyboard_led_state();
        if(leds.num_lock) { st7565_write("NUM ", false); }
        if(leds.caps_lock) { st7565_write("CAPS ", false); }
        if(leds.scroll_lock) { st7565_write("SCRL ", false); }
        if(g_mock_text) { st7565_write("MCK ", false); }
        st7565_advance_page(true);
    } else {
        // Draw logo
        static const char qmk_logo[] = {
            0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94,
            0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4,
            0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0x00
        };

        st7565_write(qmk_logo, false);
        st7565_write("  " QMK_VERSION, false);
    }

    update_screen_color();
}

#endif
