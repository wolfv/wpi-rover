# Controlling the Motor from a Program #

Now that you are able to control a motor from the command line, you should be ready to write your own program that uses EposManager. Here, we will show you how to receive and send motor information through C++. First, we will show you the full program, before breaking it down into its respective subsections.


## Sample Code ##

```
#include "ros/ros.h"
#include <EposManager/EPOSControl.h>
#include <EposManager/MotorInfo.h>
#include <ros/console.h>

ros::Publisher drivePublisher;

void motorInfoCallback(const EposManager::MotorInfo::ConstPtr& msg){
 ROS_INFO_STREAM("Node ID: " << msg->node_id);
 ROS_INFO_STREAM("Motor Name: " << msg->motor_name);
 ROS_INFO_STREAM("Motor Current: " << msg->motor_current);
 ROS_INFO_STREAM("Motor Position: " << msg->motor_position);
 ROS_INFO_STREAM("Motor Velocity: " << msg->motor_velocity);
}

void sendVelocityMessage(int nodeID, int velocity){
  EposManager::EPOSControl msg;
  msg.control_mode=EposManager::EPOSControl::VELOCITY;
  msg.node_id = nodeID;
  msg.setpoint=velocity;
  drivePublisher.publish(msg);
}

int main (int argc, char **argv){
 ros::init(argc, argv, "EPOSTest");
 ros::NodeHandle n;
 ros::Subscriber motorInfoSubscriber = n.subscribe("motors/Motor_Info", 1, motorInfoCallback);

drivePublisher = n.advertise<EposManager::EPOSControl>("motors/Motor_Control", 1);
 ros::spin();
}

```