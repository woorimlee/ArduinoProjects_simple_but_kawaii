#include <SoftwareSerial.h>

SoftwareSerial btSerial(2,3);

void setup()
{
  Serial.begin(9600);
  btSerial.begin(9600);
}

void loop()
{
  for(int i = 0; i < 50; i++) {
    //블루투스로 데이터를 보낼때 쓰는 함수
    btSerial.println(i);
    //Serial.println(i);
    delay(1000);
  }
}
