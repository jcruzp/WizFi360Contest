# Getting Started with WizFi360 AWS IoT Examples

These sections will guide you through a series of steps from configuring development environment to running WizFi360 AWS IoT examples using the **WIZnet's Wi-Fi products**.

- [**Development environment configuration**](#development_environment_configuration)
- [**Hardware requirements**](#hardware_requirements)
- [**WizFi360 AWS IoT example structure**](#wizfi360_aws_iot_example_structure)
- [**WizFi360 AWS IoT example testing**](#wizfi360_aws_iot_example_testing)



<a name="development_environment_configuration"></a>
## Development environment configuration

To test the WizFi360 AWS IoT examples, the development environment must be configured to use WizFi360-EVB-Pico.

The WizFi360 AWS IoT examples were tested by configuring the development environment for **Windows**. Please refer to the '**9.2. Building on MS Windows**' section of '**Getting started with Raspberry Pi Pico**' document below and configure accordingly.

- [**Getting started with Raspberry Pi Pico**][link-getting_started_with_raspberry_pi_pico]

**Visual Studio Code** was used during development and testing of WizFi360 AWS IoT examples, the guide document in each directory was prepared also base on development with Visual Studio Code. Please refer to corresponding document.



<a name="hardware_requirements"></a>
## Hardware requirements

The WizFi360 AWS IoT examples use **WizFi360-EVB-Pico** - Wi-Fi I/O module built on [**RP2040**][link-rp2040] and WIZnet's [**WizFi360**][link-wizfi360] Wi-Fi module.

- [**WizFi360-EVB-Pico**][link-wizfi360-evb-pico]

![][link-wizfi360-evb-pico_main]



<a name="wizfi360_aws_iot_example_structure"></a>
## WizFi360 AWS IoT example structure

Examples are available at '**WizFi360-EVB-Pico-AWS-C/examples/**' directory. As of now, following examples are provided.

- [**Connect AWS IoT through MQTT**][link-connect_aws_iot_through_mqtt]

Note that **CMSIS_5**, **CMSIS-Driver**, **CMSIS-FreeRTOS**, **mbedtls**, **aws-iot-device-sdk-embedded-C**, **pico-sdk** are needed to run WizFi360 AWS IoT examples.

- **CMSIS_5** is a set of tools, APIs, frameworks, and work flows that help to simplify software re-use, reduce the learning curve for microcontroller developers, speed-up project build and debug, and thus reduce the time to market for new applications.
- **CMSIS-Driver** contains MCU independent device driver implementations and template files.
- **CMSIS-FreeRTOS** is a common API for embedded microcontroller real-time operating system FreeRTOS.
- **IoT_Socket** contains the code of IoT Socket - a simple BSD like IP socket interface that implements the glue logic between IoT cloud connectors (IoT clients) and the underlying communication stack.
- **mbedtls** library supports additional algorithms and support related to TLS and SSL connections.
- **aws-iot-device-sdk-embedded-C** library is embedded C (C-SDK) is a collection of C source files that can be used in embedded applications to securely connect IoT devices to AWS IoT Core.
- **pico-sdk** is made available by Pico to enable developers to build software applications for the Pico platform.
- **pico-extras** has additional libraries that are not yet ready for inclusion the Pico SDK proper, or are just useful but don't necessarily belong in the Pico SDK.

Libraries are located in the '**WizFi360-EVB-Pico-AWS-C/libraries/**' directory.

- [**CMSIS_5**][link-cmsis_5]
- [**CMSIS-Driver**][link-cmsis-driver]
- [**CMSIS-FreeRTOS**][link-cmsis-freertos]
- [**IoT_Socket**][link-iot_socket]
- [**mbedtls**][link-mbedtls]
- [**aws-iot-device-sdk-embedded-C**][link-aws-iot-device-sdk-embedded-c]
- [**pico-sdk**][link-pico-sdk]
- [**pico-extras**][link-pico-extras]

If you want to modify the code that MCU-dependent and use a MCU other than **RP2040**, you can modify it in the **WizFi360-EVB-Pico-AWS-C/port/** directory.

- [**FreeRTOS-Kernel**][link-port_freertos-kernel]
- [**RTE**][link-port_rte]
- [**WizFi360**][link-port_wizfi360]
- [**mbedtls**][link-port_mbedtls]
- [**aws-iot-device-sdk-embedded-C**][link-port_aws_iot_device_sdk_embedded_c]
- [**timer**][link-port_timer]



<a name="wizfi360_aws_iot_example_testing"></a>
## WizFi360 AWS IoT example testing

1. Download

If the WizFi360 AWS IoT examples are cloned, the library set as a submodule is an empty directory. Therefore, if you want to download the library set as a submodule together, clone the WizFi360 AWS IoT examples with the following Git command.

```cpp
/* Change directory */
// change to the directory to clone
cd [user path]

// e.g.
cd D:/RP2040

/* Clone */
git clone --recurse-submodules https://github.com/Wiznet/WizFi360-EVB-Pico-AWS-C.git
```

With Visual Studio Code, the library set as a submodule is automatically downloaded, so it doesn't matter whether the library set as a submodule is an empty directory or not, so refer to it.

2. Patch

With Visual Studio Code, each library set as a submodule is automatically patched, but if you do not use Visual Studio Code, each library set as a submodule must be manually patched with the Git commands below in each library directory.

- CMSIS-FreeRTOS

```cpp
/* Change directory */
// change to the 'CMSIS-FreeRTOS' library directory
cd [user path]/WizFi360-EVB-Pico-AWS-C/libraries/CMSIS-FreeRTOS

// e.g.
cd D:/RP2040/WizFi360-EVB-Pico-AWS-C/libraries/CMSIS-FreeRTOS/CMSIS/RTOS2/FreeRTOS/Source

/* Patch */
git apply ../../../../../../patches/01_cmsis_freertos_cmsis_os2.patch
```

- aws-iot-device-sdk-embedded-C

```cpp
/* Change directory */
// change to the 'coreHTTP' library directory inside the 'aws-iot-device-sdk-embedded-C' library directory.
cd [user path]/WizFi360-EVB-Pico-AWS-C/libraries/aws-iot-device-sdk-embedded-C/libraries/standard/coreHTTP

// e.g.
cd D:/RP2040/WizFi360-EVB-Pico-AWS-C/libraries/aws-iot-device-sdk-embedded-C/libraries/standard/coreHTTP

/* Patch */
git apply --ignore-whitespace ../../../../../patches/02_aws_iot_device_sdk_embedded_c_corehttp_network_interface.patch
```

3. Test

Please refer to 'README.md' in each example directory to find detail guide for testing WizFi360 AWS IoT examples.



<!--
Link
-->

[link-getting_started_with_raspberry_pi_pico]: https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf
[link-rp2040]: https://www.raspberrypi.org/products/rp2040/
[link-wizfi360]: https://docs.wiznet.io/Product/Wi-Fi-Module/WizFi360/wizfi360
[link-wizfi360-evb-pico]: https://docs.wiznet.io/Product/Open-Source-Hardware/wizfi360-evb-pico
[link-wizfi360-evb-pico_main]: https://github.com/Wiznet/WizFi360-EVB-Pico-AWS-C/blob/main/static/images/getting_started/wizfi360-evb-pico_main.png
[link-connect_aws_iot_through_mqtt]: https://github.com/Wiznet/WizFi360-EVB-Pico-AWS-C/tree/main/examples/aws_mqtt_demo
[link-cmsis_5]: https://github.com/ARM-software/CMSIS_5
[link-cmsis-driver]: https://github.com/ARM-software/CMSIS-Driver
[link-cmsis-freertos]: https://github.com/ARM-software/CMSIS-FreeRTOS
[link-iot_socket]: https://github.com/MDK-Packs/IoT_Socket
[link-mbedtls]: https://github.com/ARMmbed/mbedtls
[link-aws-iot-device-sdk-embedded-c]: https://github.com/aws/aws-iot-device-sdk-embedded-C
[link-pico-sdk]: https://github.com/raspberrypi/pico-sdk
[link-pico-extras]: https://github.com/raspberrypi/pico-extras
[link-port_freertos-kernel]: https://github.com/Wiznet/WizFi360-EVB-Pico-AWS-C/tree/main/port/FreeRTOS-Kernel
[link-port_rte]: https://github.com/Wiznet/WizFi360-EVB-Pico-AWS-C/tree/main/port/RTE
[link-port_wizfi360]: https://github.com/Wiznet/WizFi360-EVB-Pico-AWS-C/tree/main/port/WizFi360
[link-port_mbedtls]: https://github.com/Wiznet/WizFi360-EVB-Pico-AWS-C/tree/main/port/mbedtls
[link-port_aws_iot_device_sdk_embedded_c]: https://github.com/Wiznet/WizFi360-EVB-Pico-AWS-C/tree/main/port/aws-iot-device-sdk-embedded-C
[link-port_timer]: https://github.com/Wiznet/WizFi360-EVB-Pico-AWS-C/tree/main/port/timer
