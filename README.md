# MVS-to-USB
A NeoGeo MVS JAMMA to USB converter/adapter.

Connect any compatible HID joystick/gamepad to the Pi Pico micro-USB port, each report will be converted to a JAMMA logic level.

## 2 Buttons layout (Analog/DPAD + A,B)
![sketch](./res/mvs-usb-2btn_bb.png)

## 4 and 6 buttons layout (Analog/DPAD + A,B,X,Y,[L,R])

Needs a second Darlington array IC  

![sketch](./res/mvs-usb-4btn_bb.png)