#!/bin/bash

if [ -z "$1" ]
  then
    echo "Please, give the absolute path to catkin_ws folder as an argument."
    exit 1
fi

catkin_ws_root=$1
# catkin_ws_root=/home/antti/Desktop/studying/udacity/term2/navigation/catkin_ws
world_file=${catkin_ws_root}/src/World/wall_world/wall_world.world
map_file=${catkin_ws_root}/src/World/wall_world/wall_world.yaml
cd ${catkin_ws_root}
source devel/setup.bash

#
#roslaunch home_service turtlebot_world.launch world_file:=${world_file} 
xterm  -e "roslaunch home_service turtlebot_world.launch world_file:=${world_file}" & 
sleep 5
xterm  -e "roslaunch home_service amcl.launch map_file:=${map_file}" &
sleep 5
xterm  -e "roslaunch home_service view_navigation.launch"
