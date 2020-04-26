/****************************************************
<시각 장애인을 위한 장애물 감지 발열 지팡이>
1. 지팡이 하단부에 장착된 초음파 센서에서 장애물을 감지.
2. 장애물이 있다면 부저를 울려 경고를 준다.
3. 온습도 센서를 이용해 추운 날씨에 발열패드를 동작시킨다.
****************************************************/

#include "DHT.h"

//핀 번호 정의
#define RELAYPIN 3
#define DHTPIN 4
#define BUZZERPIN 7
#define ECHOPIN 12
#define TRIGPIN 13
#define DHTTYPE DHT11
//릴레이 핀 관리를 위한 변수
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
  //초음파 거리 측정
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);
  int duration = pulseIn(ECHOPIN, HIGH); 
  int distance = duration / 29 / 2;  
  
  Serial.print(distance);
  Serial.println("cm");
  //근거리에 물체 감지시 부저 울리기
  if(distance <= 40) {
    tone(BUZZERPIN, 261.63, 500);
  }

  //온습도 센서를 이용한 온도 측정
  float t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  }
  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.println("%");

  //온도가 25도보다 낮다면 릴레이 모듈 전기 흐르게해서 지팡이 동작시키고
  if(t <= 25 && flag == true) {
    //LOW레벨 트리거 릴레이 이용함.
    digitalWrite(RELAYPIN, LOW);
    flag = false;
    Serial.println("=====");
  }
  //너무 높은 온도가 되지 않게 적절한 온도에서 발열패드 전기 끊기
  else if(t > 40 && flag == false) {
    digitalWrite(RELAYPIN, HIGH);
    flag = true;
    Serial.println("-----");
  }
  
  delay(500);
}
