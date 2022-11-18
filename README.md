# Mo-TLab
This repository is dedicated to UAV projects in the Motion and Teaming Lab
* The <ins>**dual_rotor**</ins> directory includes the main files used to test, analyze, and tune a 1 variable feedback controller.\
  Different controllers can be used by commenting out P, I, and D terms or changing the orders of operation.\
  Note that since rotary encoders are used, we had two MCUs, one specifically for interrupts, that communicates with I2C. Thus, libraries needed to run the script include <ins>**wire.h**</ins>, <ins>**Servo.h**</ins> (or one can customize PWM generation), and <ins>**JY901.h**</ins> (IMU communicates through Serial).\
  Also note that to run readSerial.py, <ins>**MatPlotLib**</ins> and <ins>**pySerial**</ins> need to be installed.
* The <ins>**models**</ins> directory includes SolidWorks and STL files used to 3D print the test stands.\
