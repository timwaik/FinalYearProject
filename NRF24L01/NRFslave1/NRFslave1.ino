#include <SPI.h>
#include "RF24.h"


const uint64_t waddresses[] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL, 0xB3B4B5B6A3LL, 0xB3B4B5B60FLL, 0xB3B4B5B605LL};
uint8_t pipenumber=0;
char incoming = 0;
char outgoing = 0;
char bufferpipe[256];
unsigned char c0 = 0;
char sender = 0;
unsigned long previousMillis = 0;
const long interval =90;

RF24 radio(7,8);

void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.setChannel(76);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
//radio.setRetries(15,15);
  
  radio.openWritingPipe(waddresses[1]);
//  Serial.println(F("1"));
  radio.openReadingPipe(1,waddresses[0]);
//  Serial.println(F("2"));

  radio.startListening(); 
}

void loop() {

  unsigned long currentMillis = millis();
  
   radio.startListening();
  if (Serial.available()){                   // If we want to send something
    
    radio.stopListening();                   // First, stop listening so we can talk.
    outgoing = Serial.read();                // Read incoming ucontroller message
//    Serial.println(outgoing);                  // Print out incoming message
    bool rslt;
    rslt = radio.write(&outgoing, sizeof(outgoing));    //Write message to pipe channel
//    Serial.println("Data sent"); 
    if (rslt){
//      Serial.println("Acknowledge received");
    }
  }
    radio.startListening();
  if( radio.available(&pipenumber)){    
      while (radio.available()) { 
//        Serial.print("The pipe number is: ");
//        Serial.println(pipenumber);
        radio.read(&incoming, sizeof(incoming) );                      // Get the payload
        bufferpipe[c0] = incoming;
        c0 = c0+1;
        sender = 1;
        previousMillis = currentMillis;
      }   
//      Serial.print(F("I got this from master: "));
//      Serial.print(incoming);  
   }
   /////////////////Timer for flushing the buffer//////////////////////////////////

  if ((currentMillis - previousMillis >= interval) && (sender == 1)){
//    Serial.print("I am sending buffer1 to the delfino now");
    for (int z=0; z<c0;z++){
    Serial.write(bufferpipe[z]);
    }
    c0=0;
    sender = 0;
    memset(bufferpipe, 0, sizeof(bufferpipe));
  }
  /////////////////////////////////////////////////////////////////////////////////
}
