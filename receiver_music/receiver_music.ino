//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <string.h>
#include <time.h>
#include <FastLED.h>
#include"pitches.h"


const int fake_sequence[] = {0, 2, 2, 0};
int sequence_counter;

//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

// constants for receiver:
char print_statement[100];
// const uint8_t PIN = 6;
char text[32] = {0};
// const int buttonPin = 2;     // the number of the pushbutton pin
// const int ledPin =  13;      // the number of the LED pin
// {RED: 0, GREEN: 1, BLUE: 2, YELLOW: 3}
const int color = 0; 
const char* colorArray[] = {"RED", "GREEN", "BLUE", "YELLOW"};
// senders 0-3 = team 1
// senders 4-7 = team 2

// variables for lights
#define NUM_LEDS 192
#define DATA_PIN_1 5
#define DATA_PIN_2 6
#define leds_per_floor 33

// const uint8_t NUM_LEDS = 192;
// const uint8_t DATA_PIN_1 = 6;
// const uint8_t DATA_PIN_2 = 5;
// const uint8_t leds_per_floor = 25;
CRGB leds_1[NUM_LEDS]; // led array for team 1
CRGB leds_2[NUM_LEDS]; // led array for team 2
int levels[] ={5, 37, 69, 101, 135}; // starting led of each floor in tower


// variables & state
int state = 0;
// state = 0, game setup
// state = 1, listening for input
// state = 2, score update
// state = 3, game over

//int buttonState;

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
/* ARCHIVED May 4
int scoreMusic[] = {
NOTE_C4, NOTE_E4, NOTE_D4, NOTE_F4, NOTE_C4, END,
};

int scoreDurations[] = {
4,4,4,4,12,4
};

/* ARCHIVED May 4
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
*/
int scoreMusic[] = {
  // NOTE_F4, NOTE_C5, 
  NOTE_A4, NOTE_C5, NOTE_AS4, NOTE_D5, NOTE_F5, END
};

int scoreDurations[] = {
  // 8, 8,
  4,4,4,4, 8, END
};
int gameStartMusic[] = {
  NOTE_C4, NOTE_F4, NOTE_A4, NOTE_A4,
  NOTE_AS4, NOTE_A4, NOTE_G4, NOTE_C4,
  NOTE_F4, NOTE_A4, NOTE_C5, 
  NOTE_AS4, NOTE_G4, NOTE_C5, END
};

int gameStartDurations[] = {
  2, 8, 6, 2,
  2, 2, 8, 4,
  8, 6, 2,
  2, 2, 12, 2,
};

int gameOverMusic[] = {
  NOTE_F4, NOTE_C5, NOTE_D5, NOTE_F5, NOTE_E5, NOTE_C5,
  NOTE_AS4, NOTE_A4, NOTE_G4, NOTE_F4, END
};

int gameOverDurations[] = {
  8, 8, 4, 4, 4, 4,
  8, 4, 4, 16, 4
};

int speed=70;  //higher value, slower notes
////////////////////////////////////////////////////

void setup() {
  //pinMode(PIN, OUTPUT);
  while (!Serial);
    Serial.begin(115200);
  delay(1500);
  sprintf(print_statement, "Setting up receiver tower");
  Serial.println(print_statement);
	FastLED.addLeds<WS2812,DATA_PIN_1,RGB>(leds_1,NUM_LEDS);
  FastLED.addLeds<WS2812,DATA_PIN_2,RGB>(leds_2,NUM_LEDS);
	FastLED.setBrightness(80);
  all_off();  
  radio.begin();
  
  //set the address
  radio.openReadingPipe(0, address);
  
  //Set module as receiver
  radio.startListening();
  radio.setPALevel(RF24_PA_MIN);

  // initialize the LED pin as an output:
  // pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  //pinMode(buttonPin, INPUT);

  // gamestate and setup
  state = 1;
  sequence_counter = 0;
  currentColor = generateColor();
  team1 = 3;
  team2 = 3;
  playmusic(gameStartMusic, gameStartDurations);
  // playmusic(scoreMusic, scoreDurations);
  // playmusic(gameOverMusic, gameOverDurations);

}

