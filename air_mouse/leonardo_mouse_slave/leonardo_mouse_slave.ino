/************************************************************************
  < 몸 불편한 사람들을 위한 무선 마우스 >
  1. 노트북과 레오나르도 hc-06(슬레이브) 연결
  2. 휨 센서, 6축 자이로(MPU-6050) 센서가 달린 장갑과 우노 hc-06(마스터) 연결
  3. 자이로 & 휨 센서 역할 : 우노는 센서의 값을 읽어 레오나르도에 값을 전송한다.
  3-1. 6축 자이로 센서 역할 : 센서 값에따라 레오나르도는 마우스를 움직인다.
  3-2. 휨 센서 역할 : 센서 값에따라 레오나르도는 마우스를 클릭하는 등의 조작을 한다.
************************************************************************/

#include <SoftwareSerial.h>
#include <Mouse.h>

SoftwareSerial mySerial(11, 10);
String RS; //수신된 문자열
int gyroX, gyroZ, fg_i, fg_m, fg_r; //마우스 조종에 필요한 변수 선언
int index1, index2, index3, index4, index5; //쉼표 분리에 필요한 인덱스 변수 선언
int flex_value = 165;

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
    if (fg_i <= flex_value && fg_m > flex_value && fg_r > flex_value) {
      if (Mouse.isPressed() == 0) {
        Mouse.press(MOUSE_LEFT);
      }
    }
    else if (fg_i > flex_value && Mouse.isPressed() == 1) {
      Mouse.release(MOUSE_LEFT);
    }
    
    //오른쪽 버튼 클릭
    if (fg_m <= flex_value && fg_i > flex_value && fg_r > flex_value) {
      if (Mouse.isPressed(MOUSE_RIGHT) == 0) {
        Mouse.press(MOUSE_RIGHT);
      }
    }
    else if (fg_m > flex_value && Mouse.isPressed(MOUSE_RIGHT) == 1) {
      Mouse.release(MOUSE_RIGHT);
    }

    //가운데 버튼 클릭
    if (fg_r <= flex_value && fg_i > flex_value && fg_m > flex_value) {
      if (Mouse.isPressed(MOUSE_MIDDLE) == 0) {
        Mouse.press(MOUSE_MIDDLE);
      }
    }
    else if (fg_r > flex_value && Mouse.isPressed(MOUSE_MIDDLE) == 1) {
      Mouse.release(MOUSE_MIDDLE);
    }

    //휠은 검지와 중지를 동시에 구부렸을때 
    if(fg_i <= flex_value && fg_m <= flex_value) {
      Mouse.move(0, 0, 1);
    }
    //중지와 약지를 구부렸을때 올라가고 내려옴
    if(fg_m <= flex_value && fg_r <= flex_value) {
      Mouse.move(0, 0, -1);
    }
    
  }
  delay(5);
}
