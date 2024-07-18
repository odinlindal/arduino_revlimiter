## Engine Rev Limiter with Launch Control for Toyota 3S-GT Engine ##

This project is an adaptation of korn101's work, 

modified by odinlindal to include adjustments via potentiometer and car-specific optimizations.

This code is designed to manage engine rev limiting and launch control for a Toyota 3S-GT engine.


### Features ###

* Adjustable rev limit via potentiometer.

* Adjustable spark cut harshness via potentiometer.

* Real-time RPM monitoring and spark cut management.

* Launch control functionality with a dedicated button.


### Hardware Requirements ###

* Arduino (tested with Arduino Uno)

* Potentiometers (2x)

* Relay module

* Push button (for launch control)

* LEDs (optional for visual indication)

* Wiring components (breadboard, jumper wires, etc.)

* [Wiring Diagram](https://github.com/user-attachments/assets/f0d0a76b-304c-4451-8c8e-292b17e0336e)


### Installation ###

***Clone the Repository:***

https://github.com/odinlindal/arduino_revlimiter.git

*or*

Download the [revlimiter.ino file](https://github.com/odinlindal/arduino_revlimiter/blob/main/revlimiter.ino)

***Upload the Code to Arduino:***

Open the .ino file in the Arduino IDE.

Select the correct board and port from the Tools menu.

Click on the upload button to upload the code to your Arduino.


***Connect the Hardware:***

Follow the provided wiring diagram to connect the hardware components to the Arduino.

Ensure the potentiometers, relay, button, and LEDs are correctly connected to the specified pins.


### Code Overview ###

The code includes several key functionalities:

***RPM Measurement:*** Using interrupts to measure the time between pulses from the engine, calculating the RPM.

***Rev Limit Adjustment:*** The rev limit can be adjusted using a potentiometer connected to A0.

***Spark Cut Harshness:*** The harshness of the spark cut can be adjusted using a potentiometer connected to A1.

***Launch Control:*** The button connected to lcButton enables launch control, momentarily limiting the RPM.

### Usage ###

***Adjust Rev Limit:***

Turn the potentiometer connected to A0 to set the desired rev limit. The rev limit is mapped from 1500 to 4000 RPM(this can be customized in line 64 of the code, by changing the two last parameters).

***Adjust Spark Cut Harshness:***

Turn the potentiometer connected to A1 to set the harshness of the spark cut. The harshness factor is mapped from 1.0 to 5.0(this can be customized in line 69 of the code, by changing the two last parameters).

***Activate Launch Control:***

Press and hold the launch control button to enable the RPM limit for launch control. The RPM will be limited to the set rev limit while the button is held.

### Troubleshooting ###

***RPM Reading Issues:***

* Ensure the interrupt pin is correctly connected to the RPM signal from the engine.

* Verify the potentiometers are functioning and connected correctly.

***Launch Control Not Activating:***

* Check the connection of the launch control button.

* Verify the button state in the code using Serial.println() for debugging.


### License ###

This project is licensed under the MIT License - see the LICENSE file for details.


### Acknowledgments ###

Original code by korn101.

Adaptations and optimizations by odinlindal.
