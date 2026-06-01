#include "Soft_layer_bus.h"

/********************** ****************************************************************************
 * @brief   Station number initialization
 * @input   none
 * @return  none
******************************************************************/
void station_number_initialization(void)
{
    receive_pulse_station_number();
    last_station_handle();
}

/******************************************************************
 * @brief   Receive station number
 * @input   none
 * @return  none
******************************************************************/
void receive_pulse_station_number(void)
{
    memset(&timer_counter, 0, sizeof(timer_counter));
    memset(&modbus_management, 0, sizeof(modbus_management));

    while(1)
    {
        module_show_status();

        if(modbus_management.flag_got_pulse == 0)
        {
            timer_counter.bus = 0;
        }

        if(timer_counter.bus > 10)
        {
            return;
        }
    }
}

/******************************************************************
 * @brief   Configure PB9 as push-pull output for BUS pulse train
 * @input   none
 * @return  none
******************************************************************/
static void bus_output_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/******************************************************************
 * @brief   Send BUS pulse train to next module
 * @input   pulse_count: number of pulses to send
 * @return  none
******************************************************************/
static void bus_send_pulse_train(unsigned char pulse_count)
{
    unsigned char i;

    for(i = 0; i < pulse_count; i++)
    {
        BUS_output = 1;
        delay_us(50);

        BUS_output = 0;
        delay_us(50);
    }
}

/******************************************************************
 * @brief   Last station handle
 * @input   none
 * @return  none
******************************************************************/
void last_station_handle(void)
{
    rs485_send_02_buf[0] = modbus_management.station_address;
    rs485_send_04_buf[0] = modbus_management.station_address;
    rs485_send_05_buf[0] = modbus_management.station_address;

    /* 读取 PB9 电平判断后面是否还有级联模块 */
    if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 1)
    {
        delay_ms(5);

        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 1)
        {
            /* 自己是最后一个模块，发送 05 帧报告末站 */
            rs485_send_05();
            return;
        }
    }

    /* 后面还有模块，配置 PB9 为输出并向后级发送脉冲 */
    bus_output_init();
    bus_send_pulse_train((unsigned char)(modbus_management.station_address + 1));
}