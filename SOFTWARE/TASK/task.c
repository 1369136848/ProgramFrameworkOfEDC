#include "task.h"
#include <rtx51tny.h>                 /* RTX-51 tiny functions & defines      */
#include "../COMMON_SOFTWARE/DATA_SCOPE/DataScope_DP.h"

//����ʾ�������ݷ��ͺ���
void taskStart() _task_ 0
{
	os_create_task (1);
	os_delete_task (0); 
}
void sendScopeData(void) _task_ 1
{
	u8 a; //
	u8 Send_Count;
	/*
	DataScope_Get_Channel_Data(yourData, 1);  //�������
	DataScope_Get_Channel_Data(yourData, 2);  //ʵ���������
	DataScope_Get_Channel_Data(yourData, 3);  //��������
	DataScope_Get_Channel_Data(yourData, 4);  //�趨�Ƕ�
	DataScope_Get_Channel_Data(yourData, 5);  //ʵ�ʽǶ�
	...
	*/
	Send_Count = DataScope_Data_Generate(5); //
	for (a = 0; a < Send_Count; a++)
	{
		TX1_write2buff(DataScope_OutPut_Buffer[a]); //����һͨ�����ݵ�����ʾ����
	}
  os_wait(K_IVL,10,0);
}

