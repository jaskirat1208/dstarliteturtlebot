##Compilation instructions


```bash
cd CATKIN_WORKSPACE/src/
git clone URL
cd ../..
catkin_make
```


To test its simulation, you first create a dynamic obstacle in gazebo and store its name in the echo command (line 8) in the file **run.sh**.
Now,
```bash
cd dstarliteturtlebot/src
bash main.sh
```
So far, only one obstacle has dealt with. Work is in progress to add more obstacles. 