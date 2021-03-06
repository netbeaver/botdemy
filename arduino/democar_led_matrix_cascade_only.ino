/*  Billobard Bot

    By Kevin Cho/Botdemy

    A simple line-following car with a billboard (four 8x8 LED matrix). It follows a line
    until it sees a stop sign (a thick penpendicular black line) and displays
    pre-programmed message.

    Max7219 code inspired by:
    http://tronixstuff.com/2013/10/11/tutorial-arduino-max7219-led-display-driver-ic/
*/

#include <avr/pgmspace.h>
#include <LedControl.h>

//message to display
const unsigned char scrollText[] PROGMEM = {"Welcome to Line-Following Robot Workshop!      \0"};
/*
//pin assignments for line following car
const int lSensor = 5; //left IR sensor input
const int rSensor = 2; //right IR sensor input

const int lMotPos = 6;  //left motor(+)
const int lMotNeg = 7;  //left motor(-1)

const int rMotNeg = 8;  //right motor (-1)
const int rMotPos = 11; //right motor (+1)

const int lMotPWM = 9;  //left motor PWM
const int rMotPWM = 10; //right motor PWM

//determine proper motor speed for left and right motor that allows the car
//to go straight and navigate turns
//int lMotSpeed = 200;  //set motor speed (1-255)
//int rMotSpeed = 165;  //set motor speed (1-255)

//int lMotSpeed = 200;  //set motor speed (1-255)
//int rMotSpeed = 165;  //set motor speed (1-255)
*/

//ping assignment for Max7219 8x8 LED matrix
//Digital ports are all used by other components so I'm using analog ports as digital
const int DIN = A0;            //DIN
const int CLK = A2;            //CLK
const int LOAD = A1;           //CS

//num of 8x8 display
const int numDevices = 4;

//init max7219
LedControl lc = LedControl(DIN, CLK, LOAD, numDevices);

//adjust scroll speed
const long scrollDelay = 75;

