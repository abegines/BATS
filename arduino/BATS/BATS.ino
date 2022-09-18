#include "Wire.h" 
#include "WiiChuck.h" 
#include "utils.h"



/* ******************************************************************************
 *  PARAMETERS AND CONFIGURATION                                                *
 ****************************************************************************** */
/*long MCM_from_1_to_22[] = 
  { 0,1, 2, 6, 12, 60, 60, 420, 840, 2520, 2520, 27720, 27720, 360360, 360360, 360360, 
    720720, 12252240, 12252240, 232792560, 232792560, 232792560, 232792560}; */

// ====================== JOYSTICK CONFIGURATION, CALIBRATION AND SCALE
#define JOYVALUE_X_LEFT -85    // CALIBRATION VALUES (left -)
#define JOYVALUE_X_CENTER 11   // CALIBRATION VALUES 
#define JOYVALUE_X_RIGHT 114   // CALIBRATION VALUES (right +)

#define JOYVALUE_Y_UP 95       // CALIBRATION VALUES (up +)
#define JOYVALUE_Y_CENTER -3   // CALIBRATION VALUES
#define JOYVALUE_Y_DOWN -102   // CALIBRATION VALUES (down -)

#define JOYSCALE_X 1000        // SCALED VALUE FOR -LEFT, +RIGHT FROM CENTER
#define JOYDEADZONE_X 50       // SCALED VALUE FOR DEAD ZONE
#define JOYSCALE_Y 1000        // SCALED VALUE FOR -DOWN, +UP FROM CENTER
#define JOYDEADZONE_Y 50       // SCALED VALUE FOR DEAD ZONE

#define FREQ_READ_CHUCK 20

// =================================================== MOTORS CONFIGURATION
#define PIN_X_STEP 2    
#define PIN_X_DIRE 5    
#define PIN_X_ENAB 8    
#define X_STEPS_FACTOR 1

#define PIN_Y_STEP 3
#define PIN_Y_DIRE 6
#define PIN_Y_ENAB 8
#define Y_STEPS_FACTOR 1

#define PIN_Z_STEP 4
#define PIN_Z_DIRE 7
#define PIN_Z_ENAB 8
#define Z_STEPS_FACTOR 1

#define usSTEPDELAY 20
#define usLOOPDELAY 200

#define MINFREQ_X 128
#define MAXFREQ_X 2
#define MINFREQ_Y 128
#define MAXFREQ_Y 2

// ================================================= JOYSTICK FUNCTIONS
WiiChuck chuck = WiiChuck(); 
int joyXPos, joyYPos;
int scaledjoyXPos, scaledjoyYPos;
bool joyCBut, joyZBut;

void readJoyPos()
{
  chuck.update();
  joyXPos = chuck.readJoyX();
  joyYPos = chuck.readJoyY();
  joyZBut = chuck.buttonZ;
  joyCBut = chuck.buttonC;
  // Converts read from chuck to scaled position with dead zone
  if (joyXPos<JOYVALUE_X_CENTER) { // go left (-)
    scaledjoyXPos = map( joyXPos, JOYVALUE_X_LEFT, JOYVALUE_X_CENTER, -JOYSCALE_X, 0);
    if (scaledjoyXPos < -JOYSCALE_X) scaledjoyXPos = -JOYSCALE_X;
    if (scaledjoyXPos > -JOYDEADZONE_X) scaledjoyXPos = 0;
  } else {                        // go right (+)
    scaledjoyXPos = map( joyXPos, JOYVALUE_X_CENTER,JOYVALUE_X_RIGHT, 0, JOYSCALE_X);
    if (scaledjoyXPos > JOYSCALE_X) scaledjoyXPos = JOYSCALE_X;
    if (scaledjoyXPos < JOYDEADZONE_X) scaledjoyXPos = 0;
  }
  if (joyYPos<JOYVALUE_Y_CENTER) { // go down (-)
    scaledjoyYPos = map( joyYPos, JOYVALUE_Y_DOWN, JOYVALUE_Y_CENTER, -JOYSCALE_Y, 0);
    if (scaledjoyYPos < -JOYSCALE_Y) scaledjoyYPos = -JOYSCALE_Y;
    if (scaledjoyYPos > -JOYDEADZONE_Y) scaledjoyYPos = 0;
  } else {                        // go up (+)
    scaledjoyYPos = map( joyYPos, JOYVALUE_Y_CENTER,JOYVALUE_Y_UP, 0, JOYSCALE_Y);
    if (scaledjoyYPos > JOYSCALE_Y) scaledjoyYPos = JOYSCALE_Y;
    if (scaledjoyYPos < JOYDEADZONE_Y) scaledjoyYPos = 0;
  }

}


