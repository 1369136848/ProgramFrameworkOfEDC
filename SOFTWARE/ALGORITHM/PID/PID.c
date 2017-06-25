/************************************************************
* ��֯���ƣ����Ӵ���С��
* �ļ�����: PID.c
* ����:     ���Ӵ���С��
* �汾 :    1.0
* ����:     10/06/17
* ����: 	 
* ��ʷ�޸ļ�¼: 
* <����> <ʱ��> <�汾 > <����>
***********************************************************/
#include "PID.h"
#ifdef PID_ALG
#include <math.h>
#define  INTEGRAL_SEPARATE //ʹ�û��ַ��뷨
static float Ki_temp,Kp_temp,Kd_temp; //����pid���������ľ�̬����

//pid�㷨�ľ�̬�ṹ��

static struct
{
  float setParameter;//�趨�Ƕ�ֵ
  float actualParameter;//ʵ�ʽǶ�ֵ
  float err;//ƫ��ֵ
  float err_last;//��һ��ƫ��ֵ
  float Kp,Ki,Kd;//���������֣�΢��ϵ��
  float integral;//����ֵ,λ��ʽpid�㷨
  float output;  //ʵ���������
} pid;
/*************************************************
* ��������: bit setBoardWithAngleAndPID(unsigned char angle)
* ����: ��pid�㷨�ķ�ڽǶȵ�������
* ����: ��
* ����ֵ: ִ��һ�νǶȵ�����ִ�гɹ�����1
*             ����˵��:
* ���ô˺��������ѭ����ֻ�е�����һ��С���������֮��Ż��˳�ѭ����
* �����޷���֤�˳�ѭ�������Ƿ���һֱ�ȶ���
* ������÷��һֱ�����ȶ������벻ͣ�ĵ��ô˺�����
*               ע�⣺
* ��ʹ��pid���ú���setPID_data���������ýǶȣ�֮���ڵ��ô˺�����
***��������****
  ����  |  ʱ��  |  ˵��
*�ܳ���	   5/16		��ʹ��pid���ú���setPID_data����
									������Ҫ�ȶ��ĽǶȣ���Ҫ��˺�����ֵ*
									 ��Ч
*************************************************/
bit PID(void)
{


  if(pid.setParameter >145.0f)
    {
      pid.setParameter =145.0f;
#ifdef MOTOR_DEBUG
      PrintString1("angle is over maximum adjusting\n");
#endif
    }
  if(pid.setParameter <35.0f)
    {
      pid.setParameter =35.0f;
#ifdef MOTOR_DEBUG
      PrintString1("angle is over miximum adjusting\n");
#endif
    }


  //  pid.setParameter      =           ANGLE; //��ʹ�ô˺���ȥ���ýǶȣ��˺���ֻ�ȶ����趨��
  pid.actualParameter   =           getAngle(LAST_ANGLE);
  pid.err           =           pid.setParameter-pid.actualParameter;
#ifdef  INTEGRAL_SEPARATE //�Ƿ�ʹ�û��ַ��뷨
  if(abs(pid.err)<15.0f)//���ַ��뷨����ֹ������ʱ�����ۻ���������𵴣�ͬʱ��С������΢��
    {
      pid.Ki        =        Ki_temp;
      pid.integral  +=       pid.err;
      pid.Kp        =        Kp_temp-0.7f;
      pid.Kd        =        Kd_temp-0.5f;
    }
  else//������ʱȥ����������ۼ���� ����ʹ�ñ�����΢�� ��ͬʱ��ԭ�����ϼӴ�΢�ֵ�����
    {
      pid.Ki        =        0;
      pid.integral  =        0;
      pid.Kd        =        Kd_temp+0.5f;
    }
#else
  pid.integral  +=       pid.err;
#endif
  pid.output        =    pid.Kp*pid.err+pid.Ki*pid.integral+pid.Kd*(pid.err-pid.err_last);
  pid.err_last=pid.err;


  return 1;

}
/*************************************************
* ��������:float getPID_data(u8 DATA)
* ����: ��ȡpid����
* ����: ���궨��
* ����ֵ: ��Ӧpid����
* ����˵��: ��û������pid������ֱ�Ӷ�ȡ������δ֪��
*************************************************/

float getPID_data(u8 DATA)
{
  switch(DATA)
    {
      case SET_PARAMETER:
        return pid.setParameter;
        break;
      case ERR:
        return pid.err;
        break;
      case ERR_LAST:
        return pid.err_last;
        break;
      case KP:
        return pid.Kp;
        break;
      case KI:
        return pid.Ki;
        break;
      case KD:
        return pid.Kd;
        break;
      case INTEGRAL:
        return pid.integral;
        break;
      case OUTPUT:
        return pid.output;
        break;
      default:
        return 0;
        break;
    }
}
/*************************************************
* ��������:void setPID_data(u8 DATA,float value)
* ����:  ����pid���������ĺ�����һ����ֻ������һ������
*        ����Ϊ����PID�����Ľӿڡ�
* ����: u8 DATA ����Ҫ���ĵĲ������ƣ�Ϊ�궨��,
        float value������ֵ
* ���: ��
* ����ֵ: ��
* ����˵��: ��
*************************************************/
void setPID_data(u8 DATA,float value)
{
  switch(DATA)
    {
      case KP:
        pid.Kp=value;
        Kp_temp=value;
        break;
      case KI:
        pid.Ki=value;
        Ki_temp=value;
        break;
      case KD:
        pid.Kd=value;
        Kd_temp=value;
        break;
      case SET_PARAMETER:
        pid.setParameter=value;
      default:
        break;
    }
}
//PID�㷨��ʼ������  .
/*************************************************
* ��������: void PID_config(float kp,float ki,float kd)
* ����:   pid�㷨��ʼ��������������������P,I,D,
*         �˺���ֻ����Ϊ��ʼ����������Ϊ����������ֵ�Ľӿ�
          �����Ҫ����������ֵ����ʹ��setPID_data()����
* ����: float kp,float ki,float kd
* ���: ��
* ����ֵ: ��
* ����˵��: ��
*************************************************/
void PID_config(float kp,float ki,float kd)//pid�㷨��ʼ��������������������PID
{
  pid.setParameter=70;//������û�����ýǶ���Ĭ��70��
  pid.actualParameter=30;
  pid.err=0;
  pid.err_last=0;
  pid.output=0;
  pid.integral=0;
  pid.Kp=kp;
  pid.Ki=ki;
  pid.Kd=kd;
  Kp_temp=pid.Kp;
  Ki_temp=pid.Ki;
  Kd_temp=pid.Kd;
}
#endif

