#include <SoftwareSerial.h>

SoftwareSerial mySerial(11, 10);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  String data;
  //int data;
  //블루투스에서 읽어서 바이트 단위로 Serial 출력
  if (mySerial.available()) {
    data = mySerial.readStringUntil('\n');
    //data = mySerial.readStringUntil('\n').toInt();
    Serial.println(data);
    /*if (data == 'a');
      {
      Serial.write("? : ");
      delay(5);
      Serial.println(mySerial.read());
      delay(5);
      Serial.write("! : ");
      Serial.write((int16_t)mySerial.read());
      }*/
  }

  //값에 따라 마우스 움직이기
  //값에 따라 클릭하기
}
