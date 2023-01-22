#include "board_layout.h"
#include "pico/stdlib.h"

void board_output_init()
{
  uint8_t output[11] = {
      DPAD_UP_PIN,
      DPAD_DOWN_PIN,
      DPAD_RIGHT_PIN,
      DPAD_LEFT_PIN,
      A_PIN,
      B_PIN,
      X_PIN,
      Y_PIN,
      START_PIN,
      SELECT_PIN,
      ALT_6_PIN,
  };

  for (uint8_t i = 0; i < 11; i++)
  {
    gpio_init(output[i]);
    gpio_set_dir(output[i], GPIO_OUT);
    gpio_put(output[i], HIGH);
  }
}