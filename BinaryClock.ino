
const int latchPin = 8;//Pin connected to ST_CP of 74HC595
const int clockPin = 12;//Pin connected to SH_CP of 74HC595 
const int dataPin = 11; //Pin connected to DS of 74HC595 


void sendData(byte b1, byte b2) {
  shiftOut(dataPin, clockPin, MSBFIRST, b1);
  shiftOut(dataPin, clockPin, MSBFIRST, b2);

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


void loop()
{
  unsigned long ms = millis();    // Time since startup
  int hour = ((int) (ms / MS_PER_HOUR))     % HOURS_PER_DAY;
  int min  = ((int) (ms / MS_PER_MIN))      % MINS_PER_HOUR;
  int sec  = ((int) (ms / MS_PER_SEC))      % SECS_PER_MIN;
  int deci = ((int) (ms / MS_PER_DECI_SEC)) % DECIS_PER_SEC;

  bool am = hour < 12;
  if (!am) hour -= 12;
  
  byte hour10 = hour / 10;
  byte hour1  = hour % 10;
  byte min10  = min / 10;
  byte min1   = min % 10;
  byte sec10  = sec / 10;
  byte sec1   = sec % 10;

  // Rightmost column is deci-seconds.  Output data for the column, then column #
  sendData(deci,   COL8);
  sendData(sec1,   COL7);
  sendData(sec10,  COL6);
  sendData(min1,   COL5);
  sendData(min10,  COL4);
  sendData(hour1,  COL3);
  sendData(hour10, COL2);
  sendData(am,     COL1);

  // Delay a short period
  delay(10);
}



