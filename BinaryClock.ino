
const int latchPin = 8;//Pin connected to ST_CP of 74HC595
const int clockPin = 12;//Pin connected to SH_CP of 74HC595 
const int dataPin = 11; //Pin connected to DS of 74HC595 


void sendData() {
  digitalWrite(latchPin,HIGH); //pull the latchPin to save the data
  delay(1); //wait for a microsecond
  digitalWrite(latchPin,LOW); //ground latchPin and hold low for as long as you are transmitting
}


void setup ()
{
  //set pins to output
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT);
}

#define HOURS_PER_DAY   (24)
#define MINS_PER_HOUR   (60)
#define SECS_PER_MIN    (60)
#define DECIS_PER_SEC   (10)
#define MS_PER_SEC      (1000)
#define MS_PER_DECI_SEC (MS_PER_SEC * DECIS_PER_SEC)
#define MS_PER_MIN      (MS_PER_SEC * SECS_PER_MIN)
#define MS_PER_HOUR     (MS_PER_SEC * SECS_PER_MIN * MINS_PER_HOUR)

// Column #'s on the display
#define COL1    ~(1 << 0)
#define COL2    ~(1 << 1)
#define COL3    ~(1 << 2)
#define COL4    ~(1 << 3)
#define COL5    ~(1 << 4)
#define COL6    ~(1 << 5)
#define COL7    ~(1 << 6)
#define COL8    ~(1 << 7)


void loop()
{
  unsigned long ms = millis();    // Time since startup
  int hour = ((int) (ms / MS_PER_HOUR))     % HOURS_PER_DAY;
  int min  = ((int) (ms / MS_PER_MIN))      % MINS_PER_HOUR;
  int sec  = ((int) (ms / MS_PER_SEC))      % SECS_PER_MIN;
  int deci = ((int) (ms / MS_PER_DECI_SEC)) % DECIS_PER_SEC;

  // Col 1 is deci-seconds.  Output data for the column, then column #
  shiftOut(dataPin, clockPin, MSBFIRST, deci);
  shiftOut(dataPin, clockPin, MSBFIRST, COL1);
  sendData();

  // Col2 is seconds
  shiftOut(dataPin, clockPin, MSBFIRST, sec);
  shiftOut(dataPin, clockPin, MSBFIRST, COL2);
  sendData();

  // Col3 is minutes
  shiftOut(dataPin, clockPin, MSBFIRST, min);
  shiftOut(dataPin, clockPin, MSBFIRST, COL3);
  sendData();

  // Col2 is hours
  shiftOut(dataPin, clockPin, MSBFIRST, hour);
  shiftOut(dataPin, clockPin, MSBFIRST, COL4);
  sendData();

  delay(10);
}



