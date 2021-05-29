/*
# Software License Agreement (MIT License)
#
# Copyright (c) 2020, UFACTORY, Inc.
# All rights reserved.
#
# Author: Vinman <vinman.wen@ufactory.cc> <vinman.cub@gmail.com>
*/

#include "xarm/wrapper/xarm_api.h"


int main(int argc, char **argv) {
	if (argc < 2) {
		printf("Please enter IP address\n");
		return 0;
	}
	std::string port(argv[1]);

	XArmAPI *arm = new XArmAPI(port);
	sleep_milliseconds(500);
	if (arm->error_code != 0) arm->clean_error();
	if (arm->warn_code != 0) arm->clean_warn();

	printf("=========================================\n");
	int ret;
	sleep_milliseconds(3000);

	for(int j = 0; j<5; j++){
		ret = arm->set_suction_cup(true, false);
		printf("open_suction_cup, ret=%d\n", ret);

		float *i;
		arm->get_gripper_position(i);
		printf("%f\n", i);

		sleep_milliseconds(1000);

		ret = arm->set_suction_cup(false, false);
		printf("close_suction_cup, ret=%d\n", ret);
		arm->get_gripper_position(i);
		printf("%f\n", i);
		sleep_milliseconds(1000);
	}
	arm->set_gripper_enable(false);
	printf("gg\n");

	sleep_milliseconds(3000);

	arm->set_gripper_enable(true);

	return 0;
}
