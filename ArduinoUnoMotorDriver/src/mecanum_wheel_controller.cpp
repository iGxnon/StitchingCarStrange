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
    analogWrite(pwmIO, (int)(dutyRatio * pwmMax));
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

void MoveLeft(float speedPer)
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
    MoveLB(FRONT, speedPer);
    MoveRB(BACK, speedPer);
}
