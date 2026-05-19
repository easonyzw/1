#include "soft_layer_rs485.h"    // 软层 RS485 协议相关声明

#include "Hardware_usart.h"      // 底层串口发送函数、DMA 相关声明

#include "string.h"              // memset 所需头文件


// 02 功能码发送缓冲区
// 含义大致是：地址、功能码0x02、数据长度/类型信息、模块类型、预留、CRC
unsigned char rs485_send_02_buf[13] =
{
    0x00, 0x02, 0x00, 0x07, IO_I16, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// 04 功能码发送缓冲区
// 含义大致是：地址、功能码0x04、长度、模块类型、输入类型、变化类型、参数区、16路输入状态、CRC
unsigned char rs485_send_04_buf[15] =
{
    0x00, 0x04, 0x00, 0x08, IO_I16, IO_input, bool_change, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// 05 功能码发送缓冲区
// 含义大致是：地址、功能码0x05、预留、站号、CRC
unsigned char rs485_send_05_buf[7] =
{
    0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00
};



/******************************************************************
 * @brief  发送 02 功能码应答帧
 * @input  none
 * @return none
 ******************************************************************/
void rs485_send_02(void)
{
    unsigned short temp;   // 保存 CRC16 计算结果
  
    temp = crc16(rs485_send_02_buf, 11);   // 对前 11 个字节做 CRC16 计算
	
    rs485_send_02_buf[11] = ((temp & 0xFF00) >> 8);     // CRC 高字节放入发送缓冲区
    rs485_send_02_buf[12] = (unsigned char)(temp & 0x00FF); // CRC 低字节放入发送缓冲区

    usart1_send_data(rs485_send_02_buf, 13);   // 通过 USART1 + DMA 发送 13 字节
}



/******************************************************************
 * @brief  发送 04 功能码应答帧
 * @input  p : 指向两个字节参数的指针（一般来自主站命令帧中的某两个字节）
 * @return none
 ******************************************************************/
void rs485_send_04(unsigned char p[2])
{
    unsigned short temp;   // 保存 CRC16 计算结果
  
    rs485_send_04_buf[7] = p[0];   // 把主站发来的第1个参数写入 04 帧的第 7 字节
    rs485_send_04_buf[8] = p[1];   // 把主站发来的第2个参数写入 04 帧的第 8 字节

    temp = crc16(rs485_send_04_buf, 13);   // 对前 13 个字节做 CRC16 计算

    rs485_send_04_buf[13] = ((temp & 0xFF00) >> 8);      // CRC 高字节
    rs485_send_04_buf[14] = (unsigned char)(temp & 0x00FF); // CRC 低字节

    usart1_send_data(rs485_send_04_buf, 15);   // 通过 USART1 + DMA 发送 15 字节	
}



/******************************************************************
 * @brief  发送 05 功能码应答帧
 * @input  none
 * @return none
 ******************************************************************/
void rs485_send_05(void)
{
    unsigned short temp;   // 保存 CRC16 计算结果
  
    rs485_send_05_buf[4] = modbus_management.station_address;   // 把本站地址写入 05 帧的第 4 字节
	
    temp = crc16(rs485_send_05_buf, 5);   // 对前 5 个字节做 CRC16 校验
          
    rs485_send_05_buf[5] = ((temp & 0xFF00) >> 8);      // CRC 高字节
    rs485_send_05_buf[6] = (unsigned char)(temp & 0x00FF); // CRC 低字节

    usart1_send_data(rs485_send_05_buf, 7);   // 通过 USART1 + DMA 发送 7 字节
}



/******************************************************************
 * @brief  RS485 接收数据处理函数
 * @input  none
 * @return none
 ******************************************************************/
void rs485_receive_data_handler(void)
{
    unsigned short temp = 0;   // 保存 CRC16 计算结果
  
    // 先判断收到的帧是不是发给本机地址的
    if(modbus_management.receiveBuffer[0] == modbus_management.station_address)
    {
        // 根据功能码进入不同处理分支
        switch(modbus_management.receiveBuffer[1])
        {
            case 0x01:   // 收到 0x01 功能码命令
            {
                // 判断命令格式中的某个长度/参数字段是否为 0
                if(modbus_management.receiveBuffer[3] == 0)
                {
                    temp = crc16(modbus_management.receiveBuffer, 4);   // 对前 4 字节做 CRC16
                    
                    // 判断接收到的 CRC 是否正确
                    if(((temp & 0xFF00) >> 8) == modbus_management.receiveBuffer[4] && 
                       (unsigned char)(temp & 0x00FF) == modbus_management.receiveBuffer[5]) 
                    {
                        rs485_send_02();   // 校验通过，则发送 02 功能码应答帧
                    }
                }
                break;
            }

            case 0x03:   // 收到 0x03 功能码命令
            {
                // 判断长度字段是否为 8，并且模块类型是否匹配 IO_I16
                if(modbus_management.receiveBuffer[3] == 8 && 
                   modbus_management.receiveBuffer[4] == IO_I16)
                {
                    temp = crc16(modbus_management.receiveBuffer, 13);   // 对前 13 字节做 CRC16
          
                    // 判断接收到的 CRC 是否正确
                    if(((temp & 0xFF00) >> 8) == modbus_management.receiveBuffer[13] &&
                       (unsigned char)(temp & 0x00FF) == modbus_management.receiveBuffer[14])
                    {
                        rs485_send_04(&modbus_management.receiveBuffer[7]);  
                        // CRC 正确，则发送 04 功能码应答帧
                        // 并把接收帧中第 7、8 字节作为参数带回去
                    }
                }
                break;
            }

            default:   // 其他功能码暂不处理
            {
                break;
            }
        }

        memset(modbus_management.receiveBuffer, 0, sizeof(modbus_management.receiveBuffer));
        // 清空接收缓冲区，防止旧数据残留影响下一帧解析

        DMA_SetCurrDataCounter(DMA1_Channel3, USART1_RECEIVE_BUFFER_SIZE);
        // 重装 DMA1 通道3 的接收计数值，准备接收下一帧

        modbus_management.receiveBuffer[0] = 0;   // 再次手动清首字节（这里有点重复，但作者为了保险）
        modbus_management.receiveBuffer[1] = 0;   // 再次手动清功能码字节
    }
}