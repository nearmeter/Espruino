# nRFClaw Hardware Port for Espruino

This development branch adds support for **NINASENSE Rev 1.1** and **HALFMOON Rev 1.0**, two nRF52832-based hardware platforms, to the upstream [Espruino](https://www.espruino.com/) project.

It adds hardware integration for LLCC68 LoRa, battery monitoring, sensors, analog/digital I/O, and the nRFClaw BLE DFU bootloader. The goal is to combine the simplicity and flexibility of JavaScript/Espruino with ultra-low-power, battery-operated hardware.

This repository keeps the original Espruino source code, history, and licensing. **NINASENSE and HALFMOON are third-party hardware platforms and are not official Espruino boards.**

## Upstream Base

Current nRFClaw branch baseline:

```text
Espruino version : 2v29.392
Upstream commit  : d8322cec9
MCU              : Nordic nRF52832
Nordic SDK       : nRF5 SDK 17
SoftDevice       : S132 7.2.0
```

Development branch:

```text
nrfclaw
```

The `master` branch is intended to remain synchronized with upstream Espruino.

## Supported Hardware

### NINASENSE Rev 1.1

NINASENSE is an ultra-low-power nRF52832 hardware platform designed for battery-powered sensing, telemetry, BLE and LoRa applications.

Main hardware:

* Nordic nRF52832
* LLCC68 LoRa transceiver
* LIS2DH12 accelerometer
* Battery voltage measurement
* Hall sensor inputs
* Digital and analog GPIO
* UART
* BLE
* 32.768 kHz crystal footprint
* nRFClaw BLE DFU bootloader support

The current Espruino configuration uses the nRF52832 internal RC low-frequency clock. `ESPR_LSE_ENABLE` is intentionally not enabled because an external LFCLK crystal is not required by this target.

### HALFMOON Rev 1.0

HALFMOON is another nRF52832 board supported by this fork.

It provides:

* Nordic nRF52832
* LLCC68 LoRa
* LIS2DH12 accelerometer
* Battery voltage measurement
* Hall sensor inputs
* Analog/digital GPIO
* UART
* BLE
* nRFClaw BLE DFU support

The LLCC68 and sensor pin assignments differ from NINASENSE and are handled by the corresponding board definition.

## Flash Layout

NINASENSE and HALFMOON use the nRFClaw bootloader-compatible memory layout:

```text
0x00000 +---------------------------+
        | MBR / S132 SoftDevice     |
        |                           |
0x26000 +---------------------------+
        | Espruino application      |
        |                           |
        | maximum application area  |
        |                           |
0x6C000 +---------------------------+
        | Espruino saved code /     |
        | persistent storage        |
        |                           |
0x76000 +---------------------------+
        | reserved                  |
        |                           |
0x7A000 +---------------------------+
        | nRFClaw BLE DFU           |
        | bootloader                |
0x7F000 +---------------------------+
        | bootloader metadata       |
0x80000 +---------------------------+
```

Application area:

```text
Start : 0x26000
End   : 0x6C000
Size  : 0x46000 = 286720 bytes = 280 KiB
```

The Espruino application must remain below `0x6C000`.

Flash write protection prevents JavaScript code from modifying:

```text
0x00000 - 0x25FFF   MBR / SoftDevice
0x7A000 - 0x7FFFF   nRFClaw bootloader and metadata
```

## BLE RAM Layout

S132 7.2.0 requires the Espruino application RAM to start at:

```text
0x20002E40
```

The board configuration therefore supplies:

```make
LD_APP_RAM_BASE=0x2E40
```

The SDK17 linker used by this fork supports a configurable RAM origin through `LD_APP_RAM_BASE`.

## Building

Clone the fork:

```bash
git clone https://github.com/nearmeter/Espruino.git
cd Espruino
git switch nrfclaw
```

Provision the dependencies required by Espruino according to the upstream build instructions.

This fork uses:

```text
nRF5 SDK 17
S132 SoftDevice 7.2.0
```

### Build NINASENSE

```bash
BOARD=NINASENSE RELEASE=1 make -j1
```

For a clean build:

```bash
BOARD=NINASENSE RELEASE=1 make clean
BOARD=NINASENSE RELEASE=1 make -j1
```

### Build HALFMOON

```bash
BOARD=HALFMOON RELEASE=1 make clean
BOARD=HALFMOON RELEASE=1 make -j1
```

## Build Outputs

A NINASENSE build generates files including:

```text
bin/espruino_2v29.xxx_ninasense.elf
bin/espruino_2v29.xxx_ninasense.hex
bin/espruino_2v29.xxx_ninasense.app_hex
bin/espruino_2v29.xxx_ninasense.app.bin
```

The `.app.bin` file is generated automatically for the NINASENSE and HALFMOON targets.

### `.hex`

The `.hex` file is the Espruino firmware image produced by the build system and is primarily useful for development and SWD-based programming.

When using the nRFClaw bootloader, use the generated `.app.bin` for application-only DFU instead.

### `.app.bin`

The `.app.bin` contains the raw Espruino application image.

Offset zero of this binary corresponds to the application's vector table at flash address:

```text
0x26000
```

This is the image intended for application-only firmware upgrades using the nRFClaw DFU bootloader and nRFClaw Studio.

Do **not** use a full binary containing the SoftDevice for application-only DFU.

## nRFClaw Studio DFU

NINASENSE/HALFMOON devices containing the nRFClaw bootloader can replace the main application without an SWD programmer.

The tested upgrade path is:

```text
Espruino build
      |
      v
*.app.bin
      |
      v
nRFClaw Studio
      |
      v
BLE DFU bootloader
      |
      v
Application written at 0x26000
      |
      v
Espruino
```

The Espruino `.app.bin` generated by this fork has been successfully installed through the nRFClaw Studio/bootloader path.

### Bootloader Recovery

The nRFClaw bootloader provides an application-independent recovery path. On NINASENSE Rev 1.1, the recovery button is connected to **P0.21**. When the device starts with the recovery condition active, the bootloader can remain in control instead of starting the application.

This allows the main firmware to be replaced through the nRFClaw DFU workflow without requiring the currently installed application to be operational and without requiring an SWD programmer. It also makes it possible to replace Espruino with another compatible application image, provided that the firmware respects the nRFClaw memory layout and bootloader requirements.

HALFMOON uses its board-specific recovery pin as documented in the pinout below.

## JavaScript Console

The development configuration provides the Espruino console through UART:

```text
TX   : P0.15
RX   : P0.16
Baud : 115200
```

Example:

```javascript
1 + 2
```

Result:

```text
=3
```

BLE remains enabled while the UART console is used.

## Battery Measurement

NINASENSE battery voltage is measured on:

```text
P0.28 / AIN4
```

The hardware voltage divider uses a measured conversion factor of approximately:

```text
1.402
```

Battery measurement is integrated with Espruino's own analog subsystem rather than creating a second independent SAADC driver.

Example:

```javascript
NINASENSE.battery();
```

The implementation uses Espruino's `jshPinAnalog()` backend.

A raw measurement is also available:

```javascript
NINASENSE.batteryRaw();
```

## LLCC68 LoRa

NINASENSE and HALFMOON include native support for the LLCC68 LoRa transceiver.

The default profile is:

```text
Frequency : 915 MHz
Power     : 14 dBm
SF        : 7
Bandwidth : 125 kHz
Coding    : 4/5
Sync word : 0x12
Preamble  : 8
```

Initialize LoRa:

```javascript
NINASENSE.loraInit();
```

Example result:

```text
=true
```

Send a packet:

```javascript
NINASENSE.loraSend("TEST");
```

Configure the radio:

```javascript
NINASENSE.loraConfig({
  frequency: 915000000,
  power: 14,
  sf: 7,
  bandwidth: 125,
  cr: 1,
  syncWord: 0x12,
  preamble: 8
});
```

Receive:

```javascript
var packet = NINASENSE.loraReceive(15000);
print(packet);
```

Put the radio into its low-power state:

```javascript
NINASENSE.loraSleep();
```

The LLCC68 driver uses a retained warm-sleep strategy and releases SPI/GPIO resources after entering sleep to reduce idle consumption.

### Periodic LoRa transmission

Example:

```javascript
NINASENSE.loraInit();

var counter = 0;

setInterval(function () {
  var msg = "T" + counter++;
  print("TX:", msg, NINASENSE.loraSend(msg));
}, 10000);
```

## LIS2DH12 Accelerometer

The accelerometer does not require a dedicated native nRFClaw driver.

Espruino already provides a JavaScript `LIS2DH12` module.

For NINASENSE:

```javascript
pinMode(D17, "output");
digitalWrite(D17, 1);

I2C1.setup({
  scl: D13,
  sda: D18,
  bitrate: 100000
});

var lis = require("LIS2DH12").connectI2C(I2C1, {});
```

Interrupt pins:

```text
INT1 : P0.22
INT2 : P0.23
```

They can be monitored using Espruino's `setWatch()` functionality.

## NINASENSE Pinout

Important pins:

```text
UART TX          P0.15
UART RX          P0.16

Recovery button  P0.21

Battery ADC      P0.28 / AIN4

Analog           P0.29 / AIN5
Analog           P0.30 / AIN6
Analog           P0.31 / AIN7

Hall 1           P0.11
Hall 2           P0.14
```

### LLCC68

```text
SCK              P0.02
NSS              P0.03
MOSI             P0.04
MISO             P0.05
BUSY             P0.06
RXEN             P0.07
TXEN             P0.08
DIO1             P0.09
DIO2             P0.10
RESET            P0.19
```

### LIS2DH12

```text
SCL              P0.13
CS               P0.17
SDA              P0.18
INT1             P0.22
INT2             P0.23
```

## HALFMOON Pinout

Important pins:

```text
UART TX          P0.15
UART RX          P0.16

Recovery button  P0.22

Battery ADC      P0.02

Hall 1           P0.12
Hall 2           P0.31

Analog/free      P0.17
Analog/free      P0.26
Analog/free      P0.27
```

### LLCC68

```text
SCK              P0.04
NSS              P0.05
MOSI             P0.06
MISO             P0.07
BUSY             P0.08
RXEN             P0.09
TXEN             P0.10
DIO1             P0.13
DIO2             P0.20
RESET            P0.21
```

### LIS2DH12

```text
SDA              P0.23
CS               P0.25
SCL              P0.28
INT1             P0.29
INT2             P0.30
```

P0.20 is reserved for LLCC68 DIO2 on HALFMOON and is therefore not used for DS18B20 by this target.

## SPI Resource Note

The current LLCC68 backend uses Nordic SPI instance 0.

On Espruino, hardware `SPI1` maps to this peripheral.

The LLCC68 driver initializes the peripheral only while performing radio operations and releases it when the radio enters sleep.

Applications should therefore not attempt to use JavaScript `SPI1` concurrently with an active NINASENSE/HALFMOON LoRa operation.

The LIS2DH12 uses I2C and does not conflict with the LoRa SPI peripheral.

## Current Validation Status

The following functionality has been validated on NINASENSE Rev 1.1:

```text
SDK17 / S132 7.2.0 build       PASS
Application @ 0x26000          PASS
BLE RAM @ 0x20002E40           PASS
Espruino interpreter           PASS
UART console                   PASS
BLE advertising                PASS
Digital GPIO engine            PASS
Analog ADC P0.28               PASS
Battery API                    PASS
LLCC68 initialization          PASS
Automatic .app.bin             PASS
nRFClaw Studio BLE DFU         PASS
```

LLCC68 TX/RX, final low-power measurements and some peripheral scenarios may continue to receive additional validation as development progresses.

HALFMOON Rev 1.0 uses the same software architecture and LLCC68 backend, with board-specific pin mappings. Its hardware validation status may differ from NINASENSE and should not be inferred from the table above.

## Upstream Compatibility

The fork intentionally separates generic Espruino improvements from nRFClaw-specific hardware support.

Generic changes include:

* nRF52832 legacy UART support with SDK17
* configurable SDK17 BLE RAM start
* correct BLE RAM requirement reporting

Hardware-specific changes include:

* NINASENSE board
* HALFMOON board
* LLCC68 support
* battery measurement
* nRFClaw flash protection
* application-only `.bin` generation for nRFClaw DFU

Where appropriate, generic fixes may be proposed independently to the upstream Espruino project.

## Related Projects

* [Espruino](https://www.espruino.com/) — JavaScript interpreter for microcontrollers
* nRFClaw — AI-programmable, ultra-low-power nRF52 platform
* NINASENSE — battery-operated nRF52832 sensing and telemetry hardware
* nRFClaw Studio — configuration, programming and BLE DFU environment

## License

This fork remains subject to the licenses of the upstream Espruino project and its included third-party components.

nRFClaw-specific source files should retain their applicable license notices.

Refer to the repository's original license files and individual source headers for details.
