#include "Arduino.h"

extern int LF_FORWARD;
extern int LF_BACKWARD;
extern int LF_PWM;
extern int LF_PWM_HIGH_IN;

extern int RF_FORWARD;
extern int RF_BACKWARD;
extern int RF_PWM;
extern int RF_PWM_HIGH_IN;

extern int LB_FORWARD;
extern int LB_BACKWARD;
extern int LB_PWM;
extern int LB_PWM_HIGH_IN;

extern int RB_FORWARD;
extern int RB_BACKWARD;
extern int RB_PWM;
extern int RB_PWM_HIGH_IN;
extern void Stop();

typedef enum
{
    FRONT,
    BACK,
} DIRECTION;

#define POWER_LF 1.0
#define POWER_RF 1.0
#define POWER_LB 0.80
#define POWER_RB 0.80

// 麦轮运动学公式
#define _A_ 10
#define _B_ 8
#define V_LF_FORMAT (v_y - v_x + w * (_A_ + _B_))
#define V_RF_FORMAT (v_y + v_x - w * (_A_ + _B_))
#define V_LB_FORMAT (v_y + v_x + w * (_A_ + _B_))
#define V_RB_FORMAT (v_y - v_x - w * (_A_ + _B_))

// 最低驱动PWM
#define LIMIT_PWM 0.03

int CONTROL = 1;

void MECANUM_WHEEL_INIT()
{
    digitalWrite(LF_FORWARD, LOW);
    digitalWrite(LF_BACKWARD, LOW);
    analogWrite(LF_PWM, 0);

    digitalWrite(RF_FORWARD, LOW);
    digitalWrite(RF_BACKWARD, LOW);
    analogWrite(RF_PWM, 0);

    digitalWrite(LB_FORWARD, LOW);
    digitalWrite(LB_BACKWARD, LOW);
    analogWrite(LB_PWM, 0);

    digitalWrite(RB_FORWARD, LOW);
    digitalWrite(RB_BACKWARD, LOW);
    analogWrite(RB_PWM, 0);
}

void move(int highIO, int lowIO, int pwmIO, float dutyRatio, int pwmInputIO)
{
    if (!CONTROL)
    {
        return;
    }
    digitalWrite(highIO, HIGH);
    digitalWrite(lowIO, LOW);
    int pwmMax = analogRead(pwmInputIO);
    analogWrite(pwmIO, dutyRatio * pwmMax);
}

void MoveBackForObstacles()
{
    CONTROL = 0;
    digitalWrite(LF_BACKWARD, HIGH);
    digitalWrite(LF_FORWARD, LOW);
    digitalWrite(LB_BACKWARD, HIGH);
    digitalWrite(LB_FORWARD, LOW);
    digitalWrite(RF_BACKWARD, HIGH);
    digitalWrite(RF_FORWARD, LOW);
    digitalWrite(RB_BACKWARD, HIGH);
    digitalWrite(RB_FORWARD, LOW);
    int pwmMax = analogRead(LF_PWM_HIGH_IN);

    analogWrite(LF_PWM, pwmMax);
    analogWrite(RF_PWM, pwmMax);
    analogWrite(LB_PWM, pwmMax);
    analogWrite(RB_PWM, pwmMax);
    delay(100);
    Stop();
    delay(100);
    CONTROL = 1;
}

void MoveLF(DIRECTION direction, float speedPer)
{
    speedPer *= POWER_LF;
    if (direction == FRONT)
    {
        move(LF_FORWARD, LF_BACKWARD, LF_PWM, speedPer, LF_PWM_HIGH_IN);
    }
    else
    {
        move(LF_BACKWARD, LF_FORWARD, LF_PWM, speedPer, LF_PWM_HIGH_IN);
    }
}

void MoveRF(DIRECTION direction, float speedPer)
{
    speedPer *= POWER_RF;
    if (direction == FRONT)
    {
        move(RF_FORWARD, RF_BACKWARD, RF_PWM, speedPer, RF_PWM_HIGH_IN);
    }
    else
    {
        move(RF_BACKWARD, RF_FORWARD, RF_PWM, speedPer, RF_PWM_HIGH_IN);
    }
}

