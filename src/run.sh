cat file.txt > temp_file.txt
# cat file.txt
# sed '1d' temp_file.txt
tail -n +2 temp_file.txt > headerless.txt
cat headerless.txt>temp_file.txt 

values=()
head -n -1 "file.txt"| while IFS='=' read -r key value
do
    keys=("$key")
    values=("$value")
    IFS='=' read -r next_x next_y < temp_file.txt
    # echo "$next_x $next_y" >> temporary.txt
    tail -n +2 temp_file.txt > headerless.txt
    cat headerless.txt>temp_file.txt 
    echo "jassi_da_model" >robot_model.txt
    rosrun dstarliteturtlebot get_obstacle_position<robot_model.txt > obstacle_pose.txt
    echo "GOING TO $keys $values and then to $next_x $next_y"
    echo "$keys"
    
    if [ $"next_x" == "" ]; then
        echo "NOTHING in here"
    fi
    # if door is closed and next vertex is (5,7) add last line followed by 5 7  and run rosrun project replanner > file.txt
    if  grep -q "$keys $values" obstacle_pose.txt  || grep -q "$next_x $next_y" obstacle_pose.txt || grep -q "\\$keys \\$values" obstacle_pose.txt  || grep -q "\\$next_x \\$next_y" obstacle_pose.txt  ; then              
        # echo "$keys $values"
        cat obstacle_pose.txt
        echo ""
        echo "----------------------------------------------------------------- ";  #door is closed
        echo " Replanning required.Replanning: "    #door is closed
        echo "----------------------------------------------------------------- ";  #door is closed
        tail -n1 file.txt |  sed -e 's/=/ /g' > update.txt
        echo "GREP 1"
        if  grep -q "$keys $values" obstacle_pose.txt  || grep -q "\\$keys \\$values" obstacle_pose.txt  ; then        
            echo "$keys $values" >> update.txt
            echo "NEXT" 
        fi
        echo "GREP 2"
        echo "$next_x $next_y" 
        if $(cmp --silent obstacle_pose.txt final_dest.txt ) ; then             
            echo "FAILURE: THE OBSTACLE IS AT THE DESTINATION. SORRY, CAN'T BE REACHED. Waiting for the obstacle to get out of there."
            while $(cmp --silent obstacle_pose.txt final_dest.txt ); do
                true
                rosrun dstarliteturtlebot get_obstacle_position<robot_model.txt > obstacle_pose.txt
            done
            echo " THE OBSTACLE is gone. So now we can PROCEED"
        fi
        if  (grep -q "$next_x $next_y" obstacle_pose.txt ||  grep -q "\\$next_x \\$next_y" obstacle_pose.txt); then
            echo "$next_x $next_y">>update.txt
            # echo "$next_x $next_y"
            # echo "NEXT TO NEXT"
        fi
        echo "CATTING UPDATE.TXT"
        cat update.txt
        rosrun dstarliteturtlebot replanner < update.txt > file.txt
        rm update.txt        
        bash run.sh
        exit 1

    fi
   
    echo "$keys $values" > temp.txt
    cat temp.txt
    rosrun dstarliteturtlebot finder < temp.txt

    sleep 2
    rm  temp.txt 
done 
rosrun dstarliteturtlebot get_obstacle_position<robot_model.txt > obstacle_pose.txt
 if $(cmp --silent obstacle_pose.txt final_dest.txt ) ; then             
            echo "FAILURE: THE OBSTACLE IS AT THE DESTINATION. SORRY, CAN'T BE REACHED. Waiting for the obstacle to get out of there."
            while $(cmp --silent obstacle_pose.txt final_dest.txt ); do
                true
                rosrun dstarliteturtlebot get_obstacle_position<robot_model.txt > obstacle_pose.txt
            done
            echo " THE OBSTACLE is gone. So now we can PROCEED"
        fi          
echo "SUCCESS. HERE WE PROCEED TO THE FINAL DESTINATION"
rosrun dstarliteturtlebot finder < final_dest.txt
rm file.txt
# echo "$next_x $next_y" >temp_file.txt
# rosrun dstarliteturtlebot finder < temp_file.txt
# rm temp_file.txt
# rm temp.txt
rm obstacle_pose.txt
# rm update.txt
# rm robot_model.txt
