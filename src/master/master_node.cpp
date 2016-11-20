#include <cstdlib>
#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Int16.h"
#include "sensor_msgs/Joy.h"

void ps3data_callback(const sensor_msgs::Joy::ConstPtr& msg) {
  ROS_INFO("R2 button: [%f]\n", msg->axes[13]);
  ROS_INFO("L2 button: [%f]\n", msg->axes[12]);
  ROS_INFO("X  button: [%d]\n", msg->buttons[14]);
  ROS_INFO("Left horizontal axes: [%f]\n", msg->axes[0]);
  ROS_INFO("Left vertical axes: [%f]\n\n\n", msg->axes[1]);
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "master_node");
  ros::NodeHandle nh;
//  ros::Publisher throttle_pub = nh.advertise<std_msgs::Int16>("throttle", 1);
//  ros::Publisher steering_pub = nh.advertise<std_msgs::Float32>("steering", 1);
  ros::Subscriber sub = nh.subscribe("joy", 1, ps3data_callback);

  ros::spin();

/*
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
*/

  return 0;
}
