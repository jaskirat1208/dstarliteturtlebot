cat file.txt > temp_file.txt
# sed '1d' temp_file.txt
tail -n +2 temp_file.txt > headerless.txt
cat headerless.txt>temp_file.txt 
values=()
while IFS='=' read -r key value
do
    keys=("$key")
    values=("$value")
    IFS='=' read -r next_x next_y < temp_file.txt
    echo "$next_x $next_y" >> temporary.txt
    tail -n +2 temp_file.txt > headerless.txt
    cat headerless.txt>temp_file.txt 
    echo "my_robot" >robot_model.txt
    rosrun dstarliteturtlebot get_obstacle_position<robot_model.txt > obstacle_pose.txt
    echo "GOING TO $keys $values and then to $next_x $next_y "
    # if door is closed and next vertex is (5,7) add last line followed by 5 7  and run rosrun project replanner > file.txt
    if grep -q "$keys $values" obstacle_pose.txt || grep -q "$keys $values" obstacle_pose.txt  ; then              
        echo "----------------------------------------------------------------- ";  #door is closed
        echo " Replanning required.Replanning: "    #door is closed
        echo "----------------------------------------------------------------- ";  #door is closed
        tail -n1 file.txt |  sed -e 's/=/ /g' > update.txt
        echo "$keys $values" >> update.txt
        rosrun dstarliteturtlebot replanner < update.txt > file.txt
        rm update.txt        
        bash run.sh
        exit 1

    fi
   
    echo "$keys $values" > temp.txt
    rosrun dstarliteturtlebot finder < temp.txt
    sleep 2
    rm  temp.txt 
done < "file.txt"
rm file.txt
# rm temp.txt
rm obstacle_pose.txt
# rm update.txt
rm robot_model.txt