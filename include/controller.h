#ifndef CONTROLLER_H
#define CONTROLLER_H

typedef struct CalibrationPoints {
  float topLeft[2];
  float topRight[2];
  float bottomLeft[2];
  float bottomRight[2];
} CalibrationPoints;

void initController();

void setArmX(float x);
void setArmY(float y);
void setArmLaser(int laser);

void setArmControl(float x, float y, int laser);
void setArmPosition(float x, float y);

void calibrateAdjustmentParams(CalibrationPoints*);

#endif