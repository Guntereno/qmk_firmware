#include "simple_visualizer.h"

enum layer_indicator
{
  LAYER_NUMPAD = 1u << 1,
  LAYER_FUNCTION = 1u << 2,
  LAYER_SPECIAL = 1u << 3
};

// This function should be implemented by the keymap visualizer
// Don't change anything else than state->target_lcd_color and state->layer_text as that's the only thing
// that the simple_visualizer assumes that you are updating
// Also make sure that the buffer passed to state->layer_text remains valid until the previous animation is
// stopped. This can be done by either double buffering it or by using constant strings
static void get_visualizer_layer_and_color(visualizer_state_t* state)
{
    uint8_t saturation = 60;
    if (state->status.leds & (1u << USB_LED_CAPS_LOCK)) {
        saturation = 255;
    }

    if ((state->status.layer & LAYER_SPECIAL) != 0)
    {
        state->target_lcd_color = LCD_COLOR(0, saturation, 255);
        state->layer_text = "Special";
    }
    else if ((state->status.layer & LAYER_FUNCTION) != 0)
    {
        state->target_lcd_color = LCD_COLOR(28, saturation, 255);
        state->layer_text = "Function";
    }
    else if ((state->status.layer & LAYER_NUMPAD) != 0)
    {
        state->target_lcd_color = LCD_COLOR(118, saturation, 255);
        state->layer_text = "Numpad";
    }
    else
    {
        state->target_lcd_color = LCD_COLOR(168, saturation, saturation);
        state->layer_text = "Default";
    }
}
