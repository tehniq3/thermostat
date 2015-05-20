// http://nicuflorica.blogspot.ro/2015/05/termostat-cu-afisaj-led.html
// http://nicuflorica.blogspot.ro/2013/07/afisaje-led-cu-7-segmente-si-arduino-ii.html
// http://nicuflorica.blogspot.ro/2015/05/termostat-cu-2-praguri-de-temperatura.html
/*
 original thermostat sketch by niq_ro (Nicu FLORICA) from http://www.tehnic.go.ro
 based on sketch for multiplexed display: 6-13-2011, Spark Fun Electronics 2011, Nathan Seidle

 This code is public domain but you buy me a beer if you use this and we meet 
 someday (Beerware license).

 4 digit 7 segment display:
 http://www.sparkfun.com/products/9483
 Datasheet: 
 http://www.sparkfun.com/datasheets/Components/LED/7-Segment/YSD-439AR6B-35.pdf

 This is an example of how to drive a 7 segment LED display from an ATmega
 without the use of current limiting resistors. This technique is very common 
 but requires some knowledge of electronics - you do run the risk of dumping 
 too much current through the segments and burning out parts of the display. 
 If you use the stock code you should be ok, but be careful editing the 
 brightness values.

 This code should work with all colors (red, blue, yellow, green) but the 
 brightness will vary from one color to the next because the forward voltage 
 drop of each color is different. This code was written and calibrated for the 
 red color.

 This code will work with most Arduinos but you may want to re-route some of 
 the pins.

 7 segments
 4 digits
 1 colon
 =
 12 pins required for full control 

 */
// modified connexion by niq_ro from http://nicuflorica.blogspot.com
// http://arduinotehniq.blogspot.com/
// for my Luckylight KW4-563ASA
// dataseet: http://www.tme.eu/ro/Document/dfc2efde2e22005fd28615e298ea2655/KW4-563XSA.pdf

byte digit1 = 11; //PWM Display pin 12 (digit1 is common anonds A1 from right side)
byte digit2 = 10; //PWM Display pin 9 (digit2 is  common A2)
byte digit3 = 9; //PWM Display pin 8 (digit3 is common anods A3)
byte digit4 = 6; //PWM Display pin 6 (digit4 is common anods, from left side)

//Pin mapping from Arduino to the ATmega DIP28 if you need it
// http://www.arduino.cc/en/Hacking/PinMapping
byte segA = 2; //Display pin 11
byte segB = 3; //Display pin 7
byte segC = 4; //Display pin 4
byte segD = 5; //Display pin 2
byte segE = 12; //Display pin 1
byte segF = 7; //Display pin 10
byte segG = 8; //Display pin 5

byte dP = 13; // decimal point

#define LMPIN A0     // what pin we're LM35 is connected

#define BUT1 A1    // - switch
#define BUT2 A2    // + switch
#define BUT3 A3    // MENU switch

#define RECE A4    // output for lower temperature
#define CALD A5    // output for upper temperature
#define BINE A6    // output for ok temperature

//int tset = 240;  // temperature set (x10)
//int dt = 5;      // dt temperature

byte meniu = 0; // if MENIU = 0 is clasical 
                // if MENIU = 1 is for temperature set
                // if MENIU = 2 is for dt temperature 
float t, t0, t2, k;
int tset, tset1, tset2, dt; 

// http://tronixstuff.com/2011/03/16/tutorial-your-arduinos-inbuilt-eeprom/
#include <EEPROM.h>


