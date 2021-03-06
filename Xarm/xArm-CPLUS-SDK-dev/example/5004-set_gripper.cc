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

    arm->motion_enable(true);
    arm->set_mode(0);
    arm->set_state(0);
    if (arm->error_code != 0) arm->clean_error();
    if (arm->warn_code != 0) arm->clean_warn();
    sleep_milliseconds(500);

    printf("=========================================\n");

    int ret;
    ret = arm->set_gripper_enable(true);
    printf("set_gripper_enable, ret=%d\n", ret);
    ret = arm->set_gripper_speed(5000);
    printf("set_gripper_speed, ret=%d\n", ret);


    while (arm->is_connected()) {
        ret = arm->set_gripper_position(600, false);
        printf("set_gripper_position, pos=600, ret=%d\n", ret);
        sleep_milliseconds(1000);
        if(ret == 22) {arm->clean_error(); sleep_milliseconds(500);};
        ret = arm->set_gripper_position(200, false);
        printf("set_gripper_position, pos=200, ret=%d\n", ret);
        sleep_milliseconds(1000);
        if(ret == 22) {arm->clean_error(); sleep_milliseconds(500);};
    }

    printf("gg\n");

    return 0;
}
