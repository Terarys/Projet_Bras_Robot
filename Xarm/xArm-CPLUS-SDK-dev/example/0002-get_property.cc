/*
# Software License Agreement (MIT License)
#
# Copyright (c) 2019, UFACTORY, Inc.
# All rights reserved.
#
# Author: Vinman <vinman.wen@ufactory.cc> <vinman.cub@gmail.com>
*/

#include "xarm/wrapper/xarm_api.h"
//#include <librealsense2/rs.hpp>
#include <iostream>

int main(int argc, char **argv) {

  //rs2::pipeline p;
  //p.start();

  if (argc < 2) {
		printf("Please enter IP address\n");
		return 0;
	}
	std::string port(argv[1]);

	XArmAPI *arm = new XArmAPI(port);
	arm->motion_enable(true);
	arm->set_mode(0);
	arm->set_state(0);
	sleep_milliseconds(1000);

	printf("=========================================\n");
	printf("default_is_radian: %d\n", arm->default_is_radian);
	printf("version: %s\n", arm->version);
	printf("state: %d\n", arm->state);
	printf("mode: %d\n", arm->mode);
	printf("cmd_num: %d\n", arm->cmd_num);
	printf("error_code: %d\n", arm->error_code);
	printf("warn_code: %d\n", arm->warn_code);
	printf("collision_sensitivity: %d\n", arm->collision_sensitivity);
	printf("teach_sensitivity: %d\n", arm->teach_sensitivity);
	print_nvect("world_offset: ", arm->world_offset, 6);
	print_nvect("gravity_direction: ", arm->gravity_direction, 3);

	printf("==========TCP==============\n");
	print_nvect("* position: ", arm->position, 6);
	printf("* tcp_jerk: %f\n", arm->tcp_jerk);
	print_nvect("* tcp_load: ", arm->tcp_load, 4);
	print_nvect("* tcp_offset: ", arm->tcp_offset, 6);
	print_nvect("* tcp_speed_limit:", arm->tcp_speed_limit, 2);
	print_nvect("* tcp_acc_limit:", arm->tcp_acc_limit, 2);

	printf("============JOINT==============\n");
	print_nvect("* angles: ", arm->angles, 7);
	printf("* joint_jerk: %f\n", arm->joint_jerk);
	print_nvect("* joint_speed_limit:", arm->joint_speed_limit, 2);
	print_nvect("* joint_acc_limit:", arm->joint_acc_limit, 2);
	print_nvect("* joints_torque:", arm->joints_torque, 7);

/*
	while (true)
    {
        // Block program until frames arrive
        rs2::frameset frames = p.wait_for_frames();

        // Try to get a frame of a depth image
        rs2::depth_frame depth = frames.get_depth_frame();

        // Get the depth frame's dimensions
        float width = depth.get_width();
        float height = depth.get_height();

        // Query the distance from the camera to the object in the center of the image
        float dist_to_center = depth.get_distance(width / 2, height / 2);

        // Print the distance
        std::cout << "The camera is facing an object " << dist_to_center << " meters away \r";
    }
*/
	return 0;


}
