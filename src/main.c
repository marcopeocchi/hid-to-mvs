#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp/board.h"
#include "tusb.h"

#include "jamma_layout.h"

#include "pico/stdlib.h"

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+
void led_blinking_task(void);

extern void cdc_task(void);
extern void hid_app_task(void);

/*------------- MAIN -------------*/
int main(void)
{
  board_init();

  // enable pins

  gpio_init(DPAD_UP_PIN);
  gpio_init(DPAD_DOWN_PIN);
  gpio_init(DPAD_RIGHT_PIN);
  gpio_init(DPAD_LEFT_PIN);

  gpio_init(A_PIN);
  gpio_init(B_PIN);

  gpio_init(COIN_PIN);
  gpio_init(START_PIN);

  // set pins as outputs

  gpio_set_dir(DPAD_UP_PIN,    GPIO_OUT);
  gpio_set_dir(DPAD_DOWN_PIN,  GPIO_OUT);
  gpio_set_dir(DPAD_RIGHT_PIN, GPIO_OUT);
  gpio_set_dir(DPAD_LEFT_PIN,  GPIO_OUT);

  gpio_set_dir(A_PIN, GPIO_OUT);
  gpio_set_dir(B_PIN, GPIO_OUT);

  gpio_set_dir(COIN_PIN, GPIO_OUT);
  gpio_set_dir(START_PIN, GPIO_OUT);

  // init all pins to be high as MVS JAMMA reads when LOW
  
  gpio_put(DPAD_UP_PIN,    0);
  gpio_put(DPAD_DOWN_PIN,  0);
  gpio_put(DPAD_RIGHT_PIN, 0);
  gpio_put(DPAD_LEFT_PIN,  0);

  gpio_put(A_PIN, 0);
  gpio_put(B_PIN, 0);

  gpio_put(COIN_PIN,  0);
  gpio_put(START_PIN, 0);

  // init USB

  tusb_init();

  while (1)
  {
    // tinyusb host task
    tuh_task();
    led_blinking_task();

#if CFG_TUH_CDC
    cdc_task();
#endif

#if CFG_TUH_HID
    hid_app_task();
#endif
  }

  return 0;
}

//--------------------------------------------------------------------+
// TinyUSB Callbacks
//--------------------------------------------------------------------+

//--------------------------------------------------------------------+
// Blinking Task
//--------------------------------------------------------------------+
void led_blinking_task(void)
{
  const uint32_t interval_ms = 1000;
  static uint32_t start_ms = 0;

  static bool led_state = false;

  // Blink every interval ms
  if ( board_millis() - start_ms < interval_ms) return; // not enough time
  start_ms += interval_ms;

  board_led_write(led_state);
  led_state = 1 - led_state; // toggle
}
