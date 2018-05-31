#include <ros/ros.h>
#include <visualization_msgs/Marker.h>

int main( int argc, char** argv )
{
  ros::init(argc, argv, "basic_shapes");
  ros::NodeHandle n; 	
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
	 ros::Rate r(0.2);
	// Set our initial shape type to be a cube
	uint32_t shape = visualization_msgs::Marker::CUBE;
	visualization_msgs::Marker marker;
	marker.header.frame_id = "/map";
	marker.header.stamp = ros::Time::now();
	marker.ns = "basic_shapes";
	marker.id = 0;
	marker.type = shape;
	marker.action = visualization_msgs::Marker::ADD;

	// pick up location
	marker.pose.position.x = 1.0;
	marker.pose.position.y = 0.0;
	marker.pose.position.z = 0.0;
	marker.pose.orientation.x = 0.0;
	marker.pose.orientation.y = 0.0;
	marker.pose.orientation.z = 0.0;
	marker.pose.orientation.w = 1.0;

	// Set the scale of the marker -- 1x1x1 here means 1m on a side
	marker.scale.x = 0.5;
	marker.scale.y = 0.5;
	marker.scale.z = 0.5;

	// Set the color -- be sure to set alpha to something non-zero!
	marker.color.r = 0.0f;
	marker.color.g = 1.0f;
	marker.color.b = 0.0f;
	marker.color.a = 1.0;
	marker.lifetime = ros::Duration(5.0);
  while (marker_pub.getNumSubscribers() < 1)
  {
      if (!ros::ok())
      {
        return 0;
      }
      ROS_WARN_ONCE("Please create a subscriber to the marker");
      r.sleep();
  }



	marker_pub.publish(marker);
	// sleep 10 seconds, marker should be shown 5 seconds		
	ros::Duration(10).sleep();
	

	// drop off place
	marker.id = 1;
	marker.header.stamp = ros::Time::now();
	marker.pose.position.x = -3.5;
	marker.pose.position.y = 5.0;
	marker.pose.position.z = 0.0;
	marker.lifetime = ros::Duration(5.0);
	marker_pub.publish(marker);
	ros::Duration(5).sleep();
}
