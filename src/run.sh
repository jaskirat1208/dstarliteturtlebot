# sed '1d' file.txt 
# cat file.txt
values=()
while IFS='=' read -r key value
do
    keys=("$key")
    values=("$value")
    echo "my_robot" >robot_model.txt
    rosrun project get_obstacle_position<robot_model.txt > obstacle_pose.txt
    echo "Hi $keys $values"
    # if door is closed and next vertex is (5,7) add last line followed by 5 7  and run rosrun project replanner > file.txt
    if grep -q "$keys $values" obstacle_pose.txt; then              
        echo "----------------------------------------------------------------- ";  #door is closed
        echo " Replanning required.Replanning: "    #door is closed
        echo "----------------------------------------------------------------- ";  #door is closed
        tail -n1 file.txt |  sed -e 's/=/ /g' > update.txt
        echo "$keys $values" >> update.txt
        rosrun project replanner < update.txt > file.txt
        rm update.txt        
        bash run.sh
        exit 1

    fi
   
    echo "$keys $values" > temp.txt
    rosrun project finder < temp.txt
    sleep 2
    rm  temp.txt 
done < "file.txt"
rm file.txt
# rm temp.txt
rm obstacle_pose.txt
# rm update.txt
rm robot_model.txt
