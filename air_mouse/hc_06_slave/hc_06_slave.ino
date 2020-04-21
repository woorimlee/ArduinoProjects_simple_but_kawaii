#include <SoftwareSerial.h>

SoftwareSerial mySerial(11, 10);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
    byte data;
    data = mySerial.read();
    if(data=='a'){
      Serial.println("a");
   }
}
