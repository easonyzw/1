#include "Hardware_usart.h"   // 包含本文件需要的串口、DMA、RS485、Modbus相关声明
#include "string.h"           // 使用 memcpy() 需要包含 string.h

// 定义一个全局的 Modbus 管理结构体
// 里面一般包含：发送缓冲区、接收缓冲区、接收长度等
sModbus_management    modbus_management;


/******************************************************************
 * @brief   RS485 / USART1 初始化函数
 * @input   none
 * @return  none
 *
 * 作用：
 * 1. 初始化 USART1 的 TX/RX/DE 引脚
 * 2. 配置 USART1 波特率、数据位、停止位等参数
 * 3. 开启 USART1 的 RS485 硬件 DE 自动方向控制
 * 4. 开启 USART1 的 IDLE 空闲中断
 * 5. 开启 USART1 的 DMA 收发功能
 * 6. 配置 DMA1_Channel2 作为串口发送 DMA
 * 7. 配置 DMA1_Channel3 作为串口接收 DMA
******************************************************************/
void RS485_initializes(void)
{
    USART_InitTypeDef USART_InitStructure;   // USART 初始化结构体
    GPIO_InitTypeDef  GPIO_InitStructure;    // GPIO 初始化结构体

    // 开启 USART1 外设时钟
    // USART1 挂在 APB2 总线上
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    // 配置 GPIOA 的复用功能
    // PA9  -> USART1_TX
    // PA10 -> USART1_RX
    // PA12 -> USART1_DE，RS485方向控制脚
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_1);

    // 配置 PA9、PA10、PA12 为复用功能模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_12;

    // 设置为复用功能模式，由 USART1 外设接管这些引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

    // 设置 GPIO 输出速度为 50MHz
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    // 设置输出类型为推挽输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

    // 设置上拉
    // 对串口 RX 来说，上拉可以避免空闲时引脚悬空
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

    // 初始化 GPIOA
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 开启 USART 8倍过采样
    // 8倍过采样可以支持更高波特率
    USART_OverSampling8Cmd(USART1, ENABLE);

    // 设置 USART1 波特率为 6000000，也就是 6Mbps
    USART_InitStructure.USART_BaudRate = 6000000;

    // 设置数据位长度为 8 位
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;

    // 设置停止位为 1 位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;

    // 不使用奇偶校验
    USART_InitStructure.USART_Parity = USART_Parity_No;

    // 不使用硬件流控
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

    // 同时开启接收和发送功能
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    // 将上面的配置写入 USART1
    USART_Init(USART1, &USART_InitStructure);


    /**************** RS485 硬件 DE 自动控制配置 ****************/

    // 使能 USART1 的 DE 功能
    // DE 是 Driver Enable，用于控制 RS485 收发方向
    // 使能后，USART1 在发送数据时会自动控制 PA12
    USART_DECmd(USART1, ENABLE);

    // 设置 DE 提前拉高时间
    // 发送数据前，DE 会提前一段时间有效
    USART_SetDEAssertionTime(USART1, 0x1F);

    // 设置 DE 有效电平为高电平
    // 即 PA12 = 1 时，RS485 芯片进入发送模式
    USART_DEPolarityConfig(USART1, USART_DEPolarity_High);

    /************************************************************/


    // 不使用 RXNE 中断
    // RXNE 是每收到一个字节就进一次中断
    // 这里用 DMA 接收，所以不需要 RXNE 中断
