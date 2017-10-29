#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;
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
void create_output_file(pair<int,int> p1,pair<int,int> p2){
	ofstream f("update.txt");
	f<<p2.first<<" "<<p2.second<<endl<<p1.first<<" "<<p1.second;

} 
int main(int argc, char const *argv[])
{
	system("rosrun dstarliteturtlebot planner > \"file.txt\"");
control:
	std::vector< pair<int,int> > v1 = getpose();
	int size=v1.size();
	for (int i = 0; i < v1.size(); ++i)
	{
		system("rosrun dstarliteturtlebot get_obstacle_position<robot_model.txt > obstacle_pose.txt");
		fstream obstacle_pose("obstacle_pose.txt");
		int obs_x,obs_y;
		obstacle_pose>>obs_x>>obs_y;
		pair<int,int> obstacle_position = make_pair(obs_x,obs_y);
		// cout<<"OBSTACLE AT "<<obs_x<<" "<<obs_y<<endl<<"SIZE= "<<size;
		if (i==size-1)
		{
			//check if robot is at the destination,
			// cout<<"OBSTACLE AT "<<obs_x<<" "<<obs_y<<endl;
			if (obstacle_position==v1[i])
			{
				cout<<"CAN'T MOVE AHEAD due to OBSTACLE HINDRANCE"<<endl;
				while(1){
					system("rosrun dstarliteturtlebot get_obstacle_position<robot_model.txt > obstacle_pose.txt");
					fstream obstacle_pose("obstacle_pose.txt");
					obstacle_pose>>obs_x>>obs_y;
					obstacle_position = make_pair(obs_x,obs_y);
					cout<<"OBSTACLE AT "<<obs_x<<" "<<obs_y<<endl;
					if (obstacle_position!=v1[i])
					{
						cout<<"HERE WE GO"<<endl;
						break;
					}
				}
			}
		}
		if (i==size-2)
		{
			if (obstacle_position==v1[i+1])
			{
				while(1){
					system("rosrun dstarliteturtlebot get_obstacle_position<robot_model.txt > obstacle_pose.txt");
					obstacle_pose>>obs_x>>obs_y;
					obstacle_position = make_pair(obs_x,obs_y);
					if (obstacle_position!=v1[i+1])
					{
						cout<<"HERE WE GO"<<endl;
						break;
					}
				}	
			}
			else if (obstacle_position == v1[i])
			{
				cout<<"............................................................................................................\nREPLANNING REQUIRED.....REPLANNING\n............................................................................................................"<<endl;
				create_output_file(obstacle_position,v1[v1.size()-1]);
				system("rosrun dstarliteturtlebot replanner < update.txt > file.txt");
				//now start over
				goto control;	
			}
		}
		if (obstacle_position==v1[i+1] || obstacle_position==v1[i+2])
		{
			for (int j = 0; j < v1.size(); ++j)
			{
				cout<<v1[j].first<<" "<<v1[j].second<<endl;
			}
			cout<<"............................................................................................................\nREPLANNING REQUIRED.....REPLANNING\n............................................................................................................"<<endl;
			create_output_file(obstacle_position,v1[v1.size()-1]);
			system("rosrun dstarliteturtlebot replanner < update.txt > file.txt");
			//now start over
			goto control;
			// v1=getpose();
			// for (int i = 0; i < v1.size(); ++i)
			// {
			// 	cout<<v1[i].first<<" "<<v1[i].second<<endl;
			// }

		}
		ofstream temp("temp.txt");
		temp<<v1[i].first<<" "<<v1[i].second<<endl;
		cout<<v1[i].first<<" "<<v1[i].second<<endl;
		system("rosrun dstarliteturtlebot finder < temp.txt");
		system("rm temp.txt");
	}
	return 0;
}