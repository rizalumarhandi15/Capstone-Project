
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>

/*//////////////////////////////////////////////////////*/

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328PB__) ||defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__)
#define CSN 10
#define LED 12
#define CE  9
#elif defined(ESP32)
#define CSN 5
#define CE  4
#else
#error "Make Config you're self"
#endif

#define Debug_mode false

/*Create a unique pipe out. The receiver has to
  wear the same unique code*/
const uint64_t pipeIn = 0x662266; //IMPORTANT: The same as in the receiver!!!
/*//////////////////////////////////////////////////////*/

/*Create the data struct we will send
  The sizeof this struct should not exceed 32 bytes
  This gives us up to 32 8 bits channals */
RF24 radio(CE, CSN); // select  CSN and CE  pins


/*//////////////////////////////////////////////////////*/
//Create a struct to send over NRF24
struct MyData {
  byte test;
};
MyData data;
/*//////////////////////////////////////////////////////*/

//This function will only set the value to  0 if the connection is lost...


/**************************************************/

void setup()
{
  pinMode(12, OUTPUT);
  Serial.begin(9600); //Set the speed to 9600 bauds if you want.
  //You should always have the same speed selected in the serial monitor
  radio.begin();
  if (Debug_mode)
    printf_begin();
  radio.setDataRate(RF24_250KBPS); //speed  RF24_250KBPS for 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS for 2Mbps
  radio.openWritingPipe(pipeIn);//Open a pipe for writing
  radio.openReadingPipe(1, pipeIn);//Open a pipe for reading
  radio.openReadingPipe(2, pipeIn);//Open a pipe for reading
  radio.openReadingPipe(3, pipeIn);//Open a pipe for reading
  radio.openReadingPipe(4, pipeIn);//Open a pipe for reading
  radio.openReadingPipe(5, pipeIn);//Open a pipe for reading
  radio.setAutoAck(true); // Ensure autoACK is enabled
  radio.setChannel(108);// Set RF communication channel.
  radio.setPALevel(RF24_PA_MAX); //translate to: RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM, and RF24_PA_HIGH=0dBm.
  radio.enableDynamicPayloads(); //This way you don't always have to send large packets just to send them once in a while. This enables dynamic payloads on ALL pipes.
  //radio.disableDynamicPayloads();//This disables dynamic payloads on ALL pipes. Since Ack Payloads requires Dynamic Payloads, Ack Payloads are also disabled. If dynamic payloads are later re-enabled and ack payloads are desired then enableAckPayload() must be called again as well.
  radio.setCRCLength(RF24_CRC_16); // Use 8-bit or 16bit CRC for performance. CRC cannot be disabled if auto-ack is enabled. Mode :RF24_CRC_DISABLED  ,RF24_CRC_8 ,RF24_CRC_16
  radio.setRetries(10, 15);//Set the number of retry attempts and delay between retry attempts when transmitting a payload. The radio is waiting for an acknowledgement (ACK) packet during the delay between retry attempts.Mode: 0-15,0-15
  radio.startListening();//Start listening on the pipes opened for reading.
}



/******Reset the received data to 0 if connection is lost******/
unsigned long lastRecvTime = 0;
void recvData()
{
  while ( radio.available() )//Check whether there are bytes available to be read
  {
    radio.read(&data, sizeof(MyData));//Read payload data from the RX FIFO buffer(s).
    lastRecvTime = millis(); //here we receive the data
  }
}
/**************************************************************/

void loop()
{
  recvData(); //Resive Data
  unsigned long now = millis();
  //Here we check if we've lost signal, if we did we reset the values
  if ( now - lastRecvTime > 1000 ) {
    // Signal lost?
  }
  Serial.print("Resive Value: "); Serial.println(data.test);
  analogWrite(12, data.test);
  delay(100);
  if (Debug_mode)
    radio.printDetails();//Show debug data

}
