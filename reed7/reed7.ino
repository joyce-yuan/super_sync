//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
// #include <TFT_eSPI.h>

//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

// constants that are sender specific:
const int senderID = 7;

const int buttonPin = 2;     // the number of the pushbutton pin
const int reedPin = 3;
const int ledPin =  13;      // the number of the LED pin

// {RED: 0, GREEN: 1, BLUE: , YELLOW: 3}
const int color = 3; 
const char* colorArray[] = {"RED", "GREEN", "BLUE", "YELLOW"};

// variables & state
int state = 0;
int buttonState;
int switchState;


//address through which two modules communicate.
const byte address[6] = "00001";

enum button_state {S0, S1, S2, S3, S4};
class Button {
  // BUTTON CLASS (from Exercise 04 "A Classy Button")
  public:
  uint32_t S2_start_time;
  uint32_t button_change_time;    
  uint32_t debounce_duration;
  uint32_t long_press_duration;
  uint8_t pin;
  uint8_t flag;
  uint8_t button_pressed;
  button_state state; // This is public for the sake of convenience
  Button(int p) {
  flag = 0;  
    state = S0;
    pin = p;
    S2_start_time = millis(); //init
    button_change_time = millis(); //init
    debounce_duration = 10;
    long_press_duration = 1000;
    button_pressed = 0;
  }
  void read() {
    uint8_t button_val = digitalRead(pin);  
    button_pressed = !button_val; //invert button
  }
  int update() {
    int flag_place = 0;
    read();
    flag = 0;
    //your code here!<------!!!!
    switch(state){
      case S0:
        if(button_pressed){
          state = S1;
          button_change_time = millis();
        }
        break;
      
      case S1:
        if (!button_pressed){
          state = S0;
          button_change_time = millis();
        }
        else{
          if (millis() - button_change_time >= debounce_duration){
            state = S2;
            S2_start_time = millis();
          }
        }
        break;

      case S2:
        flag_place = 1;
        if (!button_pressed){
          state = S4;
          button_change_time = millis();
        }
        else{
          if (millis() - S2_start_time >= long_press_duration){
            state = S3;
          }
        }
        break;

      case S3:
        flag_place = 2;
        if (!button_pressed){
          state = S4;
          button_change_time = millis();
        }
        break;
      
      case S4:
        if (button_pressed){
          if (millis() - S2_start_time < long_press_duration){
            state = S2;
            button_change_time = millis();
            flag_place = 1;
            
          }
          else{
            state = S3;
            button_change_time = millis();
            flag_place = 2; 
            
          }
        }
        else{
          if (millis() - button_change_time >= debounce_duration){
            if (millis() - S2_start_time < long_press_duration){
              flag = 1;
            }
            else if (millis() - S2_start_time >= long_press_duration){
              flag = 2;
            }
            state = S0;
          }
        }
        break;

    }
    return flag;
  }
};
Button button(2);
void setup() {
  Serial.begin(115200);
  char buff[100];
  sprintf(buff, "Setting up sender %d with color %s", senderID, colorArray[color]);
  Serial.println(buff);
  // Serial.printf("Setting up sender %d with color $s", senderID, colorArray[color]);
  radio.begin();
  
  //set the address
  radio.openWritingPipe(address);
  
  //Set module as transmitter
  radio.stopListening();

  // initialize the LED pin as an output:
 // pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(reedPin, INPUT_PULLUP);

  // gamestate
  state = 0;

//  switchState = HIGH;

}

void loop() {
  // put your main code here, to run repeatedly:
  // turn LED on:
  // digitalWrite(ledPin, HIGH);

  int buttonState = button.update();
  
   switchState = digitalRead(reedPin);

//  char buff[50];
//  sprintf(buff, "switchState %d buttonState %d", switchState, buttonState);
//  Serial.println(buff);
  
  buttonState = button.update();

  // normal state
  if (state == 0) {
    if (buttonState == 1 || buttonState == 2) {
      Serial.println("Button pressed");
      state = 1;
    }
    if (switchState == LOW){
      Serial.println("Reed Switch on");
      state = 1;
    }
  } 
  // send state
  else if (state == 1) {
    char text[10];
    sprintf(text, "%d%d", senderID, color);
    radio.write(&text, sizeof(text));
    char buff[50];
    sprintf(buff, "sender %d sent %s", senderID, text);
    Serial.println(buff);
    state = 0;
    delay(500);

  }
}
