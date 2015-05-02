// AHR: AIR HOCKEY ROBOT PROJECT
// Author: Jose Julio (@jjdrones)
// Hardware: Arduino MEGA + Ramps 1.4
// Date: 18/11/2013
// Last updated: 24/03/2014
// Version: 1.05
// Project page :
//    Spanish: http://cienciaycacharreo.blogspot.com.es/2014/02/nuevo-proyecto-air-hockey-robot-3d.html
//    English: http://cienciaycacharreo.blogspot.com.es/2014/02/new-project-air-hockey-robot-3d-printer.html
// GIT repository: https://github.com/JJulio/AHRobot
// License: Open Software GPL License

// ROBOT and USER configuration parameters
#include "Configuration.h"
#include "Definitions.h"   // Variable definitions

#include <TimerOne.h>

void setup()
{
  // STEPPER MOTOR PINS (SAME AS RAMPS 1.4)
  // X MOTOR
  //     X-STEP: A0 //   (PF0)
  //     X-DIR:  A1 //   (PF1)
  //     X-ENABLE: 2 //D38 (PD7)
  // Y MOTOR (Y-LEFT)
  //     Y-STEP: 6 //A6    (PF6)
  //     Y-DIR:  7 //A7    (PF7)
  //     Y-ENABLE: A2 // (PF2)
  // Z MOTOR (Y-RIGHT)
  //     Z-STEP: 3 //D46   (PL3)
  //     Z-DIR:  4 //D48   (PL1)
  //     Z-ENABLE: 5 //A8  (PK0)

  // STEPPER PINS
  // X_AXIS
  pinMode( X_ENABLE_PIN, OUTPUT);  // ENABLE MOTOR
  pinMode( X_STEP_PIN, OUTPUT);  // STEP MOTOR
  pinMode( X_DIR_PIN, OUTPUT);  // DIR MOTOR
  // Y_AXIS (Y-LEFT)
  pinMode( Y_ENABLE_PIN, OUTPUT);  // ENABLE MOTOR
  pinMode( Y_STEP_PIN, OUTPUT);  // STEP MOTOR
  pinMode( Y_DIR_PIN, OUTPUT);  // DIR MOTOR
  // Z_AXIS (Y-RIGHT)
  pinMode( Z_ENABLE_PIN, OUTPUT);  // ENABLE MOTOR
  pinMode( Z_STEP_PIN, OUTPUT);  // STEP MOTOR
  pinMode( Z_DIR_PIN, OUTPUT);  // DIR MOTOR

  pinMode( LED_PIN, OUTPUT);  // DEBUG PIN FOR OSCILLOSCOPE TIME MEASURES

  //pinMode(19,INPUT);  // RX1 Serial Port 1
  //pinMode(18,OUTPUT); // TX1

  //FANS and LEDS
  pinMode( HEATER_0_PIN, OUTPUT);  // 10
  pinMode( HEATER_1_PIN, OUTPUT); // 8
  pinMode( FAN_PIN, OUTPUT); // 9

  // Disable Motors
  digitalWrite( X_ENABLE_PIN, HIGH);
  digitalWrite( Y_ENABLE_PIN, HIGH);
  digitalWrite( Z_ENABLE_PIN, HIGH);

  Serial.begin(115200);
  Serial.println("AHR Robot version 1.05");
  delay(500);
  Serial.println("Initializing robot...");
  delay(2000);

  cameraProcessInit();  // Camera variable initializations

  // Robot positions initialization
  defense_position = ROBOT_DEFENSE_POSITION;   // Robot y axis defense position
  attack_position = ROBOT_DEFENSE_ATTACK_POSITION;   // Robot y axis position for defense+attack
/*
  //LED blink
  for (uint8_t k=0;k<4;k++)
  {
    digitalWrite(13,HIGH);
    delay(300);
    digitalWrite(13,LOW);
    delay(300);
  }

  // We use TIMER 1 for stepper motor X AXIS and Timer 3 for Y AXIS
  // STEPPER MOTORS INITIALIZATION
  // TIMER1 CTC MODE
  TCCR1B &= ~(1<<WGM13);
  TCCR1B |=  (1<<WGM12);
  TCCR1A &= ~(1<<WGM11);
  TCCR1A &= ~(1<<WGM10);

  // output mode = 00 (disconnected)
  TCCR1A &= ~(3<<COM1A0);
  TCCR1A &= ~(3<<COM1B0);

  // Set the timer pre-scaler
  // Generally we use a divider of 8, resulting in a 2MHz timer on 16MHz CPU
  TCCR1B = (TCCR1B & ~(0x07<<CS10)) | (2<<CS10);

  OCR1A = ZERO_SPEED;   // Motor stopped */
  dir_x = 0;
  TCNT1 = 0;
/*
  // We use TIMER 3 for stepper motor Y AXIS
  // STEPPER MOTORS INITIALIZATION
  // TIMER3 CTC MODE
  TCCR3B &= ~(1<<WGM13);
  TCCR3B |=  (1<<WGM12);
  TCCR3A &= ~(1<<WGM11);
  TCCR3A &= ~(1<<WGM10);

  // output mode = 00 (disconnected)
  TCCR3A &= ~(3<<COM1A0);
  TCCR3A &= ~(3<<COM1B0);

  // Set the timer pre-scaler
  // Generally we use a divider of 8, resulting in a 2MHz timer on 16MHz CPU
  TCCR3B = (TCCR3B & ~(0x07<<CS10)) | (2<<CS10);

  OCR3A = ZERO_SPEED;   // Motor stopped  */
  dir_y = 0;
  TCNT3 = 0;

  //Initializing init position
  position_x = ROBOT_INITIAL_POSITION_X*X_AXIS_STEPS_PER_UNIT;
  position_y = ROBOT_INITIAL_POSITION_Y*Y_AXIS_STEPS_PER_UNIT;

  delay(4000);

  Serial.println("Initializing Stepper motors...");
  delay(1000);
//  TIMSK1 |= (1<<OCIE1A);  // Enable Timer1 interrupt
//  TIMSK3 |= (1<<OCIE1A);  // Enable Timer1 interrupt

  // Enable steppers
  digitalWrite( X_ENABLE_PIN, LOW);   // X-axis
  digitalWrite( Y_ENABLE_PIN, LOW);   // Y-axis left
  digitalWrite( Z_ENABLE_PIN, LOW);   // Z-axis (Y-axis right)

  // Output parameters
  Serial.print("Max_acceleration_x: ");
  Serial.println(max_acceleration_x);
  Serial.print("Max_acceleration_y: ");
  Serial.println(max_acceleration_y);
  Serial.print("Max speed X: ");
  Serial.println(MAX_SPEED_X);
  Serial.print("Max speed Y: ");
  Serial.println(MAX_SPEED_Y);
  Serial.println("Moving to initial position...");
  Serial.println("Ready!!");
  delay(1000);

  // Initializing Robot command variables
  com_pos_x = ROBOT_CENTER_X;
  com_pos_y = defense_position;
  com_speed_x = MAX_SPEED_X;
  com_speed_y = MAX_SPEED_Y;

  setSpeedS(com_speed_x,com_speed_y);
  setPosition(com_pos_x,com_pos_y);

  timer_old = micros();
  timer_packet_old = timer_old;
  micros_old = timer_old;


  // Enable Air Hockey FANS
  //digitalWrite(9,HIGH);
  //digitalWrite(10,HIGH);
  //analogWrite(9,FAN1_SPEED);
  //analogWrite(10,FAN2_SPEED);

  // enable timer interrupt simulate TCNT1 TCNT3
  Timer1.initialize(1); // set a timer of length 1 microseconds (or 1MHz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt( timerIsr ); // attach the service routine here
}