// =================================================== MOTORS FUNCTIONS
void oneStep(int steppin)
{
   digitalWrite(steppin, HIGH);
   delayMicroseconds(usSTEPDELAY);
   digitalWrite(steppin, LOW);
}

void setDirection(int dirpin, int dir)
{
   Serial.print("dir=");
   Serial.println(dir);
   digitalWrite(dirpin, dir<0?HIGH:LOW);
}

long freqX =0, freqY =0;
long lastFreqX =0, lastFreqY =0;
int  dirX, dirY;
void determineFreqsAndDirection()
{
  lastFreqX = freqX;
  // freqX
  if (scaledjoyXPos<0) { // go left
    freqX = map (scaledjoyXPos, -JOYSCALE_X, -JOYDEADZONE_X, -MAXFREQ_X, -MINFREQ_X);
  }
  else if (scaledjoyXPos>0) { // go right
    freqX = map (scaledjoyXPos, JOYSCALE_X, JOYDEADZONE_X, MAXFREQ_X, MINFREQ_X);
  }
  else freqX=0;
  // freqY
  lastFreqY = freqY;
  if (scaledjoyYPos<0) { // go down
    freqY = map (scaledjoyYPos, -JOYSCALE_Y, -JOYDEADZONE_Y, -MAXFREQ_Y, -MINFREQ_Y);
  }
  else if (scaledjoyYPos>0) { // go up
    freqY = map (scaledjoyYPos, JOYSCALE_Y, JOYDEADZONE_Y, MAXFREQ_Y, MINFREQ_Y);
  }
  else freqY=0;

  dirX = sgn(freqX);
  if ((dirX!=sgn(lastFreqX)) && (dirX!=0)) {
    setDirection(PIN_X_DIRE, sgn(freqX));
  }
  dirY = sgn(freqY);
  if ((dirY!=sgn(lastFreqY)) && (dirY!=0)) {
    setDirection(PIN_Y_DIRE, sgn(freqY));
  }

  
  
}




// ====================================== SETUP & LOOP
void setup() {
  Serial.begin(115200); 
  Serial.println("BATS - Starting...");
  chuck.begin(); 
  pinMode(PIN_X_DIRE, OUTPUT);   
  pinMode(PIN_Y_DIRE, OUTPUT);
  pinMode(PIN_X_STEP, OUTPUT);   
  pinMode(PIN_Y_STEP, OUTPUT);
  pinMode(PIN_X_ENAB, OUTPUT);   
  pinMode(PIN_Y_ENAB, OUTPUT);

  Serial.println("Enabling motors...");
  digitalWrite(PIN_X_ENAB, LOW); 
  digitalWrite(PIN_Y_ENAB, LOW);

}

long loopsSinceLastReadChuck = 0;
long loopsSinceLastMoveX = 0;
long loopsSinceLastMoveY = 0;

void loop() {
  // read joyStick? =================================
  loopsSinceLastReadChuck++;
  if (loopsSinceLastReadChuck>=FREQ_READ_CHUCK) {
    readJoyPos();
    determineFreqsAndDirection();
    loopsSinceLastReadChuck=0;
  }
  
  // move motors? ===================================
  if (freqX!=0) {
    loopsSinceLastMoveX++;  
    //Serial.print(freqX);
    //Serial.print(" ");
    //Serial.println(loopsSinceLastMoveX);
    if (loopsSinceLastMoveX>=abs(freqX)) {
      //Serial.println("step");
      oneStep(PIN_X_STEP);
      loopsSinceLastMoveX=0;
    }
  } else loopsSinceLastMoveX =0; 

  if (freqY!=0) {
    loopsSinceLastMoveY++;  
    if (loopsSinceLastMoveY>=abs(freqY)) {
      //Serial.println("stepY");
      oneStep(PIN_Y_STEP);
      loopsSinceLastMoveY=0;
    }
  } else loopsSinceLastMoveY =0; 

  
  delayMicroseconds(usLOOPDELAY);
}
