#include "Soft_layer_bus.h"

/******************************************************************
 * @brief   Station number initialization
 * @input   none
 * @return  none
******************************************************************/
void station_number_initialization(void)
{
	receive_pulse_station_number();   // 第一步：通过 BUS 脉冲接收本站站号（统计脉冲个数得到地址）
    last_station_handle();            // 第二步：判断自己是不是最后一个模块，并做后续处理
}

/******************************************************************
 * @brief   Receive station number
 * @input   none
 * @return  none
******************************************************************/
void receive_pulse_station_number(void)
{
    memset(&timer_counter, NULL, sizeof(timer_counter));         // 清零软件计时器结构体（bus、IO_filter、LED_ms 全部清零）
    memset(&modbus_management, NULL, sizeof(modbus_management)); // 清零通信管理结构体（站号、脉冲标志、收发缓冲等全部清零）
  
	while(1)
	{
        module_show_status();   // 编址等待期间的 LED 状态显示（一般为快闪，表示当前处于编址中）

        if(modbus_management.flag_got_pulse == 1)   // 如果已经收到过脉冲（该标志通常在 EXTI 中断里被置位）
        {
            // 这里故意什么都不做
            // 意图是：一旦收到了脉冲，就让 timer_counter.bus 继续自然累加
            // 通过“bus 计时器是否超时”来判断脉冲序列是否结束
        }
        else
        {
            timer_counter.bus = 0;   // 如果当前还没有收到脉冲，则把 bus 计时清零，表示仍在等待第一个脉冲
        }

        if(timer_counter.bus > 10)   // 如果距离上一次脉冲已经过去了足够长时间（约 10 个定时节拍）
        {
            return;   // 认为本次 BUS 脉冲输入已经结束，本站地址接收完成，退出函数
        }            
	}
}

/******************************************************************
 * @brief   Last station handle
 * @input   none
 * @return  none
******************************************************************/
void last_station_handle(void)
{
  
    unsigned char i = 0;   // 循环变量，用于后面发送若干个脉冲
  
	GPIO_InitTypeDef GPIO_InitStructure;   // GPIO 初始化结构体，用于把 PB9 改成输出模式

    rs485_send_02_buf[0] = modbus_management.station_address;   // 把本站地址写入 02 功能码发送缓冲区首字节
    rs485_send_04_buf[0] = modbus_management.station_address;   // 把本站地址写入 04 功能码发送缓冲区首字节
    rs485_send_05_buf[0] = modbus_management.station_address;   // 把本站地址写入 05 功能码发送缓冲区首字节
  
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 1)   // 先读取 PB9 当前电平：尝试判断后面是否还有模块
	{
        delay_ms(5);   // 延时 5ms，让 PB9 电平稳定，避免误判（简单防抖/再确认）
    
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 1)   // 再读一次，如果仍然为高电平
		{
            /* Send 05 function code */ 
			rs485_send_05();   // 认为自己是最后一个模块，发送 05 功能码帧，向主站报告末站信息
		}
        else
        {
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;          // 选择 PB9 引脚
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      // 配置为普通输出模式
            GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     // 推挽输出
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // 输出速度 50MHz
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       // 上拉
            GPIO_Init(GPIOB, &GPIO_InitStructure);             // 初始化 PB9 为输出，用于向后级发送 BUS 脉冲
        
            for(i = 0; i < modbus_management.station_address + 1; i++)   // 按“本站地址+1”的脉冲个数向后传递
            {
                BUS_output = 1;   // 输出高电平脉冲
                delay_us(50);     // 高电平保持 50us
          
                BUS_output = 0;   // 输出低电平
                delay_us(50);     // 低电平保持 50us	 
            }
        }
	}
    else
    {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;          // 如果 PB9 一开始就不是高电平，直接认为后面还有模块
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      // 配置 PB9 为普通输出
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     // 推挽输出
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // 输出速度 50MHz
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       // 上拉
        GPIO_Init(GPIOB, &GPIO_InitStructure);             // 初始化 PB9
        
        for(i = 0; i < modbus_management.station_address + 1; i++)   // 向后级继续发送“本站地址+1”个脉冲
        {
            BUS_output = 1;   // 输出高电平
            delay_us(50);     // 保持 50us
      
            BUS_output = 0;   // 拉低
            delay_us(50);     // 保持 50us	 
        }
    
    }
}