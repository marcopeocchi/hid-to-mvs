// toggle a pin to the provided state
void board_toggle_output(uint8_t pin, uint8_t enabled);

// handle "8-way" dpad mode (same behaviour as joystick)
void dev_dpad_handler(uint8_t dpad_value);