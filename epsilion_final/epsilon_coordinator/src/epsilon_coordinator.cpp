#include <ros/ros.h>
#include <std_msgs/Int32.h>

std_msgs::Int32 output;
int feedback=0;
int done_moving=4;
void FeedbackCallback1(const std_msgs::Int32& message_holder)
{
done_moving=message_holder.data;
}

void FeedbackCallback(const std_msgs::Int32& message_holder)
{
feedback=message_holder.data;
ROS_INFO("received feedback is : %d",feedback);
switch (feedback)
{
case 1:
ros::Duration(0.1).sleep();
if(done_moving==6)

{output.data=2;}

else output.data=6; // 1(navigator) is finished, switch to 2(finder)
break;
case 2:
ros::Duration(0.1).sleep();
output.data=3; // 2(finder) is finished, switch to 3(grabber)
break;
case 3:
ros::Duration(0.1).sleep();
output.data=4; // 3(grabber) is finished, switch to 4(navigator back)
break;
case 4:
ros::Duration(0.1).sleep();
output.data=5; // 3(grabber) is finished, switch to 4(navigator back)
case 6:
break;
case 0:
ROS_INFO("begin");
break;
case 5:
ROS_INFO("finish");
break;
default: 
ROS_WARN("feedback not recognized");
 break;
 }

}



int main(int argc, char **argv) {
    ros::init(argc, argv, "epsilon_coordinator"); 
    ros::NodeHandle n;
    ros::Publisher command = n.advertise<std_msgs::Int32>("Command_Topic", 1);                 // publisher 

    ros::Subscriber my_subscriber_object=n.subscribe("Feedback_Topic",1,FeedbackCallback);        //subscriber
    ros::Subscriber my_subscriber_object1=n.subscribe("Feedback_Topic1",1,FeedbackCallback1); 
    ros::Rate sleep(20);
    output.data=1; //start with navigator
    while(ros::ok())
    {
      command.publish(output);
      sleep.sleep();
      ros::spinOnce();
    }
}

