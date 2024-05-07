#include<SoftwareSerial.h>
#include<string.h>
#include<cstring>
#define RX 16
#define TX 17
SoftwareSerial espserial(RX,TX);
String buff_rx;
void setup(){
   Serial.begin(115200);
   espserial.begin(38400);
   espserial.listen();
   Serial.println("Ready to receive data");
}
void loop(){
   while (espserial.available()){
    buff_rx = espserial.readString();
    Serial.println(buff_rx);
   }
}
