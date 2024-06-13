#include <Arduino.h>

// Define the struct to hold sensor commands
struct SensorCommands {
  const char* readCombinedPressure = "@253P?\\"; // Command to read combined pressure
  const char* readPiezoPressure = "@253P?PZ\\"; // Command to read Piezo pressure
  const char* readPiraniPressure = "@253P?MP\\"; // Command to read MEMS Pirani pressure
  const char* quickDataAcquisition = "@253Q?\\"; // Command to read quick data acquisition
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
