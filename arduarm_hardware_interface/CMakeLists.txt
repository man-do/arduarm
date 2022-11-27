cmake_minimum_required(VERSION 3.0.2)
project(arduarm_hardware_interface)

find_package(catkin REQUIRED 
    COMPONENTS 
    ros_control_boilerplate
)

catkin_package(
      INCLUDE_DIRS include
  CATKIN_DEPENDS 
  ros_control_boileplate
)
include_directories(
  include/
  ${catkin_INCLUDE_DIRS}
)

# Main control executable
add_executable(arduarm_hw_main 
  src/arduarm_hw_main.cpp 
  src/arduarm_hw_interface.cpp
)

target_link_libraries(arduarm_hw_main
  ${catkin_LIBRARIES}
)

# Install executables
install(TARGETS
    arduarm_hw_main
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

# Install header files
install(DIRECTORY include/arduarm_hardware_interface/   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})