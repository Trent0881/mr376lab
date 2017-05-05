//pub_des_state_path_client:
// illustrates how to send a request to the append_path_queue_service service

#include <ros/ros.h>
#include <mobot_pub_des_state/path.h>
#include <iostream>
#include <string>
#include <std_msgs/Int32.h>
#include <nav_msgs/Path.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
using namespace std;


////////Epsilon_modify_////////////////Epsilon_modify_////////////////Epsilon_modify_////////////////Epsilon_modify_////////////////Epsilon_modify_////////
std_msgs::Int32 feedback;
int command=0;
double current_time;
////////Epsilon_modify_////////////////Epsilon_modify_////////////////Epsilon_modify_////////////////Epsilon_modify_////////////////Epsilon_modify_////////



geometry_msgs::Quaternion convertPlanarPhi2Quaternion(double phi) {
    geometry_msgs::Quaternion quaternion;
    quaternion.x = 0.0;
    quaternion.y = 0.0;
    quaternion.z = sin(phi / 2.0);
    quaternion.w = cos(phi / 2.0);
    return quaternion;
}



////////Epsilon_modify_////////////////Epsilon_modify_////////////////Epsilon_modify_////////////////Epsilon_modify_////////////////Epsilon_modify_////////
void CommandCallback(const std_msgs::Int32& message_holder)
{
command=message_holder.data;
ROS_INFO("received command is : %d",command);
}
////////Epsilon_modify_////////////////Epsilon_modify_////////////////Epsilon_modify_////////////////Epsilon_modify_////////////////Epsilon_modify_////////



int main(int argc, char **argv) {
    ros::init(argc, argv, "append_path_client");
    ros::NodeHandle n;
    ros::ServiceClient client = n.serviceClient<mobot_pub_des_state::path>("append_path_queue_service");
    geometry_msgs::Quaternion quat;
    mobot_pub_des_state::path path_srv;
    geometry_msgs::PoseStamped pose_stamped;
    pose_stamped.header.frame_id = "map";
    geometry_msgs::Pose pose;
    ros::Rate sleep(20);
     while (!client.exists()) {
      ROS_INFO("waiting for service...");
      ros::Duration(1.0).sleep();
    }
    ROS_INFO("connected client to service");

////////Epsilon_modify_////////////////Epsilon_modify_////////////////Epsilon_modify_////////////////Epsilon_modify_////////////////Epsilon_modify_////////
    ros::Subscriber Command=n.subscribe("Command_Topic",1,CommandCallback); /// subscribe to command topic to listen to coordinator
    ros::Publisher feedback_publisher = n.advertise<std_msgs::Int32>("Feedback_Topic", 1, true);

while(ros::ok()){
    while(command!=1&&command!=4){ros::spinOnce();} /// ont hear from coordinator, wait 


if(command==1){
	feedback.data=1;

    ///MOVE_IN_path///MOVE_IN_path///MOVE_IN_path////
   ///MOVE_IN_path///MOVE_IN_path///MOVE_IN_path////
    ///MOVE_IN_path///MOVE_IN_path///MOVE_IN_path////  
    pose.position.x = 0.0; // say desired x-coord is 5
    pose.position.y = 0.0;
    pose.position.z = 0.0; // let's hope so!
    quat = convertPlanarPhi2Quaternion(0);
    pose.orientation = quat;
    pose_stamped.pose = pose;
    path_srv.request.path.poses.push_back(pose_stamped);
 
    pose.position.x = 3.9;
    pose_stamped.pose = pose;
    path_srv.request.path.poses.push_back(pose_stamped);

    pose.position.y = 1.85;
    pose_stamped.pose = pose;
    path_srv.request.path.poses.push_back(pose_stamped);

//........
   
  
    
    client.call(path_srv);


ROS_INFO("call_the service");
}

if(command==4){  /// NOTICE!!!!  you might want to add something clever here to make robot go backwards first!!!!!
	feedback.data=4;
path_srv.request.path.poses.clear();
    ///MOVE_OUT_path///MOVE_OUT_path///MOVE_OUT_path////
   ///MOVE_OUT_path///MOVE_OUT_path///MOVE_OUT_path////
    ///MOVE_OUT_path///MOVE_OUT_path///MOVE_OUT_path////  
    pose.orientation.x = 5.0;   // flag on
    pose_stamped.pose = pose;
    path_srv.request.path.poses.push_back(pose_stamped);
 
    pose.orientation.x = 0.0;   //flag off
    pose.position.y = 0.0; 
    pose_stamped.pose = pose;
    path_srv.request.path.poses.push_back(pose_stamped);
 
    pose.position.x = 0.0;
    pose_stamped.pose = pose;
    path_srv.request.path.poses.push_back(pose_stamped);


//........
   
  
    
    client.call(path_srv);

}

current_time=ros::Time::now().toSec();  // record the current system time(absolute time)
while(ros::Time::now().toSec()-current_time<2)
{;}

while(command==1||command==0||command==6||command==4||command==5)    /*(ros::Time::now().toSec())-current_time<6*/      //////////// publish the "job done signal for 3 sec"
{
feedback_publisher.publish(feedback);
ros::spinOnce();
sleep.sleep();

}
///once the feedback is received by the coordinator, the command should change to next state thus command!=1||command!=4==true


}
////////Epsilon_modify_////////////////Epsilon_modify_////////////////Epsilon_modify_////////////////Epsilon_modify_////////////////Epsilon_modify_////////

    return 0;
}
