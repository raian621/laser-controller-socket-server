#include <gtest/gtest.h>

#include "commands.h"

void loadControlCommandFrameData(float x, float y, char laser, char* data) {
  data[0] = CONTROL_CMD;
  data[1] = 'x';
  for (int i = 0; i < 4; i++) { data[i + 2] = (char)(*(int*)&x >> ((3-i)*8) & 0xff); }
  data[6] = 'y';
  for (int i = 0; i < 4; i++) { data[i + 7] = (char)(*(int*)&y >> ((3-i)*8) & 0xff); }
  data[11] = 's';
  data[12] = laser;
  data[13] = '\0';

  printf("%s\n", data);
}

TEST(CommandTests, ParseControlCommandFrameTest) {
  float x = 1.2f;
  float y = 5.2f;
  char laser = 0;
  char data[14];

  loadControlCommandFrameData(x, y, laser, data);

  Command cmd = { .type=0 };
  parseCommand(data, &cmd);
  ASSERT_EQ(cmd.type, CONTROL_CMD);
  EXPECT_NEAR(cmd.control.x, x, 0.00001);
  EXPECT_NEAR(cmd.control.y, y, 0.00001);
  ASSERT_FALSE(cmd.control.laserOn);
  ASSERT_EQ(cmd.control.enabled, X_ENABLED|Y_ENABLED|LASER_ENABLED);
}

TEST(CommandTests, TestBytesToFloat) {
  // value = 43.625
  // sign = 0
  // exponent = 5 + 127 = 132 = 10000100
  // mantissa = 32 + 8 + 2 + 1 + .5 + .125 = 101011101
  // fp representation : 0b0 10000100 010111010000000000000
  
  unsigned int binaryValue = 0b01000010001011101000000000000000;
  char bytes[] = {
    (char)(binaryValue >> 24),
    (char)(binaryValue >> 16),
    (char)(binaryValue >> 8),
    (char)binaryValue
  };

  float floatVal = bytesToFloat((unsigned char*)bytes);

  ASSERT_FLOAT_EQ(43.625f, floatVal);
}