//5x7 font ASCII library
const unsigned char font5x7 [] PROGMEM = {      //Numeric Font Matrix (Arranged as 7x font data + 1x kerning data)
  B00000000,	//Space (Char 0x20)
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  6,

  B10000000,	//!
  B10000000,
  B10000000,
  B10000000,
  B00000000,
  B00000000,
  B10000000,
  2,

  B10100000,	//"
  B10100000,
  B10100000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  4,

  B01010000,	//#
  B01010000,
  B11111000,
  B01010000,
  B11111000,
  B01010000,
  B01010000,
  6,

  B00100000,	//$
  B01111000,
  B10100000,
  B01110000,
  B00101000,
  B11110000,
  B00100000,
  6,

  B11000000,	//%
  B11001000,
  B00010000,
  B00100000,
  B01000000,
  B10011000,
  B00011000,
  6,

  B01100000,	//&
  B10010000,
  B10100000,
  B01000000,
  B10101000,
  B10010000,
  B01101000,
  6,

  B11000000,	//'
  B01000000,
  B10000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  3,

  B00100000,	//(
  B01000000,
  B10000000,
  B10000000,
  B10000000,
  B01000000,
  B00100000,
  4,

  B10000000,	//)
  B01000000,
  B00100000,
  B00100000,
  B00100000,
  B01000000,
  B10000000,
  4,

  B00000000,	//*
  B00100000,
  B10101000,
  B01110000,
  B10101000,
  B00100000,
  B00000000,
  6,

  B00000000,	//+
  B00100000,
  B00100000,
  B11111000,
  B00100000,
  B00100000,
  B00000000,
  6,

  B00000000,	//,
  B00000000,
  B00000000,
  B00000000,
  B11000000,
  B01000000,
  B10000000,
  3,

  B00000000,	//-
  B00000000,
  B11111000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  6,

  B00000000,	//.
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B11000000,
  B11000000,
  3,

  B00000000,	///
  B00001000,
  B00010000,
  B00100000,
  B01000000,
  B10000000,
  B00000000,
  6,

  B01110000,	//0
  B10001000,
  B10011000,
  B10101000,
  B11001000,
  B10001000,
  B01110000,
  6,

  B01000000,	//1
  B11000000,
  B01000000,
  B01000000,
  B01000000,
  B01000000,
  B11100000,
  4,

  B01110000,	//2
  B10001000,
  B00001000,
  B00010000,
  B00100000,
  B01000000,
  B11111000,
  6,

  B11111000,	//3
  B00010000,
  B00100000,
  B00010000,
  B00001000,
  B10001000,
  B01110000,
  6,

  B00010000,	//4
  B00110000,
  B01010000,
  B10010000,
  B11111000,
  B00010000,
  B00010000,
  6,

  B11111000,	//5
  B10000000,
  B11110000,
  B00001000,
  B00001000,
  B10001000,
  B01110000,
  6,

  B00110000,	//6
  B01000000,
  B10000000,
  B11110000,
  B10001000,
  B10001000,
  B01110000,
  6,

  B11111000,	//7
  B10001000,
  B00001000,
  B00010000,
  B00100000,
  B00100000,
  B00100000,
  6,

  B01110000,	//8
  B10001000,
  B10001000,
  B01110000,
  B10001000,
  B10001000,
  B01110000,
  6,

  B01110000,	//9
  B10001000,
  B10001000,
  B01111000,
  B00001000,
  B00010000,
  B01100000,
  6,

  B00000000,	//:
  B11000000,
  B11000000,
  B00000000,
  B11000000,
  B11000000,
  B00000000,
  3,

  B00000000,	//;
  B11000000,
  B11000000,
  B00000000,
  B11000000,
  B01000000,
  B10000000,
  3,

  B00010000,	//<
  B00100000,
  B01000000,
  B10000000,
  B01000000,
  B00100000,
  B00010000,
  5,

  B00000000,	//=
  B00000000,
  B11111000,
  B00000000,
  B11111000,
  B00000000,
  B00000000,
  6,

  B10000000,	//>
  B01000000,
  B00100000,
  B00010000,
  B00100000,
  B01000000,
  B10000000,
  5,

  B01110000,	//?
  B10001000,
  B00001000,
  B00010000,
  B00100000,
  B00000000,
  B00100000,
  6,

  B01110000,	//@
  B10001000,
  B00001000,
  B01101000,
  B10101000,
  B10101000,
  B01110000,
  6,



  B01110000,	//A
  B10001000,
  B10001000,
  B10001000,
  B11111000,
  B10001000,
  B10001000,
  6,

  B11110000,	//B
  B10001000,
  B10001000,
  B11110000,
  B10001000,
  B10001000,
  B11110000,
  6,

  B01110000,	//C
  B10001000,
  B10000000,
  B10000000,
  B10000000,
  B10001000,
  B01110000,
  6,

  B11100000,	//D
  B10010000,
  B10001000,
  B10001000,
  B10001000,
  B10010000,
  B11100000,
  6,

  B11111000,	//E
  B10000000,
  B10000000,
  B11110000,
  B10000000,
  B10000000,
  B11111000,
  6,

  B11111000,	//F
  B10000000,
  B10000000,
  B11110000,
  B10000000,
  B10000000,
  B10000000,
  6,

  B01110000,	//G
  B10001000,
  B10000000,
  B10111000,
  B10001000,
  B10001000,
  B01111000,
  6,

  B10001000,	//H
  B10001000,
  B10001000,
  B11111000,
  B10001000,
  B10001000,
  B10001000,
  6,

  B11100000,	//I
  B01000000,
  B01000000,
  B01000000,
  B01000000,
  B01000000,
  B11100000,
  4,

  B00111000,	//J
  B00010000,
  B00010000,
  B00010000,
  B00010000,
  B10010000,
  B01100000,
  6,

  B10001000,	//K
  B10010000,
  B10100000,
  B11000000,
  B10100000,
  B10010000,
  B10001000,
  6,

  B10000000,	//L
  B10000000,
  B10000000,
  B10000000,
  B10000000,
  B10000000,
  B11111000,
  6,

  B10001000,	//M
  B11011000,
  B10101000,
  B10101000,
  B10001000,
  B10001000,
  B10001000,
  6,

  B10001000,	//N
  B10001000,
  B11001000,
  B10101000,
  B10011000,
  B10001000,
  B10001000,
  6,

  B01110000,	//O
  B10001000,
  B10001000,
  B10001000,
  B10001000,
  B10001000,
  B01110000,
  6,

  B11110000,	//P
  B10001000,
  B10001000,
  B11110000,
  B10000000,
  B10000000,
  B10000000,
  6,

  B01110000,	//Q
  B10001000,
  B10001000,
  B10001000,
  B10101000,
  B10010000,
  B01101000,
  6,

  B11110000,	//R
  B10001000,
  B10001000,
  B11110000,
  B10100000,
  B10010000,
  B10001000,
  6,

  B01111000,	//S
  B10000000,
  B10000000,
  B01110000,
  B00001000,
  B00001000,
  B11110000,
  6,

  B11111000,	//T
  B00100000,
  B00100000,
  B00100000,
  B00100000,
  B00100000,
  B00100000,
  6,

  B10001000,	//U
  B10001000,
  B10001000,
  B10001000,
  B10001000,
  B10001000,
  B01110000,
  6,

  B10001000,	//V
  B10001000,
  B10001000,
  B10001000,
  B10001000,
  B01010000,
  B00100000,
  6,

  B10001000,	//W
  B10001000,
  B10001000,
  B10101000,
  B10101000,
  B10101000,
  B01010000,
  6,

  B10001000,	//X
  B10001000,
  B01010000,
  B00100000,
  B01010000,
  B10001000,
  B10001000,
  6,

  B10001000,	//Y
  B10001000,
  B10001000,
  B01010000,
  B00100000,
  B00100000,
  B00100000,
  6,

  B11111000,	//Z
  B00001000,
  B00010000,
  B00100000,
  B01000000,
  B10000000,
  B11111000,
  6,

  B11100000,	//[
  B10000000,
  B10000000,
  B10000000,
  B10000000,
  B10000000,
  B11100000,
  4,

  B00000000,	//(Backward Slash)
  B10000000,
  B01000000,
  B00100000,
  B00010000,
  B00001000,
  B00000000,
  6,

  B11100000,	//]
  B00100000,
  B00100000,
  B00100000,
  B00100000,
  B00100000,
  B11100000,
  4,

  B00100000,	//^
  B01010000,
  B10001000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  6,

  B00000000,	//_
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B11111000,
  6,

  B10000000,	//`
  B01000000,
  B00100000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  4,

  B00000000,	//a
  B00000000,
  B01110000,
  B00001000,
  B01111000,
  B10001000,
  B01111000,
  6,

  B10000000,	//b
  B10000000,
  B10110000,
  B11001000,
  B10001000,
  B10001000,
  B11110000,
  6,

  B00000000,	//c
  B00000000,
  B01110000,
  B10001000,
  B10000000,
  B10001000,
  B01110000,
  6,

  B00001000,	//d
  B00001000,
  B01101000,
  B10011000,
  B10001000,
  B10001000,
  B01111000,
  6,

  B00000000,	//e
  B00000000,
  B01110000,
  B10001000,
  B11111000,
  B10000000,
  B01110000,
  6,

  B00110000,	//f
  B01001000,
  B01000000,
  B11100000,
  B01000000,
  B01000000,
  B01000000,
  6,

  B00000000,	//g
  B01111000,
  B10001000,
  B10001000,
  B01111000,
  B00001000,
  B01110000,
  6,

  B10000000,	//h
  B10000000,
  B10110000,
  B11001000,
  B10001000,
  B10001000,
  B10001000,
  6,

  B01000000,	//i
  B00000000,
  B11000000,
  B01000000,
  B01000000,
  B01000000,
  B11100000,
  4,

  B00010000,	//j
  B00000000,
  B00110000,
  B00010000,
  B00010000,
  B10010000,
  B01100000,
  5,

  B10000000,	//k
  B10000000,
  B10010000,
  B10100000,
  B11000000,
  B10100000,
  B10010000,
  5,

  B11000000,	//l
  B01000000,
  B01000000,
  B01000000,
  B01000000,
  B01000000,
  B11100000,
  4,

  B00000000,	//m
  B00000000,
  B11010000,
  B10101000,
  B10101000,
  B10001000,
  B10001000,
  6,

  B00000000,	//n
  B00000000,
  B10110000,
  B11001000,
  B10001000,
  B10001000,
  B10001000,
  6,

  B00000000,	//o
  B00000000,
  B01110000,
  B10001000,
  B10001000,
  B10001000,
  B01110000,
  6,

  B00000000,	//p
  B00000000,
  B11110000,
  B10001000,
  B11110000,
  B10000000,
  B10000000,
  6,

  B00000000,	//q
  B00000000,
  B01101000,
  B10011000,
  B01111000,
  B00001000,
  B00001000,
  6,

  B00000000,	//r
  B00000000,
  B10110000,
  B11001000,
  B10000000,
  B10000000,
  B10000000,
  6,

  B00000000,	//s
  B00000000,
  B01110000,
  B10000000,
  B01110000,
  B00001000,
  B11110000,
  6,

  B01000000,	//t
  B01000000,
  B11100000,
  B01000000,
  B01000000,
  B01001000,
  B00110000,
  6,

  B00000000,	//u
  B00000000,
  B10001000,
  B10001000,
  B10001000,
  B10011000,
  B01101000,
  6,

  B00000000,	//v
  B00000000,
  B10001000,
  B10001000,
  B10001000,
  B01010000,
  B00100000,
  6,

  B00000000,	//w
  B00000000,
  B10001000,
  B10101000,
  B10101000,
  B10101000,
  B01010000,
  6,

  B00000000,	//x
  B00000000,
  B10001000,
  B01010000,
  B00100000,
  B01010000,
  B10001000,
  6,

  B00000000,	//y
  B00000000,
  B10001000,
  B10001000,
  B01111000,
  B00001000,
  B01110000,
  6,

  B00000000,	//z
  B00000000,
  B11111000,
  B00010000,
  B00100000,
  B01000000,
  B11111000,
  6,

  B00100000,	//{
  B01000000,
  B01000000,
  B10000000,
  B01000000,
  B01000000,
  B00100000,
  4,

  B10000000,	//|
  B10000000,
  B10000000,
  B10000000,
  B10000000,
  B10000000,
  B10000000,
  2,

  B10000000,	//}
  B01000000,
  B01000000,
  B00100000,
  B01000000,
  B01000000,
  B10000000,
  4,



  B00000000,	//~
  B00000000,
  B00000000,
  B01101000,
  B10010000,
  B00000000,
  B00000000,
  6,

  B01100000,	// (Char 0x7F)
  B10010000,
  B10010000,
  B01100000,
  B00000000,
  B00000000,
  B00000000,
  5
};

