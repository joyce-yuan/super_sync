#include"pitches.h"

int melody[] = {
NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_A4,
NOTE_G4, NOTE_C5, NOTE_AS4, NOTE_A4,                   
NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_FS4, NOTE_DS4, NOTE_D4,
NOTE_C4, NOTE_D4,0,                                 

NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_A4,
NOTE_G4, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_AS4, NOTE_C5, NOTE_AS4, NOTE_A4,      //29               //8
NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_FS4, NOTE_DS4, NOTE_D4,
NOTE_C4, NOTE_D4,0,                                       

NOTE_D4, NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_DS5, NOTE_D5,
NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_C5,
NOTE_C4, NOTE_D4, NOTE_DS4, NOTE_FS4, NOTE_D5, NOTE_C5,
NOTE_AS4, NOTE_A4, NOTE_C5, NOTE_AS4,             //58

NOTE_D4, NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_DS5, NOTE_D5,
NOTE_C5, NOTE_D5, NOTE_C5, NOTE_AS4, NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_C5, NOTE_G4,
NOTE_A4, 0, NOTE_AS4, NOTE_A4, 0, NOTE_G4,
NOTE_G4, NOTE_A4, NOTE_G4, NOTE_FS4, 0,

NOTE_C4, NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_DS4,
NOTE_C4, NOTE_D4, 0,
NOTE_C4, NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_DS4,
NOTE_C4, NOTE_D4, END

};

// note durations: 8 = quarter note, 4 = 8th note, etc.
int noteDurations[] = {       //duration of the notes
8,4,8,4,
4,4,4,12,
4,4,4,4,4,4,
4,16,4,

8,4,8,4,
4,2,1,1,2,1,1,12,
4,4,4,4,4,4,
4,16,4,

4,4,4,4,4,4,
4,4,4,12,
4,4,4,4,4,4,
4,4,4,12,

4,4,4,4,4,4,
2,1,1,2,1,1,4,8,4,
2,6,4,2,6,4,
2,1,1,16,4,

4,8,4,4,4,
4,16,4,
4,8,4,4,4,
4,20,
};

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

void setup() {

Serial.begin(9600);

playmusic(scoreMusic, scoreDurations);
//for (int thisNote = 0; gameOverMusic[thisNote]!=-1; thisNote++) {
//
//int noteDuration = speed*gameOverDurations[thisNote];
//tone(3, gameOverMusic[thisNote],noteDuration*.95);
//Serial.println(gameOverMusic[thisNote]);
//
//delay(noteDuration);
//
//noTone(3);
//}
}

void loop() {
// no need to repeat the melody.
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
