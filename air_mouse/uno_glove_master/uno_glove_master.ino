#include <SoftwareSerial.h>
#include <Wire.h>

#define TX 11
#define RX 10
#define SDA A4
#define SCL A5
#define INDEXFINGER A0
#define MIDDLEFINGER A1
#define RINGFINGER A2

const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
String TS; // 송신부 데이터

SoftwareSerial mySerial(TX, RX); //tx = 11, rx = 10

void setup()
{
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop()
{
  //MPU-6050 시작
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
    Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet
    Serial.print(" | GyX = "); Serial.print(GyX);
    Serial.print(" | GyY = "); Serial.print(GyY);
    Serial.print(" | GyZ = "); Serial.println(GyZ);*/

  //마우스 조종을 위해 필요한 값
  int gyroX, gyroZ;
  int Sensitivity = 500;
  gyroX = GyX / Sensitivity / 1.1 * -1;
  gyroZ = GyZ / Sensitivity * -1;
  Serial.print("GYROX : ");
  Serial.print(gyroX);
  Serial.print(", GYROY : ");
  Serial.print(gyroZ);

  int fg_i = analogRead(INDEXFINGER);       //index finger
  int fg_m = analogRead(MIDDLEFINGER) + 10; //middle finger +10은 값 맞춰주려고
  int fg_r = analogRead(RINGFINGER) - 20;   //ring finger +20은 값 맞춰주려고
  Serial.print(", IDFG : ");
  Serial.print(fg_i);
  Serial.print(", MDFG : ");
  Serial.print(fg_m);
  Serial.print(", RIFG : ");
  Serial.println(fg_r);

  /*******************************************************
    송신 규칙 정의
    송신할 data : MPU-6050의 값 두 개, 휨 센서 세 개
    data type : String
    송신할 String TS : gyroX,gyroZ,fg_i,fg_m,fg_r\n
    '\n'을 송신한 문자열의 끝 문자로 정의.
  *******************************************************/

  //hc-06을 통해 String data 송신하기
  TS = String(gyroX) + ',' + String(gyroZ) + ','                //mpu-6050 두 개
       + String(fg_i) + ',' + String(fg_m) + ',' + String(fg_r) //flex sensor 세 개
       + '\n';                                                  //종료 문자 설정
  mySerial.print(TS);
  delay(10);
}
