#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/Pose.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <nav_msgs/Odometry.h>
// Define a client for to send goal requests to the move_base server through a SimpleActionClient

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
using namespace std;
struct Vec3
{
    float x;
    float y;
    float z;
};

class HomeService
{
public:


    HomeService ()
    {
        sub_odom_ = nh_.subscribe("/odom", 1, &HomeService::cb_odom, this);
        marker_pub_ = nh_.advertise<visualization_msgs::Marker>("visualization_marker", 1);
        pick_up_loc_.x = 4.4;
        pick_up_loc_.y = 4.0;
        pick_up_loc_.z = 0.0;
        drop_off_loc_.x = -3.5f;
        drop_off_loc_.y = 5;
        drop_off_loc_.z = 5;
        ROS_INFO("Initialization done!");
    }

    ~HomeService(){}

    void cb_odom(const nav_msgs::Odometry::ConstPtr& msg)
    {
        pose_lock_.lock();
        robot_pose_ = msg->pose.pose.position;
        pose_lock_.unlock();
    }

    visualization_msgs::Marker create_cube_marker(Vec3 pt)
    {
        uint32_t shape = visualization_msgs::Marker::CUBE;
        visualization_msgs::Marker marker;
        marker.header.frame_id = "/map";
        marker.header.stamp = ros::Time::now();
        marker.ns = "basic_shapes";
        marker.id = 0;
        marker.type = shape;
        marker.action = visualization_msgs::Marker::ADD;

        marker.pose.position.x = pt.x;
        marker.pose.position.y = pt.y;
        marker.pose.position.z = pt.z;
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
        return marker;
    }



    bool run()
    {
        visualization_msgs::Marker pick_up_marker = create_cube_marker(pick_up_loc_);
        pick_up_marker.lifetime = ros::Duration(60*60);
        while (marker_pub_.getNumSubscribers() < 1)
        {
            if (!ros::ok())
                break;
            ROS_WARN_ONCE("Please create a subscriber to the marker");
            ros::Duration(0.5).sleep();
        }
        marker_pub_.publish(pick_up_marker);
        ros::Time starting_time = ros::Time::now();
        ros::Rate r(20);
        while (ros::ok()) {
            pose_lock_.lock();
            geometry_msgs::Point robot_pose = robot_pose_;
            pose_lock_.unlock();
            /// check if pick up pose reached
            double dist = sqrt((pick_up_loc_.x - robot_pose.x) +
                               (pick_up_loc_.y - robot_pose.y) +
                               (pick_up_loc_.z - robot_pose.z));
            ROS_INFO("Current distance between goal and robot: %f", dist);
            if ( dist < 0.6 ) {
                ROS_INFO("Goal reached!");
                ros::Duration(1).sleep();
                pick_up_marker.action = visualization_msgs::Marker::DELETE;
                marker_pub_.publish(pick_up_marker);
                return true;
            }

            if (ros::Time::now().toSec() - starting_time.toSec() > 40 ){
                ROS_INFO("40 seconds passed, goal not reached!");
                break;
            }
            ros::spinOnce();
            r.sleep();
        }
        return false;
    }




private:
    ros::NodeHandle nh_;
    ros::Subscriber sub_odom_;
    ros::Publisher marker_pub_;
    geometry_msgs::Point robot_pose_;
    // MoveBaseClient ac_;
    Vec3 pick_up_loc_;
    Vec3 drop_off_loc_;
    boost::mutex pose_lock_;



};











int main(int argc, char** argv){

    // Initialize the simple_navigation_goals node
    ros::init(argc, argv, "add_markers_node");
    HomeService hs;
    if ( hs.run() ){
        ROS_INFO("Object picked, hiding marker!");
    } else {
        ROS_INFO("Mission failed");
    }
    return 0;
}
