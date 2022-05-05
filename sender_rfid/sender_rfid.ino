//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
// #include <TFT_eSPI.h>
#include <MFRC522.h>  //include the RFID reader library
#include <string.h>

#define RFID_SS_PIN 4  //slave select pin
#define RFID_RST_PIN 3  //reset pin
// create an MFRC522 object
MFRC522 mfrc522(RFID_SS_PIN, RFID_RST_PIN);  // instatiate a MFRC522 reader object.
MFRC522::MIFARE_Key key;          //create a MIFARE_Key struct named 'key', which will hold the card information
int block=60; // where we write and read data from a tag (this block is easier to sense from further away)
byte readbackblock[18]; // used to read data from tag
char tag_team_and_color[15]; // will hold identifier for tag
char wristband_team_and_color[] = "red-1"; // this should be changed for each sending wristband - checks to see if the right color tag was touched before sending


//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

// constants that are sender specific:
const int senderID = 1;

// const int reedPin = 3;
// const int ledPin =  13;      // the number of the LED pin

// {RED: 0, GREEN: 1, BLUE: 2, YELLOW: 3}
const int color = 0; 
const char* colorArray[] = {"RED", "GREEN", "BLUE", "YELLOW"};

// variables & state
int state = 0;


//address through which two modules communicate.
const byte address[6] = "00001";


void setup() {
  Serial.begin(115200);
  delay(1500);
  char buff[100];
  sprintf(buff, "Setting up sender %d with color %s", senderID, colorArray[color]);
  Serial.println(buff);
  // Serial.printf("Setting up sender %d with color $s", senderID, colorArray[color]);
  radio.begin();
  
  //set the address
  radio.openWritingPipe(address);
  
  //Set module as transmitter
  radio.stopListening();

  // initialize reader
  mfrc522.PCD_Init();
  // Prepare the security key for the read function.
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;  //keyByte is defined in the "MIFARE_Key" 'struct' definition in the .h file of the library
  }


  // gamestate
  state = 0;

//  switchState = HIGH;

}

void loop() {
  // normal state
  if (state == 0) {
    // attempts to read a tag and get its identifier
    if (mfrc522.PICC_IsNewCardPresent()) {
      Serial.println("new card present");
    //if(noCardCount > NO_CARD_DETECTIONS_BEFORE_NEW_READ){
      if (mfrc522.PICC_ReadCardSerial()){
        // read from block 60 of tag, put identifier into tag_team_and_color (e.g. "red-1")
        Serial.println("read serial");
        readBlock(block, readbackblock);
        memset(tag_team_and_color, '\0', 15);
        memcpy(tag_team_and_color, readbackblock, sizeof(readbackblock));
        Serial.println("");
        Serial.print("the tag has ID: ");
        Serial.println(tag_team_and_color);
        state = 1;
        
      }
    }
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    delay(100);
  } 
  // send state
  else if (state == 1) {
    if(strcmp(tag_team_and_color, wristband_team_and_color) == 0){ // if the tag wristband and sender wristband are same color, send to tower
      char text[10];
      sprintf(text, "%d%d", senderID, color);
      Serial.println(text);
      radio.write(&text, sizeof(text));
      char buff[50];
      sprintf(buff, "sender %d sent %s", senderID, text);
      Serial.println(buff);
    }
    state = 0;
    delay(100);

  }
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

//Read specific block
int readBlock(int blockNumber, byte arrayAddress[]) {
  /* This example function will write the data from block 60 of an RFID tag into a byte array (arrayAddress).
     The format of the byte array once transformed into a string will be "color-teamNumber" (i.e. "red-1")
  */
  int largestModulo4Number=blockNumber/4*4;
  int trailerBlock=largestModulo4Number+3;//determine trailer block for the sector

  //authentication of the desired block for access
  byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));

  if (status != MFRC522::STATUS_OK) {
         Serial.print("PCD_Authenticate() failed (read): ");
         Serial.println(mfrc522.GetStatusCodeName(status));
         return 3;//return "3" as error message
  }

//reading a block
byte buffersize = 18;//we need to define a variable with the read buffer size, since the MIFARE_Read method below needs a pointer to the variable that contains the size... 
status = mfrc522.MIFARE_Read(blockNumber, arrayAddress, &buffersize);//&buffersize is a pointer to the buffersize variable; MIFARE_Read requires a pointer instead of just a number
  if (status != MFRC522::STATUS_OK) {
          Serial.print("MIFARE_read() failed: ");
          Serial.println(mfrc522.GetStatusCodeName(status));
          return 4;//return "4" as error message
  }
  Serial.println("block was read");
}
