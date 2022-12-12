#include "bsp/board.h"
#include "tusb.h"

#include "board_layout.h"

#include "pico/stdlib.h"
#include "hid_app.h"

// USB hid report typedef
typedef struct TU_ATTR_PACKED
{
  uint8_t x, y, z, rz; // joystick axis

  struct
  {
    uint8_t dpad : 4;  // N-W-S-E
    uint8_t west : 1;  // X
    uint8_t south : 1; // A
    uint8_t east : 1;  // B
    uint8_t north : 1; // Y
  };

  struct
  {
    uint8_t l1 : 1;
    uint8_t r1 : 1;
    uint8_t l2 : 1;
    uint8_t r2 : 1;
    uint8_t start : 1;
    uint8_t option : 1;
    uint8_t l3 : 1;
    uint8_t r3 : 1;
  };

  struct
  {
    uint8_t meta : 1;
    uint8_t touch : 1;   // track pad click
    uint8_t counter : 6; // +1 each report
  };

} controller_report_t;

//--------------------------------------------------------------------+
// MACRO TYPEDEF CONSTANT ENUM DECLARATION
//--------------------------------------------------------------------+

void hid_app_task(void) {}

//--------------------------------------------------------------------+
// TinyUSB Callbacks
//--------------------------------------------------------------------+

// Invoked when device with hid interface is mounted
// Report descriptor is also available for use. tuh_hid_parse_report_descriptor()
// can be used to parse common/simple enough descriptor.
// Note: if report descriptor length > CFG_TUH_ENUMERATION_BUFSIZE, it will be skipped
// therefore report_desc = NULL, desc_len = 0
void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const *desc_report, uint16_t desc_len)
{
  (void)desc_report;
  (void)desc_len;
  uint16_t vid, pid;
  tuh_vid_pid_get(dev_addr, &vid, &pid);

  printf("Device address = %d, instance = %d is mounted\r\n", dev_addr, instance);
  printf("VID = %04x, PID = %04x\r\n", vid, pid);

  if (!tuh_hid_receive_report(dev_addr, instance))
  {
    printf("Error: cannot request to receive report\r\n");
  }
}

// Invoked when device with hid interface is un-mounted
void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance)
{
  printf("HID device address = %d, instance = %d is unmounted\r\n", dev_addr, instance);
}

// check if different than 2
bool diff_than_2(uint8_t x, uint8_t y)
{
  return (x - y > 2) || (y - x > 2);
}

// check if 2 reports are different enough
bool diff_report(controller_report_t const *rpt1, controller_report_t const *rpt2)
{
  bool result;

  // x, y, z, rz must different than 2 to be counted
  result = diff_than_2(rpt1->x, rpt2->x) || diff_than_2(rpt1->y, rpt2->y) ||
           diff_than_2(rpt1->z, rpt2->z) || diff_than_2(rpt1->rz, rpt2->rz);

  // check the reset with mem compare
  result |= memcmp(&rpt1->rz + 1, &rpt2->rz + 1, sizeof(controller_report_t) - 4);

  return result;
}

// process HID report
void process_report(uint8_t const *report, uint16_t len)
{
  // previous report used to compare for changes
  static controller_report_t prev_report = {0};

  uint8_t const report_id = report[0];
  report++;
  len--;

  // all buttons state is stored in ID 1
  if (report_id == 1)
  {
    controller_report_t dev_report;
    memcpy(&dev_report, report, sizeof(dev_report));

    prev_report.counter = dev_report.counter;

    if (diff_report(&prev_report, &dev_report))
    {
      stick_handler(dev_report.rz, dev_report.z, 10);

      dev_dpad_handler(dev_report.dpad);

      board_toggle_output(Y_PIN, dev_report.west);
      board_toggle_output(B_PIN, dev_report.south);
      board_toggle_output(A_PIN, dev_report.east);
      board_toggle_output(X_PIN, dev_report.north);

      board_toggle_output(START_PIN, dev_report.start);
      board_toggle_output(SELECT_PIN, dev_report.meta);
      board_toggle_output(SELECT_PIN, dev_report.option);
    }

    prev_report = dev_report;
  }
}

