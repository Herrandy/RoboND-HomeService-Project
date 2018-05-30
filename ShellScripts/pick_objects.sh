#!/bin/bash
catkin_ws_root=/home/antti/Desktop/studying/udacity/term2/navigation/catkin_ws
world_file=${catkin_ws_root}/src/World/wall_world/wall_world.world
map_file=${catkin_ws_root}/src/World/wall_world/wall_world.yaml


#
#roslaunch home_service turtlebot_world.launch world_file:=${world_file} 
xterm  -e "roslaunch home_service turtlebot_world.launch world_file:=${world_file}" & 
sleep 8
xterm  -e "roslaunch home_service amcl.launch map_file:=${map_file}" &
sleep 8
xterm  -e "roslaunch home_service view_navigation.launch" &
sleep 8
xterm  -e "rosrun pick_objects pick_objects"