void setup() {
 
  analogReference(INTERNAL); // for Uno (1,1V)
  //analogReference(INTERNAL1V1); // for Mega (1,1V)

// just first time... after must put commnent (//)
// EEPROM.write(101,0);    
// EEPROM.write(102,245);
// EEPROM.write(103,5);
  
  
tset1 = EEPROM.read(101);
tset2 = EEPROM.read(102);
tset = 256 * tset1 + tset2;  // recover the number
dt = EEPROM.read(103);

  
  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);

  pinMode(digit1, OUTPUT);
  pinMode(digit2, OUTPUT);
  pinMode(digit3, OUTPUT);
  pinMode(digit4, OUTPUT);
  
  pinMode(dP, OUTPUT);
  
  pinMode(BUT1, INPUT);
  pinMode(BUT2, INPUT);
  pinMode(BUT3, INPUT);

  pinMode(RECE, OUTPUT);
  pinMode(CALD, OUTPUT);
  pinMode(BINE, OUTPUT);

  digitalWrite(BUT1, HIGH); // pull-ups on
  digitalWrite(BUT2, HIGH);
  digitalWrite(BUT3, HIGH);

  digitalWrite(RECE, LOW);
  digitalWrite(CALD, LOW);
  digitalWrite(BINE, LOW);
  
 Serial.begin(9600);
 Serial.println("test for niq_ro");

k = 0.86;  // factor corectie citire la multiplexare
delay(100);
t = analogRead(A0);
t0 = t * 110 / 1023;  // pentru referinta interna de 1,1V;
t2 = t0;
}

void loop() {

  Serial.print("meniu = ");
  Serial.println(meniu);

  Serial.print("tset = ");
  Serial.println(tset/10);
  Serial.print("dt = ");
  Serial.println(dt/10);

if (digitalRead(BUT3) == LOW) 
{ meniu = meniu + 1;
delay(250);
}
if (meniu == 3) meniu = 0;

//if (millis() < 2000) citire();
//  t = analogRead(LMPIN);
//  t0 = t *500 / 1023;
leduri();

  Serial.print(t);
  Serial.print("/1023 --> ");
  Serial.print("t = ");
  Serial.println(t0);



//displayNumber(t0*10);
    
if (meniu == 0) {
int numara = 0;
  while (meniu == 0) {
numara = numara + 1;
  if (numara == 1000)
    {
//  float t = analogRead(LMPIN);
//  float t0 = t *500 / 1023;
citire();
t0 = (t0 + t2) /2;
leduri();
 // delay(5);
  numara = 0;
  Serial.print("t = ");
  Serial.println(t0);
//  displayNumber(t0*10); // this is number to diplay
    }
  te(t0*10); // this is number to diplay
  t2 = t0;
if (digitalRead(BUT3) == LOW) 
{ meniu = 1;
delay(250);
}
//Serial.print("t = ");
//Serial.println(t0);
//delay(5);
} 
delay(5);  
}



if (meniu == 1) {
   while (meniu == 1) {
//     minim(tset/10); // this is number to diplay
   teset(tset);
   if (digitalRead(BUT1) == LOW)
  { tset = tset - 1; 
  delay(250);
  }
   if (digitalRead(BUT2) == LOW)
  { tset = tset + 1;
  delay(250);
  }
  if (digitalRead(BUT3) == LOW) 
  { meniu = 2;
  delay(250);
  }
//  delay(15);
     }
     // scriu in memorie temperatura dorita
//tset1 = tset % 256;
//tset1 = tset /= 256;
//  Serial.print("tset1z = ");
//  Serial.println(tset1);
//  Serial.print("tset2u = ");
//  Serial.println(tset2);

tset1 = tset / 256;
tset2 = tset - tset1 * 256;
EEPROM.write(101, tset1);  // partea intreaga
EEPROM.write(102, tset2);   // rest
  Serial.print("tset1z = ");
  Serial.println(tset1);
  Serial.print("tset2u = ");
  Serial.println(tset2);
  
  Serial.print("tset = ");
  Serial.println(tset/10);
   delay (100);
}

if (meniu == 2) {
 // if (tmax <= tmin) tmax = tmin + 10;
   while (meniu ==2) {
 //    maxim(dt/10); // this is number to diplay
     dete(dt); // this is number to diplay
   if (digitalRead(BUT1) == LOW) 
   { dt = dt - 1;
   delay(250);
  }
   if (digitalRead(BUT2) == LOW) 
   { dt = dt + 1;
   delay(250);
  }
   if (digitalRead(BUT3) == LOW) 
   { meniu = 0;
   delay(250);
  }
//   delay(15);
if (dt < 1) dt = 1;
    }
    
  EEPROM.write(103,dt);      
  Serial.print("dt = ");
  Serial.println(dt/10);

    delay(100);
}
}
//Given a number, we display 10:22
//After running through the 4 numbers, the display is left turned off

