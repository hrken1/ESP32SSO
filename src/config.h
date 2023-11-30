#ifndef CONFIG_H
#define CONFIG_H

// Pin Definitions
const int powerSwitchPin = 2;   // GPIO pin for the power toggle switch
const int brewSwitchPin = 3;    // GPIO pin for the brew toggle switch
const int steamSwitchPin = 4;   // GPIO pin for the steam toggle switch
const int ssrPin = 5;           // GPIO pin to control the SSR for the brewing element
const int brewPumpRelayPin = 6; // GPIO pin to control the Brew pump relay

// Temperature Thresholds
const float brewTemp = 90.0;    // Target brew temperature in degrees Celsius
const float steamTemp = 150.0;  // Target steam temperature in degrees Celsius
const float maxTemp = 160.0;    // Maximum safe operating temperature in degrees Celsius

// PID Tuning Parameters
// Note: These values will need fine-tuning based on your system's characteristics
double Kp = 2.0; // Proportional gain
double Ki = 5.0; // Integral gain
double Kd = 1.0; // Derivative gain

// Constants
const int BAUD_RATE = 9600;         // Baud rate for serial communication
const int PID_OUTPUT_MIN = 0;       // Minimum output limit for PID controller
const int PID_OUTPUT_MAX = 255;     // Maximum output limit for PID controller (matches ESP32 PWM range)

const unsigned long MAX_BREW_DURATION = 60000; // Define the brew duration in milliseconds

#endif // CONFIG_H
