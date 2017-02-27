# Getting started with Bootloader Blinky on mbed OS

This example shows how to build and use an application with a pre-built bootloader.
For steps on how to create the bootloader see [mbed-os-example-bootloader](https://github.com/ARMmbed/mbed-os-example-bootloader)

## Required hardware
* A supported board - [K64F](https://developer.mbed.org/platforms/ublox-EVK-ODIN-W2/), [Nucleo F429ZI](https://developer.mbed.org/platforms/ST-Nucleo-F429ZI/) or [K64F](https://developer.mbed.org/platforms/FRDM-K64F/)
* CI Test shield
* SD card

## Import the example application

From the command line, import the example:

```
mbed import mbed-os-example-bootloader-blinky
cd mbed-os-example-bootloader-blinky
```

## Setup application to be a bootloader

All supported boards mentioned above are set up to build as a bootloader image. To
add support for a new board the size of the bootloader must be specified in mbed_app.json:
```
    "target_overrides": {
        ...
        "NUCLEO_F429ZI": {
            "target.restrict_size": "0x20000"
        },
        ...
```

Note - ``restrict_size`` pads the build image to the requested size.


## Now compile

Invoke `mbed compile` specifying the name of your platform and your favorite toolchain (`GCC_ARM`, `ARM`, `IAR`). For example, for the ARM Compiler 5:

```
mbed compile -m NUCLEO_F429ZI -t ARM
```

Your PC may take a few minutes to compile your code. At the end you should get the following result:

```
Merging Regions:
  Filling region application with .\BUILD\NUCLEO_F429ZI\ARM\mbed-os-example-bootloader_application.bin
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

Image: .\BUILD\NUCLEO_F429ZI\ARM\mbed-os-example-bootloader.bin
```

## Next steps

If the build succeeds then you successfuly created a bootloader for your target.
The next step is to build an application which you can combine with your bootloader
to create a loadable image. A simple blinky application which makes use of this
bootloader can be found in the [mbed-os-example-bootloader-blinky](https://github.com/ARMmbed/mbed-os-example-bootloader-blinky)
project.

## Troubleshooting

1. Make sure `mbed-cli` is working correctly and its version is `>1.0.0`

 ```
 mbed --version
 ```

 If not, you can update it easily:

 ```
 pip install mbed-cli --upgrade
 ```

2. If using Keil MDK, make sure you have a license installed. [MDK-Lite](http://www.keil.com/arm/mdk.asp) has a 32KB restriction on code size.
