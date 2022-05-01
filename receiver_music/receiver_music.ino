//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <string.h>
#include <time.h>
#include"pitches.h"


//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

// constants for receiver:
char print_statement[100];
const uint8_t PIN = 6;
char text[32] = {0};
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin
// {RED: 0, GREEN: 1, BLUE: 2, YELLOW: 3}
const int color = 0; 
const char* colorArray[] = {"RED", "GREEN", "BLUE", "YELLOW"};
// senders 0-3 = team 1
// senders 4-7 = team 2

// variables & state
int state = 0;
// state = 0, game setup
// state = 1, listening for input
// state = 2, score update
// state = 3, game over

int buttonState;

int currentColor;
int team1;
int team2;
int senderColor;
int senderTeam;


//address through which two modules communicate.
const byte address[6] = "00001";

////////////////////////////////////////////
// MUSIC STUFF
// note durations: 8 = quarter note, 4 = 8th note, etc.

int scoreMusic[] = {
NOTE_C4, NOTE_E4, NOTE_D4, NOTE_F4, NOTE_C4, END,
};

int scoreDurations[] = {
4,4,4,4,12,4
};

int gameStartMusic[] = {
NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_A4,
NOTE_G4, NOTE_C5, NOTE_AS4, NOTE_A4,                   
NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_FS4, NOTE_DS4, NOTE_D4,
NOTE_C4, NOTE_D4,END,
};

int gameStartDurations[] = {
8,4,8,4,
4,4,4,12,
4,4,4,4,4,4,
4,16,4,
};

int gameOverMusic[] = {
NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_FS4, NOTE_DS4, NOTE_D4,
NOTE_C4, NOTE_D4,END,
};

int gameOverDurations[] = {
4,4,4,4,4,4,
4,16,4,
};

int speed=90;  //higher value, slower notes
////////////////////////////////////////////////////

void setup() {
  pinMode(PIN, OUTPUT);
  while (!Serial);
    Serial.begin(115200);
  sprintf(print_statement, "Setting up receiver tower");
  Serial.println(print_statement);
  radio.begin();
  
  //set the address
  radio.openReadingPipe(0, address);
  
  //Set module as receiver
  radio.startListening();

  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  // gamestate and setup
  state = 1;
  currentColor = generateColor();
  team1 = 0;
  team2 = 0;
  playmusic(gameStartMusic, gameStartDurations);

}

void loop() {
  // put your main code here, to run repeatedly:
  // turn LED on:
  digitalWrite(ledPin, HIGH);
  
  // read the state of the reed switch:
  buttonState = digitalRead(buttonPin);

  // state = 0, game setup
  // state = 1, listening for input
  // state = 2, score update
  // state = 3, game over
  
  // game setup
  if (state == 0) {
    if (buttonState == HIGH){
      delay(500);
      state = 1;
      //play starting game music
      playmusic(gameStartMusic, gameStartDurations);
    }
  }

  // listening for input
  else if (state == 1) {

    //Read the data if available in buffer
    if (radio.available()) {
      //Serial.println("reading");
      strcpy("", text);
      radio.read(&text, sizeof(text));
      Serial.println(text);
      senderTeam = (atoi(text[0]) < 4) ? 1 : 2;
      senderColor = atoi(text[1]) % 4;
      Serial.print("Team: ");
      Serial.println(senderTeam);
      Serial.print("Color: ");
      Serial.println(senderColor);

      //color check
      if (senderColor == currentColor){
        playmusic(scoreMusic, scoreDurations);
        state = 2;
      }
    }

    if (buttonState == HIGH){
      Serial.print("ending");
      resetGame();
      state = 0;
    }
  }

  //score update
  else if (state == 2) {
    updateScore();
    if (team1 == 5 || team2 == 5){
      Serial.println("someone won");
      state = 3;
      playmusic(gameOverMusic, gameOverDurations);
      //maybe play some music
    } else {
      state = 1;
      //go back to listening if no conditions met
      currentColor = generateColor();
    }
    if (buttonState == HIGH){
      resetGame();
      state = 0;
    }
  }

  //game end
  else if (state == 3) {
    if (buttonState == HIGH){
      resetGame();
      state = 0;
    }
  }

}

int generateColor() {
  srand(time(0));
  int temp = rand()%4;
  //insert LED logic for tower
  sprintf(print_statement, "tower lights up %s", colorArray[0]);
  Serial.println(print_statement);
  return 0;
  //return temp;
}

void updateScore() {
  if (senderTeam == 1){
    team1 = team1 + 1;
    // tower LED logic here
  }

  else if (senderTeam == 2){
    team2 = team2 + 1;
    // tower LED logic here
  }
  sprintf(print_statement, "updated score team 1: %d, team 2: %d", team1, team2);
  Serial.println(print_statement);
}

void resetGame() {
  Serial.println("resetting game");
  team1 = 0;
  team2 = 0;
  //reset all LEDs
  playmusic(gameOverMusic, gameOverDurations);
  delay(500);
}

void playmusic(int * notes, int * durations){
for (int thisNote = 0; notes[thisNote]!=-1; thisNote++) {

int noteDuration = speed*durations[thisNote];
tone(3, notes[thisNote],noteDuration*.95);
Serial.println(notes[thisNote]);

delay(noteDuration);

noTone(3);
}
}