//const unsigned char scrollText[] PROGMEM ={
//    "  THE QUICK BROWN FOX JUMPED OVER THE LAZY DOG 1234567890 the quick brown fox jumped over the lazy dog   \0"};





// * GLOBAL VARIABLES
unsigned long bufferLong [14] = {0};  //Buffer for scrolling text


void setup() {
/*
  //CAR
  //initialize the digital pin as an output.

  pinMode (lSensor, INPUT);
  pinMode (rSensor, INPUT);

  pinMode (lMotNeg, OUTPUT);
  pinMode (lMotPos, OUTPUT);
  pinMode (lMotPWM, OUTPUT);
  pinMode (rMotNeg, OUTPUT);
  pinMode (rMotPos, OUTPUT);
  pinMode (lMotPWM, OUTPUT);

  //Reconfig timer 1 to proper frequency for PWM to work properly
  byte mode;
  mode = 0x05;
  TCCR1B = TCCR1B & 0b11111000 | mode;

*/

  //LED
  for (int x = 0; x < numDevices; x++) {
    lc.shutdown(x, false);      //The MAX72XX is in power-saving mode on startup
    lc.setIntensity(x, 8);      // Set the brightness to default value
    lc.clearDisplay(x);         // and clear the display
  }
}

void loop() {
/*
    //check IR sensors
  int lSensStat = digitalRead (lSensor);  //0 = signal detected (white surface): 1 = black line
  int rSensStat = digitalRead (rSensor);  //0 = signal detected (white surface): 1 = black line

  if (!lSensStat && !rSensStat) {
    goForward();
  };       //both sensors detected white floor. go straight
  if (!lSensStat && rSensStat) {
    turnRight();
  };    //detected black floor on right (off), so turn right
  if (lSensStat && !rSensStat) {
    turnLeft();
  };  //detected black floor on left (off), so turn left
  if (lSensStat && rSensStat) {
    stop();
*/
  scrollMessage(scrollText);
  delay(3000);
  
  //};    //detected black floor on both sensor, so stop!

}

