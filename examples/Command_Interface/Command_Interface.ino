/*
  Command Interface Example for CH32X035_USBSerial
  
  This example creates a simple command-line interface over USB Serial.
  Demonstrates how to build interactive applications that respond to 
  user commands from a terminal or custom application.
  
  Features demonstrated:
  - Command parsing and execution
  - Help system
  - GPIO control via commands
  - System information queries
  - Parameter validation
  
  Hardware: CH32X035 board with LED on PC13 and some GPIO pins
*/

#include <CH32X035_USBSerial.h>

using namespace wch::usbcdc;

// Configuration
const int LED_PIN = PC_3;
const int TEST_PINS[] = {PA_5, PA_6, PA_7};  // GPIO pins for testing
const int NUM_TEST_PINS = sizeof(TEST_PINS) / sizeof(TEST_PINS[0]);

// State
String inputBuffer = "";
bool ledState = false;
int pinStates[NUM_TEST_PINS] = {0};  // 0=input, 1=output_low, 2=output_high

// Command structure
struct Command {
  const char* name;
  const char* description;
  void (*handler)(String args);
};

// Function declarations
void cmdHelp(String args);
void cmdLed(String args);
void cmdPin(String args);
void cmdStatus(String args);
void cmdReset(String args);
void cmdBaud(String args);
void cmdEcho(String args);
void cmdUptime(String args);

// Command table
const Command commands[] = {
  {"help", "Show this help message", cmdHelp},
  {"led", "Control LED: led on|off|toggle", cmdLed},
  {"pin", "GPIO control: pin <num> input|low|high|read", cmdPin},
  {"status", "Show system status", cmdStatus},
  {"reset", "Software reset", cmdReset},
  {"baud", "Show/set baud rate: baud [rate]", cmdBaud},
  {"echo", "Echo text: echo <text>", cmdEcho},
  {"uptime", "Show system uptime", cmdUptime},
};

const int NUM_COMMANDS = sizeof(commands) / sizeof(commands[0]);

void setup() {
  // Initialize hardware
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  // LED off
  
  // Initialize test pins as inputs
  for (int i = 0; i < NUM_TEST_PINS; i++) {
    pinMode(TEST_PINS[i], INPUT);
    pinStates[i] = 0;
  }
  
  // Initialize USB Serial
  USBSerial.begin(256);
  USBSerial.waitForPC(0);
  
  // Welcome message
  delay(1000);  // Give time for host to connect
  printWelcome();
  printPrompt();
}

void loop() {
  // Handle incoming characters
  while (USBSerial.available()) {
    char c = USBSerial.read();
    
    if (c == '\r' || c == '\n') {
      if (inputBuffer.length() > 0) {
        USBSerial.println();  // Echo newline
        processCommand(inputBuffer);
        inputBuffer = "";
        printPrompt();
      }
    } else if (c == '\b' || c == 127) {  // Backspace
      if (inputBuffer.length() > 0) {
        inputBuffer.remove(inputBuffer.length() - 1);
        USBSerial.print("\b \b");  // Erase character on terminal
      }
    } else if (c >= 32 && c <= 126) {  // Printable characters
      inputBuffer += c;
      USBSerial.write(c);  // Echo character
    }
  }
  
  // Blink LED to show activity
  static uint32_t lastBlink = 0;
  if (millis() - lastBlink > 2000) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    lastBlink = millis();
  }
}

void printWelcome() {
  USBSerial.println();
  USBSerial.println("=====================================");
  USBSerial.println("  CH32X035 Command Interface v1.0");
  USBSerial.println("=====================================");
  USBSerial.println("Type 'help' for available commands");
  USBSerial.print("Host connection: ");
  USBSerial.println(USBSerial.dtr() ? "ACTIVE" : "NONE");
  USBSerial.println();
}

void printPrompt() {
  USBSerial.print("CH32> ");
}

