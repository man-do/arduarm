#include <Arduino.h>
#include <Servo.h>
#include <ros.h>
#include <std_msgs/UInt16.h>
#include <sensor_msgs/JointState.h>

//#define servo_delay 50
#define START 550
#define END 2400
ros::NodeHandle_<ArduinoHardware, 2, 2, 80, 105> nh;
Servo servos[3];

byte num_joints = 3;
float pins[] = {9, 10, 11};
float cmds[] = {0, 0, 0};
float prev[] = {0, 0, 0};
byte positionChanged = 0;

int sgn(int x) {
  if (x > 0) return 1;
  if (x < 0) return -1;
  return 0;
}

int toMicros(int angle) {
  //angle = map(angle, -90, 90, 0, 180);
  return map(angle, 0, 180, START, END);
}

void cmd_all(int s1, int s2, int s3) {
  int current[3];
  int goal[3] = {(int)s1, (int)s2, (int)s3};
  int diff[3];

  while (*current != *goal) {
    current[0] = servos[0].read();
    current[1] = servos[1].read();
    current[2] = servos[2].read();
    diff[0] = goal[0] - current[0];
    diff[1] = goal[1] - current[1];
    diff[2] = goal[2] - current[2];
    // convert to millis
    servos[0].writeMicroseconds(toMicros(current[0] += sgn(diff[0])));

    delay(20);

    servos[1].writeMicroseconds(toMicros(current[1] += sgn(diff[1])));

    delay(20);

    servos[2].writeMicroseconds(toMicros(current[2] += sgn(diff[2])));

    delay(5);
  }
}

void servo_cb(const sensor_msgs::JointState& cmd_msg) {
  cmds[0] = radiansToDegrees(cmd_msg.position[0]);
  cmds[1] = radiansToDegrees(cmd_msg.position[1]);
  cmds[2] = radiansToDegrees(cmd_msg.position[2]);

  //  for (int i = 0; i < num_joints; ++i) {
  //    servos[i].write(cmds[i]);
  //  }

  cmd_all(cmds[0], cmds[1], cmds[2]);

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


ros::Subscriber<sensor_msgs::JointState> sub("/arduarm/pos_cmd", servo_cb);

void setup() {
  nh.getHardware()->setBaud(115200);
  nh.initNode();
  nh.subscribe(sub);

  servos[0].attach(pins[0]);
  servos[1].attach(pins[1]);
  servos[2].attach(pins[2]);

  delay(1);
//  servos[0].write(90);
//  servos[1].write(90);
//  servos[2].write(90);
  cmd_all(90, 90, 90);
}

void loop() {
  nh.spinOnce();
}

double radiansToDegrees(float position_radians)
{

  position_radians = position_radians + 1.6;

  return position_radians * 57.2958;

}
