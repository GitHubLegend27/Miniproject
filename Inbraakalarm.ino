/* the tutorial code for 3x4 Matrix Keypad with Arduino is as
This code prints the key pressed on the keypad to the serial port*/

#include "Keypad.h"

const byte Rows= 4; //number of rows on the keypad i.e. 4
const byte Cols= 4; //number of columns on the keypad i,e, 3

//we will definne the key map as on the key pad:

char keymap[Rows][Cols]=
{
{'1', '2', '3', 'A'},
{'4', '5', '6', 'B'},
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};

//  a char array is defined as it can be seen on the above


//keypad connections to the arduino terminals is given as:


/* Define the reedSwitch and beeper pins. */
int reedSwitch = 2;
int beeper = 5;

/* Define the pins for the keypad. */
byte rPins[Rows]= {6,7,8,9}; //Rows 0 to 3
byte cPins[Cols]= {10,11,12,13}; //Columns 0 to 3

// command for library forkeypad
//initializes an instance of the Keypad class
Keypad kpd= Keypad(makeKeymap(keymap), rPins, cPins, Rows, Cols);

String alarmCode = "1337";
String currentTypedCode = "";

const String STATE_OFF = "STATE_OFF";
const String STATE_ON = "STATE_ON";
const String STATE_SWITCH = "STATE_SWITCH";

/* Strings that define the state change. */
const String CHANGE_OFF_TO_ON = "CHANGE_OFF_TO_ON";
const String CHANGE_ON_TO_OFF = "CHANGE_ON_TO_OFF";
const String CHANGE_NONE = "CHANGE_NONE";

/* Variables to hold the reed switch state, and the beeper state. */
String reedSwitchState = STATE_OFF;
String beeperState = STATE_OFF;

/* Variable to hold the time the beeping alarm started. */
unsigned long beepingAlarmStart = 0;

/* Variable to hold the amount in milliseconds the beeper should beep, and the interval between beeps when the beeper state is STATE_SWITCH. */
int beeperTime = 100;
int beeperSwitchInterval = 900;

/* Variable to hold the time in milliseconds after which the beeper alarm should go from STATE_SWITCH to STATE_ON. */
int maxBeepingAlarmTime = 30000;

void setup()
{
  pinMode(reedSwitch, INPUT);
  pinMode(beeper, OUTPUT);
  Serial.begin(9600);  // initializing serail monitor
}

void loop()
{
  /* If beeperState is STATE_OFF, check for changes in the reedSwitchState, else make beeping noice and read for keypad input. */
  if(beeperState == STATE_OFF) {
    /* Check if the reed switch has changed from on to off. */
    if(getReedSwitchChange() == CHANGE_ON_TO_OFF) {
      /* Start the beeping alarm. */
      startBeepingAlarm();
    }
  } else {
    /* Check if 30 seconds have passed since the alarm started. */
    if((millis() - beepingAlarmStart) >= maxBeepingAlarmTime) {
      beeperState = STATE_ON;
    }

    /* Make the beeper noice. */
    makeBeeperNoice();
    
    /* Get the pressed key. */
    char pressedKey = kpd.getKey();
    if (pressedKey != NO_KEY)
    { 
      
        /* Check if the pressed character is '*', else add the key to the current typed code. */
        if(pressedKey == '*') {
          /* Check if the current typed code is the same as the alarmcode. If not reset the current typed code. */
          if(currentTypedCode == alarmCode) {
            cancelAlarm();
          }

          /* Reset the current typed code. */
          currentTypedCode = "";
        } else {
          currentTypedCode += pressedKey;
        }
    }
  }
  
  delay(1);
}

void startBeepingAlarm() {
  beeperState = STATE_SWITCH;
  beepingAlarmStart = millis();
}

void cancelAlarm() {
  Serial.println("Cancelling alarm: ");
  digitalWrite(beeper, LOW);
  beeperState = STATE_OFF;
}

void makeBeeperNoice() {
  if(beeperState == STATE_ON) {
    digitalWrite(beeper, HIGH);
  } else if(beeperState == STATE_SWITCH) {
    if(millis() % (beeperTime + beeperSwitchInterval) < beeperSwitchInterval) {
      digitalWrite(beeper, LOW);
    } else {
      digitalWrite(beeper, HIGH);
    }
  }
}

String getReedSwitchChange() {
  String currentState = STATE_OFF;
  String changeState = CHANGE_NONE;
  if(digitalRead(reedSwitch) == HIGH) {
    currentState = STATE_ON;
  } else {
    currentState = STATE_OFF;
  }
  
//  Serial.println("LastReedSwitchState: " + reedSwitchState +", CurrentState: " + currentState); 
  if(reedSwitchState == STATE_ON && currentState == STATE_OFF) { 
    changeState = CHANGE_ON_TO_OFF;
  } else if(reedSwitchState == STATE_OFF && currentState == STATE_ON) {
    changeState = CHANGE_OFF_TO_ON;
  }

  reedSwitchState = currentState;
  return changeState;
}

                  
