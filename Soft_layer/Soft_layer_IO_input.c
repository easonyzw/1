#include "soft_layer_IO_input.h"

sIO_parameter IO_parameter[16];

/*
 * 16 路输入通道和 16 路 LED 的对应关系表。
 *
 * 原来的 IO_state_update() 把 CH0 ~ CH15 分别写了 16 段 if/else，逻辑完全重复。
 * 这里改成“表驱动”：通道的硬件差异放在表里，处理流程统一放到循环里。
 *
 * 注意：本板子的 LED 为低电平点亮，所以 IO_led_set() 内部使用 GPIO_ResetBits() 点亮 LED。
 */
typedef struct
{
    GPIO_TypeDef *input_port;
    unsigned short input_pin;

    GPIO_TypeDef *led_port;
    unsigned short led_pin;

    unsigned char input_byte;  /* 0: IO_Input_L, 1: IO_Input_H */
    unsigned char bit_mask;    /* 对应输入状态字节中的 bit 位 */
} sIO_channel_config;

static const sIO_channel_config IO_channel_table[16] =
{
    /* input port, input pin,  led port, led pin,  byte, bit */
    {GPIOC, GPIO_Pin_6,  GPIOC, GPIO_Pin_2,  0, 0x01},  /* CH0  -> LED0  */
    {GPIOD, GPIO_Pin_2,  GPIOA, GPIO_Pin_0,  0, 0x02},  /* CH1  -> LED1  */
    {GPIOB, GPIO_Pin_15, GPIOA, GPIO_Pin_2,  0, 0x04},  /* CH2  -> LED2  */
    {GPIOB, GPIO_Pin_3,  GPIOA, GPIO_Pin_4,  0, 0x08},  /* CH3  -> LED3  */
    {GPIOA, GPIO_Pin_11, GPIOA, GPIO_Pin_6,  0, 0x10},  /* CH4  -> LED4  */
    {GPIOC, GPIO_Pin_12, GPIOC, GPIO_Pin_4,  0, 0x20},  /* CH5  -> LED5  */
    {GPIOA, GPIO_Pin_8,  GPIOB, GPIO_Pin_1,  0, 0x40},  /* CH6  -> LED6  */
    {GPIOB, GPIO_Pin_14, GPIOB, GPIO_Pin_10, 0, 0x80},  /* CH7  -> LED7  */

    {GPIOA, GPIO_Pin_15, GPIOC, GPIO_Pin_3,  1, 0x01},  /* CH8  -> LED8  */
    {GPIOC, GPIO_Pin_9,  GPIOA, GPIO_Pin_1,  1, 0x02},  /* CH9  -> LED9  */
    {GPIOF, GPIO_Pin_7,  GPIOA, GPIO_Pin_3,  1, 0x04},  /* CH10 -> LED10 */
    {GPIOC, GPIO_Pin_8,  GPIOA, GPIO_Pin_5,  1, 0x08},  /* CH11 -> LED11 */
    {GPIOF, GPIO_Pin_6,  GPIOA, GPIO_Pin_7,  1, 0x10},  /* CH12 -> LED12 */
    {GPIOB, GPIO_Pin_13, GPIOB, GPIO_Pin_0,  1, 0x20},  /* CH13 -> LED13 */
    {GPIOC, GPIO_Pin_7,  GPIOB, GPIO_Pin_2,  1, 0x40},  /* CH14 -> LED14 */
    {GPIOB, GPIO_Pin_12, GPIOB, GPIO_Pin_11, 1, 0x80}   /* CH15 -> LED15 */
};

static void IO_led_set(unsigned char channel, unsigned char on)
{
    if(on)
    {
        GPIO_ResetBits(IO_channel_table[channel].led_port, IO_channel_table[channel].led_pin);
    }
    else
    {
        GPIO_SetBits(IO_channel_table[channel].led_port, IO_channel_table[channel].led_pin);
    }
}

static void IO_status_bit_set(unsigned char channel, unsigned char high_level)
{
    unsigned char clear_mask;

    clear_mask = (unsigned char)(~IO_channel_table[channel].bit_mask);

    if(IO_channel_table[channel].input_byte == 0)
    {
        if(high_level)
        {
            IO_Input_L |= IO_channel_table[channel].bit_mask;
        }
        else
        {
            IO_Input_L &= clear_mask;
        }
    }
    else
    {
        if(high_level)
        {
            IO_Input_H |= IO_channel_table[channel].bit_mask;
        }
        else
        {
            IO_Input_H &= clear_mask;
        }
    }
}

static void IO_channel_update(unsigned char channel)
{
    unsigned char input_state;

    if(timer_counter.IO_filter[channel] < IO_parameter[channel].filter)
    {
        return;
    }

    input_state = GPIO_ReadInputDataBit(IO_channel_table[channel].input_port,
                                        IO_channel_table[channel].input_pin);

    if(input_state == SET)
    {
        IO_led_set(channel, 1);
        IO_status_bit_set(channel, 1);
    }
    else
    {
        IO_led_set(channel, 0);
        IO_status_bit_set(channel, 0);
    }

    timer_counter.IO_filter[channel] = 0;
}

/******************************************************************
 * @brief  digital input data initialize
 * @input  none
 * @return none
 ******************************************************************/
void digital_input_initialization(void)
{
    unsigned char i;

    for(i = 0; i < 16; i++)
    {
        IO_parameter[i].filter = 1;
    }
}

/******************************************************************
 * @brief  Update 16-channel input status and related LEDs
 * @input  none
 * @return none
 ******************************************************************/
void IO_state_update(void)
{
    unsigned char i;

    for(i = 0; i < 16; i++)
    {
        IO_channel_update(i);
    }

    module_show_status_running();
}

/******************************************************************
 * @brief  LED status of station number waiting
 * @input  none
 * @return none
 ******************************************************************/
void module_show_status(void)
{
    if(timer_counter.LED_ms > 500)
    {
        LED_all_close();
        timer_counter.LED_ms = 0;
    }
    else if(timer_counter.LED_ms > 250)
    {
        LED_all_open();
    }
}

void module_show_status_running(void)
{
    if((IO_Input_L != 0) || (IO_Input_H != 0))
    {
        return;
    }

    if(timer_counter.LED_ms > 5200)
    {
        LED_all_close();
        timer_counter.LED_ms = 0;
    }
    else if(timer_counter.LED_ms > 5000)
    {
        LED_all_open();
    }
}

void LED_all_open(void)
{
    unsigned char i;

    for(i = 0; i < 16; i++)
    {
        IO_led_set(i, 1);
    }
}

void LED_all_close(void)
{
    unsigned char i;

    for(i = 0; i < 16; i++)
    {
        IO_led_set(i, 0);
    }
}