// Main loop
void loop()
{
  int dt;
  uint8_t logOutput=0;

  debug_counter++;
  timer_value = micros();
  if ((timer_value-timer_old)>=1000)  // 1Khz loop
  {
    //dt = (timer_value-timer_old)/1000;
    timer_old = timer_value;

    loop_counter++;

    packetRead();
    if (newPacket)
    {
      //dt = (timer_value-timer_packet_old)/1000.0;
      dt = 16;  //60 Hz = 16.66ms
      timer_packet_old = timer_value;
      newPacket=0;
      logOutput = 1;
      //Serial.print("P");

      // Puck detection and trayectory prediction
      cameraProcess(puckPixX,puckPixY,dt);

      // Robot position detection for missing steps detection in steppers.
      robotDetection(robotPixX,robotPixY);
      missingStepsDetection();

      // Robot strategy based on puck prediction
      newDataStrategy();


    }  // End packet received



    // Strategy : Robot behaviour
    robotStrategy();

    // Console output
    if (logOutput)
      {
      logOutput = 0;
      Serial.print(robot_status);
      Serial.print(" ");
      Serial.print(com_pos_x);
      Serial.print(" ");
      Serial.print(com_pos_y);
      Serial.print(" ");
      Serial.println(speed_y);
      }
    // DEBUG : PUCK POSITION
    if (loop_counter == 4000)
      {
        Serial.print("PUCK POSITION: ");
        Serial.print(puckCoordX);
        Serial.print(",");
        Serial.println(puckCoordY);
      }

    // DEBUG: We inform of the position error of the robot as seen in the camera (util for calibrations)
    if (loop_counter == 4002)
      {
        Serial.print("ROBOT ERROR: ");
        Serial.print(robotMissingStepsErrorX);
        Serial.print(",");
        Serial.println(robotMissingStepsErrorY);
      }


    positionControl();
  } // 1Khz loop
}

