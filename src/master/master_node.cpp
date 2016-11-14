#include <cstdlib>
#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Int16.h"
#include "sensor_msgs/Joy.h"

void ps3data_callback(const sensor_msgs::int32& buttons,
                     const sensor_msgs::float32& joystick) {
  ROS_INFO("R2 button: [%]\n", msg->data.c_str());
  ROS_INFO("L2 button: [%]\n", msg->data.c_str());
  ROS_INFO("X  button: [%]\n", msg->data.c_str());
  ROS_INFO("Left axes: [%]\n\n\n", msg->data.c_str());
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "ps3_ctrl");
  ros::NodeHandle nh;
  ros::Publisher throttle_pub = nh.advertise<std_msgs::Int16>("throttle", 1);
  ros::Publisher steering_pub = nh.advertise<std_msgs::Float32>("steering", 1);
  ros::Subscriber sub = nh.subscribe("ps3joy", 1, ps3data_callback);

  ros::Rate loop_rate(10);
  while(ros::ok()) {
    std_msgs::Int16 throttle_msg;
    std_msgs::Float32 steering_msg;

    ROS_INFO("Throttle Message: %d\n", throttle_msg);
    ROS_INFO("Steering Message: %d\n", steering_msg);
    throttle_pub.publish(throttle_msg);
    steering_pub.publish(steering_msg);


    ros::spinOnce();
    loop_rate.sleep();
  }


  return 0;
}
