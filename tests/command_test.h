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

void floatToBytes(float x, unsigned char bytes[4]) {
  unsigned int rawBits = *(unsigned int*)&x;

  bytes[0] = 0xff & (rawBits >> 24);
  bytes[1] = 0xff & (rawBits >> 16);
  bytes[2] = 0xff & (rawBits >> 8);
  bytes[3] = 0xff & rawBits;
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

TEST(CommandTests, ParseCalibrationCommandFrame) {
  CalibrateCmd expected = {
    topLeft:     { 1.0f, 2.0f  },
    topRight:    { 3.0f, 4.1f  },
    bottomLeft:  { 5.1f, 1.43f },
    bottomRight: { 2.3f, 9.5f  },
  };

  char rawBytes[34] = { [0]=CALIBRATE_CMD };
  unsigned char buf[4];
  for (int i = 0; i < 4; i++) {
    floatToBytes(((Point*)&expected)[i].x, buf);
    memcpy((void*)&rawBytes[1 + 8*i], (void*)buf, 4 * sizeof(char));
    floatToBytes(((Point*)&expected)[i].y, buf);
    memcpy((void*)&rawBytes[5 + 8*i], (void*)buf, 4 * sizeof(char));
  }

  CalibrateCmd actual;
  initCalibrateCmd(&actual);
  parseCommand(rawBytes, (Command*)&actual);

  for (int i = 0; i < 4; i++) {
    EXPECT_FLOAT_EQ(((Point*)&expected)[i].x, ((Point*)&expected)[i].x);
    EXPECT_FLOAT_EQ(((Point*)&expected)[i].y, ((Point*)&expected)[i].y);
  }
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