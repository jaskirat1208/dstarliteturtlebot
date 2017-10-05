#include "Dstar.h"
#include <ros/ros.h>
#include <gazebo_msgs/GetModelState.h>
#include <iostream>
#include <geometry_msgs/Twist.h>
#include "unistd.h"
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <ctime>
#include "variable.h"
// #include <iostream>
using namespace std;

extern list<state> mypath;
extern Dstar *dstar;

int main(int argc, char **argv)
{
	
	int start_x,start_y;


	ros::init(argc,argv,"finder");
    double x,y,x_in,y_in,theta;
    /*************************************************************************************/
                                            /*Initialisation of the ros node*/
    /*************************************************************************************/
    ros::NodeHandle n;
    ros::ServiceClient
	client=n.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state");
    gazebo_msgs::GetModelState gms;
    gms.request.model_name="mobile_base";
    gms.request.relative_entity_name="";
    client.call(gms);
    start_x=round(gms.response.pose.position.x);
    start_y=round(gms.response.pose.position.y);
	dstar->updateStart(start_x,start_y);
	cin>>start_x>>start_y;
	// dstar->updateGoal(start_x,start_y);
	// dstar->updateCell(3,4,-1);     // set cell (3,4) to be non traversable
	// dstar->updateCell(-1,1,-1);     // set cell (3,4) to be non traversable
	// dstar->updateCell(1,1,-1);     // set cell (3,4) to be non traversable
	// dstar->updateCell(-1,2,-1);     // set cell (3,4) to be non traversable
	// dstar->updateCell(1,2,-1);     // set cell (3,4) to be non traversable
	// dstar->updateCell(1,-1,-1);     // set cell (3,4) to be non traversable
	// dstar->updateCell(2,-1,-1);     // set cell (3,4) to be non traversable
	// dstar->updateCell(0,-4,-1);     // set cell (3,4) to be non traversable
	// dstar->updateCell(1,-4,-1);     // set cell (3,4) to be non traversable
	// dstar->updateCell(2,-4,-1);     // set cell (3,4) to be non traversable
	// dstar->updateCell(0,-3,-1);     // set cell (3,4) to be non traversable
	// dstar->updateCell(1,-3,-1);     // set cell (3,4) to be non traversable
	// dstar->updateCell(2,-3,-1);     // set cell (3,4) to be non traversable
	// dstar->updateCell(-2,-4,-1);     // set cell (3,4) to be non traversable
	// // dstar->updateCell(1,-4,-1);     // set cell (3,4) to be non traversable
	// dstar->updateCell(-2,-3,-1);     // set cell (3,4) to be non traversable
	// dstar->updateCell(-3,-2,-1);     // set cell (3,4) to be non traversable
	// dstar->updateCell(-3,-1,-1);     // set cell (3,4) to be non traversable
	// dstar->updateCell(-4,-1,-1);     // set cell (3,4) to be non traversable
	// dstar->updateCell(-5,0,-1);     // set cell (3,4) to be non traversable
	// dstar->updateCell(-6,0,-1);     // set cell (3,4) to be non traversable
	// dstar->updateCell(0,1,-1);	//(1,1 
	// dstar->updateCell(0,2,-1);	//(1,1 
	// dstar->updateCell(4,7,-1);	//(1,1 
	int obs_x,obs_y;
	cin>>obs_x>>obs_y;
	dstar->updateCell(obs_x,obs_y,-1);     // set cell (3,4) to be non traversable
	list<state>::iterator iter2;
	cout<<"INITIAL PATH "<<mypath.size()<<endl;
	for(iter2=mypath.begin(); iter2 != mypath.end(); iter2++) {
		cout<<iter2->x<<"="<<iter2->y<<endl;
	}
	dstar->replan();               // plan a path
	mypath = dstar->getPath();     // retrieve path
	cout<<"FINAL PATH"<<endl;
	for(iter2=mypath.begin(); iter2 != mypath.end(); iter2++) {
		cout<<iter2->x<<"="<<iter2->y<<endl;
	}

	return 0;
}
