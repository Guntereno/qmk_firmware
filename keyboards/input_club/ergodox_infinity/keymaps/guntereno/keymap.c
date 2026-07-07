#include QMK_KEYBOARD_H
#include "version.h"
#include "mousekey.h"


/* ---------- Constants ---------- */

#define BASE 0
#define FUNCTION 1
#define SPECIAL 2
#define NUMPAD 3

enum custom_keycodes {
    PLACEHOLDER = SAFE_RANGE,
    CK_VRSN,
    CK_LSFT,
    CK_RSFT,
    CK_LCTR,
    CK_RCTR,
    CK_LFUN,
    CK_RFUN,
    CK_MOCK_TOGGLE,
    CK_TD_TOGGLE,
    CK_GLOBE_TOGGLE
};

enum tapdance_definitions {
    TD_ESC_CAPS,
    TD_PAUSE_PRNTSCRN,
    TD_SAFETY_RESET,
    TD_SAFETY_EEPROM
};

/* ---------- Forward Declarations ---------- */

static void dance_safety_reset(tap_dance_state_t  *state, void *user_data);
static void dance_safety_eeprom(tap_dance_state_t  *state, void *user_data);
static void dance_esc_caps_altf4(tap_dance_state_t  *state, void *user_data);


/* ---------- Types ---------- */

typedef struct {
    unsigned tapdance : 1;
    unsigned mocking : 1;
    unsigned mac : 1; // Switches the right 'fn' key to send Caps Lock, for mapping to the globe key on macOS
} ModeFlags;


/* ---------- Global Variables ---------- */

static ModeFlags g_flags = {0};

tap_dance_action_t tap_dance_actions[] = {
    [TD_ESC_CAPS]       = ACTION_TAP_DANCE_FN(dance_esc_caps_altf4),
    [TD_PAUSE_PRNTSCRN] = ACTION_TAP_DANCE_DOUBLE(KC_PSCR, KC_PAUSE),
    [TD_SAFETY_RESET]   = ACTION_TAP_DANCE_FN(dance_safety_reset),
    [TD_SAFETY_EEPROM]  = ACTION_TAP_DANCE_FN(dance_safety_eeprom),
};


/* ---------- Layer Maps ---------- */

/* BASE
 * Standard QWERTY layout (subtly different from the EZ layout).
 * Main difference is the split cursor keys (which take some getting used to!)
 */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT_ergodox(
        KC_EQL,   KC_1,    KC_2,    KC_3,    KC_4,     KC_5, TD(TD_ESC_CAPS),
        KC_NUHS,  KC_Q,    KC_W,    KC_E,    KC_R,     KC_T, KC_NUBS,
        KC_TAB,   KC_A,    KC_S,    KC_D,    KC_F,     KC_G,
        CK_LSFT,  KC_Z,    KC_X,    KC_C,    KC_V,     KC_B, MO(SPECIAL),
        CK_LCTR,  KC_LGUI, KC_LALT, KC_LEFT, KC_RIGHT,

                 KC_GRV, KC_HOME,
                         KC_END,
        KC_BSPC, KC_DEL, CK_LFUN,

        TG(NUMPAD),  KC_6, KC_7,  KC_8,    KC_9,    KC_0,           KC_MINS,
        KC_LBRC,     KC_Y, KC_U,  KC_I,    KC_O,    KC_P,           KC_RBRC,
                     KC_H, KC_J,  KC_K,    KC_L,    KC_SCLN,        KC_QUOT,
        MO(SPECIAL), KC_N, KC_M,  KC_COMM, KC_DOT,  KC_SLSH,        CK_RSFT,
                           KC_UP, KC_DOWN, KC_RALT, KC_APPLICATION, CK_RCTR,

        KC_PGUP, TD(TD_PAUSE_PRNTSCRN),
        KC_PGDN,
        CK_RFUN, KC_ENT, KC_SPC
    ),

