/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            main
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.3
 * @Target core     CH32V103R8T6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-12-04
 ********************************************************************************************************************/
//整套推荐IO查看Projecct文件夹下的TXT文本

//打开新的工程或者工程移动了位置务必执行以下操作
//右键单击工程，选择刷新

#include "headfile.h"

//定时器控制块指针
rt_timer_t timer1;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      定时器超时函数
//  @param      parameter   参数
//  @return     void
//  @note       定时器超时函数，当时间到之后会自动运行该函数
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void timerout1(void *parameter)
{
    static rt_uint8_t count = 0;
    rt_kprintf("timerout!!!\n");

    //执行10次后停止该定时器
    if(count++ >= 10)
    {
        rt_timer_stop(timer1);
        rt_kprintf("stop timer1!\n");

        rt_timer_delete(timer1);
        rt_kprintf("delete timer1!!!\n");
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      线程创建以及启动
//  @param      void        空
//  @return     void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
int timer_example(void)
{
    //创建一个定时器 周期100个tick
    timer1 = rt_timer_create(
            "timer1",                  //timer1表示定时器的名称，8个字符内。
            timerout1,                 //timerout1表示时间到了之后需要执行的函数
            RT_NULL,                   //RT_NULL表示不需要传递参数。
            100,                       //100表示定时器的超时时间为100个系统tick，系统周期为1毫秒，则100表示100毫秒
            RT_TIMER_FLAG_PERIODIC);   //RT_TIMER_FLAG_PERIODIC表示定时器以周期运行  如果设置为RT_TIMER_FLAG_ONE_SHOT则只会运行一次

    //首先检查定时器控制块不是空，则启动定时器
    if(timer1 != RT_NULL)
    {
        rt_timer_start(timer1);
    }

    return 0;
}

//使用INIT_APP_EXPORT宏自动初始化，也可以通过在其他线程内调用timer_example函数进行初始化
INIT_APP_EXPORT(timer_example);

int main(void)
{

    //此处编写用户代码(例如：外设初始化代码等)
    gpio_init(B15, GPO, 0, GPIO_PIN_CONFIG);

    while(1)
    {
        gpio_toggle(B15);
        rt_thread_mdelay(100);
    }
}
