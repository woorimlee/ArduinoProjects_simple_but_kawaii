#include <SoftwareSerial.h> 



SoftwareSerial BTSerial(2, 3);  //객체 지향 언어의 형태이고, 해당라이브러리를 사용하겠다고 선언한 내용입니다.
 
void setup()  
{
  Serial.begin(9600);
  Serial.println("Hello!");
 
  BTSerial.begin(9600);  // set the data rate for the BT port
  pinMode(13, OUTPUT);


}
 
void loop()
{
  byte data;
  data  =  BTSerial.read();
    if(data=='a'){
      digitalWrite(13, HIGH);   // set the LED on
      Serial.println("a");
    }
    if(data=='b'){
      digitalWrite(13, LOW);    // set the LED off
      Serial.println("b");
    }

}
