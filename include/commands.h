#ifndef COMMANDS_H
#define COMMANDS_H

// flags for Control enable bitmask
#define X_ENABLED     0b001
#define Y_ENABLED     0b010
#define LASER_ENABLED 0b100

// command type indicators
#define CONTROL_CMD 0
#define CALIBRATE_CMD 1

typedef struct ControlCmd {
  char enabled;
  char laserOn;
  float x;
  float y;
} ControlCmd;

typedef struct Point { float x; float y; } Point;

typedef struct CalibrateCmd {
  Point trCorner;
  Point tlCorner;
  Point blCorner;
  Point brCorner;
} CalibrateCmd;

typedef struct Command {
  char type;
  
  union {
    ControlCmd control;
    CalibrateCmd calibrate;
  };
} Command;

void initControlCmd(ControlCmd*);
void setControlX(ControlCmd*, float);
void setControlY(ControlCmd*, float);
void setControlLaser(ControlCmd*, char);

void initCalibrateCmd(CalibrateCmd*);

void initCommand(Command*, char, void*);

void parseCommand(char*, Command*);

#endif