#include "ThingSpeak.h"
#include "secrets.h"
#include <WiFi.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


char ssid[] = SECRET_SSID; 
char pass[] = SECRET_PASS; 
int keyIndex = 0; 
WiFiClient  client;

RF24 radio(4, 5); //CE = 4 dan CSN 5

const byte address[6] = "00001";

String datain;
int8_t indexOfA, indexOfB, indexOfC, indexOfD;
String data1, data2, data3, data4;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

void setup() {
  Serial.begin(115200);  // Initialize serial
  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);  // Initialize ThingSpeak

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();
}

void loop() {
  //menghubungkan nrf 
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
  // Menghubungkan ke internet
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  //Mencoba untuk menghubungkan ke internet
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
  
  // Mengirim data ke thingspeak
  ThingSpeak.setField(1, data1);
  ThingSpeak.setField(2, data2);
  ThingSpeak.setField(3, data3);
  ThingSpeak.setField(4, data4);
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Berhasl update ke thingspeak"); 
  }
  else{
    Serial.println("Gagal update. HTTP error code " + String(x));
  }
  
  delay(15000); // Tunggu 15 detik untuk update lagi ke Thingspeak
  
}
