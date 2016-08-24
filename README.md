# General

Basic ATTiny85 firmware to wake MCU up from sleep via external interrupt based on photocell + MOSFET setup (see `docs/` folder for the sketch).

Project template is generated with the following command:

```
platformio init --ide clion --board attiny85
```

Source code is uploaded to MCU via Arduino UNO ISP, see `platformio.ini`:

```
[env:attiny85]
board_f_cpu = 8000000L
platform = atmelavr
board = attiny85
upload_protocol = stk500v1
upload_flags = -P$UPLOAD_PORT -b$UPLOAD_SPEED

# Arduino ISP programmer settings
upload_port = /dev/ttyACM0
upload_speed = 19200
```