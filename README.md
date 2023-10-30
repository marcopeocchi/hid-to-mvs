# pico-hid-mvs

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

![Schematic_rp2040-ng_2023-10-30](https://github.com/marcopeocchi/hid-to-mvs/assets/35533749/6a9f3881-a418-4e64-8d32-ef382b46b315)

## Tested Controllers
- 8BitDo Ultimate C 2.4G
- 8Bitdo USB Arcade Stick (2.4G Mode)
- 8Bitdo NEOGEO Wireless Controller
- 8Bitdo M30 2.4G
