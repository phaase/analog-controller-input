#include <Joystick.h>

/* Analog Controller Input for PC
 * ------------------ 
 *
 * Realizes a gaming controller with one analog input value by reading 
 * an analog value from a potentiometer and sending the value via the 
 * joystick library (based on the HID library) to the PC.
 * Beside this the LED on the arduino blinks with a frequency 
 * dependent on the current analog value.
 *
 * The gaming controller input can be mapped in games to an input function (e.g. as an analog
 * handbrake in games like Dirt Rally, Dirt 4, Assetto Corsa, ...)
 * 
 * Based on the idea of AMSTUDIO 
 * (https://github.com/AM-STUDIO/Analog-E-Brake)
 *
 * Works with Arduino Leonardo and Arduino/Genuino Micro.
 *
 *
 * This work is licensed under a Creative Commons 
 * Attribution-ShareAlike 4.0 International (CC BY-SA 4.0). 
 * https://creativecommons.org/licenses/by-sa/4.0/ 
 *
 */

/* Use this to print out the analog and derived digital values to the serial monitor.
 * Use the digital values for calibration by setting minValue and maxValue below. 
 * Do not use this mode in regular use, since the print commands will slow down
 * the reaction significantly.
 */
//#define CALIBRATION_MODE

/* Use this value to adapt to the actual used range of output values (e.g. to adapt
 *  to the physically available range of the poti), if the full range
 *  can be used (e.g. the poti slider is not physically blocked), the min value might be 
 *  choosen to be 0 - however, you might want to add some tolerance 
 *  (e.g. if a brake should not start with the slightest movement of brake/value) 
 *  Analog input values lower than min are considered as min input.
*/
const int minValue = 295; // min of the actual usable poti range, if not blocked
/*
 * Use to adapt to the maximal available range of analog value. Should be max 1023. 
 * Analog input values greater than max are handled as max input.
 */
const int maxValue = 900; // max of the actual usable poti range

/*  Use this to set the time in milli seconds after which a send operation is forced even if the value has not changed
 *  or, to be more precisely, if the change is below changeTolerance (see below).
 */
const unsigned long maxQuietTime = 5000; //time after that send operation is called (indirectly by calling setYAxis) even when value has not changed

/* Use this to set the sensitiy of the controller. Setting the value to 0 means no tolerance = max sensity = max send operations, 
 * setting to 1 means high sensity but some tolerance against jitter between two values and hence might reduce the frequency of 
 * send operations.
 */
const int changeTolerance = 1; //set and sent new value when change of value > changeTolerance

const int potPin = A0;    // select the input pin for the potentiometer
const int ledPin = LED_BUILTIN;   // select the pin for the LED  

// Create Joystick, disable anything than the Y-axis (using 'brake' does not work with project cars) which you can map in a game as whatever you want
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_JOYSTICK,
    0, // JOYSTICK_DEFAULT_BUTTON_COUNT
    0, // JOYSTICK_DEFAULT_HATSWITCH_COUNT,
    false, // includeXAxis
    true,  // includeYAxis
    false, // includeZAxis
    false, // includeRxAxis
    false, // includeRyAxis
    false, // includeRzAxis
    false, // includeRudder
    false, // includeThrottle
    false, // includeAccelerator
    false, // includeBrake
    false); // includeSteering
  
int ledState = HIGH;
int oldValue = 0;

unsigned long previousMillisBlink = 0; //stores last time LED was updated
unsigned long previousMillisSend = 0; //stores last time send operation was called (indirectly by calling setYAxis)

void setup() {
  Serial.begin(9600);      // open the serial port at 9600 bps
  pinMode(ledPin, OUTPUT);  // declare the ledPin as an OUTPUT
  Joystick.setYAxisRange(minValue, maxValue);
  Joystick.begin();
  digitalWrite(ledPin, ledState);  // turn the ledPin on
  oldValue = -abs(changeTolerance)-1; //force send the very first time
}

void loop() {

  unsigned long currentMillis = millis();
  int value = analogRead(potPin);    // read the value from the sensor
  if((currentMillis-previousMillisSend > maxQuietTime) || (abs(value-oldValue) > changeTolerance)){
    Joystick.setYAxis(value);
    oldValue = value;
    previousMillisSend = currentMillis;
    #ifdef CALIBRATION_MODE
      Serial.print("Voltage: ");
      Serial.print(value * (5.0 / 1023.0)); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
      Serial.print(" Digital value: ");
      Serial.print(value);
      Serial.print('\n');
      Serial.flush();
    #endif
  }

  if(value < minValue){
    // set the LED off to show value below active range
    digitalWrite(ledPin, LOW);
  }
  else if(value > maxValue){
    // set the LED on to show value above active range
    digitalWrite(ledPin, HIGH);
  }
  else{
    unsigned long blinkInterval = maxValue - value;
    if (currentMillis - previousMillisBlink > blinkInterval) {
      // if the LED is off turn it on and vice-versa:
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }
      digitalWrite(ledPin, ledState);
      previousMillisBlink = currentMillis;
    }
  }
}
 
