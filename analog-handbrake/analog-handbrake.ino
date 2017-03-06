#include <Joystick.h>

/* Analog Controller Input for PC
 * ------------------ 
 *
 * Simulates an analog controller input by reading an analog value from a potentiometer 
 * and sending the value via the joystick library (based on the HID library) 
 * to the PC. Beside this the LED on the arduino blinks with a frequency 
 * dependent on the current analog value.
 *
 * This can be mapped in games to an input function (e.g. as an analog
 * handbrake in games like Dirt Rally, Dirt 4, Asseto Corsa, ...)
 * 
 * Based on the idea of AMSTUDIO 
 * (https://github.com/AM-STUDIO/Analog-E-Brake)
 *
 * Works with Arduino Leonardo and Arduino/Genuino Micro.
 *
 * Currently only one signal is send to PC. It might be useful
 * to send the analog value with different mappings as independent
 * controller values (e.g. a signal with linear mapping for an analog 
 * input signal of a games and a jump function mapping for a on/off signal. 
 *
 * This work is licensed under a Creative Commons 
 * Attribution-ShareAlike 4.0 International (CC BY-SA 4.0). 
 * https://creativecommons.org/licenses/by-sa/4.0/ 
 *
 */

/* Use this value to adapt to the actual used range of analog value (e.g. to adapt
 *  to physically available range of the poti), if the full range
 *  can be used (e.g. the poti slider is not physically blocked), the min value might be 
 *  choosen to be 0 - however, you might want to add some tolerance 
 *  (e.g. if a brake should not start with the slightest movement of brake/value) 
 *  Analog input values lower than min are considered as min input.
*/
const int minValue = 30; // min of the actual used poti range, if not blocked
/*
 * Use to adapt to the maximal available range of analog value. Should be max 1023. 
 * Analog input values greater than max are handled as max input.
 */
const int maxValue = 1000; // max of the actual used poti range

const int potPin = A0;    // select the input pin for the potentiometer
const int changeThreshold = 1; //Can be used to reduce the frequency of send operations when the signal is unchanged

// Create Joystick, disable anything than the brake which is used as handbrake
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_JOYSTICK,
    0, // JOYSTICK_DEFAULT_BUTTON_COUNT
    0, // JOYSTICK_DEFAULT_HATSWITCH_COUNT,
    false, // includeXAxis
    false, // includeYAxis
    false, // includeZAxis
    false, // includeRxAxis
    false, // includeRyAxis
    false, // includeRzAxis
    false, // includeRudder
    false, // includeThrottle
    false, // includeAccelerator
    true, // includeBrake
    false); // includeSteering

const int ledPin = LED_BUILTIN;   // select the pin for the LED    
int value = 0;       // variable to store the value coming from the sensor
int delayCounter = 0;
int delayThreshold = 0;
int ledState = HIGH;
int oldValue = 0;
unsigned long previousMillisBlink = 0;        // will store last time LED was updated

void setup() {
  Serial.begin(9600);      // open the serial port at 9600 bps
  pinMode(ledPin, OUTPUT);  // declare the ledPin as an OUTPUT
  Joystick.setBrakeRange(minValue, maxValue);
  Joystick.begin();
  digitalWrite(ledPin, ledState);  // turn the ledPin on
  oldValue = -abs(changeThreshold)-1; //force send the very first time
}

void loop() {
  
  value = analogRead(potPin);    // read the value from the sensor
  unsigned long currentMillisSend = millis();
  //if(abs(value - oldValue) > 1 || currentMillisSend - previousMillisSend > sendInterval){
  if(abs(value-oldValue) > changeThreshold){
    //Serial.print("\nSend: ");
    //Serial.print(value);
    Joystick.setBrake(value);
    oldValue = value;
    //previousMillisSend = currentMillisSend;
  }
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  // float voltage = val * (5.0 / 1023.0);
  // Serial.print(voltage)

  unsigned long currentMillisBlink = millis();
  if((value < minValue) || (value > maxValue)){
    // set the LED on to show out of range
    digitalWrite(ledPin, LOW);
  }
  else{
    unsigned long blinkInterval = maxValue - value;
    if (currentMillisBlink - previousMillisBlink > blinkInterval) {
      // save the last time you blinked the LED
      previousMillisBlink = currentMillisSend;
      
      // if the LED is off turn it on and vice-versa:
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }

      // set the LED with the ledState of the variable:
      digitalWrite(ledPin, ledState);
    }
  }
}
 
