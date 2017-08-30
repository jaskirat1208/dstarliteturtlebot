#include <ros/ros.h>
#include <gazebo_msgs/GetModelState.h>
#include <iostream>
#include <geometry_msgs/Twist.h>
#include "unistd.h"
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <string>
#include <ctime>
#include <iostream>
using namespace std;
int main(int argc, char **argv)
{
	ros::init(argc,argv,"get_obstacle_position");
	double x,y,x_in,y_in,theta;
	/*************************************************************************************/
                                /*Initialisation of the ros node*/
	/*************************************************************************************/
	ros::NodeHandle n;
	ros::ServiceClient client=n.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state");
	gazebo_msgs::GetModelState gms;
	string str;
	cin>>str;
	gms.request.model_name=str;
	gms.request.relative_entity_name="";
	client.call(gms);
	int curr_x,curr_y;
	curr_x=(int)(gms.response.pose.position.x+0.5);
	curr_y= (int)(gms.response.pose.position.y+0.5);
	cout<<curr_x<<" "<<curr_y;
	return 0;
}