/* FUNCTION
 * Replaces the numbers with function keys, has navigation keys around the
 * home position on the left side, with mouse controls on the right.
 */
    [FUNCTION] = LAYOUT_ergodox(
        KC_F13,  KC_F1,   KC_F2,   KC_F3,   KC_F4,    KC_F5,    KC_F11,
        KC_NO,   KC_NO,   KC_HOME, KC_UP,   KC_END,   KC_PGUP,  KC_NO,
                 KC_NO,   KC_NO,   KC_LEFT, KC_DOWN,  KC_RIGHT, KC_PGDN,
        KC_TRNS, KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,    KC_NO,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_NO,   KC_NO,

                 KC_NO, KC_NO,
                        KC_NO,
        KC_NO, KC_NO, KC_TRNS,

        KC_F12,  KC_F6, KC_F7,                   KC_F8,                   KC_F9,                   KC_F10,  KC_F14,
        KC_NO,   KC_NO, QK_MOUSE_WHEEL_DOWN,     QK_MOUSE_CURSOR_UP,      QK_MOUSE_WHEEL_UP,       KC_NO,   KC_NO,
                 KC_NO, QK_MOUSE_CURSOR_LEFT,    QK_MOUSE_CURSOR_DOWN,    QK_MOUSE_CURSOR_RIGHT,   KC_NO,   KC_NO,
        KC_TRNS, KC_NO, QK_MOUSE_ACCELERATION_0, QK_MOUSE_ACCELERATION_1, QK_MOUSE_ACCELERATION_2, KC_NO,   KC_TRNS,
                        KC_NO,                   KC_NO,                   KC_TRNS,                 KC_TRNS, KC_TRNS,

        KC_NO, KC_NO,
        KC_NO,
        KC_TRNS, QK_MOUSE_BUTTON_2, QK_MOUSE_BUTTON_1
    ),

/* SPECIAL
 * Adds less commonly used functions. Media keys on the left home, backlight
 * controls on the right, and safety (tap 3 times) reset and EEPROM reset in
 * the top corners.
 */
    [SPECIAL] = LAYOUT_ergodox(
        TD(TD_SAFETY_RESET),  KC_NO, KC_NO, KC_NO,               KC_NO,               KC_NO,               KC_NO,
        TD(TD_SAFETY_EEPROM), KC_NO, KC_NO, KC_AUDIO_VOL_UP,     KC_MEDIA_PLAY_PAUSE, CK_TD_TOGGLE,        KC_NO,
                              KC_NO, KC_NO, KC_MEDIA_PREV_TRACK, KC_AUDIO_VOL_DOWN,   KC_MEDIA_NEXT_TRACK, CK_GLOBE_TOGGLE,
        KC_NO,                KC_NO, KC_NO, KC_MUTE,             CK_VRSN,             KC_NO,               KC_NO,
        KC_NO,                KC_NO, KC_NO, KC_NO,               KC_NO,

                 KC_INS, KC_SCROLL_LOCK,
                        KC_NO,
        KC_NO, KC_NO, KC_NO,

        KC_NO, KC_NO,   KC_NO,          KC_NO,   KC_NO,   KC_NO,   TD(TD_SAFETY_RESET),
        KC_NO, KC_NO,   LM_PREV,        LM_BRIU, LM_NEXT, LM_FLGN, TD(TD_SAFETY_EEPROM),
               LM_TOGG, LM_SPDD,        LM_BRID, LM_SPDU, LM_FLGP, KC_NO,
        KC_NO, KC_NO,   CK_MOCK_TOGGLE, KC_NO,   KC_NO,   KC_NO,   KC_TRNS,
                        KC_NO,          KC_NO,   KC_TRNS, KC_TRNS, KC_TRNS,

        KC_NO, KC_NO,
        KC_NO,
        KC_NO, KC_NO, KC_NO
    ),

