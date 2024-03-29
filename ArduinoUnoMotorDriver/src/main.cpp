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

extern void
MECANUM_WHEEL_INIT();

extern void Stop();
extern void MoveForward(float speedPer);
extern void MoveBackward(float speedPer);
extern void TurnLeft(float speedPer);
extern void TurnRight(float speedPer);
extern void MoveLeft(float speedPer);
extern void MoveRight(float speedPer);

extern void OLED_INIT();
extern void ShowIP(String ip);
extern void PrintSerial(String str);
extern void HandleCmd(String cmd);
extern void MoveBackForObstacles();
extern void MoveX(int dx, int dy, float speedPer);

void TestMove();

void setup()
{
    Serial.begin(115200);
    Serial.setTimeout(400);
    for (int i = 2; i <= 13; i++)
    {
        pinMode(i, OUTPUT);
    }

    OLED_INIT();
    MECANUM_WHEEL_INIT();
    //TestMove();
}

void loop()
{
    if (Serial.available())
    {
        String serial_cmd = Serial.readStringUntil('\n');
        PrintSerial(serial_cmd);
        HandleCmd(serial_cmd);
    }
}

void HandleCmd(String cmd)
{
    if (cmd.startsWith("Obstacles"))
    {
        MoveBackForObstacles();
    }
    else if (cmd.startsWith("MoveX:"))
    {
        // MoveX:[dx]:[dy]:[dutyRadio]
        int dx = cmd.substring(6, 7).toInt();
        int dy = cmd.substring(8, 9).toInt();
        if (dx == -1 || dy == -1)
        {
            Stop();
            return;
        }
        float speedPer = cmd.substring(10, cmd.length() - 1).toFloat();
        MoveX(dx, dy, speedPer);
    }
    else if (cmd.startsWith("Request Go"))
    {
        MoveForward(0.6);
    }
    else if (cmd.startsWith("Request Back"))
    {
        MoveBackward(0.6);
    }
    else if (cmd.startsWith("Request Left"))
    {
        TurnLeft(0.08);
    }
    else if (cmd.startsWith("Request Right"))
    {
        TurnRight(0.08);
    }
    else if (cmd.startsWith("Request Stop"))
    {
        Stop();
    }
    else if (cmd.startsWith("MoveLeft:"))
    {
        float speedPer = cmd.substring(9, cmd.length() - 1).toFloat();
        MoveLeft(speedPer);
    }
    else if (cmd.startsWith("MoveRight:"))
    {
        float speedPer = cmd.substring(10, cmd.length() - 1).toFloat();
        MoveRight(speedPer);
    }
    else if (cmd.startsWith("NormalMove:"))
    {
        // NormalMove:[code]:[dutyRatio]
        String motion = cmd.substring(11, 12);
        if (motion.equals("4"))
        {
            Stop();
        }
        float speedPer = cmd.substring(13, cmd.length() - 1).toFloat();
        if (motion.equals("0"))
        {
            MoveForward(speedPer);
        }
        else if (motion.equals("1"))
        {
            MoveBackward(speedPer);
        }
        else if (motion.equals("2"))
        {
            TurnLeft(speedPer);
        }
        else if (motion.equals("3"))
        {
            TurnRight(speedPer);
        }
    }
}