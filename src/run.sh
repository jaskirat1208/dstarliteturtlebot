<<<<<<< HEAD
cat file.txt > temp_file.txt
# sed '1d' temp_file.txt
tail -n +2 temp_file.txt > headerless.txt
cat headerless.txt>temp_file.txt 
=======
# sed '1d' file.txt 
# cat file.txt
>>>>>>> 4ae941293b3552c0360181564f1f71f43c992362
values=()
while IFS='=' read -r key value
do
    keys=("$key")
    values=("$value")
<<<<<<< HEAD
    IFS='=' read -r next_x next_y < temp_file.txt
    echo "$next_x $next_y" >> temporary.txt
    tail -n +2 temp_file.txt > headerless.txt
    cat headerless.txt>temp_file.txt 
    echo "my_robot" >robot_model.txt
    rosrun dstarliteturtlebot get_obstacle_position<robot_model.txt > obstacle_pose.txt
    echo "GOING TO $keys $values and then to $next_x $next_y "
    # if door is closed and next vertex is (5,7) add last line followed by 5 7  and run rosrun project replanner > file.txt
    if grep -q "$keys $values" obstacle_pose.txt || grep -q "$keys $values" obstacle_pose.txt  ; then              
=======
    echo "my_robot" >robot_model.txt
    rosrun project get_obstacle_position<robot_model.txt > obstacle_pose.txt
    echo "Hi $keys $values"
    # if door is closed and next vertex is (5,7) add last line followed by 5 7  and run rosrun project replanner > file.txt
    if grep -q "$keys $values" obstacle_pose.txt; then              
>>>>>>> 4ae941293b3552c0360181564f1f71f43c992362
        echo "----------------------------------------------------------------- ";  #door is closed
        echo " Replanning required.Replanning: "    #door is closed
        echo "----------------------------------------------------------------- ";  #door is closed
        tail -n1 file.txt |  sed -e 's/=/ /g' > update.txt
        echo "$keys $values" >> update.txt
<<<<<<< HEAD
        rosrun dstarliteturtlebot replanner < update.txt > file.txt
=======
        rosrun project replanner < update.txt > file.txt
>>>>>>> 4ae941293b3552c0360181564f1f71f43c992362
        rm update.txt        
        bash run.sh
        exit 1

    fi
   
    echo "$keys $values" > temp.txt
<<<<<<< HEAD
    rosrun dstarliteturtlebot finder < temp.txt
=======
    rosrun project finder < temp.txt
>>>>>>> 4ae941293b3552c0360181564f1f71f43c992362
    sleep 2
    rm  temp.txt 
done < "file.txt"
rm file.txt
# rm temp.txt
rm obstacle_pose.txt
# rm update.txt
<<<<<<< HEAD
rm robot_model.txt
=======
rm robot_model.txt
>>>>>>> 4ae941293b3552c0360181564f1f71f43c992362