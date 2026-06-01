#ifndef __SOFT_LAYER_DATA_H
#define __SOFT_LAYER_DATA_H

#include "Hardware.h"

/******************************* Submodule type ********************************
 * 标识各个子模块的硬件类型，用于与主站通信时上报模块能力。
 *****************************************************************************/
#define IO_NP8        0x02   /* 8 路 NPN 输出                          */
#define IO_I4NP4      0x03   /* 4 路输入 + 4 路 NPN 输出               */
#define AD_VI4        0x04   /* 4 路电压型模拟量输入                    */
#define AD_VQ4        0x05   /* 4 路电压型模拟量输出(? 具体待确认)       */
#define AD_VI2Q2      0x06   /* 2 路电压输入 + 2 路电压输出             */
#define R4_RS485      0x08   /* 4 路 RS485                             */
#define R2_RS232      0x09   /* 2 路 RS232                             */
#define DA_CQ4        0x0A   /* 4 路电流型模拟量输出                    */
#define IO_I16        0x1D   /* 16 路数字量输入（本模块）               */
#define IO_NP16       0x0C   /* 16 路 NPN 输出                          */
#define IO_RE16       0x0D   /* 16 路继电器输出                         */
#define IO_RE8        0x0E   /* 8 路继电器输出                          */
#define IO_I4RE4      0x0F   /* 4 路输入 + 4 路继电器输出               */
#define IO_I8         0x1A   /* 8 路数字量输入                          */

/******************************* Device type **********************************
 * 标识数据点的物理类型，用于 Modbus 等协议中的数据类型描述。
 *****************************************************************************/
#define Analog_input_V    0x01   /* 电压型模拟量输入                      */
#define Analog_input_C    0x02   /* 电流型模拟量输入                      */
#define IO_input          0x03   /* 数字量输入（本模块使用）              */
#define IO_output         0x04   /* 数字量输出                            */
#define Analog_output_V   0x05   /* 电压型模拟量输出                      */
#define Analog_output_C   0x06   /* 电流型模拟量输出                      */
#define Business          0xFE   /* 业务/管理类数据帧                      */

/**************************** Trigger event type *****************************
 * 标识触发本次数据上报的事件类型。
 *****************************************************************************/
#define Unchanged_without_updating     0x00   /* 无变化，不更新             */
#define Unchanged_and_updated          0x01   /* 无变化，但仍上报更新        */
#define bool_change                    0x02   /* 数字量状态变化             */
#define Analog_change                  0x03   /* 模拟量值变化               */
#define Communication_CRC_error        0x04   /* 通信 CRC 校验错误          */
#define Noframe_received_communication 0x05   /* 通信超时未收到帧            */
#define Analog_overflow                0x06   /* 模拟量溢出/超限            */

/***************************** Buffer sizes **********************************/
#define USART1_SEND_BUFFER_SIZE     30
#define USART1_RECEIVE_BUFFER_SIZE  30

/************************ Modbus management **********************************
 * 全局 Modbus 通信管理结构体。
 *
 * IO_Input_L / IO_Input_H 是两个宏，分别映射到 rs485_send_04_buf[11] 和
 * rs485_send_04_buf[12]，即 04 功能码应答帧中"16 路输入状态"的两个字节。
 * 这样 IO 模块更新状态时直接操作这两个字节，RS485 发送时无需额外拷贝。
 *****************************************************************************/
typedef struct
{
    unsigned char  station_address;
    unsigned char  flag_got_pulse;
    unsigned char  receiveBuffer[USART1_RECEIVE_BUFFER_SIZE];
    unsigned char  sendBuffer[USART1_SEND_BUFFER_SIZE];
    unsigned short receive_length;
} sModbus_management;

extern sModbus_management modbus_management;

/*********************** Timer / counter *************************************
 * 软件计时器结构体，由定时器中断驱动递增。
 *
 * - bus:        编址阶段 BUS 脉冲超时计数
 * - IO_filter:  16 路输入各自的滤波计数器（每路独立）
 * - LED_ms:     LED 状态灯的时间基准（ms 级）
 *
 * 注意：IO_filter[] 为 unsigned short（最大 65535），定时器中断频率通常为
 * 1kHz 或更低，实际使用中不会溢出。若修改定时器频率，请注意此上限。
 *****************************************************************************/
typedef struct
{
    unsigned short int   bus;
    unsigned short int   IO_filter[16];
    unsigned int         LED_ms;
} sTimer_counter;

extern sTimer_counter timer_counter;

#endif