/* NUMPAD
 * Adds a standard numpad to the right keyboard and has mathematical operators
 * at the home position of the left. There's also a handy calculator shortcut.
 */
    [NUMPAD] = LAYOUT_ergodox(
        KC_NO,   KC_NO,   KC_NO,       KC_NO,          KC_NO,      KC_NO,       KC_TRNS,
        KC_NO,   KC_NO,   KC_NUHS,     LSFT(KC_4),     LSFT(KC_3), KC_NO,       KC_NO,
                 KC_TRNS, KC_KP_SLASH, KC_KP_ASTERISK, KC_KP_PLUS, KC_KP_MINUS, KC_NO,
        KC_TRNS, KC_NO,   KC_NO,       KC_NO,          KC_NO,      KC_NO,      KC_NO,
        KC_TRNS, KC_TRNS, KC_TRNS,     KC_NO,          KC_NO,

                 KC_NO, KC_TRNS,
                        KC_TRNS,
        KC_TRNS, KC_TRNS, KC_NO,

        TG(NUMPAD), KC_NO,       KC_NO,         KC_NUM_LOCK, KC_KP_SLASH, KC_KP_ASTERISK, KC_KP_MINUS,
        KC_NO,      KC_NO,       KC_NO,         KC_P7,       KC_P8,       KC_P9,          KC_KP_PLUS,
                    KC_NO,       KC_NO,         KC_P4,       KC_P5,       KC_P6,          KC_KP_ENTER,
        KC_NO,      KC_NO,       KC_CALCULATOR, KC_P1,       KC_P2,       KC_P3,          KC_TRNS,
                                 KC_KP_ENTER,   KC_KP_0,     KC_KP_0,     KC_KP_DOT,      KC_TRNS,

        KC_TRNS, KC_NO,
        KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS
    )
};

/* ---------- Custom behaviour ---------- */

void output_version(void) {
    SEND_STRING(QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
}

void rfun_pressed(void)
{
    if (g_flags.mac) {
        register_code(KC_F20);
    }
    else {
        layer_on(FUNCTION);
    }
}

void rfun_released(void)
{
    if (g_flags.mac) {
        unregister_code(KC_F20);
    }
    else {
        layer_off(FUNCTION);
    }
}

/* Mod-tap logic preserved exactly */
void handle_mod_tap_pressed(uint16_t keycode) {
    switch (keycode) {
        case CK_LSFT: register_code(KC_LSFT); return;
        case CK_RSFT: register_code(KC_RSFT); return;
        case CK_LCTR: register_code(KC_LCTL); return;
        case CK_RCTR: register_code(KC_RCTL); return;
        case CK_LFUN: layer_on(FUNCTION); return;
        case CK_RFUN: rfun_pressed(); return;
    }
}

void handle_mod_tap_released(uint16_t keycode) {
    switch (keycode) {
        case CK_LSFT: unregister_code(KC_LSFT); return;
        case CK_RSFT: unregister_code(KC_RSFT); return;
        case CK_LCTR: unregister_code(KC_LCTL); return;
        case CK_RCTR: unregister_code(KC_RCTL); return;
        case CK_LFUN: layer_off(FUNCTION); return;
        case CK_RFUN: rfun_released(); return;
    }
}

void handle_mod_tap_tapped(uint16_t keycode) {
    switch (keycode) {
        case CK_LSFT: send_string("("); return;
        case CK_RSFT: send_string(")"); return;
        case CK_LCTR: send_string("{"); return;
        case CK_RCTR: send_string("}"); return;
        case CK_LFUN: send_string("["); return;
        case CK_RFUN: send_string("]"); return;
    }
}

void handle_mod_tap(uint16_t keycode, bool pressed, uint16_t *timer,
                    void (*pressed_fn)(uint16_t),
                    void (*released_fn)(uint16_t),
                    void (*tapped_fn)(uint16_t)) {

    if (pressed) {
        *timer = timer_read();
        pressed_fn(keycode);
        return;
    }

    released_fn(keycode);

    if (!g_flags.tapdance) return;

    if (timer_elapsed(*timer) < TAPPING_TERM) {
        tapped_fn(keycode);
    }
}

#define MOD_TAP_HANDLER() \
    ({ static uint16_t timer; \
       handle_mod_tap(keycode, record->event.pressed, &timer, \
                      handle_mod_tap_pressed, handle_mod_tap_released, handle_mod_tap_tapped); })


static void trigger_hand_animation(bool is_left);

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

#ifdef ST7565_ENABLE
    if (record->event.pressed) {
        const uint8_t row = record->event.key.row;
        if (row < 9) {
            trigger_hand_animation(true);
        } else {
            trigger_hand_animation(false);
        }
    }
#endif

    // Mock mode alternates shifted letters every other alphabet key press
    if (g_flags.mocking && record->event.pressed) {
        if (keycode >= KC_A && keycode <= KC_Z) {
            static bool mock_shift;

            if (mock_shift) {
                register_code(KC_LSFT);
            }
            tap_code16(keycode);
            if (mock_shift) {
                unregister_code(KC_LSFT);
            }

            mock_shift = !mock_shift;
            return false;
        }
    }

    switch (keycode) {
        case CK_VRSN:
            if (record->event.pressed) output_version();
            return false;

        case CK_LSFT:
        case CK_RSFT:
        case CK_LCTR:
        case CK_RCTR:
        case CK_LFUN:
        case CK_RFUN:
            MOD_TAP_HANDLER();
            return false;

        case CK_MOCK_TOGGLE:
            if (record->event.pressed) {
                g_flags.mocking = !g_flags.mocking;
            }
            return false;

        case CK_TD_TOGGLE:
            if (record->event.pressed) {
                g_flags.tapdance = !g_flags.tapdance;
            }
            return false;

        case CK_GLOBE_TOGGLE:
            if (record->event.pressed) {
                g_flags.mac = !g_flags.mac;
            }
            return false;
    }

    return true;
}


