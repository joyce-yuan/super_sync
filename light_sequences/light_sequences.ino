#include <FastLED.h>

// How many leds in your strip? will change depending on tower
#define NUM_LEDS 192 

// What's the DATA PIN? (Usually the green wire)
#define DATA_PIN_1 6 // team 1 - green wire
#define DATA_PIN_2 5 // team 2 - orange wire

// Define the array of leds
CRGB leds_1[NUM_LEDS]; // array for team 1
CRGB leds_2[NUM_LEDS]; // array for team 2

// define level ranges
int levels[] = {0, 10, 20, 30, 40}; // starting led of each floor in tower
const int leds_per_floor = 10; // num of leds on each floor
long randNumber;
// const int BUTTON_PIN = 8;
// int buttonState;

void setup() { 
	Serial.begin(115200);
	Serial.println("resetting");
	FastLED.addLeds<WS2812,DATA_PIN_1,RGB>(leds_1,NUM_LEDS);
  FastLED.addLeds<WS2812,DATA_PIN_2,RGB>(leds_2,NUM_LEDS);
	FastLED.setBrightness(80);
  all_off();
  delay(2000);
}


void loop() { 
  light_by_color(3); // lights tower a solid color - to be put in generate_color()
  delay(5000);
  all_off(); // turns off all leds
  delay(3000);
  correct_color(1, 3); // flashes side of tower for team that got the point
  delay(3000);
  light_by_score(3, 4, 10); // lights up the floors according to team scores
  delay(3000);
  all_off(); 
  delay(3000);
  winning_sequence(2); // plays a rainbow sequence for winning team
  delay(1000);
  all_off();
  delay(3000);

}

void light_by_score(int score_1, int score_2, int per_floor){
  /* Lights up the tower one floor at a time to show a visual representation of a team's score
      Parameters:
        score_1 (int): the score of team 1
        score_2 (int): the score of team 2
        per_floor (int): the number of leds per floor in the tower, we don't light in between floors
      Return:
        nothing
  */

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
    delay(500); // lights floors one at a time, for visual appeal?
  }
}

void light_by_color(int color){
  int hue;
  switch (color){
    case 0: // tower is red
      hue = 96;
      break;
    case 1: // tower is green
      hue = 0;
      break;
    case 2: // tower is blue
      hue = 160;
      break;
    case 3: // tower is yellow
      hue = 80;
      break;
    default:
      hue = 192; // purple idk
      break;
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
    case 1: // tower is green
      hue = 0;
      break;
    case 2: // tower is blue
      hue = 160;
      break;
    case 3: // tower is yellow
      hue = 80;
      break;
    default:
      hue = 192; // purple idk
      break;
  }

  if (team == 1){
    for (int x = 0; x < 10; x++){
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
    for (int x = 0; x < 10; x++){
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
  for (int x=0;x<600;x++){
    Serial.println(x);
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




