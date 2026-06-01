#ifndef __SOFT_LAYER_RS485_H
#define __SOFT_LAYER_RS485_H

#include "Hardware.h"

/*
 * IO_Input_L / IO_Input_H 宏说明：
 *
 * 这两个宏将 16 路数字输入状态直接映射到 04 功能码应答帧缓冲区的对应字节。
 * - IO_Input_L → rs485_send_04_buf[11]: CH0~CH7  的输入状态（bit0~bit7）
 * - IO_Input_H → rs485_send_04_buf[12]: CH8~CH15 的输入状态（bit0~bit7）
 *
 * 设计意图：Soft_layer_IO_input.c 中更新输入状态时，通过 IO_status_bit_set()
 * 直接操作 rs485_send_04_buf 的这两个字节。这样 RS485 模块发送 04 应答帧时，
 * 无需再做状态拷贝，减少了一次内存搬运。
 */
#define IO_Input_L  rs485_send_04_buf[11]
#define IO_Input_H  rs485_send_04_buf[12]

/* RS485 发送缓冲区 — 各功能码固定帧格式所需字节数 */
extern unsigned char rs485_send_02_buf[13];
extern unsigned char rs485_send_04_buf[15];
extern unsigned char rs485_send_05_buf[7];

/* RS485 通信接口 */
void rs485_send_02(void);
void rs485_send_04(unsigned char p[2]);
void rs485_send_05(void);
void rs485_receive_data_handler(void);

#endif



