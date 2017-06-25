/************************************************************
* 组织名称：电子大赛小组
* 文件名称: MOTOR.H
* 作者:     周晨阳
* 版本 :    1.0
* 日期:
* 描述: 	 电机相关头文件
* 历史修改记录: // 历史修改记录
* <作者> <时间> <版本 > <描述>
***********************************************************/
#ifndef _MOTOR_H_
#define _MOTOR_H_
#include "../../USER/USER.h"
#include "../USER/STC15_PWM.H"                             /*********对外参数和接口函数***********/
/************************************
硬件PWMio引脚（没有pwm1）
PWM_N| io  | 第二组
PWM2 :P3.7 -> P2.7
PWM3 :P2.1 -> P4.5
PWM4 :P2.2 -> P4.4
PWM5 :P2.3 -> P4.2
PWM6 :P1.6 -> P0.7
PWM7 :P1.7 -> P0.6
************************************/

#define LEFT_MOTOR   1
#define RIGHT_MOTOR  0
#define STOP         0

typedef struct Motor
{
	bit MotorState;
};
Motor leftMotor;
Motor rightMotor;

void stopMotor(u8 motor);
void startMotor(u8 motor);
bit getMotorState(u8 motor);
/*************************************************
* 函数名称: void setMotorSpeed(bit motor,float speed)
* 描述: 设置电机速度函数
* 输入: motor：电机，头文件中定义
				speed：速度 0~1   0时电机停转
* 其他说明: 
*************************************************/
void setMotorSpeed(u8 motor,float speed);




#endif
