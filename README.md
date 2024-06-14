# digivac-S4-connect-serial-arduino
Arduino Sketch for Arduino DUE to interface with Digivac quantum pressure sensor over serial through RS232 shifter

## Overview

This Arduino sketch is designed to communicate with a Digivac DPP Quantum Pressure Sensor over an RS232 interface. The sketch reads various pressure values from the sensor and prints them to the Serial Monitor. Specifically, it reads the combined pressure, Piezo pressure, and MEMS Pirani pressure, as well as a quick data acquisition string.

## Features

- Communicates with the Digivac DPP Quantum Pressure Sensor over RS232.
- Reads and prints:
  - Combined pressure
  - Piezo pressure
  - MEMS Pirani pressure
  - Quick data acquisition string
- Prints all values in a formatted line on the Serial Monitor.

## Hardware Requirements

- Arduino DUE
- RS232 to TTL shifter module
- Digivac DPP Quantum Pressure Sensor
- Connecting wires

## Wiring

Connect the RS232 to TTL shifter module as follows:

- **RS232 Shifter TX** -> **Arduino DUE RX1 (pin 19)**
- **RS232 Shifter RX** -> **Arduino DUE TX1 (pin 18)**
- **RS232 Shifter GND** -> **Arduino DUE GND**
- **RS232 Shifter VCC** -> **Arduino DUE 3.3V**

## Installation and Setup

### Arduino IDE Setup

1. **Install Arduino IDE**: Download and install the Arduino IDE from the [official website](https://www.arduino.cc/en/software).

2. **Connect Arduino DUE**: Connect your Arduino DUE to your computer using a USB cable.

3. **Select Board and Port**:
   - Open Arduino IDE.
   - Go to `Tools` > `Board` > `Arduino Due (Programming Port)`.
   - Go to `Tools` > `Port` and select the COM port associated with your Arduino DUE.

### Flash the Sketch

1. **Create a New Sketch**:
   - Open the Arduino IDE.
   - Go to `File` > `New` to create a new sketch.
   
2. **Copy and Paste the Code**:
   - Copy the provided code into the new sketch.
   - Save the sketch with a suitable name, e.g., `PressureSensorReader`.

3. **Upload the Sketch**:
   - Click the `Upload` button (right arrow icon) in the Arduino IDE toolbar.
   - Wait for the sketch to compile and upload to the Arduino DUE.

### Code

```cpp
#include <Arduino.h>

// Define the struct to hold sensor commands
struct SensorCommands {
  const char* readCombinedPressure = "@253P?\\";
  const char* readPiezoPressure = "@253P?PZ\\";
  const char* readPiraniPressure = "@253P?MP\\";
  const char* quickDataAcquisition = "@253Q?\\";
};

// Define the struct to hold pressure values
struct PressureValues {
  float combinedPressure; // Variable to hold combined pressure value
  float piezoPressure;    // Variable to hold Piezo pressure value
  float piraniPressure;   // Variable to hold MEMS Pirani pressure value
  String quickData;       // Variable to hold quick data acquisition string
};

// Instantiate the command struct
SensorCommands commands;

// Function to send a command to the sensor and read the response
String communicateWithSensor(const char* command) {
  // Clear any existing data in the buffer
  while (Serial1.available()) {
    Serial1.read(); // Discard any available bytes
  }

  // Send the command to the sensor
  Serial1.print(command);

  // Wait for a short period to receive the response
  delay(100);

  // Initialize an empty string to hold the response
  String response = "";
  
  // Read the response from the sensor
  while (Serial1.available() > 0) {
    char ch = Serial1.read(); // Read a character from the buffer
    response += ch; // Append the character to the response string
  }

  return response; // Return the complete response
}

// Function to extract pressure value from the response
float extractPressureValue(String response) {
  int startIdx = response.indexOf("ACK") + 3; // Find the start index of the pressure value
  int endIdx = response.indexOf('\\', startIdx); // Find the end index of the pressure value
  String pressureStr = response.substring(startIdx, endIdx); // Extract the pressure value substring
  return pressureStr.toFloat(); // Convert the pressure value to a float and return it
}

void setup() {
  // Initialize Serial Monitor for debugging
  Serial.begin(9600);
  
  // Wait for the serial monitor to open (useful for Leonardo, Due, etc.)
  while (!Serial) {
    // Wait here until the serial monitor is opened
  }
  Serial.println("Serial Monitor started"); // Print a message indicating the serial monitor has started

  // Initialize RS232 Serial communication
  Serial1.begin(9600); // Start Serial1 at 9600 baud rate (adjust if necessary for your sensor)
}

void loop() {
  // Create an instance of the PressureValues struct to hold the readings
  PressureValues pressureValues;

  // Read combined pressure from the sensor
  String response = communicateWithSensor(commands.readCombinedPressure); // Send command and get response
  pressureValues.combinedPressure = extractPressureValue(response); // Extract and store the combined pressure value

  // Read Piezo pressure from the sensor
  response = communicateWithSensor(commands.readPiezoPressure); // Send command and get response
  pressureValues.piezoPressure = extractPressureValue(response); // Extract and store the Piezo pressure value

  // Read Pirani pressure from the sensor
  response = communicateWithSensor(commands.readPiraniPressure); // Send command and get response
  pressureValues.piraniPressure = extractPressureValue(response); // Extract and store the MEMS Pirani pressure value

  // Read quick data acquisition from the sensor
  response = communicateWithSensor(commands.quickDataAcquisition); // Send command and get response
  pressureValues.quickData = response; // Store the quick data acquisition string

  // Print all pressure values and quick data on one line
  Serial.print("Combined Pressure: ");
  Serial.print(pressureValues.combinedPressure); // Print the combined pressure value
  Serial.print(" | Piezo Pressure: ");
  Serial.print(pressureValues.piezoPressure); // Print the Piezo pressure value
  Serial.print(" | Pirani Pressure: ");
  Serial.print(pressureValues.piraniPressure); // Print the MEMS Pirani pressure value
  Serial.print(" | Quick Data: ");
  Serial.println(pressureValues.quickData); // Print the quick data acquisition string

  // Add a delay before the next request to avoid flooding the sensor with requests
  delay(2000); // Delay for 2 seconds
}
```

### Using the Serial Monitor

1.    **Open Serial Monitor**:
        After uploading the sketch, open the Serial Monitor in the Arduino IDE.
        Go to Tools > Serial Monitor or click the Serial Monitor icon in the toolbar.

2.    **Set Baud Rate**:
        Ensure the baud rate at the bottom right of the Serial Monitor window is set to 9600 to match the baud rate in the sketch.

3.    **View Output**:
        The Serial Monitor will display the combined pressure, Piezo pressure, MEMS Pirani pressure, and quick data acquisition values in a formatted line every 2 seconds.

### Summary

This sketch facilitates the communication between an Arduino DUE and a Digivac DPP Quantum Pressure Sensor using RS232. It reads multiple pressure values from the sensor and displays them in the Serial Monitor, providing a simple and effective way to monitor the sensor's data in real-time. Adjust the baud rate and serial commands as necessary to match your specific sensor configuration.