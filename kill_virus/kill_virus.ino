/****************************************************
  <>
  1. 일정 수치 [이상, 이하]의 습도를 기준으로 바이러스 제거하기

****************************************************/

#include "DHT.h" //온습도 센서를 사용하기 위해 포함해야하는 파일

int DHTPIN = 3; //DHT11 습도 센서 꽂을 핀
int LEDPIN = 5; //LED 꽂을 핀
int BUZZERPIN = 6; //부저 꽂을 핀
int HHum = 60; //High Humidity 줄임말
int LHum = 40; //Low Humidity 줄임말

#define DHTTYPE DHT11 //온습도 센서를 사용하기 위한 방법 1
DHT dht(DHTPIN, DHTTYPE); //온습도 센서를 사용하기 위한 방법 2

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //시리얼 모니터를 통해 습도 센서를 확인하기 위한 코드
  dht.begin(); //온습도 센서를 사용하기 위한 방법 3
  pinMode(LEDPIN, OUTPUT); //LED 센서에 전기를 보내주기 위한 방법
  pinMode(BUZZERPIN, OUTPUT); //부저에 전기를 보내주기 위한 방법
}

void loop() {
  delay(500); //0.5초마다 습도를 검사한다.

  float h = dht.readHumidity(); //습도 값을 읽어서 h라는 변수에 저장한다.

  Serial.print(F("Humidity: ")); // 시리얼 모니터에 습도 값을 출력하는 두 줄
  Serial.println(h);

  //만약 습도가 60도 이상이거나 40도 이하면
  if (h >= HHum || h <= LHum) {
    //LED불을 키고,부저를 울린다.
    analogWrite(LEDPIN, 30); //LED 불 켜기
    tone(BUZZERPIN, 523, 1000); //부저 울리기
    delay(1500); //1.5초 기다리기
  }
  else {
    //위 환경이 아니라면 -> 안전한 환경이라면
    digitalWrite(LEDPIN, LOW); //LED 불 끄기
  }
}
