/*
  Basic USB Serial Example for CH32X035_USBSerial
  
  This example demonstrates the basic functionality of the USB CDC Serial interface.
  
  Features demonstrated:
  - Basic USB Serial initialization
  - Waiting for host connection
  - Reading and writing data
  - Connection status monitoring
  
  Hardware: CH32X035 board with USB pins PC16(D-) and PC17(D+)
*/

#include <CH32X035_USBSerial.h>

using namespace wch::usbcdc;

void setup() {
  // Initialize USB Serial with default buffer size, to set buffer size USBSerial.begin(buffer size)
  USBSerial.begin();
  
  // Wait for host to connect (optional timeout in ms, 0 = wait forever)
  USBSerial.waitForPC(0);  // Wait indefinitely
  
  // Send welcome message
  USBSerial.println("=== CH32X035 USB Serial ===");
}

void loop() {
  //Send periodic heartbeat (every second)
  static uint32_t lastHeartbeat = 0;
  if (millis() - lastHeartbeat > 1000) {
    if (USBSerial.dtr()) {  // Only if host is connected
      USBSerial.print("♥ Heartbeat: ");
      USBSerial.print(millis() / 1000);
      USBSerial.println("s");
    }
    lastHeartbeat = millis();
  }
}