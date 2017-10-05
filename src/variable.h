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

list<state> mypath;
Dstar *dstar = new Dstar();