/*
void goForward ()
{
  //set left motor control parameter
  digitalWrite (lMotNeg, LOW);
  digitalWrite (lMotPos, HIGH);
  analogWrite(lMotPWM, lMotSpeed);

  //set right motor contro parameters
  digitalWrite (rMotNeg, LOW);
  digitalWrite (rMotPos, HIGH);
  analogWrite(rMotPWM, rMotSpeed);
}

void turnLeft ()
{
  digitalWrite (lMotNeg, HIGH);  //left motor reverse
  digitalWrite (lMotPos, LOW);
  analogWrite(lMotPWM, lMotSpeed);

  digitalWrite (rMotNeg, LOW);    //right motor forward
  digitalWrite (rMotPos, HIGH);
  analogWrite(rMotPWM, rMotSpeed);
}

void turnRight ()
{
  digitalWrite (lMotNeg, LOW);    //left motor forward
  digitalWrite (lMotPos, HIGH);
  analogWrite(lMotPWM, lMotSpeed);

  digitalWrite (rMotNeg, HIGH);    //right motor reverse
  digitalWrite (rMotPos, LOW);
  analogWrite(rMotPWM, rMotSpeed);
}

void goBackward ()
{
  //set left motor control parameter
  digitalWrite (lMotNeg, HIGH);
  digitalWrite (lMotPos, LOW);
  analogWrite(lMotPWM, lMotSpeed);

  //set right motor contro parameters
  digitalWrite (rMotNeg, HIGH);
  digitalWrite (rMotPos, LOW);
  analogWrite(rMotPWM, rMotSpeed);
}

void stop() {
  digitalWrite (lMotNeg, LOW);
  digitalWrite (lMotPos, LOW);
  digitalWrite (rMotNeg, LOW);
  digitalWrite (rMotPos, LOW);
}

*/

