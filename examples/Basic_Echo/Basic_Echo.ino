#include <CH32X035_USBSerial.h>

using namespace wch::usbcdc;

void setup() {

  USBSerial.begin(128);
  USBSerial.waitForPC(0);
  USBSerial.println("USB CDC ready");
}

void loop() {
  // Echo from USB to USB
  while (USBSerial.available()) {
    int c = USBSerial.read();
    if (c >= 0) USBSerial.write((uint8_t)c);
  }
}

