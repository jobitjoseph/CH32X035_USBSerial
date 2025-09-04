# CH32X035 USB Serial Library

A USB CDC (Communications Device Class) Serial library for CH32X035 microcontrollers that provides Arduino Stream/Print API compatibility.

[![Donate with PayPal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/paypalme/jobitjoseph)

## Features

- **Stream Compatible**: Implements Arduino Stream and Print interfaces
- **Coexists with UART**: Can be used alongside hardware Serial without conflicts
- **Host Detection**: Built-in DTR/RTS monitoring and host connection detection
- **Configurable**: Adjustable FIFO buffer sizes
- **Unique Serial Numbers**: Automatically generates unique serial numbers from chip UID

## Installation

1. Download or clone this library
2. Place in your Arduino libraries folder: `~/Documents/Arduino/libraries/CH32X035_USBSerial/`
3. Restart Arduino IDE

## Usage

### Basic Echo Example

```cpp
#include <CH32X035_USBSerial.h>

using namespace wch::usbcdc;

void setup() {
  USBSerial.begin();
  USBSerial.waitForPC(); // Wait for host connection
  USBSerial.println("USB CDC ready!");
}

void loop() {
  // Echo received data back to host
  while (USBSerial.available()) {
    int c = USBSerial.read();
    if (c >= 0) {
      USBSerial.write((uint8_t)c);
    }
  }
}
```

### Bridge UART and USB

```cpp
#include <CH32X035_USBSerial.h>

using namespace wch::usbcdc;

void setup() {
  Serial.begin(115200);    // Hardware UART
  USBSerial.begin();       // USB CDC
  USBSerial.waitForPC();
}

void loop() {
  // Forward data between UART and USB
  while (Serial.available()) {
    USBSerial.write(Serial.read());
  }
  while (USBSerial.available()) {
    Serial.write(USBSerial.read());
  }
}
```

## API Reference

### Core Methods

- `bool begin(uint16_t rxTxFifoSize = 0)` - Initialize USB CDC interface
- `void end()` - Shutdown USB CDC interface
- `int available()` - Returns number of bytes available to read
- `int read()` - Read next byte (-1 if none available)
- `int peek()` - Peek at next byte without consuming it (returns -1)
- `size_t write(uint8_t c)` - Write a single byte
- `void flush()` - Flush output buffer

### Control & Status

- `bool dtr()` - Get DTR (Data Terminal Ready) state
- `bool rts()` - Get RTS (Request To Send) state
- `uint32_t baud()` - Get current baud rate setting
- `bool setBaud(uint32_t baud)` - Set baud rate

### Utility

- `bool waitForPC(uint32_t timeoutMs = 0)` - Wait for host connection (0 = no timeout)

### Global Instance

The library provides a global `USBSerial` object ready to use, similar to the standard `Serial` object.

## Configuration

Edit `wch_usbcdc_config.h` to customize:

- **USB Strings**: Manufacturer, Product, Interface descriptions
- **USB IDs**: Vendor ID, Product ID (default uses VOTI test IDs)
- **Serial Prefix**: Custom prefix for auto-generated serial numbers
- **Power Consumption**: USB power requirements

## Hardware Requirements

- **Microcontroller**: CH32X035 series
- **USB Pins**: PC16 (D-), PC17 (D+)
- **Core**: WCH CH32V Arduino core

## Compatibility

- **Arduino IDE**: 1.8.x and 2.x
- **Platform**: CH32V architecture
- **Operating Systems**: Windows, macOS, Linux

## Examples

See the `examples/` folder for:
- The examples/ folder contains ready-to-use sketches:

- Basic_USBSerial
Basic initialization and print test. Prints messages to the USB serial monitor and waits for host connection.

- Basic_Echo
Minimal USB CDC echo server. Reads characters from the USB host and echoes them back.

- Advanced_Echo
Demonstrates line-buffered echoing, printing input strings back with additional formatting and diagnostics.

- Command_Interface
Shows how to implement a simple text-based command interface over USB CDC (e.g., help, status, reset).

- USB_UART_Passthrough
Bridges hardware UART and USB CDC. Useful for debugging or USB-to-serial adapters.

## License

This library is provided under the MIT License. See LICENSE file for details.


## Support

For support and questions:
- Check the examples and documentation
- Open an issue on the project repository
- Consult the CH32V Arduino core documentation

---

**Note**: This library uses test USB Vendor/Product IDs by default. For commercial use, please obtain your own USB IDs or use appropriate open-source alternatives.