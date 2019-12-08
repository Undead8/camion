#include <Wire.h> //Needed for I2C to Qwiic MP3 Trigger
#include "SparkFun_Qwiic_MP3_Trigger_Arduino_Library.h" //http://librarymanager/All#SparkFun_MP3_Trigger


class Flasher
{
  int ledPin;      // the number of the LED pin
  long OnTime;     // milliseconds of on-time
  long OffTime;    // milliseconds of off-time
  long duration;   // Total duration of all the blinks in ms
  long endBlink;   // Time when the blinks end in ms
  int ledState;    // ledState used to set the LED
  unsigned long previousMillis;   // will store last time LED was updated

  public:
  Flasher(int pin, long on, long off, long dur)
  {
  ledPin = pin;
  pinMode(ledPin, OUTPUT);     
    
  OnTime = on;
  OffTime = off;
  duration = dur;
  endBlink = 0;
  
  ledState = LOW; 
  previousMillis = 0;
  }

  void Update()
  {
    unsigned long currentMillis = millis();
     
    if((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
    {
      ledState = LOW;  // Turn it off
      previousMillis = currentMillis;  // Remember the time
      digitalWrite(ledPin, ledState);  // Update the actual LED
    }
    else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime) && (currentMillis < endBlink))
    {
      ledState = HIGH;  // turn it on
      previousMillis = currentMillis;   // Remember the time
      digitalWrite(ledPin, ledState);   // Update the actual LED
    }
  }

  void setEndBlink()
  {
    endBlink = (millis() + duration);
  }
};


// Constants and variables used in playSongBtn, setup and loop
// Qwiic MP3 trigger object
MP3TRIGGER mp3;

// Pin constants
const int OLAF_LED = 3;
const int ELSA_LED = 4;
const int OLAF_BTN = 8;
const int ELSA_BTN = 6;
const int KLAX_BTN = 7;
const int BEEP_BTN = 5;
const int ONE_BTN = 9;
const int TWO_BTN = 10;
const int THREE_BTN = 11;

// Buttons management
int olaf_btn_prev = HIGH;
int elsa_btn_prev = HIGH;
int klax_btn_prev = HIGH;
int beep_btn_prev = HIGH;
int one_btn_prev = HIGH;
int two_btn_prev = HIGH;
int three_btn_prev = HIGH;

int olaf_btn_state = HIGH;
int elsa_btn_state = HIGH;
int klax_btn_state = HIGH;
int beep_btn_state = HIGH;
int one_btn_state = HIGH;
int two_btn_state = HIGH;
int three_btn_state = HIGH;

// Debouncer
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 200;    // the debounce time; increase if the output flickers

// Flashers
Flasher olaf_flash(OLAF_LED, 1000, 1000, 30000);
Flasher elsa_flash(ELSA_LED, 1000, 1000, 27000);


void startSong(int btn, int song)
{
  mp3.playFile(song);
  SerialUSB.print("Pressed button number ");
  SerialUSB.println(btn);

  if (btn == OLAF_BTN) 
  {
    olaf_flash.setEndBlink();
   }
   else if (btn == ELSA_BTN) 
   {
     elsa_flash.setEndBlink();
   } 
}


void playSongBtn(int btn, int *btn_state, int *btn_prev, int song)
{
  int reading;
  reading = digitalRead(btn);

  // Debouncing: If the button changed, due to noise or pressing, reset the timer
  if (reading != *btn_prev) { lastDebounceTime = millis(); }

  // Debouncing: If the button is stable, set it as the current state
  if ((millis() - lastDebounceTime) > debounceDelay) {

    // If the button is now reading LOW from HIGH, play the song
    if ((*btn_state == HIGH) && (reading == LOW)) { startSong(btn, song); }

    // Set the button to its stable state
    *btn_state = reading;
  }
  
  *btn_prev = reading;
}


void setup()
{

  // Initialize the Qwiic MP3 Trigger
  Wire.begin();

  //Check to see if Qwiic MP3 is present on the bus
  if (mp3.begin() == false)
  {
    SerialUSB.println("Failed to initialize MP3 trigger");
    while (1);
  }

  mp3.setVolume(9); //Volume can be 0 (off) to 31 (max)

  //pinMode setup
  pinMode(OLAF_BTN, INPUT_PULLUP);
  pinMode(ELSA_BTN, INPUT_PULLUP);
  pinMode(KLAX_BTN, INPUT_PULLUP);
  pinMode(BEEP_BTN, INPUT_PULLUP);
  pinMode(ONE_BTN, INPUT_PULLUP);
  pinMode(TWO_BTN, INPUT_PULLUP);
  pinMode(THREE_BTN, INPUT_PULLUP);

  // Serial initialize
  SerialUSB.begin(9600);
  //while (!SerialUSB);
  //SerialUSB.println("SerialUSB initialized");

}


void loop()
{
  // Update flashing leds
  olaf_flash.Update();
  elsa_flash.Update();

  // Actions for all the buttons
  playSongBtn(OLAF_BTN, &olaf_btn_state, &olaf_btn_prev, 5);
  playSongBtn(ELSA_BTN, &elsa_btn_state, &elsa_btn_prev, 6);
  playSongBtn(KLAX_BTN, &klax_btn_state, &klax_btn_prev, 7);
  playSongBtn(BEEP_BTN, &beep_btn_state, &beep_btn_prev, 8);
  playSongBtn(ONE_BTN, &one_btn_state, &one_btn_prev, 9);
  playSongBtn(TWO_BTN, &two_btn_state, &two_btn_prev, 10);
  playSongBtn(THREE_BTN, &three_btn_state, &three_btn_prev, 11); 
}
