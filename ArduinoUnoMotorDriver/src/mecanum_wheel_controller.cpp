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

typedef enum
{
    FRONT,
    BACK,
} DIRECTION;

// 麦轮运动学公式
#define _A_ 10
#define _B_ 8
#define V_LF_FORMAT (v_y - v_x + w * (_A_ + _B_)) * 1.0
#define V_RF_FORMAT (v_y + v_x - w * (_A_ + _B_)) * 1.0
#define V_LB_FORMAT (v_y + v_x + w * (_A_ + _B_)) * 1.0
#define V_RB_FORMAT (v_y - v_x - w * (_A_ + _B_)) * 1.0

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
    digitalWrite(highIO, HIGH);
    digitalWrite(lowIO, LOW);
    int pwmMax = analogRead(pwmInputIO);
    analogWrite(pwmIO, map((int)(dutyRatio * pwmMax), 0, pwmMax, 0, 1023));
}

void MoveLF(DIRECTION direction, float speedPer)
{
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

void TurnLeft(float speedPer)
{
    MoveLF(BACK, speedPer);
    MoveRF(FRONT, speedPer);
    MoveLB(BACK, speedPer);
    MoveRB(FRONT, speedPer);
}

void MoveLeft(float speedPer)
{
    MoveLF(FRONT, speedPer);
    MoveRF(BACK, speedPer);
    MoveLB(BACK, speedPer);
    MoveRB(FRONT, speedPer);
}

void TurnRight(float speedPer)
{
    MoveLF(FRONT, speedPer);
    MoveRF(BACK, speedPer);
    MoveLB(FRONT, speedPer);
    MoveRB(BACK, speedPer);
}

void MoveRight(float speedPer)
{
    MoveLF(BACK, speedPer);
    MoveRF(FRONT, speedPer);
    MoveLB(FRONT, speedPer);
    MoveRB(BACK, speedPer);
}

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
