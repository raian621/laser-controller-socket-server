#include "commands.h"

void initControlCmd(ControlCmd* cmd) {
  cmd->enabled = 0;
  cmd->laserOn = 0;
  cmd->x = 0.0f;
  cmd->y = 0.0f;
}

void setControlX(ControlCmd* cmd, float x) {
  cmd->x = x;
  cmd->enabled |= X_ENABLED;
}

void setControlY(ControlCmd* cmd, float y) {
  cmd->y = y;
  cmd->enabled |= Y_ENABLED;
}

void setControlLaser(ControlCmd* cmd, char laserOn) {
  cmd->laserOn = laserOn;
  cmd->enabled |= LASER_ENABLED;
}

void initCalibrateCmd(CalibrateCmd* cmd) {
  for (int i = 0; i < 4; i++) {
    ((Point*)cmd)[1].x = 0.0f;
    ((Point*)cmd)[1].y = 0.0f;
  }
}

void initCommand(Command* cmd, char type, void* c) {
  cmd->type = type;
  cmd->calibrate = (CalibrateCmd*)c;
}

void parseCommand(char* data, Command* cmd) {
  // type data
  // - calibrate | x1 | y1 | x2 | y2 | x3 | y3 | x4 | y4
  // - control {
  //   x = _x
  //   y = _y
  //   l = _laser 
  // }
  
}