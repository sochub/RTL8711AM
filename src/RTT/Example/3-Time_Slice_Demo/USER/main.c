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
rt_uint32_t count = 0;
//-------------------------------------------------------------------------------------------------------------------
//  @brief      线程1入口函数
//  @param      parameter   参数
//  @return     void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void thread_slice_10_entry(void *parameter)
{
    rt_thread_mdelay(10);           //等待FINSH初始化完毕

    while(1)
    {
        if(0 == (count % 5))
        {
            rt_enter_critical();
            rt_kprintf("time slice 10 is running, count = %d\n", count);
            rt_exit_critical();
            if(200 <= count)
            {
                return;
            }
        }
        count++;
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      线程2入口函数
//  @param      parameter   参数
//  @return     void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void thread_slice_20_entry(void *parameter)
{
    rt_thread_mdelay(10);           //等待FINSH初始化完毕
    while(1)
    {
        if(0 == (count % 5))
        {
            rt_enter_critical();
            rt_kprintf("time slice 20 is running, count = %d\n", count);
            rt_exit_critical();
            if(200 <= count)
            {
                break;
            }
        }
        count++;
    }
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      线程创建以及启动
//  @param      void
//  @return     void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
int time_slice_20_thread_example(void)
{
    //线程控制块指针
    rt_thread_t tid1;
    //创建动态线程
    tid1 = rt_thread_create("thread_20",    //线程名称
            thread_slice_20_entry,          //线程入口函数
            RT_NULL,                        //线程参数
            512,                            //512个字节的栈空间
            5,                              //线程优先级为5，数值越小，优先级越高，0为最高优先级。
                                            //可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
            10);                            //时间片

    rt_kprintf("create time slice 20 thread...\n\n");
    if(tid1 != RT_NULL)                     //线程创建成功
    {
        rt_kprintf("time slice 20 thread create OK...\n\n");
        rt_thread_startup(tid1);            //运行该线程
    }
    else                                    //线程创建失败
    {
        rt_kprintf("time slice 20 thread create ERROR...\n\n");
        return 1;
    }

    return 0;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      动态线程创建以及启动
//  @param      void        空
//  @return     void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
int time_slice_10_thread_example(void)
{
    //线程控制块指针
    rt_thread_t tid1;
    //创建动态线程
    tid1 = rt_thread_create("thread_10",    //线程名称
            thread_slice_10_entry,          //线程入口函数
            RT_NULL,                        //线程参数
            512,                            //512个字节的栈空间
            5,                              //线程优先级为5，数值越小，优先级越高，0为最高优先级。
                                            //可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
            20);                            //时间片为20

    rt_kprintf("create time slice 10 thread...\n\n");
    if(tid1 != RT_NULL)                     //线程创建成功
    {
        rt_kprintf("time slice 10 thread create OK...\n\n");
        rt_thread_startup(tid1);            //运行该线程
    }
    else                                    //线程创建失败
    {
        rt_kprintf("time slice 10 thread create ERROR...\n\n");
        return 1;
    }

    return 0;
}


//使用INIT_APP_EXPORT宏自动初始化，也可以通过在其他线程内调用time_slice_10_thread_example函数进行初始化
INIT_APP_EXPORT(time_slice_10_thread_example);
//使用INIT_APP_EXPORT宏自动初始化，也可以通过在其他线程内调用time_slice_20_thread_example函数进行初始化
INIT_APP_EXPORT(time_slice_20_thread_example);

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