//  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    // 开启 USART1 空闲中断
    // 当串口接收完一串数据后，线路空闲，会触发 IDLE 中断
    // 这个常用来判断“一帧数据接收完成”
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

    // 使能 USART1 的 DMA 请求
    // USART_DMAReq_Rx：允许 USART1 接收数据时请求 DMA
    // USART_DMAReq_Tx：允许 USART1 发送数据时请求 DMA
    USART_DMACmd(USART1, USART_DMAReq_Rx | USART_DMAReq_Tx, ENABLE);

    // 使能 USART1
    USART_Cmd(USART1, ENABLE);

    // 清除发送完成标志 TC, 上电初始化
    USART_ClearFlag(USART1, USART_FLAG_TC);

    // 配置 DMA1_Channel2 作为 USART1 发送 DMA
    // 数据流向：modbus_management.sendBuffer -> USART1->TDR
    // TDR 是 USART 的发送数据寄存器
    DMA_Config_TX(DMA1_Channel2,
                  (unsigned int)&USART1->TDR,
                  (unsigned int)modbus_management.sendBuffer,
                  USART1_SEND_BUFFER_SIZE);

    // 配置 DMA1_Channel3 作为 USART1 接收 DMA
    // 数据流向：USART1->RDR -> modbus_management.receiveBuffer
    // RDR 是 USART 的接收数据寄存器
    DMA_Config_RX(DMA1_Channel3,
                  (unsigned int)&USART1->RDR,
                  (unsigned int)modbus_management.receiveBuffer,
                  USART1_RECEIVE_BUFFER_SIZE);
}


/******************************************************************
 * @brief   DMA 发送通道配置函数
 * @input   DMA_CHx : DMA 通道，例如 DMA1_Channel2
 *          cpar    : 外设地址，例如 USART1->TDR 地址
 *          cmar    : 内存地址，例如 sendBuffer 首地址
 *          cndtr   : DMA 传输数量
 * @return  none
 *
 * 作用：
 * 配置 DMA，把内存中的发送缓冲区数据搬运到 USART1 的发送寄存器 TDR。
 *
 * 对应数据方向：
 * sendBuffer  --->  USART1->TDR  --->  TX 引脚发送出去
******************************************************************/
void DMA_Config_TX(DMA_Channel_TypeDef* DMA_CHx,
                   unsigned int cpar,
                   unsigned int cmar,
                   unsigned short int cndtr)
{
    DMA_InitTypeDef DMA_InitStructure;   // DMA 初始化结构体

    // 开启 DMA1 时钟
    // DMA1 挂在 AHB 总线上
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    // 设置 DMA 外设地址
    // 发送时，外设地址是 USART1->TDR
    DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;

    // 设置 DMA 内存地址
    // 发送时，内存地址是 sendBuffer
    DMA_InitStructure.DMA_MemoryBaseAddr = cmar;

    // 设置 DMA 方向：内存 -> 外设
    // PeripheralDST 表示外设是目的地
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;

    // 设置传输数量
    // 表示本次 DMA 要搬运多少个数据单元
    DMA_InitStructure.DMA_BufferSize = cndtr;

    // 外设地址不自增
    // 因为始终写入 USART1->TDR 这一个寄存器
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

    // 内存地址自增
    // 因为 sendBuffer 中的数据要一个字节一个字节往后取
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

    // 外设数据宽度为字节
    // USART 发送寄存器按 8 位数据发送
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

    // 内存数据宽度为字节
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;

    // 普通模式
    // 发送完指定长度后 DMA 自动停止
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;

    // DMA 优先级设置为很高
    // 因为波特率较高，优先级高可以降低数据搬运不及时的风险
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;

    // 关闭内存到内存模式
    // 这里是内存到外设，不是内存到内存
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    // 初始化指定 DMA 通道
    DMA_Init(DMA_CHx, &DMA_InitStructure);
}