void MoveLB(DIRECTION direction, float speedPer)
{
    speedPer *= POWER_LB;
    if (direction == FRONT)
    {
        move(LB_FORWARD, LB_BACKWARD, LB_PWM, speedPer, LB_PWM_HIGH_IN);
    }
    else
    {
        move(LB_BACKWARD, LB_FORWARD, LB_PWM, speedPer, LB_PWM_HIGH_IN);
    }
}

void MoveRB(DIRECTION direction, float speedPer)
{
    speedPer *= POWER_RB;
    if (direction == FRONT)
    {
        move(RB_FORWARD, RB_BACKWARD, RB_PWM, speedPer, RB_PWM_HIGH_IN);
    }
    else
    {
        move(RB_BACKWARD, RB_FORWARD, RB_PWM, speedPer, RB_PWM_HIGH_IN);
    }
}

void Stop()
{
    MECANUM_WHEEL_INIT();
}

void MoveForward(float speedPer)
{
    MoveLF(FRONT, speedPer);
    MoveRF(FRONT, speedPer);
    MoveLB(FRONT, speedPer);
    MoveRB(FRONT, speedPer);
}

void MoveBackward(float speedPer)
{
    MoveLF(BACK, speedPer);
    MoveRF(BACK, speedPer);
    MoveLB(BACK, speedPer);
    MoveRB(BACK, speedPer);
}

// raw移动
void TurnLeft(float speedPer)
{
    MoveLF(BACK, speedPer);
    MoveRF(FRONT, speedPer);
    MoveLB(BACK, speedPer);
    MoveRB(FRONT, speedPer);
}

void MoveRight(float speedPer)
{
    MoveLF(FRONT, speedPer);
    MoveRF(BACK, speedPer);
    MoveLB(BACK, speedPer);
    MoveRB(FRONT, speedPer);
}

// raw移动
void TurnRight(float speedPer)
{
    MoveLF(FRONT, speedPer);
    MoveRF(BACK, speedPer);
    MoveLB(FRONT, speedPer);
    MoveRB(BACK, speedPer);
}

void MoveLeft(float speedPer)
{
    MoveLF(BACK, speedPer);
    MoveRF(FRONT, speedPer);
    MoveLB(FRONT, speedPer);
    MoveRB(BACK, speedPer);
}

// X 形状移动
void MoveX(DIRECTION dx, DIRECTION dy, float speedPer)
{
    if (dx == FRONT)
    {
        if (dy == FRONT)
        {
            MoveLF(FRONT, speedPer);
            MoveRB(FRONT, speedPer);
            // 加一点反向电压给RF和LB轮，一定程度减少它们被带动
            MoveRF(BACK, LIMIT_PWM);
            MoveLB(BACK, LIMIT_PWM);
        }
        else
        {
            MoveRF(BACK, speedPer);
            MoveLB(BACK, speedPer);

            MoveLF(FRONT, LIMIT_PWM);
            MoveRB(FRONT, LIMIT_PWM);
        }
    }
    else
    {
        if (dy == FRONT)
        {
            MoveRF(FRONT, speedPer);
            MoveLB(FRONT, speedPer);

            MoveLF(BACK, LIMIT_PWM);
            MoveRB(BACK, LIMIT_PWM);
        }
        else
        {
            MoveLF(BACK, speedPer);
            MoveRB(BACK, speedPer);

            MoveRF(FRONT, LIMIT_PWM);
            MoveLB(FRONT, LIMIT_PWM);
        }
    }
}

// 该函数不适用于直流有刷电机（就那种小玩具里面的那种电机
// 调试参数会变得不幸
void MoveWithDelta(float v_x, float v_y, float w, float speedPer)
{
    float v[4] = {V_LF_FORMAT, V_RF_FORMAT, V_LB_FORMAT, V_RB_FORMAT};
    float *iter = v;
    float max_v = fabs(*iter);
    while (iter < v + 4)
    {
        max_v = max(fabs(max_v), fabs(*iter));
        iter++;
    }
    iter = v;
    while (iter < v + 4)
    {
        *iter /= max_v;
        *iter *= speedPer;
        iter++;
    }
    MoveLF(v[0] > 0 ? FRONT : BACK, fabs(v[0]));
    MoveRF(v[1] > 0 ? FRONT : BACK, fabs(v[1]));
    MoveLB(v[2] > 0 ? FRONT : BACK, fabs(v[2]));
    MoveRB(v[3] > 0 ? FRONT : BACK, fabs(v[3]));
}

void TestMove()
{
    MoveBackForObstacles();
}