#include "xarm/wrapper/xarm_api.h"

int main(int argc, char **argv) {
  char IP_bras[] = "192.168.1.208";
  std::string port(IP_bras);

  XArmAPI *arm = new XArmAPI(port);
  sleep_milliseconds(500);
  if (arm->error_code != 0) arm->clean_error();
  if (arm->warn_code != 0) arm->clean_warn();
  arm->motion_enable(true);
  arm->set_mode(0);
  arm->set_state(0);
  sleep_milliseconds(500);
  arm->set_gripper_enable(true);
  arm->set_gripper_speed(5000);

  printf("=========================================\n");

  int ret;
  arm->reset(true);
  fp32 poses[6] = {207, 0, 600, 180, 0, 0};
  fp32 angles[] = {180, -36.6, -69.6, 0, 106.1, 0, 0};

  arm->set_gripper_position(10, false);
  ret = arm->set_position(poses, true);
  printf("set_position, ret=%d\n", ret);
  ret = arm->set_servo_angle(angles, true);
  printf("set_position, ret=%d\n", ret);
  return 0;
}
