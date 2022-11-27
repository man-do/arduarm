#include <ros_control_boilerplate/generic_hw_control_loop.h>
#include <arduarm_hardware_interface/arduarm_hw_interface.h>

int main(int argc, char** argv)
{
  ros::init(argc, argv, "arduarm_hw_interface");
  ros::NodeHandle nh;

  // NOTE: We run the ROS loop in a separate thread as external calls such
  // as service callbacks to load controllers can block the (main) control loop
  ros::AsyncSpinner spinner(3);
  spinner.start();

  // Create the hardware interface specific to your robot
  std::shared_ptr<arduarm_hardware_interface::ArduarmHWInterface> arduarm_hw_interface(new arduarm_hardware_interface::ArduarmHWInterface(nh));
  arduarm_hw_interface->init();

  // Start the control loop
  ros_control_boilerplate::GenericHWControlLoop control_loop(nh, arduarm_hw_interface);
  control_loop.run();

  return 0;
}
