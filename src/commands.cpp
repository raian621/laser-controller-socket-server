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
}

float bytesToFloat(char* data) {
  // big endian
  return (float)(
    (int)data[1] << 0x18 |
    (int)data[2] << 0x10 |
    (int)data[3] << 0x08 |
    (int)data[4]
  );
} 

void parseControlCmd(char* data, ControlCmd* cmd) {
  for (int i = 1; data[i] != '\0'; i++) {
    switch (data[i]) {
      case 'x':
        setControlX(cmd, bytesToFloat(&data[i+1]));
        i += 4;
        break;
      case 'y':
        setControlY(cmd, bytesToFloat(&data[i+1]));
        i += 4;
        break;
      case 's':
        setControlLaser(cmd, data[i+1]);
        i += 1;
        break;
    }
  }
}

void parseCalibrateCmd(char* data, CalibrateCmd* cmd) {
  cmd->blCorner = { };
}

void parseCommand(char* data, Command* cmd) {
  // type data
  // - calibrate | x1 | y1 | x2 | y2 | x3 | y3 | x4 | y4
  // - control {
  //   x = _x
  //   y = _y
  //   l = _laser 
  // }
  switch (data[0]) {
    case CONTROL_CMD:
      cmd->type = CONTROL_CMD;
      parseControlCmd(data, &cmd->control);
      break;
    case CALIBRATE_CMD:
      cmd->type = CALIBRATE_CMD;
      parseCalibrateCmd(data, &cmd->calibrate);
      break;
  }
}