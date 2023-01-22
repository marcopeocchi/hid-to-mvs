#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef CFG_TUSB_MCU
#error CFG_TUSB_MCU must be defined
#endif

#define CFG_TUSB_RHPORT0_MODE OPT_MODE_HOST

#ifndef CFG_TUSB_OS
#define CFG_TUSB_OS OPT_OS_NONE
#endif

#define CFG_TUSB_DEBUG 0

#ifndef CFG_TUSB_MEM_SECTION
#define CFG_TUSB_MEM_SECTION
#endif

#ifndef CFG_TUSB_MEM_ALIGN
#define CFG_TUSB_MEM_ALIGN __attribute__((aligned(4)))
#endif

#define CFG_TUH_ENUMERATION_BUFSIZE 256

#define CFG_TUH_HUB 0
#define CFG_TUH_CDC 0
#define CFG_TUH_HID 4
#define CFG_TUH_MSC 0
#define CFG_TUH_VENDOR 0

#define CFG_TUH_DEVICE_MAX (CFG_TUH_HUB ? 4 : 1)

#define CFG_TUH_HID_EP_BUFSIZE 64

#ifdef __cplusplus
}
#endif

#endif
