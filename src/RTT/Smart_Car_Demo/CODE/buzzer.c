#include "buzzer.h"

#define BUZZER_PIN   D2       //�������������

rt_mailbox_t buzzer_mailbox;


void buzzer_entry(void *parameter)
{
    uint32 mb_data;
    while(1)
    {
        //�����������ݣ����û������������ȴ����ͷ�CPU����Ȩ
        rt_mb_recv(buzzer_mailbox, &mb_data, RT_WAITING_FOREVER);

        gpio_set(BUZZER_PIN, 1);    //�򿪷�����
        rt_thread_mdelay(mb_data);  //��ʱ
        gpio_set(BUZZER_PIN, 0);    //�رշ�����
    }
}





void buzzer_init(void)
{
    rt_thread_t tid;
    
    //��ʼ����������ʹ�õ�GPIO
    gpio_init(BUZZER_PIN, GPO, 0, GPIO_PIN_CONFIG);
    
    //��������
    buzzer_mailbox = rt_mb_create("buzzer", 5, RT_IPC_FLAG_FIFO);
    
    //�������������߳�
    tid = rt_thread_create("buzzer", buzzer_entry, RT_NULL, 512, 5, 2);
    
    //�����߳�
    if(RT_NULL != tid)
    {
        rt_thread_startup(tid);
    }
}