void loop() {
  // put your main code here, to run repeatedly:
  // turn LED on:
  // digitalWrite(ledPin, HIGH);
  
  // read the state of the reed switch:
  //buttonState = digitalRead(buttonPin);

  // state = 0, game setup
  // state = 1, listening for input
  // state = 2, score update
  // state = 3, game over
  
  // game setup
  // if (state == 0) {
  //   // if (buttonState == LOW){
  //   //   delay(500);
  //   //   state = 1;
  //   //   //play starting game music
  //   //   playmusic(gameStartMusic, gameStartDurations);
  //   // }
  // }

  // listening for input
  if (state == 1) {
    // Serial.println("state 1");
    //Read the data if available in buffer
    if (radio.available()) {
      Serial.println("reading");
      strcpy("", text);
      radio.read(&text, sizeof(text));
      Serial.println(text);

      // numbering system for 00, 01, 02, 03, 10, 11, 12, 13
       senderColor = text[1] - '0';
       senderTeam = text[0] - '0';
       senderTeam = (senderTeam < 4) ? 1: 2;

      // numbering system for 0 - 7, untested
      //  senderColor = atoi(text) % 4;
      //  senderTeam = (atoi(text) < 4) ? 1: 2;
      
      
      Serial.print("Team: ");
      Serial.println(senderTeam);
      Serial.print("Color: ");
      Serial.println(senderColor);

      //color check
      if (senderColor == currentColor){
        all_off();
        correct_color(senderTeam, senderColor);
        playmusic(scoreMusic, scoreDurations);
        state = 2;
      }
    }

    // if (buttonState ==LOW){
    //   Serial.print("ending");
    //   resetGame();
    //   state = 0;
    // }
  }

  //score update
  else if (state == 2) {
    all_off();
    updateScore();
    light_by_score(team1, team2, leds_per_floor);
    delay(1000);
    if (team1 == 5 || team2 == 5){
      //Serial.println("someone won");
      state = 3;
      //playmusic(gameOverMusic, gameOverDurations);
      all_off();
      winning_sequence((team1 == 5)? 1: 2);
      all_off();
      //maybe play some music
    } else {
      state = 1;
      //go back to listening if no conditions met
      sequence_counter++;
      currentColor = generateColor();
    }
    // if (buttonState == LOW){
    //   resetGame();
    //   state = 0;
    //   // all_off();
    // }
  }

  //game end
  // else if (state == 3) {
  //   if (buttonState == LOW){
  //     all_off();
  //     resetGame();
  //     state = 0;
  //   }
  // }

}

int generateColor() {
  // srand(time(0));
  // int temp = rand()%4;
  //insert LED logic for tower
  int temp = fake_sequence[sequence_counter];
  //sequence_counter+=1;
  sprintf(print_statement, "tower lights up %s", colorArray[temp]);
  light_by_color(temp);
  Serial.println(print_statement);
  //return 0;
  return temp;
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

// void resetGame() {
//   Serial.println("resetting game");
//   team1 = 0;
//   team2 = 0;
//   //reset all LEDs
//   // playmusic(gameOverMusic, gameOverDurations);
//   delay(500);
// }

void playmusic(int * notes, int * durations){
  for (int thisNote = 0; notes[thisNote]!=-1; thisNote++) {
    int noteDuration = speed*durations[thisNote];
    tone(3, notes[thisNote],noteDuration*.95);
    //Serial.println(notes[thisNote]);
    delay(noteDuration);
    noTone(3);
  }
}

void light_by_score(int score_1, int score_2, int per_floor){
  for (int i=0;i<5;i++){
    for(int j=levels[i];j<levels[i] + per_floor; j++){
      // light level for team 1 if score high enough
      if (score_1 > i){
        leds_1[j] = CHSV(0, 0, 255);
      }
      // light level for team 2 if score high enough
      if (score_2 > i){
        leds_2[j] = CHSV(0, 0, 255);
      }
    }
    FastLED.show();
    delay(200); // lights floors one at a time, for visual appeal?
  }
}

void light_by_color(int color){
  int hue;
  switch (color){
    case 0: // tower is red
      hue = 96;
      break;
    // case 1: // tower is green
    //   hue = 0;
    //   break;
    case 2: // tower is blue
      hue = 160;
      break;
    // case 3: // tower is yellow
    //   hue = 80;
    //   break;
    // default:
    //   hue = 192; // purple idk
    //   break;
  }

  for (int i = 0; i<NUM_LEDS; i++){
    leds_1[i] = CHSV(hue, 255, 255);
    leds_2[i] = CHSV(hue, 255, 255);
  }
  FastLED.show();
}

void correct_color(int team, int color){
  int hue;
  switch (color){
    case 0: // tower is red
      hue = 96;
      break;
    // case 1: // tower is green
    //   hue = 0;
    //   break;
    case 2: // tower is blue
      hue = 160;
      break;
    // case 3: // tower is yellow
    //   hue = 80;
    //   break;
    // default:
    //   hue = 192; // purple idk
    //   break;
  }

  if (team == 1){
    for (int x = 0; x < 5; x++){
      for (int i = 0; i < NUM_LEDS; i++){
        leds_1[i] = CHSV(hue, 255, 255);
      }
      FastLED.show();
      delay(100);
      all_off();
      delay(100);
    }
  }
  else if (team == 2){
    for (int x = 0; x < 5; x++){
      for (int i = 0; i < NUM_LEDS; i++){
        leds_2[i] = CHSV(hue, 255, 255);
      }
      FastLED.show();
      delay(100);
      all_off();
      delay(100);
    }
  }

}

void all_off(){
  for (int i = 0; i < NUM_LEDS; i++){
    leds_1[i] = CRGB::Black;
    leds_2[i] = CRGB::Black;
  }
  FastLED.show();
}

void winning_sequence(int team){
  for (int x=0;x<800;x++){
    // Serial.println(x);
    //uint8_t thisHue = beatsin8(30, 0, 255);
    uint8_t thisHue = beat8(100, 255);
    if (team == 1){
      fill_rainbow(leds_1, NUM_LEDS, thisHue, 10);
    }
    else{
      fill_rainbow(leds_2, NUM_LEDS, thisHue, 10);
    }
    
    FastLED.show();
  }

}
