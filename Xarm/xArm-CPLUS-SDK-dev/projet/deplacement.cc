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

	fp32 poses[30][6] = {
		{259.3, -149.7, 759.6, 179.6, 0.9, -53},
		{259.3, -149.7, -2.2, 179.6, 0.9, -53},
		{259.3, -149.7, 759.6, 179.6, 0.9, -53},
		{300.5, -173.5, 839.1, -159.8, -39.2, -60.3},
		{272.1, -157.1, 942.6, -75.8, -66.4, -133},
		{210.3, -121.4, 979.4, -22.3, -41.8, -178.4},
		{141.9, -82, 964.5, -3.3, -7.6, -173.2},
		{163.7, 8.7, 964.5, -3.3, -7.6, -153.8},
		{163.7, 8.7, 964.5, -3.3, -7.6, -153.8},
		{112.8, 118.9, 964.5, -3.3, -7.6, -110.3},
		{-63.3, 151.2, 964.5, -3.3, -7.6, -44.1},
		{-144.8, 76.9, 964.5, -3.3, -7.6, -4.8},
		{-262.1, 139.2, 957.7, -53.1, -62, 28.5},
		{-276.6, 151.5, 773.8, -177.3, -8.8, 126.4},
		{-271.2, 148.4, 175.3, -179, -5, 126.9},
		{-271.2, 148.4, 276.8, -179, -5, 126.9},
	};
	for (int i = 0; i < 16; i++) {
		ret = arm->set_position(poses[i], false, false, true);
		printf("set_position_aa, ret=%d\n", ret);
	}

	arm->reset(true);
	return 0;
}
