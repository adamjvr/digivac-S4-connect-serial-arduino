#include <Arduino.h>

// Define the struct to hold sensor commands
struct SensorCommands {
  const char* readCombinedPressure = "@254P?\\";
  const char* readPiezoPressure = "@254P?PZ\\";
  const char* readPiraniPressure = "@254P?MP\\";
  const char* quickDataAcquisition = "@254Q?\\";
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
