#define X_STEP_PIN         A0
#define X_DIR_PIN          A1
#define X_ENABLE_PIN       2

#define Y_STEP_PIN         7
#define Y_DIR_PIN          6
#define Y_ENABLE_PIN       A2

#define Z_STEP_PIN         3
#define Z_DIR_PIN          4
#define Z_ENABLE_PIN       5

#define LED_PIN            13

#define FAN_PIN            9

#define HEATER_0_PIN       10
#define HEATER_1_PIN       8

#include <TimerOne.h>

void setup() {
  pinMode(FAN_PIN , OUTPUT);
  pinMode(HEATER_0_PIN , OUTPUT);
  pinMode(HEATER_1_PIN , OUTPUT);
  pinMode(LED_PIN  , OUTPUT);
  
  pinMode(X_STEP_PIN  , OUTPUT);
  pinMode(X_DIR_PIN    , OUTPUT);
  pinMode(X_ENABLE_PIN    , OUTPUT);
  
  pinMode(Y_STEP_PIN  , OUTPUT);
  pinMode(Y_DIR_PIN    , OUTPUT);
  pinMode(Y_ENABLE_PIN    , OUTPUT);
  
  pinMode(Z_STEP_PIN  , OUTPUT);
  pinMode(Z_DIR_PIN    , OUTPUT);
  pinMode(Z_ENABLE_PIN    , OUTPUT);
  
  digitalWrite(X_ENABLE_PIN    , LOW);
  digitalWrite(Y_ENABLE_PIN    , LOW);
  digitalWrite(Z_ENABLE_PIN    , LOW);
  
  digitalWrite(X_DIR_PIN    , HIGH);
  digitalWrite(Y_DIR_PIN    , HIGH);
  digitalWrite(Z_DIR_PIN    , HIGH);
  
  digitalWrite(X_STEP_PIN    , LOW);
  digitalWrite(Y_STEP_PIN    , LOW);
  digitalWrite(Z_STEP_PIN    , LOW);

  Timer1.initialize(1000); // set a timer of length 1000 microseconds (or 0.001 sec - or 1KHz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt( timerIsr ); // attach the service routine here
}





void loop () {
  
  if (millis() %1000 <500) 
    digitalWrite(LED_PIN, HIGH);
  else
   digitalWrite(LED_PIN, LOW);
  
  if (millis() %1000 <300) {
    digitalWrite(HEATER_0_PIN, HIGH);
    digitalWrite(HEATER_1_PIN, LOW);
    digitalWrite(FAN_PIN, LOW);
  } else if (millis() %1000 <600) {
    digitalWrite(HEATER_0_PIN, LOW);
    digitalWrite(HEATER_1_PIN, HIGH);
    digitalWrite(FAN_PIN, LOW);
  } else  {
    digitalWrite(HEATER_0_PIN, LOW);
    digitalWrite(HEATER_1_PIN, LOW);
    digitalWrite(FAN_PIN, HIGH);
  }
  
  
  if (millis() %10000 <5000) {
    digitalWrite(X_DIR_PIN    , HIGH);
    digitalWrite(Y_DIR_PIN    , HIGH);
    digitalWrite(Z_DIR_PIN    , HIGH);
  }
  else {
    digitalWrite(X_DIR_PIN    , LOW);
    digitalWrite(Y_DIR_PIN    , LOW);
    digitalWrite(Z_DIR_PIN    , LOW);
  }
  
  /*
  digitalWrite(X_STEP_PIN    , HIGH);
  digitalWrite(Y_STEP_PIN    , HIGH);
  digitalWrite(Z_STEP_PIN    , HIGH);
  delay(1); 
  digitalWrite(X_STEP_PIN    , LOW);
  digitalWrite(Y_STEP_PIN    , LOW);
  digitalWrite(Z_STEP_PIN    , LOW);
  // 1Khz loop
  if( millis() %2 <1 ){
    // even number 1/1000 sec: pull pull step 
    digitalWrite(X_STEP_PIN    , HIGH);
    digitalWrite(Y_STEP_PIN    , HIGH);
    digitalWrite(Z_STEP_PIN    , HIGH);
  }else{
    // odd number 1/1000 sec: step pull LOW 
    digitalWrite(X_STEP_PIN    , LOW);
    digitalWrite(Y_STEP_PIN    , LOW);
    digitalWrite(Z_STEP_PIN    , LOW);
  }
  */
}

void timerIsr()
{
    // Toggle LED & stepper test
    // digitalWrite( 13, digitalRead( 13 ) ^ 1 );
    digitalWrite( X_STEP_PIN, digitalRead(X_STEP_PIN) ^ 1 );
    digitalWrite( Y_STEP_PIN, digitalRead(Y_STEP_PIN) ^ 1 );
    digitalWrite( Z_STEP_PIN, digitalRead(Z_STEP_PIN) ^ 1 );
}

