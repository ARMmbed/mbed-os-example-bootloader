# Getting started with a bootloader example on mbed OS

This example shows how to create a bootloader. To read more about creating a bootloader, please visit [the bootloader tutorial](https://docs.mbed.com/docs/mbed-os-handbook/en/latest/advanced/bootloader/).

## Required hardware
* A supported board - [u-blox EVK-ODIN-W2](https://developer.mbed.org/platforms/ublox-EVK-ODIN-W2/), [Nucleo F429ZI](https://developer.mbed.org/platforms/ST-Nucleo-F429ZI/) or [K64F](https://developer.mbed.org/platforms/FRDM-K64F/).
* CI test shield or on board SD card socket.
* SD card.

## Import the example bootloader application

```
mbed import mbed-os-example-bootloader
cd mbed-os-example-bootloader
```

## Set up the application to be a bootloader

Running the example with a board other than one listed above will require local modifications to the mbed_app.json file for your board. To determine the size of the bootloader you can compile the example and then specify the next sectors starting address which is greater than the example size.

Note - `restrict_size` pads the build image to the requested size.

To do this, set the target (replace `<TARGET_NAME>` with your target name) value `restrict_size` in mbed_app.json:

```
...    
    "target_overrides": {
        "<TARGET_NAME>": {
            "target.restrict_size": "0x20000"
        }
    }
...
```

```
mbed compile -m <TARGET_NAME> -t <toolchain>
...
Merging Regions:
  Filling region application with .\BUILD\<TARGET_NAME>\ARM\mbed-os-example-bootloader_application.bin
  Padding region application with 0x11420 bytes
Space used after regions merged: 0x20000
...
Image: .\BUILD\<TARGET_NAME>\ARM\mbed-os-example-bootloader.bin
```

This creates two binary files. The original unmodified image is in the output directory, <project-name>_application.bin, and the padded bootloader image is <project-name>.bin.

Copy the <project-name>.bin image to your board and without an SD card present or application file loaded on the SD card you should see an output similar to:
```
???What does this example print out???
```

## Set up any application to use the example bootloader

The next step is to build an application you can load with your bootloader. 

Import any example program and update the mbed_app.json to define the bootloader that will be installed on your board. This will direct the tools to build the example program with the proper offset to be loaded by the newly created bootloader image. To do this, set the target (replace `<TARGET_NAME>` with your target name) value `bootloader_img` to the file path of the bootloader image that has been copied into the example program you want to remotely load. A directory root level directory named `bootloader/` is a good place to store these files.

```
    "target_overrides": {
        ...
        "<TARGET_NAME>": {
            "target.bootloader_img": "bootloader/<TARGET_NAME>.bin"
        },
        ...
```

```
mbed compile -m <TARGET_NAME> -t <toolchain>
```

### Program application using SD card

1. Connect the SD card to your computer.
1. Copy the <project-name>_application.bin binary to the root of the SD card. Make sure this is the same name as the file in the example bootloader that will be looked for and applied.
1. Remove the SD card from your PC, and plug it into the mbed board.
1. Press the reset button to start the firmware update.

If a terminal is open, the following prints:

```
Firmware update found
Starting application
```

### Programming the application using a standard drag-n-drop workflow

If loading the application via a bootloader during developemnt is a hassle you can always take the example application <project-name>.bin and load via mass storage or with a debugger. This is the combined and padded bootloader + application that you'd be developing.
