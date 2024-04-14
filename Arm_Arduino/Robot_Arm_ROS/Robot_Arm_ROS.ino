// libraries
#include "Gripper.h"
#include "Joint.h"

// ros libraries
#include <ros.h>
#include <std_msgs/Bool.h>
#include <geometry_msgs/Twist.h>

// ros node
ros::NodeHandle node_handle;

// ros msg
std_msgs::Bool complete_msg;
geometry_msgs::Twist command_msg;

// motor variables
const int dirPin_1 = 29;
const int stepPin_1 = 28;

const int dirPin_2 = 45;
const int stepPin_2 = 44;

const int dirPin_3 = 51;
const int stepPin_3 = 50;

#define motorInterfaceType 1

// // motor power
// const int powerPin = 53;

// void powerOn(bool on){

//     pinMode(powerPin, OUTPUT);

//     if (on) {
//       digitalWrite(powerPin, HIGH);
//     }
//     else {
//       digitalWrite(powerPin, LOW);
//     }
// }

// // gripper variables
// const int feedbackPin = A0;
// const int gripperPin = 22;
// const int gripperPin2 = 23;

// object initiation
// Gripper gripper(gripperPin, feedbackPin);
// Gripper gripper2(gripperPin2, feedbackPin);
Joint joint1(motorInterfaceType, stepPin_1, dirPin_1);
Joint joint2(motorInterfaceType, stepPin_2, dirPin_2);
Joint joint3(motorInterfaceType, stepPin_3, dirPin_3);

// ros publisher
ros::Publisher complete_publisher("arm_complete", &complete_msg);

// initial joint position and velocity
int joint1Pos = 0;
int joint2Pos = 0;
int joint3Pos = 0;
int joint1Vel = 0;
int joint2Vel = 0;
int joint3Vel = 0;

// ros subscriber call back
void subscriberCallback(const geometry_msgs::Twist& command_msg) {

    // send incompletion ros msg
    complete_msg.data = false;
    complete_publisher.publish(&complete_msg);

    if (int(command_msg.linear.x) >= -180 && int(command_msg.linear.x) <= 180 && 
        int(command_msg.linear.y) >= -180 && int(command_msg.linear.y) <= 180 && 
        int(command_msg.linear.z) >= -180 && int(command_msg.linear.z) <= 180)
    {

        // assign joint position and velocity
        joint1Pos = int(command_msg.linear.x*2700/180);
        joint2Pos = int(command_msg.linear.y*2700/180);
        joint3Pos = int(command_msg.linear.z*2700/180);

        joint1Vel = int(command_msg.angular.x);
        joint2Vel = int(command_msg.angular.y);
        joint3Vel = int(command_msg.angular.z);

        // Set the speed in RPM
        joint1.setSpeed(joint1Vel); 
        joint2.setSpeed(joint2Vel);
        joint3.setSpeed(joint3Vel);

        //open the gripper
        // gripper.moveTo(90); 
    }
}

// ros subscriber
ros::Subscriber<geometry_msgs::Twist> command_subscriber("arm_command", &subscriberCallback);

void setup() {

    // // turn on power
    // powerOn(true);

    // turn on motors
    joint1.setup();
    joint2.setup();
    joint3.setup();

    // enable ros
    node_handle.initNode();
    node_handle.advertise(complete_publisher);
    node_handle.subscribe(command_subscriber);
}

void loop() {
    
    // ros spin
    node_handle.spinOnce();

    // move the joints
    joint1.moveTo(joint1Pos);
    joint2.moveTo(joint2Pos);
    joint3.moveTo(joint3Pos);

    if (joint1.noDistToGo() && joint2.noDistToGo() && joint3.noDistToGo() && !complete_msg.data){
        
        // close the gripper if motion completes
        // gripper.moveTo(180);

        // send completion ros msg
        complete_msg.data = true;
        complete_publisher.publish(&complete_msg);
    }
}