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
const int OLAF_BTN = 5;
const int ELSA_BTN = 6;
const int KLAX_BTN = 7;
const int BEEP_BTN = 8;
const int ONE_BTN = 9;
const int TWO_BTN = 10;
const int THREE_BTN = 11;

// Buttons management
int olaf_btn_prev;
int elsa_btn_prev;
int klax_btn_prev;
int beep_btn_prev;
int one_btn_prev;
int two_btn_prev;
int three_btn_prev;

Flasher olaf_flash(OLAF_LED, 1000, 1000, 30000);
Flasher elsa_flash(ELSA_LED, 1000, 1000, 27000);


int playSongBtn(int btn, int btn_prev, int song)
{
  int btn_state;
  btn_state = digitalRead(btn);

  if ( (btn_prev == HIGH) && (btn_state == LOW) ) {
    mp3.playFile(song);

    if (btn == OLAF_BTN) 
    {
      olaf_flash.setEndBlink();
    }
    else if (btn == ELSA_BTN) 
    {
      elsa_flash.setEndBlink();
    }
  }
  return btn_state;
}


void setup()
{

  // Initialize the Qwiic MP3 Trigger
  Wire.begin();

  //Check to see if Qwiic MP3 is present on the bus
  if (mp3.begin() == false)
  {
    while (1);
  }

  mp3.setVolume(11); //Volume can be 0 (off) to 31 (max)

  //pinMode setup
  pinMode(OLAF_BTN, INPUT_PULLUP);
  pinMode(ELSA_BTN, INPUT_PULLUP);
  pinMode(KLAX_BTN, INPUT_PULLUP);
  pinMode(BEEP_BTN, INPUT_PULLUP);
  pinMode(ONE_BTN, INPUT_PULLUP);
  pinMode(TWO_BTN, INPUT_PULLUP);
  pinMode(THREE_BTN, INPUT_PULLUP);

}


void loop()
{
  // Update flashing leds
  olaf_flash.Update();
  elsa_flash.Update();

  // Actions for all the buttons
  olaf_btn_prev = playSongBtn(OLAF_BTN, olaf_btn_prev, 5);
  elsa_btn_prev = playSongBtn(ELSA_BTN, elsa_btn_prev, 6);
  klax_btn_prev = playSongBtn(KLAX_BTN, klax_btn_prev, 7);
  beep_btn_prev = playSongBtn(BEEP_BTN, beep_btn_prev, 8);
  one_btn_prev = playSongBtn(ONE_BTN, one_btn_prev, 9);
  two_btn_prev = playSongBtn(TWO_BTN, two_btn_prev, 10);
  three_btn_prev = playSongBtn(THREE_BTN, three_btn_prev, 11); 
}
