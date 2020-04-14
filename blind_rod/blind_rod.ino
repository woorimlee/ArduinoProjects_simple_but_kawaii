/****************************************************
<시각 장애인을 위한 장애물 감지 발열 지팡이>
1. 지팡이 하단부에 장착된 초음파 센서에서 장애물을 감지.
2. 장애물이 있다면 부저를 울려 경고를 준다.
3. 온습도 센서를 이용해 추운 날씨에 발열패드를 동작시킨다.
****************************************************/

#include "DHT.h"

#define RELAYPIN 3
#define DHTPIN 4
#define ECHOPIN 12
#define TRIGPIN 13
#define DHTTYPE DHT11
bool flag = true;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  pinMode(RELAYPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  pinMode(TRIGPIN, OUTPUT);
  dht.begin();
}

void loop() {
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);
  int duration = pulseIn(ECHOPIN, HIGH); 
  int distance = duration / 29 / 2;  
  
  Serial.print(distance);
  Serial.println("cm");
  if(distance <= 40) {
    tone(8, 261.63, 500);
  }
  
  float t = dht.readTemperature();

  if (isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  }
  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.println("%");

  if(t <= 25 && flag == true) {
    digitalWrite(RELAYPIN, LOW);
    flag = false;
    Serial.println("=====");
  }
  else if(t > 40 && flag == false) {
    digitalWrite(RELAYPIN, HIGH);
    flag = true;
    Serial.println("-----");
  }
  
  delay(500);
}
