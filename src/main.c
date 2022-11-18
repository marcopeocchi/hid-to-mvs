#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp/board.h"
#include "tusb.h"

#include "board_layout.h"
#include "board_output.h"

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
  board_output_init();
  
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
// Blinking Task
//--------------------------------------------------------------------+
void led_blinking_task(void)
{
  const uint32_t interval_ms = 1000;
  static uint32_t start_ms = 0;

  static bool led_state = false;

  if (board_millis() - start_ms < interval_ms) return; 
  start_ms += interval_ms;

  board_led_write(led_state);
  led_state = 1 - led_state;
}
