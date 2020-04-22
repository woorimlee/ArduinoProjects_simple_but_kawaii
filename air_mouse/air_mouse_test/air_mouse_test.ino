/************************************************************************
  < 몸 불편한 사람들을 위한 무선 마우스 >
  1. 노트북과 레오나르도 hc-06(슬레이브) 연결
  2. 휨 센서, 6축 자이로(MPU-6050) 센서가 달린 장갑과 우노 hc-06(마스터) 연결
  3. 자이로 & 휨 센서 역할 : 우노는 센서의 값을 읽어 레오나르도에 값을 전송한다.
  3-1. 6축 자이로 센서 역할 : 센서 값에따라 레오나르도는 마우스를 움직인다.
  3-2. 휨 센서 역할 : 센서 값에따라 레오나르도는 마우스를 클릭하는 등의 조작을 한다.
************************************************************************/

#include <Wire.h>
#include <Mouse.h>

const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  //Serial.begin(9600);
  Serial.begin(115200);
  Mouse.begin();
  Serial.println("Mouse Start!!");
}

void loop() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  /*Serial.print("AcX = "); Serial.print(AcX);
    Serial.print(" | AcY = "); Serial.print(AcY);
    Serial.print(" | AcZ = "); Serial.print(AcZ);
    Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet*/
  /*Serial.print(" | GyX = "); Serial.print(GyX);
    Serial.print(" | GyY = "); Serial.print(GyY);
    Serial.print(" | GyZ = "); Serial.println(GyZ);*/

  int16_t gyroX, gyroZ;
  int Sensitivity = 500;
  gyroX = GyX / Sensitivity / 1.1 * -1;
  gyroZ = GyZ / Sensitivity * -1;
  Mouse.move(gyroZ, gyroX);

  int sensor_value = analogRead(A0);
  //Serial.println(sensor_value);
  if (sensor_value <= 150) {
    if (Mouse.isPressed() == 0)
      Mouse.press(MOUSE_LEFT);
  }
  else if (sensor_value > 150 && Mouse.isPressed() == 1) {
    Mouse.release(MOUSE_LEFT);
  }

  delay(10);
}
