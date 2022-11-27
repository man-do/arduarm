#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#endif

#include <Servo.h>
#include <ros.h>
#include <std_msgs/UInt16.h>
#include <sensor_msgs/JointState.h>

ros::NodeHandle_<ArduinoHardware, 2, 2, 80, 105> nh;

Servo servos[3];

byte num_joints = 3;
double pins[] = {9, 10, 11};
double cmds[] = {0, 0, 0};
double prev[] = {0, 0, 0};
//int8_t initial[] = {90, 90, 90};

byte positionChanged = 0;

void servo_cb(const sensor_msgs::JointState& cmd_msg) {
  cmds[0] = radiansToDegrees(cmd_msg.position[0]);
  cmds[1] = radiansToDegrees(cmd_msg.position[1]);
  cmds[2] = radiansToDegrees(cmd_msg.position[2]);

  for (int i = 0; i < num_joints; ++i) {
    servos[i].write(cmds[i]);
  }

  if (prev[0] == cmds[0] && prev[1] == cmds[1] && prev[2] == cmds[2] && positionChanged == 0)
  {
    positionChanged = 1;
  }
  else if ((prev[0] != cmds[0] || prev[1] != cmds[1] || prev[2] != cmds[2]) && positionChanged == 1)
  {
    positionChanged = 0;
  }

  prev[0] = cmds[0];
  prev[1] = cmds[1];
  prev[2] = cmds[2];
}


ros::Subscriber<sensor_msgs::JointState> sub("pos_cmd", servo_cb);

void setup() {
  nh.getHardware()->setBaud(115200);
  nh.initNode();
  nh.subscribe(sub);

  servos[0].attach(pins[0]);
  servos[1].attach(pins[1]);
  servos[2].attach(pins[2]);

  delay(1);
  servos[0].write(90);
  servos[1].write(90);
  servos[2].write(90);
}

void loop() {
  nh.spinOnce();
}

double radiansToDegrees(float position_radians)
{

  position_radians = position_radians + 1.6;

  return position_radians * 57.2958;

}