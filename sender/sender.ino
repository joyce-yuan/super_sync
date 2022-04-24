//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// constants that are sender specific:
const int senderID = 0;
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin
// {RED: 0, GREEN: 1, BLUE: 2, YELLOW: 3}
const int color = 0; 
const char* colorArray[] = {"RED", "GREEN", "BLUE", "YELLOW"};

// variables & state
int state = 0;
int buttonState;


//address through which two modules communicate.
const byte address[6] = "00001";

void setup() {
  Serial.begin(115200);
  Serial.printf("Setting up sender %d with color $s", senderID, colorArray[color]);
  radio.begin();
  
  //set the address
  radio.openWritingPipe(address);
  
  //Set module as transmitter
  radio.stopListening();

  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  // gamestate
  state = 0;

}

void loop() {
  // put your main code here, to run repeatedly:
  // turn LED on:
  digitalWrite(ledPin, HIGH);
  
  // read the state of the reed switch:
  buttonState = digitalRead(buttonPin);
  
  // normal state
  if (state == 0) {
    if (buttonState == HIGH) {
      state = 1;
    }
  } 
  // send state
  else if (state == 1) {
    const char text[] = strcat(itao(senderID), itao(color));
    radio.write(&text, sizeof(text));
    state = 0;
    Serial.printf("sender %d sent $s", senderID, text);

  }
}
