#include "fixedPulser.h"
#include "../BSP/STC15_PWM.h"
#include <limits.h>

struct Pulser //���巢�����йز������ݽṹ
{
	u16 count;  //�������
	u16 Hz;     //����Ƶ��
	u8 state;   //���巢������ǰ״̬

};

static struct Pulser g_pulser[PULSER_NUM];  //���巢������Ϣ�洢����
bool setPulse(u8 pulser_num, u16 Hz, u16 count)
{
	set_PWM_period(Hz);
	g_pulser[pulser_num].Hz = Hz;
	g_pulser[pulser_num].count = count;

	return 1;
}
bool openPulser(u8 pulser_num)
{
	g_pulser[pulser_num].state = ON;
	PWM_UNLOCK;
	PWM_N_EN(pulser_num + 5);
	PWM_LOCK;

	return 1;
}
bool closePulser(u8 pulser_num)
{

	PWM_UNLOCK;
	PWM_N_NO(pulser_num + 5);
	PWM_LOCK;

	g_pulser[pulser_num].state = OFF;
	return 1;

}
bool getPulserState(u8 pulser_num)
{

	return g_pulser[pulser_num].state;

}
static u16 g_PWMtmp = 0;
static void PWM_Routine(void) interrupt 22   //�ж�ִ�г���
{

	if (PWMIF == 0100000)
	{
		if (g_PWMtmp++ >= g_pulser[PULSER_1].count)
		{
			PWM_UNLOCK;
			PWM_N_NO(7);
			PWM_LOCK;
		}
	}
}
static void PWMFD_Routine(void) interrupt 23
{

}
void PulserInit(void)//��ʼ������
{

	PWM_InitTypeDef  PWM_InitStructure;
	set_PWM_duty(PWM_7, PWM_DEFAULT_DUTY);
	set_PWM_duty(PWM_6, PWM_DEFAULT_DUTY);

	PWM_UNLOCK;
	PWM_InitStructure.PWM_GOTO_ADC = DISABLE;            //ENABLE=����������ʱ ����ADC
	PWM_InitStructure.PWM_V_INIT = PWM_LOW;             //PWM_HIGHT=��ʼ�ߵ�ƽ  PWM_LOW=��ʼ�͵�ƽ
	PWM_InitStructure.PWM_0ISR_EN = DISABLE;           //ENABLE=ʹ��PWM�����������ж�  DISABLE=�ر�PWM�����������ж� �� CBIF��Ȼ�ᱻӲ����λ
	PWM_InitStructure.PWM_OUT_EN = ENABLE;               //ENABLE=PWMͨ��x�Ķ˿�ΪPWM��������� ��PWM���η���������
	PWM_InitStructure.PWM_UNUSUAL_EN = DISABLE;         //ENABLE=ʹ��PWM���ⲿ�쳣��⹦��
	PWM_InitStructure.PWM_UNUSUAL_OUT = DISABLE;       //ENABLE=�����쳣ʱ��PWM��Ӧ������ڻ��� ��������ģʽ
	PWM_InitStructure.PWM_UNUSUAL_ISR_EN = DISABLE;      //ENABLE=ʹ���쳣����ж� DISABLE=�ر��쳣����ж� ��FDIF��Ȼ�ᱻӲ����λ
	PWM_InitStructure.PWM_UNUSUAL_CMP0_EN = DISABLE;     //ENABLE=�쳣���ԴΪ�Ƚ�������� ���ȽϽ��Ϊ��ʱ ����PWM�쳣
	PWM_InitStructure.PWM_UNUSUAL_P24_EN = DISABLE;      //ENABLE=�쳣���ԴΪP24/PWMFLT ��P24=1 ����PWM�쳣
	PWM_InitStructure.PWM_CLOCK = PWM_Clock_NT;          //PWM_Clock_NT=PWM��ʱ��Դ��ϵͳʱ�Ӿ���Ƶ���ʱ��  PWM_Clock_Timer2_OF=PWM��ʱ��Դ��TMER2���������
	PWM_InitStructure.PWM_CLOCK_DIV = 0x00;              //��PWM_CLOCK=PWM_Clock_NTʱ PWM��ʱ��Դ��ϵͳʱ��/(PS[3:0]+1)
	PWM_InitStructure.PWM_SELECTx_IO = PWM_SELECT_N;     //PWM_SELECT_N=PWM��һѡ��IO�� PWM_SELECT_N_2=PWM�ڶ�ѡ��IO��
	PWM_InitStructure.PWM_ISRx_EN = ENABLE;           //ENABLE=ʹ��PWMx�ж� ʹ�ܵ�һ��ת��ڶ���ת�ж�
	PWM_InitStructure.PWM_T1x_EN = DISABLE;           //ENABLE=ʹ�ܵ�һ��ת�ж�
	PWM_InitStructure.PWM_T2x_EN = ENABLE;          //ENABLE=ʹ�ܵڶ���ת�ж�
	PWM_InitStructure.PWM_EN = DISABLE;                //ENABLE=PWMʹ�� ������PWM�������úú�������� ������رպ��ڴ򿪣���PWM���������´�0����
	PWM_Inilize(PWM_6, &PWM_InitStructure);
	PWM_Inilize(PWM_7, &PWM_InitStructure);

	PWM_LOCK;
}
