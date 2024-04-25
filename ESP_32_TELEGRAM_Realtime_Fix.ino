#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(4, 5); // CE = 4 dan CSN 5

const byte address[6] = "00001";

// Initialize Wifi connection to the router
char ssid[] = "Skalabumi";     // isi dengan nama wifi
char password[] = "stujacoffe"; // isi dengan password wifi

// Initialize Telegram BOT
#define BOTtoken "7047737692:AAFi2sFT2CIpvaTBMmupvXqQVjD_MdDs5wA" // isi dengan Token Bot

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

//Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

void sendTelegramMessage(String message) {
  bot.sendMessage("1144019258", message, ""); // Ganti "Chat_ID" dengan ID chat penerima notifikasi Telegram
}

void setup() {
  Serial.begin(115200);
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();

  client.setInsecure();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  char text[34] = "";
  while (radio.available() > 0) {
    radio.read(&text, sizeof(text));
    String transData = String(text);

    transData.trim();

    // Ubah substring menjadi nilai integer
    int e = transData.substring(0, 2).toInt();
    int f = transData.substring(3, 5).toInt();
    int g = transData.substring(6, 8).toInt();
    int h = transData.substring(9).toInt();

    Serial.println("(Sensor 1) Kelembapan = " + String(e));
    Serial.println("(Sensor 1) Suhu = " + String(f));
    Serial.println("(Sensor 2) Kelembapan = " + String(g));
    Serial.println("(Sensor 2) Suhu = " + String(h));
    Serial.println("===================");

    String message = "Notifikasi dari Perangkat ESP32-NRF24L01\n";
    message += "Sensor 1 - Kelembaban: " + String(e) + "%, Suhu: " + String(f) + "°C\n";
    message += "Sensor 2 - Kelembaban: " + String(g) + "%, Suhu: " + String(h) + "°C\n";

    sendTelegramMessage(message);

    if (millis() > lastTimeBotRan + botRequestDelay) {
      int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

      while (numNewMessages) {
        numNewMessages = bot.getUpdates(bot.last_message_received + 1);
      }

      lastTimeBotRan = millis();
    }
  }
}