//Display brightness
//Each digit is on for a certain amount of microseconds
//Then it is off until we have reached a total of 20ms for the function call
//Let's assume each digit is on for 1000us
//Each digit is on for 1ms, there are 4 digits, so the display is off for 16ms.
//That's a ratio of 1ms to 16ms or 6.25% on time (PWM).
//Let's define a variable called brightness that varies from:
//5000 blindingly bright (15.7mA current draw per digit)
//2000 shockingly bright (11.4mA current draw per digit)
//1000 pretty bright (5.9mA)
//500 normal (3mA)
//200 dim but readable (1.4mA)
//50 dim but readable (0.56mA)
//5 dim but readable (0.31mA)
//1 dim but readable in dark (0.28mA)



void dete(int toDisplay) {
#define DISPLAY_BRIGHTNESS  750

#define DIGIT_ON  HIGH
#define DIGIT_OFF  LOW

  for(int digit = 4 ; digit > 0 ; digit--) {

    //Turn on a digit for a short amount of time
    switch(digit) {
    case 1:
      digitalWrite(digit1, DIGIT_ON);
      lightNumber(15); // display degree symbol
      delayMicroseconds(DISPLAY_BRIGHTNESS); 
      break;

    case 2:
      digitalWrite(digit2, DIGIT_ON);
       digitalWrite(13, LOW);
      lightNumber(toDisplay % 10);
      toDisplay /= 10;
      delayMicroseconds(DISPLAY_BRIGHTNESS); 
      break;
   case 3:
      digitalWrite(digit3, DIGIT_ON);
//      digitalWrite(13, HIGH);
//      digitalWrite(13, LOW);
      lightNumber(toDisplay % 10);
      toDisplay /= 10;
      delayMicroseconds(DISPLAY_BRIGHTNESS); 
      break;
   case 4:
      digitalWrite(digit4, DIGIT_ON);
      lightNumber(12); // display C letter
      delayMicroseconds(DISPLAY_BRIGHTNESS); 
      break;
    }
     //Turn off all segments
    lightNumber(10); 

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF);
}
} 

void te(int toDisplay) {
#define DISPLAY_BRIGHTNESS  500

#define DIGIT_ON  HIGH
#define DIGIT_OFF  LOW
 for(int digit = 4 ; digit > 0 ; digit--) {

    //Turn on a digit for a short amount of time
    switch(digit) {
    case 1:
      digitalWrite(digit1, DIGIT_ON);
      digitalWrite(13, HIGH);
      lightNumber(toDisplay % 10);
      toDisplay /= 10;
      break;
   case 2:
      digitalWrite(digit2, DIGIT_ON);
      digitalWrite(13, LOW);
      lightNumber(toDisplay % 10);
      toDisplay /= 10;
      break;
    case 3:
      digitalWrite(digit3, DIGIT_ON);
      digitalWrite(13, HIGH);
      lightNumber(toDisplay % 10);
      toDisplay /= 10;
      break;
    case 4:
      digitalWrite(digit4, DIGIT_ON);
      lightNumber(12); // display C letter
      digitalWrite(13, HIGH);
      break;
    }
 //   lightNumber(toDisplay % 10);
 //   toDisplay /= 10;
    delayMicroseconds(DISPLAY_BRIGHTNESS); 

     //Turn off all segments
    lightNumber(10); 

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF);
}
} 


