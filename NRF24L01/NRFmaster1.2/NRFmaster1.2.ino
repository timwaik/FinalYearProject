#include <SPI.h>
#include "RF24.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5); // RX, TX

const uint64_t waddresses[] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL, 0xB3B4B5B6A3LL, 0xB3B4B5B60FLL, 0xB3B4B5B605LL};    //addresses of the wireless modules

char out[32];
byte d='#';
char incomingpipe1 = 0;
char incomingpipe2 = 0;
char incomingpipe3 = 0;
char incomingpipe4 = 0;
char bufferpipe1[256];
char bufferpipe2[256];
char bufferpipe3[256];
char bufferpipe4[256];
char outgoing = 0;
char sender1 = 0;
char sender2 = 0;
char sender3 = 0;
char sender4 = 0;
uint8_t pipenumber;
int i = 0;
int c1 = 0;
int c2 = 0;
int c3 = 0;
int c4 = 0;
int timeout;
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;
unsigned long previousMillis4 = 0;
const long interval = 90;                       //set this value to the interval it waits before flushing the buffer
bool enable = 0;

RF24 radio(7,8);   //initialise the NRF24L01


void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  radio.begin();
  radio.setChannel(76);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.setRetries(15,15);
  radio.setAutoAck(enable);

  radio.openWritingPipe(waddresses[0]);

  radio.openReadingPipe(1,waddresses[1]);

  radio.openReadingPipe(2,waddresses[2]);

  radio.openReadingPipe(3,waddresses[3]);

  radio.openReadingPipe(4,waddresses[4]);


  radio.startListening();
  

}

void loop() {
    
  unsigned long currentMillis = millis();

////////////////////SENDING DATA///////////////////////////

    if (Serial.available()){                               // If we want to send something
      radio.stopListening();                               // First, stop listening so we can talk.
      outgoing = Serial.read();                // Read incoming ucontroller message
      radio.write(&outgoing, sizeof(outgoing));
    }

//////////////////////////////////////////////////////////

///////////////////RECEIVING DATA/////////////////////////
     
 radio.startListening();                                   // Start listening
 if(radio.available(&pipenumber)){                         // If there is an incoming message
  switch (pipenumber) {
    case 1:
    while (radio.available()){
      radio.read(&incomingpipe1, sizeof(incomingpipe1));

      bufferpipe1[c1] = incomingpipe1;
      c1 = c1+1;
      sender1 = 1;
      previousMillis1 = currentMillis; 
    }    
    break;
    
    case 2:
    while (radio.available()){
      radio.read(&incomingpipe2, sizeof(incomingpipe2));
      bufferpipe2[c2] = incomingpipe2;
      c2 = c2+1;
      sender2 = 1;
      previousMillis2 = currentMillis;
    }
    break;
    
    case 3:
    while (radio.available()){
      radio.read(&incomingpipe3, sizeof(incomingpipe3));
      bufferpipe3[c3] = incomingpipe3;
      c3 = c3+1;
      sender3 = 1;
      previousMillis3 = currentMillis;
    }
    break;
    
    case 4:
    while (radio.available()){
      radio.read(&incomingpipe4, sizeof(incomingpipe4));
      bufferpipe1[c4] = incomingpipe4;
      c4 = c4+1;
      sender4 = 1;
      previousMillis4 = currentMillis;
    }
    break;
    }
  }
////////////////////////////////////////////////////////////////////////////////

/////////////////Timer for flushing the buffer//////////////////////////////////

  if ((currentMillis - previousMillis1 >= interval) && (sender1 == 1)){
    for (int z=0; z<c1;z++){
    Serial.write(bufferpipe1[z]);
    }
    c1=0;
    sender1 = 0;
    memset(bufferpipe1, 0, sizeof(bufferpipe1));
  }
  if ((currentMillis - previousMillis1 >= interval) && (sender2 == 1)){
    for (int z=0; z<c2;z++){
    Serial.write(bufferpipe2[z]);
    }
    c2=0;
    sender2 = 0;
    memset(bufferpipe2, 0, sizeof(bufferpipe2));
  }
  if ((currentMillis - previousMillis1 >= interval) && (sender3 == 1)){
    for (int z=0; z<c3;z++){
    Serial.write(bufferpipe3[z]);
    }
    c3=0;
    sender3 = 0;
    memset(bufferpipe3, 0, sizeof(bufferpipe3));
  }
  if ((currentMillis - previousMillis1 >= interval) && (sender4 == 1)){
    for (int z=0; z<c4;z++){
    Serial.write(bufferpipe4[z]);
    }
    c4=0;
    sender4 = 0;
    memset(bufferpipe4, 0, sizeof(bufferpipe4));
  }
}
///////////////////////////////////////////////////////////

