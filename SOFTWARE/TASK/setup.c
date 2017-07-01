#include "setup.h"
#include "../../HARDWARE/DEVICES/BUTTON/BUTTON.h"	
#include "../../HARDWARE/DEVICES/MOTOR/DC_MOTOR/MOTOR.h"
#include "../../HARDWARE/DEVICES/LED/LED.h"
#include "../../HARDWARE/BSP/USART1.h"
#include "../../HARDWARE/BSP/timer.h"

/*************  ����1��ʼ������ *****************/
/*************************************************
* ��������: void    UART1_init(unsigned long BandRate)
* ����: ����1��ʼ������
* ����: unsigned long BandRate �����ڵĲ����ʣ�������ֵ�����ĸ"L"����ֹ������
* ���: ���ô˺����ᷢ��һ�β������ �� STC15w4k58s4 UART1 is open ��
* ����ֵ: ��
* ����˵��: ��
*************************************************/
void    UART1_init(unsigned long BandRate)
{
  COMx_InitDefine     COMx_InitStructure;
  COMx_InitStructure.UART_Mode = UART_8bit_BRTx;
  COMx_InitStructure.UART_BRT_Use = BRT_Timer2;
  COMx_InitStructure.UART_BaudRate = BandRate;
  COMx_InitStructure.UART_RxEnable = ENABLE;
  COMx_InitStructure.BaudRateDouble = DISABLE;
  COMx_InitStructure.UART_Interrupt = ENABLE;
  COMx_InitStructure.UART_Polity = PolityHigh;
  COMx_InitStructure.UART_P_SW = UART1_SW_P30_P31;
  COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;
  USART_Configuration(USART1, &COMx_InitStructure);
 // PrintString1("STC15w4k58s4's UART1 is open \r\n");   //���ڲ���
}
void setup(void)
{
	 UART1_init(115200L);
   timerInit();
	 Board_LED_Init();
   Button_config();
	 EA=1;

}