//Given a number, turns on those segments
//If number == 10, then turn off number
void lightNumber(int numberToDisplay) {

#define SEGMENT_ON  LOW
#define SEGMENT_OFF HIGH

  switch (numberToDisplay){

  case 0:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_OFF);
    digitalWrite(13, SEGMENT_OFF);
    break;

  case 1:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    digitalWrite(13, SEGMENT_OFF);
    break;

  case 2:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    digitalWrite(13, SEGMENT_OFF);
    break;

  case 3:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    digitalWrite(13, SEGMENT_OFF);
    break;

  case 4:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    digitalWrite(13, SEGMENT_OFF);
    break;

  case 5:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    digitalWrite(13, SEGMENT_OFF);
    break;

  case 6:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    digitalWrite(13, SEGMENT_OFF);
    break;

  case 7:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    digitalWrite(13, SEGMENT_OFF);
    break;

  case 8:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    digitalWrite(13, SEGMENT_OFF);
    break;

  case 9:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    digitalWrite(13, SEGMENT_OFF);
    break;

  // all segment are ON
  case 10:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    digitalWrite(13, SEGMENT_OFF);
    break;
  
  // degree symbol made by niq_ro
  case 11:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    digitalWrite(13, SEGMENT_OFF);
    break;

  // C letter made by niq_ro
  case 12:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_OFF);
    digitalWrite(13, SEGMENT_OFF);
    break;
  
  // c letter made by niq_ro
  case 13:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    digitalWrite(13, SEGMENT_OFF);
    break;

  case 14:  // lower line (d segment)
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    digitalWrite(13, SEGMENT_OFF);
    break;

case 15:  // upper line (a segment)
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    digitalWrite(13, SEGMENT_OFF);
    break;
    
  }
}


void leduri() {
 digitalWrite(CALD, LOW);
 digitalWrite(RECE, LOW);
 digitalWrite(BINE, LOW);

if ( t0*10 < tset ) 
{  digitalWrite(RECE, HIGH);
   digitalWrite(CALD, LOW);
   digitalWrite(BINE, LOW);
}
if ( t0*10 > tset + dt ) 
{  digitalWrite(CALD, HIGH);
   digitalWrite(RECE, LOW);
   digitalWrite(BINE, LOW);
}
if (( t0*10 <= tset+dt ) && ( t0*10 >= tset ))
{  digitalWrite(CALD, LOW);
   digitalWrite(RECE, LOW);
   digitalWrite(BINE, HIGH);
}
}



void teset(int toDisplay) {
#define DISPLAY_BRIGHTNESS  500

#define DIGIT_ON  HIGH
#define DIGIT_OFF  LOW
 for(int digit = 4 ; digit > 0 ; digit--) {

    //Turn on a digit for a short amount of time
    switch(digit) {
    case 1:
      digitalWrite(digit1, DIGIT_ON);
      digitalWrite(13, HIGH);
      lightNumber(toDisplay % 10);
      toDisplay /= 10;
      break;
   case 2:
      digitalWrite(digit2, DIGIT_ON);
      digitalWrite(13, LOW);
      lightNumber(toDisplay % 10);
      toDisplay /= 10;
      break;
    case 3:
      digitalWrite(digit3, DIGIT_ON);
      digitalWrite(13, HIGH);
      lightNumber(toDisplay % 10);
      toDisplay /= 10;
      break;
    case 4:
      digitalWrite(digit4, DIGIT_ON);
      lightNumber(13); // display C letter
      digitalWrite(dP, HIGH);
      break;
    }
 //   lightNumber(toDisplay % 10);
 //   toDisplay /= 10;
    delayMicroseconds(DISPLAY_BRIGHTNESS); 

     //Turn off all segments
    lightNumber(10); 

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF);

}
} 

float citire() {
  t = analogRead(A0);
  t0 = t * k *110 / 1023;  // pentru referinta interna de 1,1V;
//  t0 = t *500 / 1023;
  return t0;
}
