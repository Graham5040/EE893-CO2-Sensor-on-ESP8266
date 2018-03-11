# EE893-CO2-Sensor-on-ESP8266
EE893 Sensor has arduino example with Atmel registers to lower clock speed  that doesn't work on ESP devices
Orignal Arduino library by blueberrye-io, https://github.com/blueberrye-io/BB-EE893

this file is based on the Arduino example and changed to Swire libray. Setting Wire.setClock() to low rates e.g. 5000 causes the ESP8266 to reset so use alternative SWire library by RaemondBW at https://github.com/RaemondBW/SWire/archive/master.zip
that has delay control to stretch pulses for I2C.

changes made:

change "#include <Wire.h>" to "#include <SWire.h>".

Replace all instances of "Wire" from your project with "SWire" calls.

Replace your "Wire.begin()" with "SWire.begin(SDA, SCL), SWire defaults to pins 6,7. 
so change values to 4,5 2,0 21,22 etc depending on ESP used. You can use most pins.

Remark out the TWBR TWSR lines.
Add "SWire.delay_time_us = 820;" after "SWire.begin()" to slow bus speeds down. 

testing shows "Returncode 0" is seen on Serial to show valid reading. if 2 or 4 seen then increase SWire.delay_time_us as necessay
my device was OK at 250 but set value to 820 to match Arduino timings.

Example ouput

Next Measurement
Returncode 0
CO2 [ppm] fast mode 704
Returncode 0
CO2 [ppm] accurate mode 685

There is some error just under 1% when Returncode is 2, i2c data not available.
 
Don't forget that the EE893 is 5V to 7V so need i2C level shifters to the ESP.
