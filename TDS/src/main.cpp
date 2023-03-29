#include <EEPROM.h>
#include "GravityTDS.h"
// #include <OneWire.h> 
// #include <DallasTemperature.h>
 
// #define ONE_WIRE_BUS 7 // Digitalpin where Temp sensor is connected
#define TdsSensorPin A1 // Where Analog pin of TDS sensor is connected to arduino
 
// OneWire oneWire(ONE_WIRE_BUS); 
GravityTDS gravityTds;
 
// DallasTemperature sensors(&oneWire);
 
float tdsValue = 0;
 
void setup()
{
Serial.begin(9600);
// sensors.begin();
gravityTds.setPin(TdsSensorPin);
gravityTds.setAref(5.0); //reference voltage on ADC, default 5.0V on Arduino UNO
gravityTds.setAdcRange(1024); //1024 for 10bit ADC;4096 for 12bit ADC
gravityTds.begin(); //initialization
}
 
void loop()
{
// sensors.requestTemperatures();
 
// gravityTds.setTemperature(sensors.getTempCByIndex(0)); // grab the temperature from sensor and execute temperature compensation
gravityTds.update(); //calculation done here from gravity library
tdsValue = gravityTds.getTdsValue(); // then get the TDS value
 
Serial.print("TDS value is:");
Serial.print(tdsValue,0);
Serial.println("ppm");
// Serial.print("Temperature is: "); 
// Serial.print(sensors.getTempCByIndex(0));
 
delay(1500);
 
}