// Invoked when received report from device via interrupt endpoint
void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance,
                                uint8_t const *report, uint16_t len)
{
  process_report(report, len);

  // continue to request to receive report
  if (!tuh_hid_receive_report(dev_addr, instance))
  {
    printf("Error: cannot request to receive report\r\n");
  }
}

inline void board_toggle_output(uint8_t pin, uint8_t enabled)
{
  enabled ? gpio_put(pin, LOW) : gpio_put(pin, HIGH);
}

static inline void dev_dpad_handler(uint8_t dpad_value)
{
  switch (dpad_value)
  {
  case 0:
    // DPAD North
    gpio_put(DPAD_UP_PIN, LOW);
    gpio_put(DPAD_RIGHT_PIN, HIGH);
    gpio_put(DPAD_DOWN_PIN, HIGH);
    gpio_put(DPAD_LEFT_PIN, HIGH);
    break;
  case 1:
    // DPAD North-East
    gpio_put(DPAD_UP_PIN, LOW);
    gpio_put(DPAD_RIGHT_PIN, LOW);
    gpio_put(DPAD_DOWN_PIN, HIGH);
    gpio_put(DPAD_LEFT_PIN, HIGH);
    break;
  case 2:
    // DPAD East
    gpio_put(DPAD_UP_PIN, HIGH);
    gpio_put(DPAD_RIGHT_PIN, LOW);
    gpio_put(DPAD_DOWN_PIN, HIGH);
    gpio_put(DPAD_LEFT_PIN, HIGH);
    break;
  case 3:
    // DPAD South-East
    gpio_put(DPAD_UP_PIN, HIGH);
    gpio_put(DPAD_RIGHT_PIN, LOW);
    gpio_put(DPAD_DOWN_PIN, LOW);
    gpio_put(DPAD_LEFT_PIN, HIGH);
    break;
  case 4:
    // DPAD South
    gpio_put(DPAD_UP_PIN, HIGH);
    gpio_put(DPAD_RIGHT_PIN, HIGH);
    gpio_put(DPAD_DOWN_PIN, LOW);
    gpio_put(DPAD_LEFT_PIN, HIGH);
    break;
  case 5:
    // DPAD South-West
    gpio_put(DPAD_UP_PIN, HIGH);
    gpio_put(DPAD_RIGHT_PIN, HIGH);
    gpio_put(DPAD_DOWN_PIN, LOW);
    gpio_put(DPAD_LEFT_PIN, LOW);
    break;
  case 6:
    // DPAD West
    gpio_put(DPAD_UP_PIN, HIGH);
    gpio_put(DPAD_RIGHT_PIN, HIGH);
    gpio_put(DPAD_DOWN_PIN, HIGH);
    gpio_put(DPAD_LEFT_PIN, LOW);
    break;
  case 7:
    // DPAD North-West
    gpio_put(DPAD_UP_PIN, LOW);
    gpio_put(DPAD_RIGHT_PIN, HIGH);
    gpio_put(DPAD_DOWN_PIN, HIGH);
    gpio_put(DPAD_LEFT_PIN, LOW);
    break;
  case 8:
    // DPAD Released
    gpio_put(DPAD_UP_PIN, HIGH);
    gpio_put(DPAD_RIGHT_PIN, HIGH);
    gpio_put(DPAD_DOWN_PIN, HIGH);
    gpio_put(DPAD_LEFT_PIN, HIGH);
    break;
  default:
    break;
  }
}

static inline void stick_handler(uint8_t deg_value_y, uint8_t deg_value_z,
                                 uint8_t dead_zone)
{
  if (deg_value_y > (127 + dead_zone))
  {
    gpio_put(DPAD_DOWN_PIN, LOW);
  }
  if (deg_value_y < (127 - dead_zone))
  {
    gpio_put(DPAD_UP_PIN, LOW);
  }
  if (deg_value_y == 127)
  {
    gpio_put(DPAD_UP_PIN, HIGH);
    gpio_put(DPAD_DOWN_PIN, HIGH);
  }
  deg_value_z == 255
      ? gpio_put(DPAD_RIGHT_PIN, LOW)
      : gpio_put(DPAD_RIGHT_PIN, HIGH);
  deg_value_z == 0
      ? gpio_put(DPAD_LEFT_PIN, LOW)
      : gpio_put(DPAD_LEFT_PIN, HIGH);
}