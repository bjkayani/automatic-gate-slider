# Automatic Gate Slider
Controller for an Arduino based Automatic Gate Slider

## Background
This project was started becasue I was looking for an afforable solution to automate the movement of my sliding gate. When I was unable to find one, I decided to build it myself. This is the code for the controller that is the brains of the project. 

## Functionality
The operation is quiet simple actually, the controller recieves input from either a 433 Mhz remote or manual switches. Based on the input, it moves the gate and stops when it hits the limit switches. I have hard coded the RF codes for the remotes I am using. I also have some code for the case when the gate is locked. The controller senses voltage and current to avoid damage in case of a jammed gate or low battery. This motor is controlled using a 4 Relay H Bridge module. 

## Code Files
|File Name|Function|
|-|-|
|automatic-gate-slider|This is the file containing the main setup and control loop|
|io|This file contains the pin definitions. Modify this for your wiring setup|
|variables|This file contains variable definitions|
|motor_control|This file contains functions to operate the motor|
|measure|This file contrins functions to measure the voltage and current|

## Future Work
- Change to Switch Case statement for the state machine
- Create arrays for the RC codes
- Add support for PWM motor driver with ramp up and down

More details and build instructions can be found on [Instructables](https://www.instructables.com/id/Automatic-Gate-Slider-Under-100/)


