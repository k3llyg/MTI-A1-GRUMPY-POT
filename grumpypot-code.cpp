#include <LedControl.h>
//Moisture sensor pin on input pin A0
int moisturePin = A0;

int hydrationLevel; // Declare soil moisture level as global variable
const int dehydrated = 771; // Soil too dry, gonna die!
const int hydrated = 482; // Soil too wet, also gonna die!

//Buzzer pin on D3 (digital pin 3)
const int plant_mouth = 3;

//Declare LED matrix pin
int DIN = 10;
int CS =  9;
int CLK = 8;

//Declare switch pin
#define switch_of_fate 2
bool On = false;


//Cooking ingredients: LED UIs
byte normal[8]=   {0x3c,0x42,0x99,0xbd,0xbd,0x99,0x42,0x3c};
byte happy[8]=   {0x3c,0x42,0x99,0xa5,0xa5,0x81,0x42,0x3c};
byte sad[8]=   {0x3c,0x42,0x81,0xa5,0xa5,0x99,0x42,0x3c};
byte shuteye[8]= {0x3c,0x42,0x81,0x81,0xbd,0x81,0x42,0x3c};
byte finger[8]= {0x18,0x18,0x18,0x18,0x7e,0x7e,0x7e,0x3c};
byte death[8]= {0x3c,0x42,0xa5,0x99,0x99,0xa5,0x42,0x3c};

LedControl lc=LedControl(DIN,CLK,CS,1); // Create an instance of the LedControl class

//Define LED function  
void printByte(byte character [])
{
  int i = 0; // Initialize a loop counter variable
  for(i=0;i<8;i++) // Loop through each row of the character array
  {
    lc.setRow(0,i,character[i]); // Set the LEDs in a row of the LED matrix
  }
} 

//Plant emotion UIs
void happyFace(){
    printByte(happy);     
    delay(1000);

    printByte(normal);     
    delay(1000);
}

void speechlessFace(){
    printByte(normal);     
    delay(1000);

    printByte(shuteye);     
    delay(1000);
}

void madFace() {
      printByte(sad);
      delay(500);
      
      printByte(death);
      delay(500);
    
      printByte(finger);
      delay(1000);
    }

//Cooking ingredients: music (death threats in plant language)!
#define NOTE_A 440
#define NOTE_G 392
#define NOTE_F 349
#define NOTE_E 330
#define NOTE_D 294
#define NOTE_CS 277

//Toccata and Fugue in D Minors
int melody1[] = { //Music Notes
  NOTE_A, NOTE_G, NOTE_A, NOTE_G, NOTE_F, NOTE_E, NOTE_D, NOTE_CS, NOTE_D
}; 

//Rythmn of Toccata and Fugue in D Minor
int noteDurations1[] = {
  4, int(8/0.8), 1, int(4/0.3), int(4/0.3), int(4/0.3), int(4/0.3), int(4/0.3), 2
};

void mp3Threat1() {   //Iterate through notes and melody
  for (int i = 0; i < 9; i++) {
    int noteDuration = 1000/noteDurations1[i]; // calculate note duration
    tone(plant_mouth, melody1[i], noteDuration); // plays the melody
    int pauseBetweenNotes = noteDuration * 1.30; // pause between notes
    delay(pauseBetweenNotes); 
    noTone(plant_mouth); // stops the tone, so next not starts from silence
  }
  delay(500); // pause 0.5 seconds between melody
}

//Toccata and Fugue in D Minor (reversed)
void mp3Threat2(){
    for (int i = 8; i >= 0; i--) {   //Start from the end of notes and melody array
    int noteDuration = 1000/noteDurations1[i]; // calculate note duration
    tone(plant_mouth, melody1[i], noteDuration); // plays the melody
    int pauseBetweenNotes = noteDuration * 1.30;// pause between notes
    delay(pauseBetweenNotes); 
    noTone(plant_mouth); // stops the tone, so next not starts from silence
  }
  delay(500); // pause 0.5 seconds between melody
}

//Declare millis variables
unsigned long currentMillis = 0; // Millis = current time
unsigned long previousMillisVisual = 0;
unsigned long previousMillisAudio = 0;  // Declare previousMillisAudio as a global variable
const long intervalVisual = 1000;
const long intervalAudio = 1000; 
void updateGraphics() {
  // Function is intentionally left empty
}

//Setup input and outputs
void setup() {
   Serial.begin(9600);
   pinMode(plant_mouth, OUTPUT);  // set buzzer as output

   lc.shutdown(0,false);  //turn on LED matrix
   lc.setIntensity(0,5);  //set LED matrix brightness
   lc.clearDisplay(0); //turns off LED matrix to clear display

   pinMode(moisturePin, INPUT); // set moisture sensor as input
   pinMode(switch_of_fate, INPUT_PULLUP); // set switch as input
   
}


//Main program logic (everything everywhere all at once )
void moistureDetection(){
  unsigned long currentMillis = millis(); //call millis function
// If moisture level less than 40%
  if (hydrationLevel < 40){
    // Play this audio non-blocking function
    // If current time - 0 is greater than the interval of 1 second, do the following
    if (currentMillis - previousMillisAudio >= intervalAudio) {
      digitalWrite(plant_mouth, HIGH); // Start audio (horror music)
      mp3Threat1(); // Play Toccata and Fugue in D Minor
      previousMillisAudio = currentMillis; // Update timing for next loop
      } 

      else{  //Otherwise, turnoff audio
        digitalWrite(plant_mouth, LOW);
      }

    // Play this visual non-blocking function
    // If current time - 0 is greater than the interval of 1 second, do the following
    if (currentMillis - previousMillisVisual >= intervalVisual) {
      madFace(); // Display visual: mad face
      previousMillisVisual = currentMillis; // Update timing for next loop
    }
  }

// If moisture level is at 40-85%
  else if (hydrationLevel >= 40 && hydrationLevel <= 85){
    // No horror music thank god
    digitalWrite(plant_mouth, LOW);
    // Display visual: happy face
    happyFace();
  }

// If moisture level is more than 85%
  else {
    // Play don't put too much water audio reminder (horror music)
    digitalWrite(plant_mouth, HIGH);
    mp3Threat2(); // Play Toccata and Fugue in D Minor (reveresed)

    // Display visual: speechless face
    speechlessFace();
  }
}


void loop() {

// If the switch is turned off
if (digitalRead(switch_of_fate) == LOW) {
  On = true;  // Turns on the switch
  delay(200); // pause for 0.2 seconds to debounce the switch
}

if (On) { // If the switch is on
// Read analog value from moisture sensor
int moistureVal = analogRead(moisturePin);
// Convert soil moisture analog value into precentage
// dry soil (0%) wet soil (100%)
    hydrationLevel = map(moistureVal, dehydrated, hydrated, 0, 100); 

  Serial.println(hydrationLevel);
  Serial.print("%"); // allows us to monitor soil mositure (%) in serial monitor

// Loop this funciton, gives different responses according to different conditions
  moistureDetection(); 

// Pause for 0.1 second before the next loop
  delay(100);
  }
} 