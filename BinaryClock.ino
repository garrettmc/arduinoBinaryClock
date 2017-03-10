
const int latchPin = 8;//Pin connected to ST_CP of 74HC595
const int clockPin = 12;//Pin connected to SH_CP of 74HC595 
const int dataPin = 11; //Pin connected to DS of 74HC595 

// Buttons:
const int setHourPin = 5;
const int setMinPin  = 4;

// Set in loop() to millis(), so don't have to call wherever needed
unsigned long now;

// Used for setting the time using the buttons
int hourOffset = 0;
int minOffset  = 0;


// Button numbering for debounce logic.
#define BUTTON_HOUR 0
#define BUTTON_MIN  1


#define HOURS_PER_DAY   (24)
#define MINS_PER_HOUR   (60)
#define SECS_PER_MIN    (60)
#define DECIS_PER_SEC   (10)
#define MS_PER_SEC      (1000)
#define MS_PER_DECI_SEC (MS_PER_SEC / DECIS_PER_SEC)
#define MS_PER_MIN      (MS_PER_SEC * SECS_PER_MIN)
#define MS_PER_HOUR     (MS_PER_SEC * SECS_PER_MIN * MINS_PER_HOUR)

// Column #'s on the display
#define COL1    ((byte)~(1 << 0))
#define COL2    ((byte)~(1 << 1))
#define COL3    ((byte)~(1 << 2))
#define COL4    ((byte)~(1 << 3))
#define COL5    ((byte)~(1 << 4))
#define COL6    ((byte)~(1 << 5))
#define COL7    ((byte)~(1 << 6))
#define COL8    ((byte)~(1 << 7))



void setup ()
{
  // For debugging
  //Serial.begin(9600);
  
  //set pins to output
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT);
  pinMode(setHourPin, INPUT);
  pinMode(setMinPin, INPUT);
}


// Uses shiftOut to send out two bytes, then toggles the latchpin to have
// the the data written by the shifter to the display.
void sendData(byte b1, byte b2) {
  shiftOut(dataPin, clockPin, MSBFIRST, b1);
  shiftOut(dataPin, clockPin, MSBFIRST, b2);

  digitalWrite(latchPin,HIGH); //pull the latchPin to save the data
  delay(1); //wait for a microsecond
  digitalWrite(latchPin,LOW); //ground latchPin and hold low for as long as you are transmitting
}


// To "debounce" the buttons
#define DEBOUNCE_INTERVAL 50

// If hold down button, every this many ms, increment the item:
#define REPEAT_FREQUENCY  1000


int lastButtonState[] = {LOW, LOW};
unsigned long lastButtonStateTime[] = {0, 0};

// When pressing a button, it can bounce states back and forth before it settles down.
// So when a button changes state, note the time of state change, and only act after the button
// has remained in that state for a period of time.
void setButtonState(int buttonNum, int state) {
  // State change
  if (lastButtonState[buttonNum] != state) {
    lastButtonState[buttonNum] = state;
    lastButtonStateTime[buttonNum] = now;
  }

  // If pressed, and the debounce interval has elapsed, do the action, and reset
  // the time, to not do again until time elapses again.
  if ((state == HIGH) && 
      (now > lastButtonStateTime[buttonNum]) &&       // Unsigned, so check for underflow
      (now - lastButtonStateTime[buttonNum] > DEBOUNCE_INTERVAL))   // before subtract
  {
    if (buttonNum == BUTTON_HOUR) {
      // User changed the hour
      hourOffset++;
      // Don't increment again for REPEAT_FREQUENCY ms, done by setting the button's
      // down time as in the future by REPEAT_FREQUENCY minus the debounce interval...
      lastButtonStateTime[buttonNum] = now + REPEAT_FREQUENCY - DEBOUNCE_INTERVAL;
    } else if (buttonNum == BUTTON_MIN) {
      // User changed the minute.
      minOffset++;
      // Don't increment again for REPEAT_FREQUENCY ms, done by setting the button's
      // down time as in the future by REPEAT_FREQUENCY minus the debounce interval...
      lastButtonStateTime[buttonNum] = now + REPEAT_FREQUENCY - DEBOUNCE_INTERVAL;
    }
  }
}



void loop()
{
  // Handle the buttons
  setButtonState(BUTTON_HOUR, digitalRead(setHourPin));
  setButtonState(BUTTON_MIN,  digitalRead(setMinPin));
  
  now = millis();    // Time since startup
  int hour = ((int) (now / MS_PER_HOUR) + hourOffset) % HOURS_PER_DAY;
  int min  = ((int) (now / MS_PER_MIN)  + minOffset)  % MINS_PER_HOUR;
  int sec  = ((int) (now / MS_PER_SEC))  % SECS_PER_MIN;

  bool am = hour < 12;
  if (!am) hour -= 12;
  
  byte hour10 = hour / 10;
  byte hour1  = hour % 10;
  byte min10  = min / 10;
  byte min1   = min % 10;
  byte sec10  = sec / 10;
  byte sec1   = sec % 10;

  // Rightmost column is deci-seconds.  Output data for the column, then column #
  //sendData(deci,   COL8);
  sendData(sec1,   COL8);
  sendData(sec10,  COL7);
  sendData(min1,   COL6);
  sendData(min10,  COL5);
  sendData(hour1,  COL4);
  sendData(hour10, COL3);
  sendData(am,     COL1);

  // Delay a short period
  delay(10);
}



