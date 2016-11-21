/*
    MILKSHAKE master_node.cpp
    written by: Kevin HUo

    Description:
    A node that subscribes to /joy and publishes the processed joystick data
    to the steering and throttle nodes.
*/

#include <cstdlib>
#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "sensor_msgs/Joy.h"

//#define DEBUG

#define LH_AXIS     0
#define L2_BUTTON   8
#define R2_BUTTON   9
#define L2_AXIS     12
#define R2_AXIS     13

class Controller {
    public:
        ros::Publisher throttle_pub, steering_pub;
        ros::Subscriber sub;
        float throttle, steering;

        Controller() {
            throttle = 0;
            steering = 0;
            ros::NodeHandle nh;
            sub = nh.subscribe("joy", 1, &Controller::ps3data_callback, this);
            throttle_pub = nh.advertise<std_msgs::Float32>("throttle", 1);
            steering_pub = nh.advertise<std_msgs::Float32>("steering", 1);

        }

        void ps3data_callback(const sensor_msgs::Joy::ConstPtr& msg) {
            // Map R2 and L2 buttons for throttle. L2 = reverse, R2 = forward.
            float forward = (1 - msg->axes[R2_AXIS]) / 2 * msg->buttons[R2_BUTTON];
            float reverse = (1 - msg->axes[L2_AXIS]) / 2 * msg->buttons[L2_BUTTON];

            this->throttle = (forward - reverse) * 100;

            // Convert the steering values into radians. -0.52 is left, 0.52 is right.
            this->steering = -1 * msg->axes[LH_AXIS] * 0.52;

        }

        void publish_ctrl_data(void) {
            std_msgs::Float32 throttle_msg, steering_msg;

            throttle_msg.data = this->throttle;
            steering_msg.data = this->steering;

            throttle_pub.publish(throttle_msg);
            steering_pub.publish(steering_msg);
        }
};

int main(int argc, char **argv) {
  ros::init(argc, argv, "master_node");

  Controller ctrl;

  ros::Rate loop_rate(10);
  while(ros::ok()) {
    ctrl.publish_ctrl_data();

#ifdef DEBUG    
    ROS_INFO("Throttle: [%f]\n", ctrl.throttle);
    ROS_INFO("Steering: [%f]\n", ctrl.steering);
#endif
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
