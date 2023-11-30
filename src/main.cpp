#include <Arduino.h> 
#include <PID_v1.h>
#include "config.h"



// PID Variables
double setPoint, input, output;
PID myPID(&input, &output, &setPoint, Kp, Ki, Kd, DIRECT);

// Other variables
float currentTemp = 0.0;
unsigned long brewStartTime = 0;

// Function prototypes
float readTemperature(); // Implement this function to read temperature from your sensor
void updateDisplay(const String& message); // Implement this for Nextion display updates
void handleBrewProcess();
void handleSteamProcess();
void handlePowerSwitch();

void setup() {
  // Initialize the pins
  pinMode(powerSwitchPin, INPUT);
  pinMode(brewSwitchPin, INPUT);
  pinMode(steamSwitchPin, INPUT);
  pinMode(ssrPin, OUTPUT);
  pinMode(brewPumpRelayPin, OUTPUT);

  // Initialize serial communication for debugging
  Serial.begin(BAUD_RATE);

  // Initialize PID controller
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(PID_OUTPUT_MIN, PID_OUTPUT_MAX);

  // Initialize other components (display, temperature sensor, etc.)
}

void loop() {
  currentTemp = readTemperature(); // Read the current temperature
  if (currentTemp < 0) return; // Error reading temperature
  input = currentTemp;

  // Safety Check: Over-temperature
  if (currentTemp > maxTemp) {
    digitalWrite(ssrPin, LOW); // Turn off the heating element
    updateDisplay("Error: Over-Temperature!");
    return; // Skip further processing
  }

  // Power switch handling
  handlePowerSwitch();
}

void handleBrewProcess() {
  static unsigned long brewStartTime = millis(); // Record start time of brewing
  static bool isBrewing = false; // Track if brewing process has started

  if (!isBrewing) {
    digitalWrite(brewPumpRelayPin, HIGH); // Activate the brew pump
    isBrewing = true;
    brewStartTime = millis(); // Reset start time
    updateDisplay("Brewing...");
  }

  // Check if brewing duration is complete or if a stop condition is met
  if (millis() - brewStartTime > MAX_BREW_DURATION || digitalRead(brewSwitchPin) == LOW) {
    digitalWrite(brewPumpRelayPin, LOW); // Deactivate the brew pump
    isBrewing = false;
    updateDisplay("Brew Complete. Enjoy Your Coffee!");
  } else {
    // Continue brewing and update display with current temperature
    updateDisplay("Brewing... Temp: " + String(currentTemp) + " C");
  }

  // Continuously maintain brew temperature using PID
  setPoint = brewTemp;


  myPID.Compute();
  analogWrite(ssrPin, output);
}


void handleSteamProcess() {
  // Implement the logic for the steam process
  // Example: Increase temperature, update display
}

float readTemperature() {
  // Implement temperature reading from your sensor
  float temp = 0.0; // Placeholder return value

  if (isnan(temp)) {
    Serial.println("Failed to read from temperature sensor!");
    return -1;
  }

  return temp;
}

void updateDisplay(const String& message) {
  // Implement display update logic
  // Example: Send message to Nextion display
}

void handlePowerSwitch() {
  if (digitalRead(powerSwitchPin) == HIGH) {
    // Set the appropriate setpoint based on the mode
    if (digitalRead(brewSwitchPin) == HIGH) {
      setPoint = brewTemp;
      handleBrewProcess();
    } else if (digitalRead(steamSwitchPin) == HIGH) {
      setPoint = steamTemp;
      handleSteamProcess();
    } else {
      setPoint = brewTemp; // Default to brew temp
    }

    // Compute PID
    myPID.Compute();
    analogWrite(ssrPin, output); // Adjust this line to suit how you control the SSR

    // Update display with current temperature and state
    updateDisplay("Temp: " + String(currentTemp) + " C");
  } else {
    // Power is off
    digitalWrite(ssrPin, LOW); // Ensure the heater is off
    digitalWrite(brewPumpRelayPin, LOW); // Ensure the pump is off
    updateDisplay("Power Off");
  }
}