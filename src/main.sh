echo "Enter the coordinates where you want to go: "
rosrun dstarliteturtlebot planner > "file.txt"
tail -n1 file.txt |  sed -e 's/=/ /g' > update.txt
cat update.txt > final_dest.txt

keys=()
bash run.sh
