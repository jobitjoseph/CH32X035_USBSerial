#include "CH32X035_USBSerial.h"
#include "internal/wch_usbcdc_config.h"

// Pull in WCH device headers from the installed core
extern "C" {
#include <ch32x035.h>
}

extern "C" {
  void USB_init(void);
  void USB_EP0_copyDescr(uint8_t len);
  void CDC_init(void);
  void CDC_flush(void);
  char CDC_read(void);
  void CDC_write(char c);
  uint8_t CDC_available(void);
  uint8_t CDC_ready(void);
  extern volatile uint8_t CDC_controlLineState;
  typedef struct {
    uint32_t baudrate; uint8_t stopbits; uint8_t parity; uint8_t databits;
  } CDC_LINE_CODING_TYPE;
  extern CDC_LINE_CODING_TYPE CDC_lineCoding;
}

using namespace wch::usbcdc;

USBSerialCH32::USBSerialCH32() : _initialized(false), _fifoSize(WCH_USBCDC_DEFAULT_FIFO_SIZE) {}

bool USBSerialCH32::begin(uint16_t rxTxFifoSize) {
  if (rxTxFifoSize) _fifoSize = rxTxFifoSize;
  if (_initialized) return true;
  
  // Ensure USB is completely off first
  USBFSD->BASE_CTRL = 0x00;
  delay(100);  // Much longer delay in Arduino
  
  CDC_init();
  
  // Wait much longer for enumeration - critical for Windows
  delay(1000);  
  
  _initialized = true;
  return true;
}

void USBSerialCH32::end() {
  _initialized = false;
}

int USBSerialCH32::available() {
  return CDC_available();
}

int USBSerialCH32::read() {
  if (!CDC_available()) return -1;
  return (int)(uint8_t)CDC_read();
}

int USBSerialCH32::peek() {
  // Not supported in low-level; emulate via read/unread if needed. Return -1.
  return -1;
}

void USBSerialCH32::flush() {
  CDC_flush();
}

size_t USBSerialCH32::write(uint8_t c) {
  if (!CDC_ready()) {}
  CDC_write((char)c);
  // Immediate flush to send without batching
  CDC_flush();
  return 1;
}

bool USBSerialCH32::dtr() const { return (CDC_controlLineState & 0x01) != 0; }
bool USBSerialCH32::rts() const { return (CDC_controlLineState & 0x02) != 0; }
uint32_t USBSerialCH32::baud() const { return CDC_lineCoding.baudrate; }
bool USBSerialCH32::setBaud(uint32_t baud) {
  CDC_lineCoding.baudrate = baud;
  return true;
}

bool USBSerialCH32::waitForPC(uint32_t timeoutMs) {
  uint32_t start = millis();
  while (!dtr()) {
    if (timeoutMs && (millis() - start >= timeoutMs)) return false;
    delay(1);
  }
  return true;
}

void USBSerialCH32::_irqHandler() {
  // ISR is handled in C code
}

wch::usbcdc::USBSerialCH32 wch::usbcdc::USBSerial;

