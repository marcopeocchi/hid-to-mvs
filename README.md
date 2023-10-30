# pico-hid-mvs

`pico-hid-mvs` is a budget-oriented adaptor between any supergun and a USB HID controller.  
It's based on the Rapsberry Pi pico and designed to be a plug and play solution.

It uses PIO for controlling the USB system and latency should be always in range of 1-2 ms.

Compiler: `arm-gcc-none-eabi`

## Setup
```
export TINYUSB_PATH=...
export PICO_SDK_PATH=...
```

## Build
```
mkdir build
cd build
cmake -DFAMILY=rp2040 ..
```

Under the `hw` directory there is all the necessary to produce and assembly the device.

![Schematic_rp2040-ng_2023-10-30](https://github.com/marcopeocchi/hid-to-mvs/assets/35533749/b6dc0a2d-1c49-4163-ab82-408159b6241b)

## Tested Controllers
- 8BitDo Ultimate C 2.4G
- 8Bitdo USB Arcade Stick (2.4G Mode)
- 8Bitdo NEOGEO Wireless Controller
- 8Bitdo M30 2.4G
