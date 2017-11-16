#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <ros/ros.h>
#include <gazebo_msgs/GetModelState.h>
#include <geometry_msgs/Twist.h>
#include "Dstar.h"
using namespace std;
/*-----------------------------------------Global variables-----------------------------------------*/
ros::Publisher vel_pub;
/*---------------------------------------------------------------------------------------------------*/
/*------------------------------------------FUNCTIONS------------------------------------------------*/
std::vector<pair<int,int> > getpose();
void create_output_file(pair<int,int> p1,pair<int,int> p2);
pair<int,int> get_obstacle_position(ros::NodeHandle n);
double getangle(double z,double w);
void rotate(double x, double y, double x_in, double y_in, double theta,ros::NodeHandle n);
void translate(double x, double y, double x_in, double y_in, double theta, ros::NodeHandle n);
void move_goal(double x, double y,ros::NodeHandle n);
/*---------------------------------------------------------------------------------------------------*/
int main(int argc, char  **argv)
{
	ros::init(argc,argv,"driver");
	ros::NodeHandle n;
	ros::ServiceClient client=n.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state");
	/*Declarations*/
	Dstar *dstar = new Dstar(); 
	int x,y;
	pair<int,int> obstacle_position;
	
	gazebo_msgs::GetModelState g;
	list<state> mypath;
	/*initializations*/
	vel_pub = n.advertise<geometry_msgs::Twist>("/mobile_base/commands/velocity",10);
	cin>>x>>y;
	g.request.model_name="mobile_base";
	client.call(g);
	int x_cur = g.response.pose.position.x,y_cur = g.response.pose.position.y;
	dstar->init(x_cur,y_cur,x,y);
	dstar->replan();               // plan a path
	mypath = dstar->getPath();
	// cout<<mypath.size();
	// system("rosrun dstarliteturtlebot planner > \"file.txt\"");
	cout<<"PATH PLANNED"<<endl;
	int max_iter=0;
	list<state>::iterator iter = mypath.begin(),iter2;
    for(iter2=mypath.begin(); iter2 != mypath.end(); iter2++) {
        //glVertex3f(iter2->x, iter2->y, 0.3);
        cout<<"("<<iter2->x<<","<<iter2->y<<")"<<endl;
        // cout<<"Path comprises of ("<<iter2->x<<","<<iter2->y<<")"<<endl;
    }

/*CODE FOR PLANNER HERE*/
	while( iter!=mypath.end() ){
		cout<<"Going to: "<<iter->x<<" "<<iter->y<<endl;
		obstacle_position = get_obstacle_position(n);
		state s ;
		s.x = obstacle_position.first;
		s.y = obstacle_position.second;
		list<state>::iterator iter_for_find = find(mypath.begin(), mypath.end(),s);
		if ( iter_for_find!=mypath.end())	
		{
			max_iter++;
			client.call(g);
			cout<<"REPLANNING"<<iter_for_find->x<<","<<iter_for_find->y<<"NEW PATH:"<<endl;
			dstar->updateStart(g.response.pose.position.x,g.response.pose.position.y);
			dstar->updateCell(iter_for_find->x,iter_for_find->y,-1); 		//obstacle 
			dstar->replan();
			mypath = dstar->getPath();
			iter = mypath.begin();
			dstar->updateCell(iter_for_find->x,iter_for_find->y,0);		//reset the cost so that it does not become a hindrance in future.
            for(iter2=mypath.begin(); iter2 != mypath.end(); iter2++) {
                cout<<"("<<iter2->x<<","<<iter2->y<<")"<<endl;
                // cout<<"Path comprises of ("<<iter2->x<<","<<iter2->y<<")"<<endl;
            }
		}
		move_goal(iter->x,iter->y,n);
		++iter;
	}
	return 0;
}


void create_output_file(pair<int,int> p1,pair<int,int> p2){
	ofstream f("update.txt");
	f<<p2.first<<" "<<p2.second<<endl<<p1.first<<" "<<p1.second;

} 
std::vector<pair<int,int> > getpose(){
	fstream plan_file("file.txt");
	string line;
	std::vector< pair<int,int> > v;
	stringstream ss;
	while(getline(plan_file,line)){
	// explode(line,'=');
	for (int i = 0; i < line.size(); ++i)
	{
		if (line[i]=='=')
		{
			line[i]=' ';
		}
	}
	ss<<line<<" ";
	int x,y;
	ss>>x>>y;
	v.push_back(make_pair(x,y));
	}
	return v;
}
pair<int,int> get_obstacle_position(ros::NodeHandle n){
	ros::ServiceClient client=n.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state");
	gazebo_msgs::GetModelState gms;
	string str = "jassi_da_model";
	// cin>>str;
	gms.request.model_name=str;
	gms.request.relative_entity_name="";
	client.call(gms);
	int curr_x,curr_y;
	curr_x=round(gms.response.pose.position.x);
	curr_y= round(gms.response.pose.position.y);

	cout<< "OBSTACLE " << str <<"is at "<<curr_x<<"," <<curr_y<<endl;
	return make_pair(curr_x,curr_y);
}
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
void move_goal(double x, double y,ros::NodeHandle n){
        double x_in,y_in,theta;
        ros::ServiceClient client=n.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state");
        gazebo_msgs::GetModelState gms;
        gms.request.model_name="jassi_da_model";
        gms.request.relative_entity_name="";        client.call(gms);
        x_in=gms.response.pose.position.x;
        y_in=gms.response.pose.position.y;
        theta=gms.response.pose.orientation.z;
        // cout<<x_in<<endl<<y_in<<endl<<theta;
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
        cout<<"DESTINATION REACHED"<< x <<" " <<y<<endl;
}