/* ---------- Tap Dance ---------- */

static bool process_safety_tapdance(tap_dance_state_t *state) {
    if (state->count >= 3) {
        reset_tap_dance(state);
        return true;
    }
    return false;
}

static void dance_safety_reset(tap_dance_state_t *state, void *user_data) {
    if (process_safety_tapdance(state)) reset_keyboard();
}

static void dance_safety_eeprom(tap_dance_state_t *state, void *user_data) {
    if (process_safety_tapdance(state)) eeconfig_init();
}

static void dance_esc_caps_altf4(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1: tap_code(KC_ESC); break;
        case 2: tap_code(KC_CAPS); break;
        case 3:
            register_code(KC_LALT);
            tap_code(KC_F4);
            unregister_code(KC_LALT);
            break;
    }
}


/* ---------- LCD Screens ---------- */
#ifdef ST7565_ENABLE


/* Colours to match Scrabble keycaps */
#define COLOUR_TAN 0xeee0c1
#define COLOUR_PINK 0xffc0b4
#define COLOUR_LIGHT_BLUE 0xd2e3e2
#define COLOUR_DARK_BLUE 0x70c9e3
#define COLOUR_RED 0xff9e80

#define EXPAND_HEX(hex) \
    (((((hex) >> 16) & 0xFF) * 257)), \
    (((((hex) >> 8)  & 0xFF) * 257)), \
    ((((hex)         & 0xFF) * 257))

#define HAND_ANIM_DELAY_MS 200

#define BONGO_CAT_WIDTH 6
#define BONGO_CAT_HEIGHT 3
#define ANIM_SPEED_MS 100

static char bongo_cat_buffer[BONGO_CAT_HEIGHT][BONGO_CAT_WIDTH + 1] = {
    {0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x00},
    {0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0x00},
    {0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0x00}
};

static uint16_t left_hand_timer = 0;
static uint16_t right_hand_timer = 0;
static bool left_hand_down = false;
static bool right_hand_down = false;

static void set_lcd_color_for_layer(layer_state_t state) {
    switch (get_highest_layer(state)) {

    case FUNCTION:
        ergodox_infinity_lcd_color(EXPAND_HEX(COLOUR_RED));
        break;
    case SPECIAL:
        ergodox_infinity_lcd_color(EXPAND_HEX(COLOUR_DARK_BLUE));
        break;
    case NUMPAD:
        ergodox_infinity_lcd_color(EXPAND_HEX(COLOUR_PINK));
        break;
    case BASE:
    default:
        ergodox_infinity_lcd_color(EXPAND_HEX(COLOUR_TAN));
        break;
    }
}

void matrix_init_user(void) {
    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();

    set_lcd_color_for_layer(layer_state);
}


void matrix_scan_user(void) {};


