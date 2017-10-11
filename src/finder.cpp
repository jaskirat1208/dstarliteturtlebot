#include <ros/ros.h>
#include <gazebo_msgs/GetModelState.h>
#include <iostream>
#include <geometry_msgs/Twist.h>
#include "unistd.h"
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <ctime>
using namespace std;
ros::Publisher vel_pub;
double getangle(double z,double w){
        double halfangle=atan2(z,w);
        // cout<<halfangle<<endl;
        return 2*halfangle;
}

void rotate(double x, double y, double x_in, double y_in, double theta,
ros::NodeHandle n){
        ros::ServiceClient client=n.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state");
        gazebo_msgs::GetModelState gms;
        gms.request.model_name="mobile_base";
        gms.request.relative_entity_name="";
        client.call(gms);
        cout<<"Rotating: "<<endl;
        geometry_msgs::Twist vel_msg;                              //to be published 

        vel_msg.linear.x=0;
        vel_msg.linear.y=0;
        vel_msg.linear.z=0;
        double delta_y,delta_x;
        delta_x=x-x_in;
        delta_y=y-y_in;
        vel_msg.angular.x=0;
        vel_msg.angular.y=0;
        vel_msg.angular.z=0;
        double threshold =0.01;
        // cout<<"FINAL: "<<atan2(delta_y,delta_x)<<endl<<"CURRENT: "<<theta;
        ros::Rate loop(10);
        while(abs(atan2(delta_y,delta_x)-theta)>threshold){
                client.call(gms);
                theta=getangle(gms.response.pose.orientation.z,gms.response.pose.orientation.w);
                vel_msg.angular.z=atan2(delta_y,delta_x)-theta;
                vel_pub.publish(vel_msg);
                ros::spinOnce();
                loop.sleep();
                // cout<<"FINAL: "<<atan2(delta_y,delta_x)<<endl<<"CURRENT: "<<theta;
        
        }
        vel_msg.angular.z=0;
        vel_pub.publish(vel_msg);
        cout<<"END OF ROTATION"<<endl;
}



void translate(double x, double y, double x_in, double y_in, double theta,
ros::NodeHandle n){
        ros::ServiceClient client=n.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state");
        gazebo_msgs::GetModelState gms;
        gms.request.model_name="mobile_base";
        gms.request.relative_entity_name="";
        client.call(gms);
        /*************************************************************************************/
                 /*client.call() returns the current position of the turtlebot*/
        /*************************************************************************************/
        cout<<"TRANSLATION: "<<endl;
        geometry_msgs::Twist vel_msg;                              //to be published 

        vel_msg.linear.x=0;
        vel_msg.linear.y=0;
        vel_msg.linear.z=0;
        double delta_y,delta_x;
        delta_x=x-x_in;
        delta_y=y-y_in;
        vel_msg.angular.x=0;
        vel_msg.angular.y=0;
        vel_msg.angular.z=0;
        
        ros::Rate loop(10);
        double threshold=0.1;
        double distance=sqrt(delta_y*delta_y+delta_x*delta_x);
                
        while(distance>threshold){
                client.call(gms);
                double x_curr,y_curr;
                x_curr=gms.response.pose.position.x;
                y_curr=gms.response.pose.position.y;
                delta_x=x-x_curr;
                delta_y=y-y_curr;
                distance=sqrt(delta_y*delta_y+delta_x*delta_x);
                vel_msg.linear.x=0.5*distance;
                vel_msg.angular.z=4*(atan2(delta_y,delta_x)-(getangle(gms.response.pose.orientation.z,gms.response.pose.orientation.w)));
                // cout<<"Currently at ("<<x_curr<<","<<y_curr<<")"<<endl;
                vel_pub.publish(vel_msg);
                ros::spinOnce();
                loop.sleep();        
        }
        vel_msg.angular.z=0;
        vel_pub.publish(vel_msg);
        cout<<"END OF TRANSLATION.";
}
void move_goal(double x, double y, double x_in, double y_in, double theta,
ros::NodeHandle n){
        double delta_x=x-x_in;
        double delta_y=y-y_in;
        // double theta;
        /*************************************************************************************/
                        /*It rotates the robot from the current orientation theta to*/
                                   /*the desired orientation atan2(delta_y, delta_x)*/
                                     /*Then it just moves linearly to that point*/
        /*************************************************************************************/
        
        rotate(x,y,x_in,y_in,theta,n);                                                                        //rotates along the desired orientation
        translate(x,y,x_in,y_in,theta,n);                                                                        //move it in the required direction
        cout<<"DESTINATION REACHED"<< x <<" " <<y;
}
int main(int argc, char **argv)
{
        // int n;
        ros::init(argc,argv,"finder");
        double x,y,x_in,y_in,theta;
        /*************************************************************************************/
                                                /*Initialisation of the ros node*/
        /*************************************************************************************/
        ros::NodeHandle n;
        ros::ServiceClient client=n.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state");
        gazebo_msgs::GetModelState gms;
        gms.request.model_name="mobile_base";
        gms.request.relative_entity_name="";
        vel_pub = n.advertise<geometry_msgs::Twist>("/mobile_base/commands/velocity",10);
        client.call(gms);
        /***********************************************************************/
                /*gms returns the current location of the turtlebot*/
        /***********************************************************************/
        // cout<<"Enter the coordinates where you want to go: "<<endl;
        // cout<<"X= ";
        cin>>x;
        // cout<<"Y= ";
        cin>>y;
        x_in=gms.response.pose.position.x;
        y_in=gms.response.pose.position.y;
        theta=gms.response.pose.orientation.z;
        cout<<x_in<<endl<<y_in<<endl<<theta;
        /*************************************************************************************/
                                                                        /*x,y: final positions*/
                                                                /*x_in, y_in: initial positions*/
                                                        /*theta: initial orientation of the turtlebot*/
        /*************************************************************************************/
        move_goal(x,y,x_in,y_in,theta,n);
        return 0;
}