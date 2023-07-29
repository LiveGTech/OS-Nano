# LiveG OS Nano
The edition of the LiveG OS family that runs on low-power, embedded devices.

Licensed by the [LiveG Open-Source Licence](LICENCE.md).

## Prerequisites
Before building LiveG OS Nano, you'll need to install the PlatformIO SDK. [Follow the instructions in the PlatformIO docs](https://docs.platformio.org/en/latest/core/installation/methods/installer-script.html) to install PlatformIO.

## Building
To build LiveG OS Nano and flash it to a target device, run the following:

```bash
platformio run --target upload --environment um_feathers3
```