#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//RF24 radio(7,8); //CE = 7 dan CSN 8
//RF24 radio(8,7); //CE = 8 dan CSN 7
RF24 radio(4, 5); //CE = 8 dan CSN 10


const byte address[6] = "00001";

String datain;
int8_t indexOfA, indexOfB, indexOfC, indexOfD;
String data1, data2, data3, data4;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();
}

void loop() {
  char text[34] = "";
  while (radio.available() > 0) {
    radio.read(&text, sizeof(text));
    String transData = String(text);
    
    transData.trim(); // menghilangkan enter atau "\n"
    Serial.println(transData);
    //teknik parsing 
    indexOfA = transData.indexOf("A");
    indexOfB = transData.indexOf("B");
    indexOfC = transData.indexOf("C");
    indexOfD = transData.indexOf("D");
    
    data1 = transData.substring (0, indexOfA);
    data2 = transData.substring (indexOfA+1, indexOfB);
    data3 = transData.substring (indexOfB+1, indexOfC);
    data4 = transData.substring (indexOfC+1, indexOfD);
    
    Serial.println((String)"(Sensor 1) Kelembapan = " + data1);
    Serial.println((String)"(Sensor 1) Suhu = "+data2);
    Serial.println((String)"(Sensor 2) Kelembapan = " + data3);
    Serial.println((String)"(Sensor 2) Suhu = "+data4);
    Serial.println("===================");
  }
}
