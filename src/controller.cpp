#ifndef MOCK_PIGPIO
#include <pigpio.h>
#endif
#include <stdio.h>
#include <math.h>

#include "controller.h"

#define X_SERVO_GPIO_PIN 12
#define Y_SERVO_GPIO_PIN 13
#define LASER_GPIO_PIN 0

#define X_DIMENSION 0
#define Y_DIMEMSION 1

/**
 * Angle adjustment parameters:
 * 
 * Gamma parameters adjust the 1D origins of the rotational axes,
 * Beta parameters adjust the range of rotation
*/
static float betaX = 0.0f;
static float betaY = 0.0f;
static float gammaX = 0.0f;
static float gammaY = 0.0f;

float adjustAngle(float, float, float);
void calibrateAdjustmentParams(CalibrationPoints*);

void initController() {
  #ifndef MOCK_PIGPIO
  if (gpioInitialise() < 0) {
    perror("pigpio failed to initialize");
  }
  gpioSetMode(X_SERVO_GPIO_PIN, PI_OUTPUT);
  gpioSetMode(Y_SERVO_GPIO_PIN, PI_OUTPUT);
  gpioSetMode(LASER_GPIO_PIN, PI_OUTPUT);
  #else
  printf("initializing pigpio\n");
  #endif
}

void setArmX(float x) {
  x = adjustAngle(x, gammaX, betaX);
  #ifndef MOCK_PIGPIO
  #else
  printf("set x=%0.3f\n", x);
  #endif
}

void setArmY(float y) {
  y = adjustAngle(y, gammaY, betaY);
  #ifndef MOCK_PIGPIO
  
  #else
  printf("set y=%0.3f\n", y);
  #endif
}

void setArmLaser(int laser) {
  #ifndef MOCK_PIGPIO
  int value = (laser > 0 ? 1 : 0);
  gpioWrite(LASER_GPIO_PIN, value);
  #else
  printf("set laser=%d\n", laser);
  #endif
}

void setArmControl(float x, float y, int laser) {
  setArmX(x);
  setArmY(y);
  setArmLaser(laser); 
}

void setArmPosition(float x, float y) {
  setArmY(x);
  setArmX(y);
}

float adjustAngle(float n, float c, float b) {
  return c * acos(n-b);
}

void calibrateAdjustmentParams(CalibrationPoints* points) {
  betaX = (
    points->topLeft[0] + 
    points->topRight[0] + 
    points->bottomLeft[0] + 
    points->bottomRight[0]
  ) / 4;
  betaY = (
    points->topLeft[1] + 
    points->topRight[1] + 
    points->bottomLeft[1] + 
    points->bottomRight[1]
  ) / 4;

  gammaX = (
    points->topLeft[0] - points->topRight[0] + 
    points->bottomLeft[0] - points->bottomRight[0]
  ) / 2;
  if (gammaX < 0) gammaX = -gammaX;

  gammaY = (
    points->topLeft[1] - points->bottomLeft[1] + 
    points->topRight[1] - points->bottomRight[1]
  ) / 2;
  if (gammaY < 0) gammaY = gammaY;
}