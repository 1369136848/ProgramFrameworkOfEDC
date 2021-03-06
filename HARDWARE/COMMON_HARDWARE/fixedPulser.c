#include "fixedPulser.h"
#include "../BSP/STC15_PWM.h"
#include <limits.h>

struct Pulser //脉冲发生器有关参数数据结构
{
	u16 count;  //脉冲个数
	u16 Hz;     //脉冲频率
	u8 state;   //脉冲发生器当前状态

};

static struct Pulser g_pulser[PULSER_NUM];  //脉冲发生器信息存储数组
bool setPulse(u8 pulser_num, u16 Hz, u16 count)
{
	set_PWM_period(Hz);
	g_pulser[pulser_num].Hz = Hz;
	g_pulser[pulser_num].count = count;

	return 1;
}
bool openPulser(u8 pulser_num)
{
	open_PWM_N(pulser_num + 5);
	g_pulser[pulser_num].state = ON;

	return 1;
}
bool closePulser(u8 pulser_num)
{

	close_PWM_N(pulser_num + 5);

	g_pulser[pulser_num].state = OFF;
	return 1;

}
bool getPulserState(u8 pulser_num)
{

	return g_pulser[pulser_num].state;

}
void PulserInit(void)//初始化程序
{
   //使用PWM6，7作为两个步进电机的输出
		//GPIO_InitTypeDef    GPIO_InitStructure;     //结构定义

	PWM_InitTypeDef  PWM_InitStructure;
	set_PWM_duty(PWM_7, PWM_DEFAULT_DUTY); //使用默认0.5的占空比
	set_PWM_duty(PWM_6, PWM_DEFAULT_DUTY);

	PWM_UNLOCK;
	PWM_InitStructure.PWM_GOTO_ADC = DISABLE;            //ENABLE=计数器归零时 触发ADC
	PWM_InitStructure.PWM_V_INIT = PWM_LOW;             //PWM_HIGHT=初始高电平  PWM_LOW=初始低电平
	PWM_InitStructure.PWM_0ISR_EN = DISABLE;           //ENABLE=使能PWM计数器归零中断  DISABLE=关闭PWM计数器归零中断 但 CBIF仍然会被硬件置位
	PWM_InitStructure.PWM_OUT_EN = ENABLE;               //ENABLE=PWM通道x的端口为PWM波形输出口 受PWM波形发生器控制
	PWM_InitStructure.PWM_UNUSUAL_EN = DISABLE;         //ENABLE=使能PWM的外部异常检测功能
	PWM_InitStructure.PWM_UNUSUAL_OUT = DISABLE;       //ENABLE=发生异常时，PWM对应的输出口会变成 高阻输入模式
	PWM_InitStructure.PWM_UNUSUAL_ISR_EN = DISABLE;      //ENABLE=使能异常检测中断 DISABLE=关闭异常检测中断 但FDIF仍然会被硬件置位
	PWM_InitStructure.PWM_UNUSUAL_CMP0_EN = DISABLE;     //ENABLE=异常检测源为比较器的输出 当比较结果为高时 触发PWM异常
	PWM_InitStructure.PWM_UNUSUAL_P24_EN = DISABLE;      //ENABLE=异常检测源为P24/PWMFLT 当P24=1 触发PWM异常
	PWM_InitStructure.PWM_CLOCK = PWM_Clock_NT;          //PWM_Clock_NT=PWM的时钟源是系统时钟经分频后的时钟  PWM_Clock_Timer2_OF=PWM的时钟源是TMER2的溢出脉冲
	PWM_InitStructure.PWM_CLOCK_DIV = 0x00;              //当PWM_CLOCK=PWM_Clock_NT时 PWM的时钟源是系统时钟/(PS[3:0]+1)
	PWM_InitStructure.PWM_SELECTx_IO = PWM_SELECT_N;     //PWM_SELECT_N=PWM第一选择IO口 PWM_SELECT_N_2=PWM第二选择IO口
	PWM_InitStructure.PWM_ISRx_EN = ENABLE;           //ENABLE=使能PWMx中断 使能第一翻转或第二翻转中断
	PWM_InitStructure.PWM_T1x_EN = DISABLE;           //ENABLE=使能第一翻转中断
	PWM_InitStructure.PWM_T2x_EN = ENABLE;          //ENABLE=使能第二翻转中断
	PWM_InitStructure.PWM_EN = DISABLE;                //ENABLE=PWM使能 在其他PWM参数设置好后最后设置 如果被关闭后在打开，则PWM计数器重新从0计数
	PWM_Inilize(PWM_6, &PWM_InitStructure);
	PWM_Inilize(PWM_7, &PWM_InitStructure);

	PWM_LOCK;
}

static u16 g_PWMtmp = 0;//计数变量
/***************！以下为私有函数，不建议更改！********************************/
static void PWM_Routine(void) interrupt 22   //中断执行程序
{

	if (PWMIF == 0100000)
	{
		if (g_PWMtmp++ >= g_pulser[PULSER_1].count)
		{
			g_pulser[PULSER_1].count = 0;
			PWM_UNLOCK;
			PWM_N_NO(7);
			PWM_LOCK;
		}
	}
}
//static void PWMFD_Routine(void) interrupt 23
//{
//
//}
