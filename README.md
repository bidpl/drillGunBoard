# 12V Motor Testing Rig

Code for FRC control board. Manual control FRC motors and air compressor.

We used 12v drills to power 12v FRC motors, (CIMs, BAGs, etc) by removing drill spindle and connecting to motor leads. However, it can not supply large amounts of power. It can not drive Neo motors due to their brushless nature. 

This code is for a meant for a test rig that take a pot input and controls PWM motor controlers (Talon SRXs and Spark Maxes are used in this build).

Features:
Control multiple motors (up to 4) (Control moudes v1.1 and v1.2 have support for 5)
LCD readout for motor power

Documentation is a bit scuffed, I only used the repo while developing the first version (v1.0), didn't for later iterations (v1.1/1.2)

v1.0 was based on an arduino uno with regular potentiometers. The arduino by itself was limited to Talon SRXs since it could not generate a stable enough PWM signal for the Spark Max. This version used regular pin headers and cheap dupont wires so it suffered some durability issues (connections were terrible, shaking the testing rig could disconnect parts).

v1.1/1.2 is uses a custom control module (based on an ATMEGA32P, programs exactly like an UNO) that has an additional PWM driver chip (PCA9685). This chip can generate a proper 50hz PWM signal so it can control Spark Maxes as well. This version also uses locking connectors (JST-XH between input modules and the control module, Molex SL connectors for control module to motor controllers).

## PCB Modules
![Control Module v1.1/v1.2](/images/controlModulev1-1.png)
![Input Module v1.1](/images/inputModule-v1-1.png)

## Testing Rig (The Bomb, name inspired from Die Hard with a Vengeance)
![Testing Rig Front View](/images/bomb-frontView.jpg)
![Testing Rig Back View](/images/bomb-backView.jpg)

## Change Log
Input Module v1.1:
- Switched from rotate to linear potentiometer
- Switched from horizontal to vertical PCB shape

Control Module v1.2:
- Fixed PCA9685 CE signal (supposed to be active low, was tied high in prev version)

Control Module v1.1:
- Moved from arduino to custom PCB
- Added PCA9685 as PWM signal generator, arduino couldn't get a clean enough PWM signal for some motor controllers (Controlling the LCD delayed the signal too much)
- Switched from loose dupont wire to locking Molex-SL and JST-XH connectors
