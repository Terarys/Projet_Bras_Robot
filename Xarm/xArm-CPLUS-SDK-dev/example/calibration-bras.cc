#include "xarm/wrapper/xarm_api.h"
#include <cstdlib>
#include <iostream>
#include <librealsense2/rs.hpp>

float RATIO_y = 485.0 / 480.0;
float RATIO_x = 587.5 / 640.0;

int main(int argc, char **argv) {

  if (argc < 9) {
      printf("No enought arguments\n");
      return 0;
  }

  int posX = atoi(argv[1]);
  int posY = atoi(argv[2]);
  int calibration = atoi(argv[3]);
  int descente = atoi(argv[4]);
  int depot = atoi(argv[5]);
  int offsetZ = atoi(argv[6]);
  printf("\n");
  printf("\n");

  //----------------------------
  // Initialisation de la caméra
  //----------------------------
  rs2::pipeline p;
  rs2::config cfg;
  cfg.enable_stream(RS2_STREAM_DEPTH, 1280, 720, RS2_FORMAT_Z16, 30);
  p.start(cfg);

  // Camera warmup - dropping several first frames to let auto-exposure stabilize
  rs2::frameset frames;
  for(int i = 0; i < 30; i++)
  {
      //Wait for all configured streams to produce a frame
      frames = p.wait_for_frames();
  }

  float dist_sum = 0;
  int n_frames = 0;
  int n_frames_tot = 5;
  while(n_frames < n_frames_tot){
    frames = p.wait_for_frames();
    rs2::depth_frame depth = frames.get_depth_frame();
    float tmp = depth.get_distance(posX, posY);
    if(tmp != 0){
      dist_sum += tmp;
      n_frames++;
    }
  }

  float dist_tmp = dist_sum/n_frames_tot;
  //------------------------------------------------
  // Déplacement du bras pour s'approcher de l'objet
  //------------------------------------------------
  printf("Distance à l'objet : %f \n", dist_tmp);
  p.stop();

  posX -= 320;
  posY -= 120;

  //52.4cm = 480 pixs


  if(calibration || descente || depot){
    //-----------------------
    // Initialisaiton du bras
    //-----------------------

    char IP_bras[] = "192.168.1.208";
    std::string port(IP_bras);
    XArmAPI *arm = new XArmAPI(IP_bras);
    sleep_milliseconds(500);
    arm->motion_enable(true);
    arm->set_mode(0);
    arm->set_state(0);
    if (arm->error_code != 0) arm->clean_error();
    if (arm->warn_code != 0) arm->clean_warn();
    sleep_milliseconds(500);
    arm->set_gripper_enable(true);
    arm->set_gripper_speed(5000);

    fp32 position[6];
    arm->get_position(position);

    if(calibration && !descente && !depot)
      {
        printf("calibration\n");
	    fp32 offsetX = (posX-320)*RATIO_y;
	    fp32 offsetY = (posY-240)*RATIO_x;

	    position[0] += offsetY;
	    position[1] += offsetX;

	    position[0] -= 140;
	    position[1] -= 40;


	    printf("posX : %i ; posY : %i\n", posX, posY);
	    printf("RATIO_x : %f; RATIO_y : %f\n", RATIO_x, RATIO_y);
	    printf("dX : %f, dY : %f\n", offsetX, offsetY);
	    printf("X : %f, Y : %f\n", position[0], position[1]);
	    arm->set_position(position,true);
      }
    else if(!calibration && descente && !depot)
      {
        printf("descente\n");
      	//int offsetZ = dist_tmp*1000 - 115 + 20;
      	offsetZ = dist_tmp*1000 - 115 + 15;
      	fp32 angles[] = {180, -36.6, -69.6, 0, 106.1, 0, 0};
      	arm->set_gripper_position(850, false);
      	position[2] -= offsetZ;
      	printf("OffsetZ = %d\n", offsetZ);
      	arm->set_position(position, true);
      	arm->set_gripper_position(10, false);
      	sleep_milliseconds(1000);
      	arm->set_servo_angle(angles, true);
      }
    else if(!calibration && !descente && depot)
      {
        printf("depot\n");
      	fp32 angles[] = {180, -36.6, -69.6, 0, 106.1, 0, 0};
      	position[2] -= offsetZ;
      	position[2] += 10;
      	arm->set_position(position, true);
      	arm->set_gripper_position(850, false);
      	sleep_milliseconds(1000);
      	arm->set_servo_angle(angles, true);
      	offsetZ = 0;
      }
  }
  std::cerr << offsetZ;
  return offsetZ;
}
