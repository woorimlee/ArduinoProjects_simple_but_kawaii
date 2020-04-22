#include <SoftwareSerial.h>
#include <Mouse.h>

SoftwareSerial mySerial(11, 10);
String RS; //송신된 문자열
int gyroX, gyroZ, fg_i, fg_m, fg_r; //마우스 조종에 필요한 변수 선언
int index1, index2, index3, index4, index5; //쉼표 분리에 필요한 인덱스 변수 선언

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  Mouse.begin();
}

void loop() {
  if (mySerial.available()) {

    RS = mySerial.readStringUntil('\n'); //'\n' 전까지 값 읽어서 하나의 String 생성

    //gyroX분리
    index1 = RS.indexOf(',');
    gyroX = RS.substring(0, index1).toInt();

    //gyroZ분리
    index2 = RS.indexOf(',', index1 + 1);
    gyroZ = RS.substring(index1 + 1, index2).toInt();

    //fg_i 분리
    index3 = RS.indexOf(',', index2 + 1);
    fg_i = RS.substring(index2 + 1, index3).toInt();

    //fg_m 분리
    index4 = RS.indexOf(',', index3 + 1);
    fg_m = RS.substring(index3 + 1, index4).toInt();

    //fg_r 분리
    index5 = RS.length();
    fg_r = RS.substring(index4 + 1, index5).toInt();

    Serial.print("GYROX : ");
    Serial.print(gyroX);
    Serial.print(", GYROZ : ");
    Serial.print(gyroZ);
    Serial.print(", Index : ");
    Serial.print(fg_i);
    Serial.print(", Middle : ");
    Serial.print(fg_m);
    Serial.print(", Ring : ");
    Serial.println(fg_r);

    //마우스 움직이기
    Mouse.move(gyroZ, gyroX);

    //왼쪽 버튼 클릭
    if (fg_i <= 150 && fg_m > 150 && fg_r > 150) {
      if (Mouse.isPressed() == 0) {
        Mouse.press(MOUSE_LEFT);
      }
    }
    else if (fg_i > 150 && Mouse.isPressed() == 1) {
      Mouse.release(MOUSE_LEFT);
    }
    
    //오른쪽 버튼 클릭
    if (fg_m <= 150 && fg_i > 150 && fg_r > 150) {
      if (Mouse.isPressed(MOUSE_RIGHT) == 0) {
        Mouse.press(MOUSE_RIGHT);
      }
    }
    else if (fg_m > 150 && Mouse.isPressed(MOUSE_RIGHT) == 1) {
      Mouse.release(MOUSE_RIGHT);
    }

    //가운데 버튼 클릭
    if (fg_r <= 150 && fg_i > 150 && fg_m > 150) {
      if (Mouse.isPressed(MOUSE_MIDDLE) == 0) {
        Mouse.press(MOUSE_MIDDLE);
      }
    }
    else if (fg_r > 150 && Mouse.isPressed(MOUSE_MIDDLE) == 1) {
      Mouse.release(MOUSE_MIDDLE);
    }

    //휠 
    if(fg_i <= 150 && fg_m <= 150) {
      Mouse.move(0, 0, 1);
    }
    if(fg_m <= 150 && fg_r <= 150) {
      Mouse.move(0, 0, -1);
    }
    
  }
  delay(5);
}
