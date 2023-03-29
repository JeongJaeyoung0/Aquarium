#include <Arduino.h>
#include <swRTC.h> // 날짜
#include <DHT.h> // DHT22(기체 온습도)
#include <OneWire.h> // DS18B20(수온)
#include <DallasTemperature.h> // DS18B20(수온)
#include <Wire.h> // GAOHOU(pH)
#include <SimpleTimer.h> // GAOHOU(pH)
#include <EEPROM.h> // TDS
#include "GravityTDS.h" // TDS

 
//// 개체 선언
// 날짜
swRTC rtc;

// DHT22
DHT dht(8, DHT22); // 8번 PIN에서 데이터가 들어오고, 센서는 DHT22 센서를 사용한다고 정의 

// DS18B20
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

// pH
float calibration_value = 21.34 + 1.16;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;
float ph_act;

// TDS
#define TdsSensorPin A1 // Where Analog pin of TDS sensor is connected to arduino
GravityTDS gravityTds;
float tdsValue = 0;


void setup()
{
    Serial.begin(9600);
    // sensors.begin();

    // 날짜
    rtc.stopRTC();            //정지
    rtc.setTime(10, 46, 10);  //시, 분, 초
    rtc.setDate(26, 5, 2018); //일, 월, 년
    rtc.startRTC();           //시작

    // DHT22
    dht.begin(); // DHT22센서의 사용시작을 정의

    // pH
    Wire.begin();

    // TDS
    gravityTds.setPin(TdsSensorPin);
    gravityTds.setAref(5.0); //reference voltage on ADC, default 5.0V on Arduino UNO
    gravityTds.setAdcRange(1024); //1024 for 10bit ADC;4096 for 12bit ADC
    gravityTds.begin(); //initialization
}
 
void loop()
{
    delay(2000); // 측정 2초 딜레이
    Serial.println("");

    // 날짜
    Serial.print(rtc.getYear(), DEC);
    Serial.print("/");
    Serial.print(rtc.getMonth(), DEC);
    Serial.print("/");
    Serial.print(rtc.getDay(), DEC);
    Serial.print(" ");
    Serial.print(rtc.getHours(), DEC);
    Serial.print(":");
    Serial.print(rtc.getMinutes(), DEC);
    Serial.print(":");
    Serial.println(rtc.getSeconds(), DEC);

    // DHT22
    float h = dht.readHumidity();  // 습도값을 읽어온후 h변수값에 값을 저장 
    float t = dht.readTemperature(); // 온도값을 읽어온후 t변수값에 값을 저장 
    float hic = dht.computeHeatIndex(t, h, false); // DHT22는 열지수(체감온도)를 나타내줄수있는 함수 포함
    Serial.print("상온: ");
    Serial.print(t); // 온도 출력
    Serial.print(" \xe2\x84\x83\t");
    Serial.print("체감온도: ");
    Serial.print(hic); // 열지수(체감온도) 출력
    Serial.print(" \xe2\x84\x83\t");
    Serial.print("습도: ");
    Serial.print(h); // 습도 출력
    Serial.println(" %");


    // DS18B20
    sensors.requestTemperatures(); 
    Serial.print("수온: ");
    Serial.print(sensors.getTempCByIndex(0));
    Serial.println(" \xe2\x84\x83");//shows degrees character
    // Serial.print("  ");
    // Serial.print((sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0);
    // Serial.println("\xe2\x84\x89");//shows degrees character


    // pH
    for(int i=0;i<10;i++) 
    { 
        buffer_arr[i]=analogRead(A0);
        delay(30);
    }
    for(int i=0;i<9;i++)
    {
        for(int j=i+1;j<10;j++)
        {
            if(buffer_arr[i]>buffer_arr[j])
            {
                temp=buffer_arr[i];
                buffer_arr[i]=buffer_arr[j];
                buffer_arr[j]=temp;
            }
        }
    }
    avgval=0;
    for(int i=2;i<8;i++)
    avgval+=buffer_arr[i];
    float volt=(float)avgval*5.0/1024/6; 
    ph_act = -5.70 * volt + calibration_value;

    Serial.print("pH: ");
    Serial.println(ph_act);


    // TDS
    gravityTds.update(); //calculation done here from gravity library
    tdsValue = gravityTds.getTdsValue(); // then get the TDS value
    Serial.print("TDS: ");
    Serial.print(tdsValue,0);
    Serial.println(" ppm");
}