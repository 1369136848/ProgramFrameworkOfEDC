/************************************************************
* ��֯���ƣ����Ӵ���С��
* �ļ�����: MOTOR.H
* ����:     �ܳ���
* �汾 :    1.0
* ����:
* ����: 	 ������ͷ�ļ�
* ��ʷ�޸ļ�¼: // ��ʷ�޸ļ�¼
* <����> <ʱ��> <�汾 > <����>
***********************************************************/
#ifndef _MOTOR_H_
#define _MOTOR_H_
#include <math.h>
#include "../../../BSP/config.h"
/************************************
Ӳ��PWMio���ţ�û��pwm1��
PWM_N| io  | �ڶ���
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


void stopMotor(u8 motor);
void startMotor(u8 motor);
bit getMotorState(u8 motor);
/*************************************************
* ��������: void setMotorSpeed(bit motor,float speed)
* ����: ���õ���ٶȺ���
* ����: motor�������ͷ�ļ��ж���
				speed���ٶ� 0~1   0ʱ���ͣת
* ����˵��: 
*************************************************/
void setMotorSpeed(u8 motor,float speed);




#endif
