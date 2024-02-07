#ifndef COMMANDS_H
#define COMMANDS_H

// flags for Control enable bitmask
#define X_ENABLED     0b001
#define Y_ENABLED     0b010
#define LASER_ENABLED 0b100

typedef enum CommandType {
  CONTROL_CMD,
  CALIBRATE_CMD
} CommandType;

typedef struct ControlCmd {
  char enabled;
  char laserOn;
  float x;
  float y;
} ControlCmd;

typedef struct Point { float x; float y; } Point;

typedef struct CalibrateCmd {
  Point topLeft;
  Point topRight;
  Point bottomLeft;
  Point bottomRight;
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
void initCommand(Command*bottomLeft, char, void*);
void parseCommand(char*, Command*);
float bytesToFloat(unsigned char* data);

#endif