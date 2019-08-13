# Arduino PID test code for XD37GB3525 BLDC
This is the PID test code by Arduino(tested on Arduino Leonardo) for [XD-37GB3525 BLDC(Brushless DC Motor)](https://item.taobao.com/item.htm?id=596439838484) with built-in driver using Arduino PID library developed by [Brett Beauregard](https://github.com/br3ttb/Arduino-PID-Library).

Connection(Note: the voltage level on motor is 5V compatible. Need 5V-3.3V level convertor when using micro-controller with 3.3V GPIO, such as esp32/esp8266 or most ARM MCUs):

1. Main DC Power: connect DC 12V power supply with 5521 plug type to Arduino Leonardo's on-board 5521 DC power barrel socket.
2. Motor<->Arduino Leonardo:
- Power: Connect power wire(RED and BLACK) on motor to Vin and GND pin on Arduino Leonardo: RED<->Vin / BLACK<->GND.
- GPIO:
  - PWM for RPM control: Connect RPM control wire(BLUE) on motor to pin D9 on Arduino Leonardo: BLUE<->D9.
  - Speed sensor data reading: Connect speed sensor data wire(YELLOW) on motor to pin D7 on Arduino Leonardo: YELLOW<->D7.
  - Direction control wire: White wire. Not used for this test. Could be connected with any digital pin on Arduino Leonardo.
  
Usage guide:
- After upload the code to Arduino Leonardo with Arduino IDE, open up Serial Monitor(Ctrl+Shift+M) in IDE and input the targeted RPM with a 1/10 factor, i.e. input 20 for a targetd speed of 200RPM.
