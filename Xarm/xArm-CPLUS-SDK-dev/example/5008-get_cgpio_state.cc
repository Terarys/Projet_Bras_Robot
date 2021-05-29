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
    int state[2];
    int digit_io[4];
    float analog[4];
    int input_conf[8];
    int output_conf[8];
    ret = arm->get_cgpio_state(state, digit_io, analog, input_conf, output_conf);
    printf("get_cgpio_state, ret=%d\n", ret);
    printf("* state=%d, err_code: %d\n", state[0], state[1]);
    printf("* digital input functional gpio state=%d\n", digit_io[0]);
    printf("* digital input configuring gpio state=%d\n", digit_io[1]);
    printf("* digital output functional gpio state=%d\n", digit_io[2]);
    printf("* digital output configuring gpio state=%d\n", digit_io[3]);
    printf("* analog-0 input val=%f\n", analog[0]);
    printf("* analog-1 input val=%f\n", analog[1]);
    printf("* analog-0 output val=%f\n", analog[2]);
    printf("* analog-1 output val=%f\n", analog[3]);
    printf("* digital input functional info");
    for (int i = 0; i< 8; ++i) {
        printf(", io%d=%d", i, input_conf[i]);
    }
    printf("\n");
    printf("* digital output functional info");
    for (int i = 0; i< 8; ++i) {
        printf(", io%d=%d", i, output_conf[i]);
    }
    printf("\n");

    return 0;
}