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
extern list<state> mypath;
using namespace std;
int main(int argc, char **argv)
{
	Dstar *dstar = new Dstar();
	int start_x,start_y,goal_x,goal_y;


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
    // vel_pub = n.advertise<geometry_msgs::Twist>("/mobile_base/commands/velocity",10);
    client.call(gms);
    start_x=round(gms.response.pose.position.x);
    start_y=round(gms.response.pose.position.y);

	// cin>>start_x>>start_y;
	// dstar->updateStart(start_x,start_y);
	cin>>goal_x>>goal_y;
	dstar->init(start_x,start_y,goal_x,goal_y);         // set start to (0,0) and goal to (10,5)
	// dstar->updateGoal(goal_x,goal_y);
  	// list<state> mypath;
	// dstar->updateCell(3,4,-1);     // set cell (3,4) to be non traversable
	// dstar->updateCell(1,4,-1);     // set cell (3,4) to be non traversable
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
	// dstar->updateCell(-2,-2,-1);     // set cell (3,4) to be non traversable
	
	// dstar->updateCell(-3,-2,-1);     // set cell (3,4) to be non traversable
	// dstar->updateCell(-3,-1,-1);     // set cell (3,4) to be non traversable
	// dstar->updateCell(-4,-1,-1);     // set cell (3,4) to be non traversable
	// dstar->updateCell(-4,0,-1);     // set cell (3,4) to be non traversable
	// // dstar->updateCell(-6,0,-1);     // set cell (3,4) to be non traversable
	// dstar->updateCell(-5,0,-1);     // set cell (3,4) to be non traversable
	// dstar->updateCell(-6,0,-1);     // set cell (3,4) to be non traversable
	// dstar->updateCell(4,7,-1);     // set cell (3,4) to be non traversable


	// dstar->updateCell(2,2,42.432); // set set (2,2) to have cost 42.432

	list<state>::iterator iter2;
	// dstar->replan();               // plan a path
	mypath = dstar->getPath();     // retrieve path
	// for(iter2=mypath.begin(); iter2 != mypath.end(); iter2++) {
	// 	cout<<"Path comprises of ("<<iter2->x<<","<<iter2->y<<")"<<endl;
	// }
				//
	// dstar->updateStart(0,0);      // move start to (10,2)
	// dstar->replan();               // plan a path
	// mypath = dstar->getPath();     // retrieve path

	// dstar->updateGoal(0,4);        // move goal to (0,1)
	dstar->updateCell(0,1,-1);	//(1,1 
	dstar->updateCell(0,2,-1);	//(1,1 

	dstar->replan();               // plan a path
	mypath = dstar->getPath();     // retrieve path
	// cout<<mypath.size()<<endl;
	for(iter2=mypath.begin(); iter2 != mypath.end(); iter2++) {
		//glVertex3f(iter2->x, iter2->y, 0.3);
		cout<<iter2->x<<"="<<iter2->y<<endl;
		// cout<<"Path comprises of ("<<iter2->x<<","<<iter2->y<<")"<<endl;
	}

	return 0;
}