// motor pin pull high
// WRD:
// 1 = PORTF
// 2 = PORTL
void SET(int portWRD, int portNUM){
    if( portWRD==1 && portNUM==0 ){ digitalWrite( X_STEP_PIN, HIGH); }
    if( portWRD==1 && portNUM==6 ){ digitalWrite( Y_STEP_PIN, HIGH); }
    if( portWRD==2 && portNUM==3 ){ digitalWrite( Z_STEP_PIN, HIGH); }
    if( portWRD==1 && portNUM==1 ){ digitalWrite( X_DIR_PIN, HIGH); }
    if( portWRD==1 && portNUM==7 ){ digitalWrite( Y_DIR_PIN, HIGH); }
    if( portWRD==2 && portNUM==1 ){ digitalWrite( Z_DIR_PIN, HIGH); }
}

// motor pin pull low
void CLR(int portWRD, int portNUM){
    if( portWRD==1 && portNUM==0 ){ digitalWrite( X_STEP_PIN, LOW); }
    if( portWRD==1 && portNUM==6 ){ digitalWrite( Y_STEP_PIN, LOW); }
    if( portWRD==2 && portNUM==3 ){ digitalWrite( Z_STEP_PIN, LOW); }
    if( portWRD==1 && portNUM==1 ){ digitalWrite( X_DIR_PIN, LOW); }
    if( portWRD==1 && portNUM==7 ){ digitalWrite( Y_DIR_PIN, LOW); }
    if( portWRD==2 && portNUM==1 ){ digitalWrite( Z_DIR_PIN, LOW); }
}

//update timer by interrupt
void timerIsr(){
    TCNT1 = TCNT1+1;
    TCNT3 = TCNT3+1;
    if (dir_x==0){
        // dir off, don't move
    }
    // 1KHz X stepper
    else if(TCNT1%2000<1000){
        SET(PORTF,0);
    }
    else{
        CLR(PORTF,0);
        TCNT1 = 0;
    }
    // 1KHz Y stepper
    if (dir_y==0){
        // dir off, don't move
    }
    // 1KHz Y stepper
    else if(TCNT3%2000<1000){
        SET(PORTF,6);
        SET(PORTL,3);
    }
    else{
        CLR(PORTF,6);
        CLR(PORTL,3);
        TCNT3 = 0;
    }

}
