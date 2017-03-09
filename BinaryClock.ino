
const int latchPin = 8;//Pin connected to ST_CP of 74HC595
const int clockPin = 12;//Pin connected to SH_CP of 74HC595 
const int dataPin = 11; //Pin connected to DS of 74HC595 

void setup ()
{
  //set pins to output
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT);
}

byte data = 0;
int count = 0;

void loop()
{
  // Increment the value periodically
  if (0 == (++count % 100)) {
    data++;
  }

  // 
  for (int row=0;  row < 8;  row++) {
    shiftOut(dataPin, clockPin, MSBFIRST, data); // Row data (all rows)
    shiftOut(dataPin, clockPin, MSBFIRST, 0x01 << row); // Column data
    digitalWrite(latchPin,HIGH); //pull the latchPin to save the data
    delay(1); //wait for a microsecond
    digitalWrite(latchPin,LOW); //ground latchPin and hold low for as long as you are transmitting
    delay(1);
  }

  delay(10);
}