/******************************************************************
 * @brief   DMA 接收通道配置函数
 * @input   DMA_CHx : DMA 通道，例如 DMA1_Channel3
 *          cpar    : 外设地址，例如 USART1->RDR 地址
 *          cmar    : 内存地址，例如 receiveBuffer 首地址
 *          cndtr   : DMA 接收缓冲区大小
 * @return  none
 *
 * 作用：
 * 配置 DMA，把 USART1 接收到的数据自动搬运到 receiveBuffer 中。
 *
 * 对应数据方向：
 * RX 引脚收到数据 ---> USART1->RDR ---> receiveBuffer
******************************************************************/
void DMA_Config_RX(DMA_Channel_TypeDef* DMA_CHx,
                   unsigned int cpar,
                   unsigned int cmar,
                   unsigned short int cndtr)
{
    DMA_InitTypeDef DMA_InitStructure;   // DMA 初始化结构体

    // 将指定 DMA 通道恢复到默认状态
    DMA_DeInit(DMA_CHx);

    // 设置 DMA 外设地址
    // 接收时，外设地址是 USART1->RDR
    DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;

    // 设置 DMA 内存地址
    // 接收时，内存地址是 receiveBuffer
    DMA_InitStructure.DMA_MemoryBaseAddr = cmar;

    // 设置 DMA 方向：外设 -> 内存
    // PeripheralSRC 表示外设是数据来源
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;

    // 设置 DMA 接收缓冲区大小
    DMA_InitStructure.DMA_BufferSize = cndtr;

    // 外设地址不自增
    // 因为始终从 USART1->RDR 这一个寄存器读取数据
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

    // 内存地址自增
    // 每收到一个字节，就依次存入 receiveBuffer 的下一个位置
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

    // 外设数据宽度为字节
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

    // 内存数据宽度为字节
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;

    // 循环模式
    // 接收缓冲区收满后，DMA 会从缓冲区开头继续接收
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;

    // 设置 DMA 优先级为很高
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;

    // 关闭内存到内存模式
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    // 初始化指定 DMA 通道
    DMA_Init(DMA_CHx, &DMA_InitStructure);

    // 使能 DMA 接收通道
    // 开启后，USART1 收到数据时，DMA 会自动搬运到 receiveBuffer
    DMA_Cmd(DMA_CHx, ENABLE);
}


/******************************************************************
 * @brief   重新启动 DMA 通道
 * @input   DMA_CHx : DMA 通道
 *          CNDTR   : 本次 DMA 要传输的数据数量
 * @return  none
 *
 * 作用：
 * 1. 关闭 DMA 通道
 * 2. 重新设置 CNDTR 传输数量
 * 3. 再次打开 DMA 通道
 *
 * 常用于：
 * 1. 发送前重新设置发送长度
 * 2. 接收完一帧后，重新装载接收缓冲区长度
******************************************************************/
void DMA_Enable(DMA_Channel_TypeDef* DMA_CHx, u16 CNDTR)
{
    // 关闭 DMA 通道
    // CCR 的 bit0 是 EN 位，清 0 表示关闭 DMA
    DMA_CHx->CCR &= ~(1 << 0);

    // 设置 DMA 本次传输数量
    // 发送时表示要发送多少字节
    // 接收时表示接收缓冲区大小
    DMA_CHx->CNDTR = CNDTR;

    // 重新开启 DMA 通道
    // CCR 的 bit0 置 1 表示使能 DMA
    DMA_CHx->CCR |= 1 << 0;
}


/******************************************************************
 * @brief   RS485 发送函数
 * @input   data : 要发送的数据首地址
 *          len  : 要发送的数据长度
 * @return  none
 *
 * 作用：
 * 将 data 指向的数据复制到 sendBuffer，
 * 然后启动 DMA1_Channel2，把数据通过 USART1 发送出去。
 *
 * 发送路径：
 * data -> sendBuffer -> DMA1_Channel2 -> USART1->TDR -> USART1_TX -> RS485总线
******************************************************************/
void usart1_send_data(unsigned char* data, unsigned char len)
{
    // 清除 DMA1_Channel2 的传输完成标志
    // Channel2 是 USART1_TX 对应的 DMA 通道
    DMA_ClearFlag(DMA1_FLAG_TC2);

    // 发送前先关闭 DMA1_Channel2
    // 修改 CNDTR 传输数量之前，必须先关闭 DMA
    DMA_Cmd(DMA1_Channel2, DISABLE);

    // 将用户要发送的数据复制到 Modbus 发送缓冲区
    // 注意：这里没有判断 len 是否超过 sendBuffer 大小
    memcpy(modbus_management.sendBuffer, data, len);

    // 重新设置 DMA 发送长度，并启动 DMA 发送
    // DMA 会自动把 sendBuffer 中的 len 个字节搬到 USART1->TDR
    DMA_Enable(DMA1_Channel2, len);
}


