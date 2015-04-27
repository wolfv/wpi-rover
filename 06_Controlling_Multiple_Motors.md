# Controlling Multiple Motors #

Once you have gotten EposManager to work with one motor, it is extremely easy to get it to work with multiple motors. Here, we will show you how to control and read information from multiple motors in a single EPOS chain.

## Parameter File ##
Make a new parameter file, lets call it MyOtherEpos.yaml, in the params directory. If you are using the same motor, it can have the same parameters, otherwise, fill it in with what you need. Change the Node\_ID to 2, and the Motor\_Name to Other\_Motor.

## Launch File ##
Create a second launch file named "MultipleMotorTest.launch" in the launch directory. Copy and paster the below code into it.

```
<launch>
        <rosparam command="load" file="$(find epos_tutorials)/params/MyEpos.yaml" ns="motors/MyEpos" />
		<rosparam command="load" file="$(find epos_tutorials)/params/MyOtherEpos.yaml" ns="motors/MyOtherEpos" />

        <node name="NewEpos" pkg="EposManager" type="EposManager"  ns="motors" output="screen" args="MyEpos MyOtherEpos" >
                <param name="Protocol" value="USB" />
                <param name="Port" value="USB0" />
                <param name="Publish_Rate" value="10" />
        </node>
</launch>
```

## The Program ##
Now its time to start writing a program to control multiple motors.  Below is some sample source code for sending information to and receiving information from multiple EPOS controllers in a chain.

```
#include "ros/ros.h"
#include <EposManager/EPOSControl.h>
#include <EposManager/MotorInfo.h>
#include <EposManager/GroupEPOSControl.h>
#include <EposManager/GroupMotorInfo.h>
#include <ros/console.h>

ros::Publisher groupDrivePublisher;

void motorInfoCallback(const EposManager::GroupMotorInfo::ConstPtr& msg){
  vector<EposManager::MotorInfo> motor_group;
  motor_group = msg->motor_group;
  for(int i=0; i< motor_group.size();i++){
   ROS_INFO_STREAM("Node ID: " << motor_group[i].node_id);
   ROS_INFO_STREAM("Motor Name: " << motor_group[i].motor_name);
   ROS_INFO_STREAM("Motor Current: " << motor_group[i].motor_current);
   ROS_INFO_STREAM("Motor Position: " << motor_group[i].motor_position);
   ROS_INFO_STREAM("Motor Velocity: " << motor_group[i].motor_velocity);
  }
}

void sendVelocityMessage(int motorOneVelocity, int motorTwoVelocity){
  EposManager::GroupEPOSControl msg;
  EposManager::EPOSControl motorOne, motorTwo;
  motorOne.control_mode=EposManager::EPOSControl::VELOCITY;
  motorTwo.control_mode=EposManager::EPOSControl::VELOCITY;
  
  motorOne.node_id = 1;
  motorTwo.node_id = 2;
  
  motorOne.setpoint=motorOneVelocity;
  motorTwo.setpoint=motorTwoVelocity;
  
  msg.motor_group.push_back(motorOne);
  msg.motor_group.push_back(motorTwo);
  groupDrivePublisher.publish(msg);
}

int main (int argc, char **argv){
 ros::init(argc, argv, "EPOSTest");
 ros::NodeHandle n;
 ros::Subscriber motorInfoSubscriber = n.subscribe("motors/Group_Motor_Info", 1, motorInfoCallback);

 groupDrivePublisher = n.advertise<EposManager::GroupEPOSControl>("motors/Group_Motor_Control", 1);
 ros::spin();
}

```