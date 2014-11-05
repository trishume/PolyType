# PolyType Firmware

This the firmware for my custom keyboard. It supports macros, layout switching, media keys and a screen.

It runs on the Teensy 3.1 using Teensyduino. Much of it is specific to my hardware but it wouldn't take much effort
to get it to work for other hardware.

[More info on my keyboard](http://thume.ca/2014/09/08/creating-a-keyboard-1-hardware/)

## Architecture

Unlike most other firmware, this one uses a pipeline architecture for fancy stream processing functionality.

This is so that implementing fanciness like chording can be done easily and can be enabled/disabled without trouble.
