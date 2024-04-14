// libraries
#include "Gripper.h"
#include "Joint.h"

// motor variables
const int dirPin_1 = 28;
const int stepPin_1 = 29;

const int dirPin_2 = 42;
const int stepPin_2 = 43;

const int dirPin_3 = 50;
const int stepPin_3 = 51;

#define motorInterfaceType 1

// const int powerPin = 27;

// gripper variables
const int feedbackPin = A0;
const int gripperPin = 22;
const int gripperPin2 = 23;

// object initiation
// Gripper gripper(gripperPin, feedbackPin);
// Gripper gripper2(gripperPin2, feedbackPin);
Joint joint1(motorInterfaceType, stepPin_1, dirPin_1);
Joint joint2(motorInterfaceType, stepPin_2, dirPin_2);
Joint joint3(motorInterfaceType, stepPin_3, dirPin_3);

void setup() {

    // serial communication
    Serial.begin(115200);

    // // turn on power
    // pinMode(powerPin, OUTPUT);
    // digitalWrite(powerPin, HIGH);

    // turn on motors
    joint1.setup();
    joint1.setSpeed(1000);
    joint2.setup();
    joint2.setSpeed(1000);
    joint3.setup();
    joint3.setSpeed(1000);

    // turn on gripper
    // gripper.setup();
    // gripper2.setup();
}

// initial desired position
int j1_desiredPos = 0;
int j2_desiredPos = 0;
int j3_desiredPos = 0;
// int gripper_desiredPos = 180;

void loop() {

    // send serial command in the format 180, 180, 180, 180
    if (Serial.available() > 0) {
      
      String receivedData = Serial.readStringUntil('\n');
      
      int values[4];
      int index = 0;
      char *ptr = strtok(const_cast<char *>(receivedData.c_str()), ",");
      
      while (ptr != nullptr && index < 4) {
        values[index] = atoi(ptr);
        index++;
        ptr = strtok(nullptr, ",");
      }

      for (int i = 0; i < 4; i++){

        if (i < 3){
          if (values[i] >= -180 && values[i] <= 180){}
          else{ 
            Serial.println("Warning: joints values should be from -180 to 180");
            break; 
          }
        }

        if (i == 3){
          if (values[3] >= 0 && values[3] <= 180){
            
            j1_desiredPos = float(values[0] / 180.0) * 2700;
            j2_desiredPos = float(values[1] / 180.0) * 2700;
            j3_desiredPos = float(values[2] / 180.0) * 2700;
            // gripper_desiredPos = values[3];   

            //gripper2.moveTo(90); //open the gripper

            Serial.print("Joint1: ");
            Serial.print(j1_desiredPos);
            Serial.print("  Joint2: ");
            Serial.print(j2_desiredPos);
            Serial.print("  Joint3: ");
            Serial.println(j3_desiredPos);
            // Serial.print("  Gripper: ");
            // Serial.println(gripper_desiredPos);
          }
          else{ 
            Serial.println("Warning: gripper values should be from 0 to 180");
            break; 
          }
        }
      }
    }

    // move the joints
    joint1.moveTo(j1_desiredPos);
    joint2.moveTo(j2_desiredPos);
    joint3.moveTo(j3_desiredPos);
    //gripper.moveTo(gripper_desiredPos);
    //gripper2.moveTo(gripper_desiredPos);

    if (joint1.noDistToGo() && joint2.noDistToGo() && joint3.noDistToGo()){
      //delay(2000);
      //gripper2.moveTo(20); // close the gripper if motion completes
    }
}