# Getting started with bootloader on mbed OS

This example shows how to create a bootloader.

To read more about the bootloader, please visit [bootloader tutorial](https://docs.mbed.com/docs/mbed-os-handbook/en/latest/advanced/bootloader/).

## Required hardware
* A supported target - [u-blox EVK-ODIN-W2](https://developer.mbed.org/platforms/ublox-EVK-ODIN-W2/), [Nucleo F429ZI](https://developer.mbed.org/platforms/ST-Nucleo-F429ZI/) or [K64F](https://developer.mbed.org/platforms/FRDM-K64F/).
* SD card.
* (If no onboard SD card port is present) Shield or breakout board connecting the SD card to the SPI pins.

## Import the example application

From the command-line, import the example:

```
mbed import mbed-os-example-bootloader
cd mbed-os-example-bootloader
```

## Connecting the SD card
This example is configured to use the **onboard SD card port** for the K64F and the UBLOX_EVK_ODIN_W2.

For other targets, the following Arduino form-factor SPI pins are used by default:
- `D11` - `MOSI`
- `D12` - `MISO`
- `D13` - `SCK`
- `D10` - `CS`


To use different SPI pins, you will need to modify the `mbed_app.json` file. Add a key for your target to the `target_overrides` section with the following data (replace `<TARGET_NAME>` with your target's name):

```
{
    ...
    "target_overrides": {
        ...
        "<TARGET_NAME>": {
            "sd_card_mosi": "<MOSI_PIN>",
            "sd_card_miso": "<MISO_PIN>",
            "sd_card_sck": "<SCK_PIN>",
            "sd_card_cs": "<CS_PIN>"
        }
        ...
    }
    ...
}
```

## Preparing the bootloader

All supported targets mentioned above are set up to build as a bootloader image. To add support for a new target, you must specify the size of the bootloader.

To do this, set the target value `restrict_size` to the maximum bootloader size in `mbed_app.json` (replace `<TARGET_NAME>` with your target name):

```
    "target_overrides": {
        ...
        "<TARGET_NAME>": {
            "target.restrict_size": "0x20000"
        },
        ...
```

<span class="tips">**Note:** `restrict_size` pads the build image to the requested size.</span>

## Building

Invoke `mbed compile`, and specify the name of your target and your favorite toolchain (`GCC_ARM`, `ARM`, `IAR`). For example, for the ARM Compiler 5:

```
mbed compile -m <TARGET_NAME> -t ARM
```

Your PC may take a few minutes to compile your code. At the end, you see the following result:

```
Merging Regions:
  Filling region application with .\BUILD\<TARGET_NAME>\ARM\mbed-os-example-bootloader_application.bin
  Padding region application with 0x11420 bytes
Space used after regions merged: 0x20000
+-----------------------------+-------+-------+-------+
| Module                      | .text | .data |  .bss |
+-----------------------------+-------+-------+-------+
| Misc                        | 18481 |    24 |  1268 |
| drivers                     |  2132 |     8 |   184 |
| features/FEATURE_LWIP       |    46 |     0 | 12560 |
| features/filesystem         | 12756 |    12 |   540 |
| hal                         |   414 |     8 |     0 |
| hal/TARGET_FLASH_CMSIS_ALGO |   520 |    28 |     0 |
| platform                    |  2759 |    32 |   380 |
| rtos                        |   144 |     8 |     0 |
| rtos/rtx                    |  6954 |   100 |  8396 |
| targets/TARGET_STM          | 15560 |   568 |   736 |
| Subtotals                   | 59766 |   788 | 24064 |
+-----------------------------+-------+-------+-------+
Allocated Heap: unknown
Allocated Stack: unknown
Total Static RAM memory (data + bss): 24852 bytes
Total RAM memory (data + bss + heap + stack): 24852 bytes
Total Flash memory (text + data + misc): 60554 bytes

Image: .\BUILD\<TARGET_NAME>\ARM\mbed-os-example-bootloader.bin
```

After the build completes, the build system saves two binary images in the `BUILD/<TARGET_NAME>/<TOOLCHAIN_NAME>` directory. The original unpadded image is saved with the name `<project-name>_application.bin`. The padded bootloader image (used for combining with applications) is saved with the name `<project-name>.bin`.

In this example, the update binary for the bootloader is defined in `main.cpp`  as `mbed-os-example-bootloader-blinky_application.bin` (specified under the macro `UPDATE_FILE`). The bootloader will look for this file in the root of the SD card, flash it to memory, and then jump to the application.

## Set up application to use bootloader

The next step is to build an application you can combine with your bootloader to create a loadable image. Any application works; however, you may wish to start with the [Blinky example](https://github.com/ARMmbed/mbed-os-example-blinky).

1. Instruct the build system to use the newly created padded bootloader image (named `<project-name>.bin` in the previous section). To do this, create a `target_overrides` section in your application's `mbed_app.json` file and add a key for your target (see `<TARGET_NAME>` in the example below). If `mbed_app.json` does not already exist in your application, create one.

2. Set the target value `bootloader_img` in `mbed_app.json` to the file path of the padded bootloader image . For the below example, copy the padded bootloader image to the project directory.

```
{
    "target_overrides": {
        ...
        "<TARGET_NAME>": {
            "target.bootloader_img": "<path to padded bootloader binary>"
        },
        ...
}
```

2. Invoke `mbed compile`, and specify the name of your target and the toolchain you are using (`GCC_ARM`, `ARM`, `IAR`). For example, for the ARM Compiler 5:

    ```
    mbed compile -m <TARGET_NAME> -t ARM
    ```

After the build completes, the build system saves two binary images in the `BUILD/<TARGET_NAME>/<TOOLCHAIN_NAME>` directory. The image that contains the combined bootloader and application (suitable for flashing to the device's ROM directly) is named `<project-name>.bin`. Use the image saved with the name `<project-name>_application.bin` as the update binary (the one you place on the SD card).

### Program bootloader and application

1. Connect your mbed device to the computer over USB.
1. Copy the combined bootloader and application binary file (named `<project-name>.bin` in the previous section) to the mbed device.
1. Press the reset button to start the program.

### Program application using SD card

<span class="tips">**Note:** You must first flash the combined bootloader and application image or just the bootloader image (padded or unpadded) before proceeding with the following steps.</span>

1. Connect the SD card to your computer.
1. Copy the update application binary (named `<project-name>_application.bin` in the previous section) to the root of the SD card.
1. Remove the SD card from your PC, and connect it to your mbed (see the above section [Setting up the SD card](#setting-up-the-sd-card))
1. Press the reset button to start the firmware update.

If a terminal is open, the following should print out at a baud rate of 9600:

```
Firmware update found
Starting application
```

## Troubleshooting

1. Make sure `mbed-cli` is working correctly and its version is `>1.0.0`.

    ```
    mbed --version
    ```

 If not, you can update it:

    ```
    pip install mbed-cli --upgrade
    ```

2. If using Keil MDK, make sure you have a license installed. [MDK-Lite](http://www.keil.com/arm/mdk.asp) has a 32 KB restriction on code size.
