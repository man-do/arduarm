#include <Servo.h>
#include <ros.h>
#include <std_msgs/Int32MultiArray.h>

#define NR_SRV 3
#define START 550
#define END 2400
#define HOME (END-START)/2 // 90 deg
//#define ACCELERATION 2

static Servo servos[NR_SRV];
int8_t joint_position[] = {0, 0, 0};

ros::NodeHandle_<ArduinoHardware, 2, 2, 80, 105> nh;
std_msgs::Int32MultiArray states;
long real_data_storage[3];


ros::Publisher states_pub("/arduarm/joint_pos", &states);

void messageCb( const std_msgs::Int32MultiArray& msg) {
  cmd_all(msg.data[0], msg.data[1], msg.data[2]);
}

ros::Subscriber<std_msgs::Int32MultiArray> cmd_sub("/arduarm/pos_cmd", &messageCb );

void setup() {
  Serial.begin(115200);
  nh.getHardware()->setBaud(115200);
  nh.initNode();
  nh.advertise(states_pub);
  nh.subscribe(cmd_sub);
  nh.negotiateTopics();
  
  servos[0].attach(9);
  servos[1].attach(10);
  servos[2].attach(11);
  
  states.data_length = 3;
  states.data = real_data_storage;

//  states.data[0] = joint_position[0];
//  states.data[1] = joint_position[1];
//  states.data[2] = joint_position[2];
}

void loop() {
  states.data[0] = joint_position[0];
  states.data[1] = joint_position[1];
  states.data[2] = joint_position[2];
  
  states_pub.publish(&states);
  nh.spinOnce();
  delay(1);
}
