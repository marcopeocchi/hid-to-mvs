#include "bsp/board.h"
#include "tusb.h"

#include "pico/stdlib.h"

// basic functions: 4 buttons layout mapping

#define A_PIN 2           // Board: 4
#define B_PIN 3           // Board: 5
#define X_PIN 4           // Board: 6
#define Y_PIN 5           // Board: 7

// d-pad as analog stick mapping

#define DPAD_UP_PIN    6  // Board: 9
#define DPAD_DOWN_PIN  7  // Board: 10
#define DPAD_RIGHT_PIN 8  // Board: 11
#define DPAD_LEFT_PIN  9  // Board: 12

// extra functions: 6 buttons layout mapping

#define C_PIN 10          // Board: 14
#define Z_PIN 11          // Board: 15

// utility functions: coinup and start

#define COIN_PIN  12      // Board: 16
#define START_PIN 13      // Board: 17

// shorthand

#define HIGH 1
#define LOW  0

// Sony DS4 report layout detail https://www.psdevwiki.com/ps4/DS4-USB
typedef struct TU_ATTR_PACKED
{
  uint8_t x, y, z, rz; // joystick

  struct {
    uint8_t dpad     : 4; // 0x08
    uint8_t square   : 1; // west
    uint8_t cross    : 1; // south
    uint8_t circle   : 1; // east
    uint8_t triangle : 1; // north
  };

  struct {
    uint8_t l1     : 1;
    uint8_t r1     : 1;
    uint8_t l2     : 1;
    uint8_t r2     : 1;
    uint8_t share  : 1;
    uint8_t option : 1;
    uint8_t l3     : 1;
    uint8_t r3     : 1;
  };

  struct {
    uint8_t ps      : 1; 
    uint8_t tpad    : 1; // track pad click
    uint8_t counter : 6; // +1 each report
  };

} controller_report_t;

// check if device is Sony DualShock 4
static inline bool is_compatible(uint8_t dev_addr)
{
  uint16_t vid, pid;
  tuh_vid_pid_get(dev_addr, &vid, &pid);

  return ((vid == 0x054c && (pid == 0x09cc || pid == 0x05c4)) // Sony DualShock4 
           || (vid == 0x0f0d && pid == 0x005e)                // Hori FC4 
           || (vid == 0x0f0d && pid == 0x00ee)                // Hori PS4 Mini (PS4-099U) 
           || (vid == 0x1f4f && pid == 0x1002)                // ASW GG xrd controller
           || (vid == 0x0ca3 && pid == 0x0024)                // 8bit do m30 2.4G
           || (vid == 0x057e && pid == 0x2009)                // 8bit do m30 usb
         );
}

//--------------------------------------------------------------------+
// MACRO TYPEDEF CONSTANT ENUM DECLARATION
//--------------------------------------------------------------------+

void hid_app_task(void)
{
  // nothing to do
}

//--------------------------------------------------------------------+
// TinyUSB Callbacks
//--------------------------------------------------------------------+

// Invoked when device with hid interface is mounted
// Report descriptor is also available for use. tuh_hid_parse_report_descriptor()
// can be used to parse common/simple enough descriptor.
// Note: if report descriptor length > CFG_TUH_ENUMERATION_BUFSIZE, it will be skipped
// therefore report_desc = NULL, desc_len = 0
void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report, uint16_t desc_len)
{
  (void)desc_report;
  (void)desc_len;
  uint16_t vid, pid;
  tuh_vid_pid_get(dev_addr, &vid, &pid);

  printf("HID device address = %d, instance = %d is mounted\r\n", dev_addr, instance);
  printf("VID = %04x, PID = %04x\r\n", vid, pid);

  // Sony DualShock 4 [CUH-ZCT2x]
  if ( is_compatible(dev_addr) )
  {
    // request to receive report
    // tuh_hid_report_received_cb() will be invoked when report is available
    if ( !tuh_hid_receive_report(dev_addr, instance) )
    {
      printf("Error: cannot request to receive report\r\n");
    }
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
bool diff_report(controller_report_t const* rpt1, controller_report_t const* rpt2)
{
  bool result;

  // x, y, z, rz must different than 2 to be counted
  result = diff_than_2(rpt1->x, rpt2->x) || diff_than_2(rpt1->y , rpt2->y ) ||
           diff_than_2(rpt1->z, rpt2->z) || diff_than_2(rpt1->rz, rpt2->rz);

  // check the reset with mem compare
  result |= memcmp(&rpt1->rz + 1, &rpt2->rz + 1, sizeof(controller_report_t)-4);

  return result;
}

void process_compatible(uint8_t const* report, uint16_t len)
{
  const char* dpad_str[] = { "N", "NE", "E", "SE", "S", "SW", "W", "NW", "none" };

  // previous report used to compare for changes
  static controller_report_t prev_report = { 0 };

  uint8_t const report_id = report[0];
  report++;
  len--;

  // all buttons state is stored in ID 1
  if (report_id == 1)
  {
    controller_report_t dev_report;
    memcpy(&dev_report, report, sizeof(dev_report));

    // counter is +1, assign to make it easier to compare 2 report
    prev_report.counter = dev_report.counter;

    // only print if changes since it is polled ~ 5ms
    // Since count+1 after each report and  x, y, z, rz fluctuate within 1 or 2
    // We need more than memcmp to check if report is different enough
    if (diff_report(&prev_report, &dev_report))
    {
      dev_report.rz == 0    ? gpio_put(DPAD_UP_PIN, HIGH)    : gpio_put(DPAD_UP_PIN, LOW);
      dev_report.rz == 0xff ? gpio_put(DPAD_DOWN_PIN, HIGH)  : gpio_put(DPAD_DOWN_PIN, LOW);
      dev_report.z  == 0xff ? gpio_put(DPAD_RIGHT_PIN, HIGH) : gpio_put(DPAD_RIGHT_PIN, LOW);
      dev_report.z  == 0    ? gpio_put(DPAD_LEFT_PIN, HIGH)  : gpio_put(DPAD_LEFT_PIN, LOW);

      dev_report.circle   ? gpio_put(A_PIN, HIGH) : gpio_put(A_PIN, LOW);
      dev_report.cross    ? gpio_put(B_PIN, HIGH) : gpio_put(B_PIN, LOW);
      dev_report.triangle ? gpio_put(X_PIN, HIGH) : gpio_put(X_PIN, LOW);
      dev_report.square   ? gpio_put(Y_PIN, HIGH) : gpio_put(Y_PIN, LOW);

      dev_report.l1 ? gpio_put(Z_PIN, HIGH) : gpio_put(Z_PIN, LOW);
      dev_report.r1 ? gpio_put(C_PIN, HIGH) : gpio_put(C_PIN, LOW);

      dev_report.share  ? gpio_put(START_PIN, HIGH) : gpio_put(START_PIN, LOW);
      dev_report.option ? gpio_put(COIN_PIN, HIGH)  : gpio_put(COIN_PIN, LOW);
    }

    prev_report = dev_report;
  }
}

// Invoked when received report from device via interrupt endpoint
void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len)
{
  if ( is_compatible(dev_addr) )
  {
    process_compatible(report, len);
  }

  // continue to request to receive report
  if ( !tuh_hid_receive_report(dev_addr, instance) )
  {
    printf("Error: cannot request to receive report\r\n");
  }
}
