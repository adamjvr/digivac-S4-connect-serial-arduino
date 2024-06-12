#include <Arduino.h>

// Define the struct to hold sensor commands
struct SensorCommands {
  const char* initS4Connect = "@254S4-Connect\\"; // Assumed command to enter S4-Connect mode
  const char* readCombinedPressure = "@253P?\\";
  const char* readPiezoPressure = "@253P?PZ\\";
  const char* readPiraniPressure = "@253P?MP\\";
  const char* quickDataAcquisition = "@253Q?\\";
};

// Instantiate the command struct
SensorCommands commands;

// Function to send a command to the sensor and read the response
String communicateWithSensor(const char* command) {
  // Clear any existing data in the buffer
  while (Serial1.available()) {
    Serial1.read();
  }

  // Send the command to the sensor
  Serial1.print(command);

  // Wait for a short period to receive the response
  delay(100);

  // Read the response from the sensor
  String response = "";
  while (Serial1.available() > 0) {
    char ch = Serial1.read();
    response += ch;
  }

  return response;
}

// Function to extract pressure value from the response
float extractPressureValue(String response) {
  int startIdx = response.indexOf("ACK") + 3;
  int endIdx = response.indexOf('\\', startIdx);
  String pressureStr = response.substring(startIdx, endIdx);
  return pressureStr.toFloat();
}

void setup() {
  // Initialize Serial Monitor for debugging
  Serial.begin(9600);
  while (!Serial) {
    // Wait for the serial monitor to open (useful for Leonardo, Due, etc.)
  }
  Serial.println("Serial Monitor started");

  // Initialize RS232 Serial communication
  Serial1.begin(9600); // Adjust the baud rate according to your sensor's specification

  // Put the sensor in S4-Connect mode using the global address
  Serial.println("Initializing sensor to S4-Connect mode...");
  String initResponse = communicateWithSensor(commands.initS4Connect);
  Serial.println("Initialization Response: " + initResponse);

  // Check if the initialization was successful
  if (initResponse.indexOf("ACK") == -1) {
    Serial.println("Failed to enter S4-Connect mode. Check the initialization command.");
  } else {
    Serial.println("Successfully entered S4-Connect mode.");
  }
}

void loop() {
  // Read combined pressure from the sensor
  String response = communicateWithSensor(commands.readCombinedPressure);
  Serial.println("Sensor Response: " + response);

  // Extract and print the pressure value
  float pressure = extractPressureValue(response);
  Serial.print("Combined Pressure: ");
  Serial.println(pressure);

  // Add a delay before the next request
  delay(2000);
}