void scrollFont() {
  for (int counter = 0x20; counter < 0x80; counter++) {
    loadBufferLong(counter);
    delay(500);
  }
}

// Scroll Message
void scrollMessage(const unsigned char * messageString) {
  int counter = 0;
  int myChar = 0;
  do {
    // read back a char
    myChar =  pgm_read_byte_near(messageString + counter);
    if (myChar != 0) {
      loadBufferLong(myChar);
    }
    counter++;
  }
  while (myChar != 0);
}
// Load character into scroll buffer
void loadBufferLong(int ascii) {
  if (ascii >= 0x20 && ascii <= 0x7f) {
    for (int a = 0; a < 7; a++) {               // Loop 7 times for a 5x7 font
      unsigned long c = pgm_read_byte_near(font5x7 + ((ascii - 0x20) * 8) + a);     // Index into character table to get row data
      unsigned long x = bufferLong [a * 2];   // Load current scroll buffer
      x = x | c;                              // OR the new character onto end of current
      bufferLong [a * 2] = x;                 // Store in buffer
    }
    byte count = pgm_read_byte_near(font5x7 + ((ascii - 0x20) * 8) + 7);    // Index into character table for kerning data
    for (byte x = 0; x < count; x++) {
      rotateBufferLong();
      printBufferLong();
      delay(scrollDelay);
    }
  }
}
// Rotate the buffer
void rotateBufferLong() {
  for (int a = 0; a < 7; a++) {               // Loop 7 times for a 5x7 font
    unsigned long x = bufferLong [a * 2];   // Get low buffer entry
    byte b = bitRead(x, 31);                // Copy high order bit that gets lost in rotation
    x = x << 1;                             // Rotate left one bit
    bufferLong [a * 2] = x;                 // Store new low buffer
    x = bufferLong [a * 2 + 1];             // Get high buffer entry
    x = x << 1;                             // Rotate left one bit
    bitWrite(x, 0, b);                      // Store saved bit
    bufferLong [a * 2 + 1] = x;             // Store new high buffer
  }
}
// Display Buffer on LED matrix
void printBufferLong() {
  for (int a = 0; a < 7; a++) {             // Loop 7 times for a 5x7 font
    unsigned long x = bufferLong [a * 2 + 1]; // Get high buffer entry
    byte y = x;                             // Mask off first character
    lc.setRow(3, a, y);                     // Send row to relevent MAX7219 chip
    x = bufferLong [a * 2];                 // Get low buffer entry
    y = (x >> 24);                          // Mask off second character
    lc.setRow(2, a, y);                     // Send row to relevent MAX7219 chip
    y = (x >> 16);                          // Mask off third character
    lc.setRow(1, a, y);                     // Send row to relevent MAX7219 chip
    y = (x >> 8);                           // Mask off forth character
    lc.setRow(0, a, y);                     // Send row to relevent MAX7219 chip
  }
}



