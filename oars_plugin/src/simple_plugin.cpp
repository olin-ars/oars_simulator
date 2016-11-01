#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/common/Plugin.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>

#include <stdio.h>

namespace gazebo
{
	//class WorldPluginTutorial : public WorldPlugin{
	//	public:
	//		WorldPluginTutorial() : WorldPlugin(){
	//		}
	//		void Load(physics::WorldPtr _world, sdf::ElementPtr _sdf){
	//			if(!ros::isInitialized){
	//				ROS_FATAL_STREAM(" ROS NOT INITIALIZED " << "Load the Gazebo system plugin 'libgazebo_ros_api_plugin.so' in the gazebo_ros package)");
	//				return;
	//			}

	//			ROS_INFO("Hello World!");
	//		}

	//};
	//GZ_REGISTER_WORLD_PLUGIN(WorldPluginTutorial)

	class BoatPlugin : public ModelPlugin{
		private:
			physics::ModelPtr model;
			event::ConnectionPtr updateConnection;
			ros::NodeHandle nh; // hold on to its own handle
			ros::Subscriber vel_sub;
			math::Vector3 lin_vel;
		public:
			void velCallback(const geometry_msgs::TwistConstPtr& p){
				lin_vel.x = p->linear.x;
				lin_vel.y = p->linear.y;
				lin_vel.z = p->linear.z;
			}

			void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf){
				nh = ros::NodeHandle("oars_plugin");
				vel_sub = nh.subscribe<geometry_msgs::Twist>("cmd_vel", 10, &BoatPlugin::velCallback, this);

				if(!ros::isInitialized()){

					ROS_FATAL_STREAM(" ROS NOT INITIALIZED " << "Load the Gazebo system plugin 'libgazebo_ros_api_plugin.so' in the gazebo_ros package)");
				}

				ROS_INFO("Is this getting called?");

				this->model = _parent;

				this->updateConnection = event::Events::ConnectWorldUpdateBegin(
						boost::bind(&BoatPlugin::OnUpdate, this, _1));

				ROS_INFO("Hello!! LOADED MODEL!");
			}

			void OnUpdate(const common::UpdateInfo& _info){
				ROS_INFO("updating model position...");
				//printf("??\n");					
				this->model->SetLinearVel(lin_vel);
			}
	};
	GZ_REGISTER_MODEL_PLUGIN(BoatPlugin)
};
