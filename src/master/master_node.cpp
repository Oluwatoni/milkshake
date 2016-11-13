#include <cstdlib>
#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Int16.h"

int main(int argc, char **argv)
{
  ros::init(argc, argv, "");
  ros::NodeHandle nh;
  ros::Publisher throttle_pub = nh.advertise<std_msgs::Int16>("throttle", 1);
  ros::Publisher steering_pub = nh.advertise<std_msgs::Float32>("steering", 1);

  ros::Rate loop_rate(10);
  while(ros::ok()) {
    std_msgs::Int16 throttle_msg;
    std_msgs::Float32 steering_msg;

    ROS_INFO("%s", msg.data.c_str());
    throttle_pub.publish(throttle_msg);
    steering_pub.publish(steering_msg);


    ros::spinOnce();
    loop_rate.sleep();
  }


  return 0;
}
