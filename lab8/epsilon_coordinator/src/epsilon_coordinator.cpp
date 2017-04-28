#include <ros/ros.h>
#include <std_msgs/Int32.h>

std_msgs::Int32 output;
int feedback=0;

void FeedbackCallback(const std_msgs::Int32& message_holder)
{
feedback=message_holder.data;
ROS_INFO("received feedback is : %d",feedback);
switch (feedback)
{
case 1:
ros::Duration(1).sleep();
output.data=2; // 1(navigator) is finished, switch to 2(finder)
break;
case 2:
ros::Duration(1).sleep();
output.data=3; // 2(finder) is finished, switch to 3(grabber)
break;
case 3:
ros::Duration(1).sleep();
output.data=4; // 3(grabber) is finished, switch to 4(navigator back)
break;
case 0:
ROS_INFO("000");
default: 
ROS_WARN("feedback not recognized");
 break;
 }

}



int main(int argc, char **argv) {
    ros::init(argc, argv, "epsilon_coordinator"); 
    ros::NodeHandle n;

    ROS_INFO("epsilon_coordinator started; publishing to cmd and subscribing to fb rostopics!");

    ros::Publisher command = n.advertise<std_msgs::Int32>("cmd", 1);                 // publisher 

    ros::Subscriber my_subscriber_object=n.subscribe("fb",1,FeedbackCallback);        //subscriber
    
    ros::Rate sleep(20);
    ros::Duration(1).sleep(); //delay 1 sec
    output.data=1; //start with navigator
    while(ros::ok())
    {
      command.publish(output);
      sleep.sleep();
      ros::spinOnce();
    }
}

