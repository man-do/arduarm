#ifndef ARDUARM_HW_INTERFACE_H
#define ARDUARM_HW_INTERFACE_H

#include <ros_control_boilerplate/generic_hw_interface.h>
#include <std_msgs/String.h>
#include <std_msgs/Int32MultiArray.h>
#include <sensor_msgs/JointState.h>

namespace arduarm_hardware_interface
{
  
class ArduarmHWInterface : public ros_control_boilerplate::GenericHWInterface
{
public:
  ArduarmHWInterface(ros::NodeHandle& nh, urdf::Model* urdf_model = NULL);

  virtual void read(ros::Duration& elapsed_time);

  virtual void write(ros::Duration& elapsed_time);

  virtual void enforceLimits(ros::Duration& period);

  void callback(const sensor_msgs::JointState::ConstPtr&);

private:
  ros::Publisher cmd_pub;
  ros::Subscriber state_sub;
  sensor_msgs::JointState cmds;
  sensor_msgs::JointState states;  
};

}

#endif
