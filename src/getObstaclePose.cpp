#include <ros/ros.h>
#include <gazebo_msgs/GetModelState.h>
#include <iostream>
#include <geometry_msgs/Twist.h>
#include "unistd.h"
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <sensor_msgs/LaserScan.h>
#include <string>
#include <ctime>
#include <iostream>
using namespace std;
double min_scan_angle_ = -40.0/180*M_PI;
double max_scan_angle_ = +40.0/180*M_PI;
double middle_proximity_range_ = 1.50;
double min_obstacle_distance_ = 0.6;
void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
	// Find the closest range between the defined minimum and maximum angles
	int minIndex = 0;
	int maxIndex = round((scan->angle_max - scan->angle_min) / scan->angle_increment);
	float theta = minIndex; 
	middle_proximity_range_ = scan->ranges[(minIndex+maxIndex)/2];
	// cout<<middle_proximity_range_;
	float closestRange = 1000;
	for (int currIndex = minIndex ; currIndex < maxIndex; currIndex++) {
		if (scan->ranges[currIndex] < closestRange ) {
			
			// cout<<"HI";
			closestRange = scan->ranges[currIndex];
			theta = scan->angle_min + currIndex*scan->angle_increment;
			// cout<<closestRange;
		}
		// cout<<scan->ranges[currIndex]<<" ";
	}
	ROS_INFO("Robot Closest range: %lf at theta = %f ",closestRange,theta);
	min_obstacle_distance_ = closestRange;
	cout<<"HELLO";
	//min_obstacle_distance_ = middle_proximity_range_;
	//ROS_INFO_STREAM("Robot: %d Closest range: " << closestRange);
	
	
}
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
	curr_x=round(gms.response.pose.position.x);
	curr_y= round(gms.response.pose.position.y);
	cout<<curr_x<<" "<<curr_y<<endl;
	// ros::NodeHandle n;
	// ros::Subscriber sub = n.subscribe("/scan", 10, scanCallback);
	//  ros::spin();
	// cout<<"HELLOWORLD";
	return 0;
}