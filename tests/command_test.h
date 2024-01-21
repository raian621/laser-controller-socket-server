#include <gtest/gtest.h>

#include "commands.h"

void loadControlCommandFrameData(float x, float y, char laser, char* data) {
  data[0] = CONTROL_CMD;
  data[1] = 'x';
  for (int i = 0; i < 4; i++) { data[i + 2] = (char)((int)x >> i & 0xff); }
  data[5] = 'y';
  for (int i = 0; i < 4; i++) { data[i + 6] = (char)((int)y >> i & 0xff); }
  data[9] = 's';
  data[13] = '\0';
}

TEST(CommandTests, ParseControlCommandFrameTest) {
  float x = 1.2f;
  float y = 5.2f;
  char laser = 0;
  char data[12];

  loadControlCommandFrameData(x, y, laser, data);

  Command cmd = { .type=0 };
  parseCommand(data, &cmd);
  
  ASSERT_EQ(cmd.type, CONTROL_CMD);
  ASSERT_FLOAT_EQ(cmd.control->x, x);
  ASSERT_FLOAT_EQ(cmd.control->x, x);
  ASSERT_FALSE(cmd.control->laserOn);
  ASSERT_EQ(cmd.control->enabled, X_ENABLED|Y_ENABLED|LASER_ENABLED);
}