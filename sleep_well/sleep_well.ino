/****************************************************

  //1. 10분동안 평균값 산출
  //2. 습도 측정할 때 : check = 1;
  //   온도 측정할 때 : check = 2;
  //   조도 측정할 때 : check = 3;
  //   소리 측정할 때 : check = 4;
  //3. 수면 환경에 대한 LED 처리(습도와 온도만)
  //4. 360분동안 측정하며, 10분에 한 번씩 현재 값을 검출.
  //5. 최종 평균값 측정후 프로그램 종료

****************************************************/

#include "DHT.h" //온습도 센서를 사용하기 위해 포함해야하는 파일

DHT dht(3, DHT11); //온습도 센서를 사용하기 위한 방법

int check = 1;
int i = 0;
int led1 = 9;
int led2 = 10;
int led3 = 11;
float result[40] = {0};

void setup() {
  Serial.begin(9600); //시리얼 모니터를 통해 습도 센서를 확인하기 위한 코드
  dht.begin(); //온습도 센서를 사용하기 위한 방법
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  Serial.println("검사 시작");

  int k = 0;
  float res = 0.0;
  float h;
  for (k = 0 ; k < 10; k++) {
    if (check == 1)
      h = dht.readHumidity();
    else if (check == 2)
      h = dht.readTemperature();
    else if (check == 3)
      h = analogRead(A0);
    //else if(check == 4)
    res += h;
    //delay(1000);
    delay(60000);
  }
  res /= 10;
  Serial.print("측정 평균 값 : ");
  Serial.println(res);
  Serial.println("프로그램 시작");
}

void loop() {
  /*
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    Serial.print("습도(%) : ");
    Serial.print(h);
    Serial.print("온도(°C) : ");
    Serial.print(t);
    int pt = analogRead(A0);
    Serial.print(" 조도 : ");
    Serial.println(pt);
  */


  if (check == 1) { //습도
    float h = dht.readHumidity();
    Serial.print(i + 1);
    Serial.print("번째 측정 : ");
    Serial.println(h);
    result[i] = h;
    i++;
    if (h < 40) {
      analogWrite(led1, 0);//초록
      analogWrite(led2, 5);//노랑
      analogWrite(led3, 0);//빨강
    }
    else if (40 <= h && h <= 60) {
      analogWrite(led1, 5);
      analogWrite(led2, 0);
      analogWrite(led3, 0);
    }
    else {
      analogWrite(led1, 0);
      analogWrite(led2, 0);
      analogWrite(led3, 5);
    }
  }
  else if (check == 2) { //온도
    float h = dht.readTemperature();
    Serial.print(i + 1);
    Serial.print("번째 측정 : ");
    Serial.println(h);
    result[i] = h;
    i++;
    if (16 <= h && h <= 22) {
      analogWrite(led1, 5);//초록
      analogWrite(led2, 0);//노랑
      analogWrite(led3, 0);//빨강
    }
    else if (22 < h && h <= 28) {
      analogWrite(led1, 0);
      analogWrite(led2, 5);
      analogWrite(led3, 0);
    }
    else if (28 < h) {
      analogWrite(led1, 0);
      analogWrite(led2, 0);
      analogWrite(led3, 5);
    }
  }

  else if (check == 3) { //조도
    int h = analogRead(A0);
    Serial.print(i + 1);
    Serial.print("번째 측정 : ");
    Serial.println(h);
    result[i] = h;
    i++;
  }
  else if (check == 4) { //소리

  }

  //delay(1000);
  delay(600000);

  if (i == 36) {
    Serial.print("전체 배열의 값 : ");
    int j = 0;
    float res = 0.0;
    for (j = 0 ; j < 36; j++) {
      Serial.print(result[j]);
      Serial.print(" ");
      res += result[j];
    }
    res /= 36;
    Serial.println();
    Serial.print("최종 평균 값 : ");
    Serial.println(res);
    check = 0;
    i = 0;
  }
}
