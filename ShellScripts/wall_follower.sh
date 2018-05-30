#!/bin/bash
#source /opt/ros/kinetic/setup.bash
catkin_ws_root=/home/antti/Desktop/studying/udacity/term2/navigation/catkin_ws
world_file=${catkin_ws_root}/src/World/wall_world/wall_world.world



#
#roslaunch home_service turtlebot_world.launch world_file:=${world_file} 
xterm  -e "roslaunch home_service turtlebot_world.launch world_file:=${world_file}" & 
sleep 5
xterm  -e "roslaunch home_service gmapping.launch" & 
sleep 5
xterm  -e "roslaunch home_service view_navigation.launch" & 
sleep 5
xterm  -e "rosrun wall_follower wall_follower"

