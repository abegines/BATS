
#define PIN_X_STEP 2    // Define el Pin de STEP para Motor de eje X
#define PIN_X_DIRE 5    // Define el Pin de DIR  para Motor de eje X
#define PIN_X_ENAB 8    // Define el Pin de ENABLE  para Motor de eje X
#define X_STEPS_MAX_VALUE 48000

#define PIN_Y_STEP 3
#define PIN_Y_DIRE 6
#define PIN_Y_ENAB 8
#define X_STEPS_MAX_VALUE 48000

#define PIN_Z_STEP 4
#define PIN_Z_DIRE 7
#define PIN_Z_ENAB 8

void oneStep(int pin, int usdelay)
{
   digitalWrite(pin, HIGH);
   delayMicroseconds(usdelay);
   digitalWrite(pin, LOW);
   delayMicroseconds(usdelay);
}

void setup() {
   // Marcar los pines como salida
   delay(1000);
   
   pinMode(PIN_X_DIRE, OUTPUT);   
   pinMode(PIN_Y_DIRE, OUTPUT);
   pinMode(PIN_X_STEP, OUTPUT);   
   pinMode(PIN_Y_STEP, OUTPUT);
   pinMode(PIN_X_ENAB, OUTPUT);   
   pinMode(PIN_Y_ENAB, OUTPUT);
   
   digitalWrite(PIN_X_ENAB, LOW); 
   digitalWrite(PIN_Y_ENAB, LOW);
}




void loop() {

  const long steps = X_STEPS_MAX_VALUE / 4;
  
  int stepDelay;
  
   digitalWrite(PIN_X_DIRE, HIGH);
   digitalWrite(PIN_Y_DIRE, HIGH);
   stepDelay = 300;
   for (long x = 0; x < steps * 1; x++) {
      oneStep(PIN_X_STEP, stepDelay);
      oneStep(PIN_Y_STEP, stepDelay);
   }
   
   delay(1000);
   
   digitalWrite(PIN_X_DIRE, LOW);
   digitalWrite(PIN_Y_DIRE, LOW);
   stepDelay = 100;
   for (long x = 0; x < steps * 1; x++) {
      oneStep(PIN_X_STEP, stepDelay);
      oneStep(PIN_Y_STEP, stepDelay);
   }
   delay(1000);

}