/******************************************************************
 * @brief   USART1 中断服务函数
 * @input   none
 * @return  none
 *
 * 作用：
 * 主要处理 USART1 的 IDLE 空闲中断。
 *
 * 接收逻辑：
 * 1. USART1 接收到数据后，DMA 自动搬运到 receiveBuffer
 * 2. 当一帧数据结束，串口线路空闲，产生 IDLE 中断
 * 3. 在中断中关闭 DMA
 * 4. 根据 DMA 剩余计数计算本次接收长度
 * 5. 重新启动 DMA，准备接收下一帧
 * 6. 如果长度有效，调用 rs485_receive_data_handler() 解析数据
******************************************************************/
void USART1_IRQHandler(void)
{
    // 临时变量
    // 用于读取 USART 状态寄存器和数据寄存器，从而清除 IDLE 标志
    volatile unsigned char temp;

    // 判断 USART1 是否产生 IDLE 空闲中断
    // IDLE 通常表示一帧数据接收结束
    if(USART_GetFlagStatus(USART1, USART_FLAG_IDLE) != RESET)
    {
        // 关闭 DMA1_Channel3 接收通道
        // 目的是冻结当前 DMA 接收状态，避免后面计算长度时数据继续变化
        DMA_Cmd(DMA1_Channel3, DISABLE);

        // 计算本次实际接收到的数据长度
        //
        // USART1_RECEIVE_BUFFER_SIZE 是接收缓冲区总大小
        // DMA_GetCurrDataCounter() 返回 DMA 当前还剩多少个数据没接收
        //
        // 实际接收长度 = 总长度 - 剩余未接收长度
        modbus_management.receive_length =
            USART1_RECEIVE_BUFFER_SIZE - DMA_GetCurrDataCounter(DMA1_Channel3);

        // 重新装载 DMA 接收长度，并重新开启 DMA1_Channel3
        // 这样可以准备接收下一帧数据
        DMA_Enable(DMA1_Channel3, USART1_RECEIVE_BUFFER_SIZE);

        // 清除 USART1 的 IDLE 中断挂起标志
        USART_ClearITPendingBit(USART1, USART_IT_IDLE);

        // 读取 USART 状态寄存器
        // 某些 STM32 清除 IDLE 标志需要先读状态寄存器
        temp = USART1->ISR;

        // 再读取接收数据寄存器
        // 配合读取 ISR，用于彻底清除 IDLE 状态
        temp = USART1->RDR;

        // 判断接收到的数据长度是否大于 5
        // 大于 5 才认为可能是一帧有效 Modbus / RS485 数据
        if(modbus_management.receive_length > 5)
        {
            // 调用 RS485 接收数据处理函数
            // 这里面一般会做：
            // 1. 地址判断
            // 2. 功能码解析
            // 3. CRC校验
            // 4. 根据命令执行对应操作
            // 5. 必要时组织应答数据并发送
            rs485_receive_data_handler();
        }
    }

    // 再次清除 IDLE 中断挂起标志
    // 防止 IDLE 标志残留导致反复进入中断
    USART_ClearITPendingBit(USART1, USART_IT_IDLE);

    // 判断 USART1 是否出现 ORE 溢出错误
    // ORE = Overrun Error
    // 表示新数据来了，但旧数据没有及时读取，导致接收溢出
    if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) == SET)
    {
        // 清除 ORE 溢出错误标志
        // 避免该错误影响后续串口接收
        USART_ClearFlag(USART1, USART_FLAG_ORE);
    }
}