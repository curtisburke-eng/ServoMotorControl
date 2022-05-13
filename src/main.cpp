/*    File Information
* main.cpp - main program file to control stepper motor lead-screw
*       combo for the prototype Chemical Pump Solenoid stack
*
* Project: Chemical Pump Soleniod Lead Screw Prototype
* Company: IES Combustors
* Author: Curtis Burke
* Date Created: 12-08-2021
* Date Modified: 12-08-2021
*
*/

/*    Mechanical Design
*
*   [-----]    [______________________________________________________________]  [------]
*   | MOT |-------------------------------------------------------------|    [0]-[ HOME ]
*   [-----]    [__________ 4 __________ 3 __________ 2 __________ 1 __________]  [------]
*
*                     <--------- BACKWARD            FORWARD --------->
*
*/
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Include Libraries
#include <Arduino.h>
#include <AFMotor.h>
#include <stdlib.h>                                                   // For abs function

// Include Function Files
#include "driveMotor.h"

// Declare & Define Variables
int target;                                                           // The port number for the desired position
int current;                                                          // The port number for the current position

int magnitude;                                                        // The number of steps to move to reach target
int direction;                                                        // The direction to move to reach target

bool homed = 0;                                                       // Homed status (initialized to false)

int steps_per_rev = 200;                                              // Motor parameter (1.8 deg per step)

/* steps_between_ports Calculation:
*   
*   Lead Screw Treads: TR8x8
*   Lead (advanment of nut per one revolution of screw): 8mm/rev (0.315"/rev)
*   
*   Spacing between ports: 1.375"
*   1.375/0.315 = 4.365 revs between ports
* 
*   200 steps/rev * 4.365 revs = 873 steps between ports 
* 
*/
int steps_between_ports = 873;                                        // Calculated value based on # of steps/rev & lead screw Lead

/* steps_0_to_1 Calculation:
*   
*   Lead Screw Treads: TR8x8
*   Lead (advanment of nut per one revolution of screw): 8mm/rev (0.315"/rev)
*   
*   Spacing between ports: 0.3215"
*   0.3125/0.315 = 0.99 revs between ports
* 
*   200 steps/rev * 0.99 revs = 198 steps between ports 
* 
*/
int steps_0_to_1 = 198;

#define FORWARD 1
#define BACKWARD 2

AF_Stepper motor(steps_per_rev, 2);                                   // Connect motor to position #2 (M3 and M4) [OSEPP Motor Driver]


int loop_delay = 5000;                                                // Amount of runtime for each loop


#define DEBUG                                                         // Debug flag - if defined Serial is acive


// --------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup() {
  // ------ Initialize ------
  #ifdef DEBUG
    Serial.begin(9600);                                                 // set up Serial connection at 9600 bps
    
    Serial.println("Initializing...");
  #endif

  motor.setSpeed(100);                                                // Set speed in rpm
  delay(5000);                                                        // delay for stability

  // ------ Initial Home ------
  #ifdef DEBUG
    Serial.println("Homing...");
  #endif
  
  // TODO: move motor forward until home limit switch is tripped. 

  delay(2000);                                                        // delay for stability
  
  // Update state 
  homed = 1;                                                          // Motor has been homed 
  current = 0;                                                        // Set current position to "home"

}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() {

  // ------ Find Target ------
    #ifdef DEBUG
      Serial.println("Generating Random target...");
    #endif

    target = rand() % 4 + 1;                                          // random target between 1 and 4

    delay(1000);

    #ifdef DEBUG
      Serial.print("Target received: ");
      Serial.println(target);
    #endif
    //target = 1;
    

  // ----- Choose Direction ------
  if (target > current)
  { // BACKWARD
    direction = BACKWARD;
  }
  else if (target < current)
  { // FORWARD
    direction = FORWARD;
  }
  else
  { // DONT MOVE
    direction = 0;
  }

  #ifdef DEBUG
    Serial.print("Direction:");
    Serial.println(direction);
  #endif

  // ----- Find Magnitude ------
  if(current == 0)
  {
    current = 1;
    magnitude = (abs(target - current) * steps_between_ports) + steps_0_to_1;
  }
  else
    magnitude = abs(target - current) * steps_between_ports;

  #ifdef DEBUG
    Serial.print("Magnitude (steps):");
    Serial.println(magnitude);
  #endif

  // ----- Move Motor ------
  if (direction != 0)
    #ifdef DEBUG
      Serial.println("Moving to target.");
    #endif
    motor.step(magnitude, direction, DOUBLE); 

  // ----- Update current ------
  current = target;

  #ifdef DEBUG
    Serial.print("Current Position:");
    Serial.println(current);    
    Serial.println();
    Serial.println("---------------------------");
  #endif

  delay(loop_delay);

}
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------
