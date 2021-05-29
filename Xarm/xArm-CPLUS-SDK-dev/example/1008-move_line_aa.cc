/*
# Software License Agreement (MIT License)
#
# Copyright (c) 2019, UFACTORY, Inc.
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
	arm->motion_enable(true);
	arm->set_mode(0);
	arm->set_state(0);
	sleep_milliseconds(500);

	printf("=========================================\n");

	int ret;
	arm->reset(true);

	fp32 firstPose[6] = { 207, 0, 112, 180, 0, 0 };
	ret = arm->set_position(firstPose, true);

	printf("arm = %d\n", ret);

	fp32 poses[6][6] = {
		{207, 0, 534, 180, 0, 0},
		{207, 0, 26, 180, 0, 0},
		{400, 0, 112, 180, 0, 0},
		{207, 0, 112, 180, 0, 0},
	};
	for (int i = 0; i < 4; i++) {
		ret = arm->set_position(poses[i], false, true, true);
		printf("set_position_aa, ret=%d\n", ret);
	}

	arm->reset(true);
	return 0;
}
