/****************************************************
  <>
  1. 일정 수치 [이상, 이하]의 습도를 기준으로 바이러스 제거하기

****************************************************/

#include "DHT.h"

#define DHTPIN 3
#define LEDPIN 5
#define BUZZERPIN 6
#define DHTTYPE DHT11
#define HHum 60 //High Humidity

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
  pinMode(LEDPIN, OUTPUT);
  pinMode(BUZZERPIN, OUTPUT);
}

void loop() {
  delay(500);

  float h = dht.readHumidity();

  Serial.print(F("Humidity: "));
  Serial.println(h);
  if (h >= HHum) {
    analogWrite(LEDPIN, 30);
    tone(BUZZERPIN, 523, 1000);
    delay(1500);
  }
  else {
    digitalWrite(LEDPIN, LOW);
  }
}
