#include <arduarm_hardware_interface/arduarm_hw_interface.h>
#include <std_msgs/Float64.h>
#include <sensor_msgs/JointState.h>
#include <cmath>

namespace arduarm_hardware_interface
{

void ArduarmHWInterface::callback(const sensor_msgs::JointState::ConstPtr& msg){
    for(int i = 0; i < num_joints_ ; ++i){
      joint_position_[i] = msg->position[i] * M_PI / 180.0;
  }
}

ArduarmHWInterface::ArduarmHWInterface(ros::NodeHandle& nh, urdf::Model* urdf_model)
  :ros_control_boilerplate::GenericHWInterface(nh, urdf_model)
{
  ROS_INFO_NAMED("arduarm_hw_interface", "ArduarmHWInterface Ready.");
  cmd_pub = nh_.advertise<sensor_msgs::JointState>("/arduarm/pos_cmd", 1000);
  state_sub = nh_.subscribe("/arduarm/joint_pos", 1000, &ArduarmHWInterface::callback, this);

  cmds.position.resize(3);

  // cmds.layout.dim.push_back(std_msgs::MultiArrayDimension());
  // cmds.layout.dim[0].label = "positions";
  // cmds.layout.dim[0].size = 3;
  // cmds.layout.dim[0].stride = 3;
  // cmds.layout.data_offset = 0;

  // states.layout.dim.push_back(std_msgs::MultiArrayDimension());
  // states.layout.dim[0].label = "positions";
  // states.layout.dim[0].size = 3;
}

void ArduarmHWInterface::read(ros::Duration& elapsed_time)
{
}

void ArduarmHWInterface::write(ros::Duration& elapsed_time)
{
  // Safety
  enforceLimits(elapsed_time);

  // std::vector<int> vec(num_joints_);

  for (int i = 0; i < num_joints_; ++i){
    int temp = (int)(joint_position_command_[i] * 180.0 / M_PI);
    cmds.position[i] = temp;
  }
  // cmds.position = vec;

  cmd_pub.publish(cmds);
}

void ArduarmHWInterface::enforceLimits(ros::Duration& period)
{
  pos_jnt_sat_interface_.enforceLimits(period);
}

}