void animate_hand_left(bool is_up)
{
    if (is_up) {
        bongo_cat_buffer[1][0] = 0xA0;
        bongo_cat_buffer[1][1] = 0xA1;
        bongo_cat_buffer[2][0] = 0xC1;
        bongo_cat_buffer[2][1] = 0xC2;
    }
    else {
        bongo_cat_buffer[1][0] = 0x86;
        bongo_cat_buffer[1][1] = 0x87;
        bongo_cat_buffer[2][0] = 0xA6;
        bongo_cat_buffer[2][1] = 0xA7;
    }
}

void animate_hand_right(bool is_up)
{
    if (is_up) {
        bongo_cat_buffer[1][3] = 0xA3;
        bongo_cat_buffer[1][4] = 0xA4;
        bongo_cat_buffer[2][4] = 0xC3;
        bongo_cat_buffer[2][4] = 0xC4;
    }
    else {
        bongo_cat_buffer[1][3] = 0x88;
        bongo_cat_buffer[1][4] = 0x89;
        bongo_cat_buffer[2][4] = 0xA8;
        bongo_cat_buffer[2][4] = 0xA9;
    }
}

static void trigger_hand_animation(bool is_left) {
    if (is_left) {
        left_hand_timer = timer_read();
        left_hand_down = true;
        animate_hand_left(false);
    } else {
        right_hand_timer = timer_read();
        right_hand_down = true;
        animate_hand_right(false);
    }
}

static void update_bongo_cat(void) {
    if (left_hand_down && timer_elapsed(left_hand_timer) >= HAND_ANIM_DELAY_MS) {
        animate_hand_left(true);
        left_hand_down = false;
    }
    if (right_hand_down && timer_elapsed(right_hand_timer) >= HAND_ANIM_DELAY_MS) {
        animate_hand_right(true);
        right_hand_down = false;
    }
}

void write_bongo_cat(uint8_t col, uint8_t line) {
    for (uint8_t i = 0; i < BONGO_CAT_HEIGHT; i++) {
        st7565_set_cursor(col, line + i);
        st7565_write(bongo_cat_buffer[i], false);
    }
}

void write_lighting_screen(void) {
    st7565_clear();

    char buffer[22];

    sprintf((char *)buffer, "Brightness: %3d\n", led_matrix_get_val());
    st7565_write_P(buffer, false);

    sprintf((char *)buffer, "Speed: %3d\n", led_matrix_get_speed());
    st7565_write_P(buffer, false);

    const char* effect_name = led_matrix_get_mode_name(led_matrix_get_mode());
    st7565_write_P(effect_name, false);
}

void write_status_screen(void) {
    // Host Keyboard Layer Status
    switch (get_highest_layer(layer_state)) {
        case BASE:
            st7565_write_P(PSTR("Default\n"), false);
            break;
        case FUNCTION:
            st7565_write_P(PSTR("Function\n"), false);
            break;
        case SPECIAL:
            st7565_write_P(PSTR("Special\n"), false);
            break;
        case NUMPAD:
            st7565_write_P(PSTR("Numpad\n"), false);
            break;
        default:
            st7565_write_ln_P(PSTR("Undefined\n"), false);
    }

    // Host Keyboard LED Status
    led_t led_state = host_keyboard_led_state();
    st7565_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
    st7565_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
    st7565_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);
    st7565_write_P("\n", false);

    st7565_write_P(g_flags.tapdance ? PSTR("TAP ") : PSTR("    "), false);
    st7565_write_P(g_flags.mocking ? PSTR("MCK ") : PSTR("    "), false);
    st7565_write_P(g_flags.mac ? PSTR("MAC ") : PSTR("    "), false);
    st7565_write_P("\n", false);

    static const char ic_logo[] = {0xC6, 0xC7};
    st7565_write_P(ic_logo, false);
    st7565_write_P(" Infinity Ergodox\n", false);

    if (is_keyboard_master())
    {
        write_bongo_cat(15, 0);
    }
}

void st7565_task_user(void) {
    set_lcd_color_for_layer(layer_state);
    update_bongo_cat();

    if (is_keyboard_master()) {
        write_status_screen();
    } else {
        if (get_highest_layer(layer_state) == SPECIAL) {
            write_lighting_screen();
        } else {
            write_status_screen();
        }
    }
}

#endif
