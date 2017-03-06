# Analog Controller Input for PC
  ------------------ 
 
  Simulates an analog controller input by reading an analog value from a potentiometer 
  and sending the value via the joystick library (based on the HID library) 
  to the PC. Beside this the LED on the arduino blinks with a frequency 
  dependent on the current analog value.
 
  This can be mapped in games to an input function (e.g. as an analog
  handbrake in games like Dirt Rally, Dirt 4, Asseto Corsa, ...)
  
  Based on the idea of AMSTUDIO 
  (https://github.com/AM-STUDIO/Analog-E-Brake)
 
  Works with Arduino Leonardo and Arduino/Genuino Micro.
 
  Currently only one signal is send to PC. It might be useful
  to send the analog value with different mappings as independent
  controller values (e.g. a signal with linear mapping for an analog 
  input signal of a games and a jump function mapping for a on/off signal. 
 
