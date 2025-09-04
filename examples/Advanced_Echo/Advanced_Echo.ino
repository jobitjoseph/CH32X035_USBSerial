/*
  Basic Echo Example for CH32X035_USBSerial
  
  This example demonstrates the basic functionality of the USB CDC Serial interface.
  It echoes back any data received from the host computer.
  
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
  USBSerial.println("=== CH32X035 USB Serial Echo Test ===");
  USBSerial.println("Type something and press Enter.");
  USBSerial.println("Characters will be echoed back.");
  USBSerial.println();
}

void loop() {
  // Check if data is available to read
  if (USBSerial.available()) {
    // Read the incoming byte
    int incomingByte = USBSerial.read();
    
    if (incomingByte >= 0) {
      // Echo the character back
      USBSerial.write((uint8_t)incomingByte);
      
      // If it's a newline, add some extra info
      if (incomingByte == '\n') {
        USBSerial.print("  [Echoed ");
        USBSerial.print(millis());
        USBSerial.println("ms]");
      }
    }
  }
  
  // Optional: Send periodic heartbeat (every 5 seconds)
  static uint32_t lastHeartbeat = 0;
  if (millis() - lastHeartbeat > 5000) {
    if (USBSerial.dtr()) {  // Only if host is connected
      USBSerial.print("♥ Heartbeat: ");
      USBSerial.print(millis() / 1000);
      USBSerial.println("s");
    }
    lastHeartbeat = millis();
  }
}