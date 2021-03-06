//EE893 CO2 sensor on ESP

//change #include <Wire.h> to #include <SWire.h>.
//Replace all instances of "Wire" from your project with "SWire" calls.
//Replace your "Wire.begin()" with "SWire.begin(SDA, SCL), defaults dangerously to pins 6,7. 
//so change values to 4,5 2,0 21,22 etc depending on ESP used. You can use most pins.
//Remark out TWBR TWSR lines.
//Add "SWire.delay_time_us = 250;" after SWire.begin() to slow bus speeds down. 

// ensure "Returncode 0" is seen on Serial to show valid reading. 
//Increase SWire.delay_time_us if necessay

/*
Next Measurement
Returncode 0
CO2 [ppm] fast mode 704
Returncode 0
CO2 [ppm] accurate mode 685
 */
 
#include "SWire.h" //https://github.com/RaemondBW/SWire/archive/master.zip

byte EE893_read_byte(byte control_byte, byte *result)
{
  byte control, dev_addr, i2c_addr, checksum;

  control = control_byte;
  dev_addr = 0x00;

  // Send Control Byte
  i2c_addr = ((control) | (dev_addr << 1)) >> 1;


  //Wert lesen:
  SWire.requestFrom(i2c_addr, 2); //change Wire to SWire
  if (SWire.available()) {
    *result = SWire.read(); // data byte 
    checksum = SWire.read();

    if (checksum != ((*result + control) % 0x100)) {
       return 1;
     }
  }
  else {
    return 2;
  }
  return 0;
}


byte EE893_read_co2(byte *hi, byte *lo, boolean slow)
{
  byte result, result_byte;

  if (slow) {
    result = EE893_read_byte(0xE1, lo);
    if (result != 0) {
      return result;
    }
    result = EE893_read_byte(0xF1, hi);
    if (result != 0) {
      return result;
    }
  } else {
    result = EE893_read_byte(0xC1, lo);
    if (result != 0) {
      return result + 2;
    }
    result = EE893_read_byte(0xD1, hi);
    if (result != 0) {
      return result + 2;
    }
  }
  return 0;
}

void setup() {
  // put your setup code here, to run once:
  SWire.begin(4, 5); // wake up I2C bus, (SDA, SCL) 
  SWire.delay_time_us = 820; //stretch pulses to make it to work
  
  //remove the Atmel register changes
  //TWBR = 200;  // set the TWBR register (bypass wire library) reduce the speed about 3 times
  //TWSR |= bit (TWPS0); //changes prescaler divide ratio
  //TWSR |= bit (TWPS1); //changes prescaler divide ratio
  
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  byte hi, lo;
  Serial.println("\n\nNext Measurement");  

  Serial.print("Returncode ");
  Serial.println(EE893_read_co2(&hi , &lo, false));
  delay(10000);

  //Serial.print(hi, HEX); Serial.print(" "); Serial.println(lo, HEX);
  Serial.print("CO2 [ppm] fast mode "); Serial.println(hi * 256 + lo);

  Serial.print("Returncode ");
  Serial.println(EE893_read_co2(&hi , &lo, true));
  delay(20000);

  //Serial.print(hi, HEX); Serial.print(" "); Serial.println(lo, HEX);
  Serial.print("CO2 [ppm] accurate mode "); Serial.println(hi * 256 + lo);
  
}
