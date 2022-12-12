//-----------------------------+
// HID app function PROTOTYPES
//-----------------------------+

// toggle a pin to the provided state
static inline void board_toggle_output(uint8_t pin, uint8_t enabled);

// handle "8-way" dpad mode (same behaviour as joystick)
static inline void dev_dpad_handler(uint8_t dpad_value);

// handle joystick as "8-way" dpad
static inline void stick_handler(uint8_t deg_value_y, uint8_t deg_value_z,
                                 uint8_t dead_zone);