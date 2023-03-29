#include <Arduino.h>
#include <DHT.h> //DHT22센서를 사용하기위하여 라이브러리를 불러오기

DHT dht(8, DHT22); // 8번 PIN에서 데이터가 들어오고, 센서는 DHT22 센서를 사용한다고 정의

void setup() {
  Serial.begin(9600); // PC모니터를 이용하기 위하여, 시리얼통신을 정의
  dht.begin(); // DHT22센서의 사용시작을 정의
}

void loop() {
  delay(2000); // 측정 2초 딜레이

  float h = dht.readHumidity();  // 습도값을 읽어온후 h변수값에 값을 저장 
  float t = dht.readTemperature(); // 온도값을 읽어온후 t변수값에 값을 저장 
  float hic = dht.computeHeatIndex(t, h, false); // DHT22는 열지수(체감온도)를 나타내줄수있는 함수 포함

  Serial.print("Humidity: ");
  Serial.print(h); // 습도 출력
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t); // 온도 출력
  Serial.print(" °C\t");
  Serial.print("Heat index: ");
  Serial.print(hic); // 열지수(체감온도) 출력
  Serial.println(" °C ");
} 