void processCommand(String input) {
  input.trim();
  if (input.length() == 0) return;
  
  // Split command and arguments
  int spaceIndex = input.indexOf(' ');
  String cmd = (spaceIndex == -1) ? input : input.substring(0, spaceIndex);
  String args = (spaceIndex == -1) ? "" : input.substring(spaceIndex + 1);
  
  cmd.toLowerCase();
  
  // Find and execute command
  bool found = false;
  for (int i = 0; i < NUM_COMMANDS; i++) {
    if (cmd == commands[i].name) {
      commands[i].handler(args);
      found = true;
      break;
    }
  }
  
  if (!found) {
    USBSerial.print("Unknown command: ");
    USBSerial.println(cmd);
    USBSerial.println("Type 'help' for available commands");
  }
}

void cmdHelp(String args) {
  USBSerial.println("Available commands:");
  for (int i = 0; i < NUM_COMMANDS; i++) {
    USBSerial.print("  ");
    USBSerial.print(commands[i].name);
    USBSerial.print(" - ");
    USBSerial.println(commands[i].description);
  }
}

void cmdLed(String args) {
  args.trim();
  args.toLowerCase();
  
  if (args == "on") {
    digitalWrite(LED_PIN, LOW);  // Active low
    ledState = true;
    USBSerial.println("LED ON");
  } else if (args == "off") {
    digitalWrite(LED_PIN, HIGH);
    ledState = false;
    USBSerial.println("LED OFF");
  } else if (args == "toggle") {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState ? LOW : HIGH);
    USBSerial.print("LED ");
    USBSerial.println(ledState ? "ON" : "OFF");
  } else {
    USBSerial.println("Usage: led on|off|toggle");
  }
}

void cmdPin(String args) {
  // Parse pin number and command
  int firstSpace = args.indexOf(' ');
  if (firstSpace == -1) {
    USBSerial.println("Usage: pin <num> input|low|high|read");
    return;
  }
  
  int pinNum = args.substring(0, firstSpace).toInt();
  String pinCmd = args.substring(firstSpace + 1);
  pinCmd.trim();
  pinCmd.toLowerCase();
  
  // Find pin in our test pins array
  int pinIndex = -1;
  for (int i = 0; i < NUM_TEST_PINS; i++) {
    if (TEST_PINS[i] == pinNum) {
      pinIndex = i;
      break;
    }
  }
  
  if (pinIndex == -1) {
    USBSerial.print("Invalid pin. Available pins: ");
    for (int i = 0; i < NUM_TEST_PINS; i++) {
      USBSerial.print(TEST_PINS[i]);
      if (i < NUM_TEST_PINS - 1) USBSerial.print(", ");
    }
    USBSerial.println();
    return;
  }
  
  // Execute pin command
  if (pinCmd == "input") {
    pinMode(TEST_PINS[pinIndex], INPUT);
    pinStates[pinIndex] = 0;
    USBSerial.print("Pin ");
    USBSerial.print(pinNum);
    USBSerial.println(" set to INPUT");
  } else if (pinCmd == "low") {
    pinMode(TEST_PINS[pinIndex], OUTPUT);
    digitalWrite(TEST_PINS[pinIndex], LOW);
    pinStates[pinIndex] = 1;
    USBSerial.print("Pin ");
    USBSerial.print(pinNum);
    USBSerial.println(" set to OUTPUT LOW");
  } else if (pinCmd == "high") {
    pinMode(TEST_PINS[pinIndex], OUTPUT);
    digitalWrite(TEST_PINS[pinIndex], HIGH);
    pinStates[pinIndex] = 2;
    USBSerial.print("Pin ");
    USBSerial.print(pinNum);
    USBSerial.println(" set to OUTPUT HIGH");
  } else if (pinCmd == "read") {
    int value = digitalRead(TEST_PINS[pinIndex]);
    USBSerial.print("Pin ");
    USBSerial.print(pinNum);
    USBSerial.print(" reads: ");
    USBSerial.println(value ? "HIGH" : "LOW");
  } else {
    USBSerial.println("Usage: pin <num> input|low|high|read");
  }
}

