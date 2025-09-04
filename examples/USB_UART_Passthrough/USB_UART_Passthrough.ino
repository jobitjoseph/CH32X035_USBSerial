/*
  Simple USB-UART Passthrough
  
  Minimal code for transparent data transfer between USB and UART.
  No status messages, no LED, no overhead - just pure data passthrough.
  
  Hardware:
  - CH32X035 board
  - USART1 TX: PB10 (connect to target device RX)
  - USART1 RX: PB11 (connect to target device TX)
  - USB: PC16 (D-), PC17 (D+)
  
  Alternative UART options:
  - USART2: PA2 (TX), PA3 (RX)
  - USART3: PB3 (TX), PB4 (RX)  
  - USART4: PB0 (TX), PB1 (RX)
*/

#include <CH32X035_USBSerial.h>

using namespace wch::usbcdc;

void setup() {
  Serial.begin(115200);    // Hardware UART
  USBSerial.begin();       // USB CDC Serial
}

void loop() {
  // USB → UART
  if (USBSerial.available()) {
    Serial.write(USBSerial.read());
  }
  
  // UART → USB  
  if (Serial.available()) {
    USBSerial.write(Serial.read());
  }
}