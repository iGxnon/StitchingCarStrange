#include <Arduino.h>

/// 左前轮IO口
int LF_FORWARD = 2;
int LF_BACKWARD = 3;
int LF_PWM = 5;
int LF_PWM_HIGH_IN = A0; // PWM最高电平输入

/// 右前轮IO口
int RF_FORWARD = 4;
int RF_BACKWARD = 7;
int RF_PWM = 6;
int RF_PWM_HIGH_IN = A1;

/// 左后轮IO口
int LB_FORWARD = 8;
int LB_BACKWARD = 9;
int LB_PWM = 10;
int LB_PWM_HIGH_IN = A2;

/// 右后轮IO口
int RB_FORWARD = 12;
int RB_BACKWARD = 13;
int RB_PWM = 11;
int RB_PWM_HIGH_IN = A3;

extern void OLED_INIT();
extern void MECANUM_WHEEL_INIT();
extern unsigned char *point;
extern void MoveForward(float speedPer);
extern void Stop();

void setup()
{
    MoveForward(0.75);
}

void loop()
{
}