String getPinName(int pin) {
  switch(pin) {
    case PA_0:  return "PA0";
    case PA_1:  return "PA1";
    case PA_2:  return "PA2";
    case PA_3:  return "PA3";
    case PA_4:  return "PA4";
    case PA_5:  return "PA5";
    case PA_6:  return "PA6"; 
    case PA_7:  return "PA7";
    case PB_0:  return "PB0";
    case PB_1:  return "PB1";
    case PB_3:  return "PB3";
    case PB_4:  return "PB4";
    case PB_6:  return "PB6";
    case PB_7:  return "PB7";
    case PB_8:  return "PB8";
    case PB_9:  return "PB9";
    case PB_10: return "PB10";
    case PB_11: return "PB11";
    case PB_12: return "PB12";
    case PC_0:  return "PC0";
    case PC_3:  return "PC3";
    case PC_14: return "PC14";
    case PC_15: return "PC15";
    case PC_16: return "PC16";
    case PC_17: return "PC17";
    case PC_18: return "PC18";
    case PC_19: return "PC19";
    case PA0:  return "PA0";
    case PA1:  return "PA1";
    case PA2:  return "PA2";
    case PA3:  return "PA3";
    case PA4:  return "PA4";
    case PA5:  return "PA5";
    case PA6:  return "PA6"; 
    case PA7:  return "PA7";
    case PB0:  return "PB0";
    case PB1:  return "PB1";
    case PC0:  return "PC0";
    case PC3:  return "PC3";
    default: return "PIN" + String(pin);
  }
}

void cmdStatus(String args) {
  USBSerial.println("=== System Status ===");
  USBSerial.print("Uptime: ");
  USBSerial.print(millis() / 1000);
  USBSerial.println(" seconds");
  
  USBSerial.print("LED State: ");
  USBSerial.println(ledState ? "ON" : "OFF");
  
  USBSerial.println("GPIO States:");
  for (int i = 0; i < NUM_TEST_PINS; i++) {
    USBSerial.print("  Pin ");
    USBSerial.print(getPinName(TEST_PINS[i]));
    USBSerial.print(": ");
    switch (pinStates[i]) {
      case 0: USBSerial.println("INPUT"); break;
      case 1: USBSerial.println("OUTPUT_LOW"); break;
      case 2: USBSerial.println("OUTPUT_HIGH"); break;
    }
  }
  
  USBSerial.print("USB Host DTR: ");
  USBSerial.println(USBSerial.dtr() ? "ACTIVE" : "INACTIVE");
  
  USBSerial.print("USB Host RTS: ");
  USBSerial.println(USBSerial.rts() ? "ACTIVE" : "INACTIVE");
  
  USBSerial.print("USB Baud Rate: ");
  USBSerial.println(USBSerial.baud());
}

void cmdReset(String args) {
  USBSerial.println("Performing software reset...");
  USBSerial.flush();
  delay(100);
  NVIC_SystemReset();
}

void cmdBaud(String args) {
  args.trim();
  if (args.length() == 0) {
    USBSerial.print("Current baud rate: ");
    USBSerial.println(USBSerial.baud());
  } else {
    uint32_t newBaud = args.toInt();
    if (newBaud >= 1200 && newBaud <= 921600) {
      USBSerial.setBaud(newBaud);
      USBSerial.print("Baud rate set to: ");
      USBSerial.println(newBaud);
    } else {
      USBSerial.println("Invalid baud rate (1200-921600)");
    }
  }
}

void cmdEcho(String args) {
  if (args.length() == 0) {
    USBSerial.println("Usage: echo <text>");
  } else {
    USBSerial.print("Echo: ");
    USBSerial.println(args);
  }
}

void cmdUptime(String args) {
  uint32_t uptimeMs = millis();
  uint32_t uptimeS = uptimeMs / 1000;
  uint32_t uptimeM = uptimeS / 60;
  uint32_t uptimeH = uptimeM / 60;
  
  USBSerial.print("Uptime: ");
  if (uptimeH > 0) {
    USBSerial.print(uptimeH);
    USBSerial.print("h ");
  }
  if (uptimeM % 60 > 0 || uptimeH > 0) {
    USBSerial.print(uptimeM % 60);
    USBSerial.print("m ");
  }
  USBSerial.print(uptimeS % 60);
  USBSerial.print("s (");
  USBSerial.print(uptimeMs);
  USBSerial.println("ms)");
}