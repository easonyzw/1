#include "Hardware_usart.h"
#include "string.h"

sModbus_management    modbus_management;


/******************************************************************
 * @brief   USART Initializes
 * @input   none
 * @return  none
******************************************************************/
void RS485_initializes(void)
{
  USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
  USART_OverSampling8Cmd(USART1,ENABLE);
	USART_InitStructure.USART_BaudRate = 6000000;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
  /*********************************************/
	USART_DECmd(USART1,ENABLE);
	USART_SetDEAssertionTime(USART1,0x1F);
	USART_DEPolarityConfig(USART1,USART_DEPolarity_High);
	/*********************************************/
	
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
	USART_DMACmd(USART1,USART_DMAReq_Rx | USART_DMAReq_Tx,ENABLE);
	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1,USART_FLAG_TC);
	DMA_Config_TX(DMA1_Channel2,(unsigned int)&USART1->TDR,(unsigned int)modbus_management.sendBuffer,USART1_SEND_BUFFER_SIZE); 
  DMA_Config_RX(DMA1_Channel3,(unsigned int)&USART1->RDR,(unsigned int)modbus_management.receiveBuffer,USART1_RECEIVE_BUFFER_SIZE);
	
}

/******************************************************************
 * @brief   DMA_Config_TX
 * @input   DMA_CHx :
 *          cpar    :  DMA_PeripheralBaseAddr
 *          cmar    :  DMA_MemoryBaseAddr
 *          cndtr   :  DMA_BufferSize
 * @return  none
******************************************************************/
void DMA_Config_TX(DMA_Channel_TypeDef* DMA_CHx,      // DMA通道号，例如 DMA1_Channel2
                   unsigned int cpar,                 // 外设地址，例如 USART1->TDR 地址
                   unsigned int cmar,                 // 内存地址，例如 sendBuffer 首地址
                   unsigned short int cndtr)          // 传输数量，即这次要发送多少字节
{
    DMA_InitTypeDef DMA_InitStructure;                              // 定义 DMA 初始化结构体变量
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);             // 开启 DMA1 外设时钟（DMA 挂在 AHB 总线上）
	
    DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;               // 外设地址：这里一般传 USART 的 TDR 地址
    DMA_InitStructure.DMA_MemoryBaseAddr = cmar;                   // 内存地址：这里一般传发送缓冲区首地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;             // 传输方向：从内存搬到外设（外设作为目的地）
    DMA_InitStructure.DMA_BufferSize = cndtr;                      // DMA 传输数据数量（要搬多少个数据单元）
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // 外设地址不自增，因为始终写同一个寄存器 TDR
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;        // 内存地址自增，这样才能把缓冲区数据一个个往后发送
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 外设数据宽度：按字节传输
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; // 内存数据宽度：按字节传输
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                  // 普通模式：传完指定数量后停止，不循环发送
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;        // DMA 优先级设为很高，保证串口高速发送时及时搬运
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                  // 关闭存储器到存储器模式，因为这里是“内存->外设”
    DMA_Init(DMA_CHx, &DMA_InitStructure);                         // 将上面配置写入指定 DMA 通道
}
/******************************************************************
 * @brief   DMA_Config_RX
 * @input   DMA_CHx :
 *          cpar    :  DMA_PeripheralBaseAddr
 *          cmar    :  DMA_MemoryBaseAddr
 *          cndtr   :  DMA_BufferSize
 * @return  none
******************************************************************/
void DMA_Config_RX(DMA_Channel_TypeDef* DMA_CHx,unsigned int cpar,unsigned int cmar,unsigned short int cndtr)
{
    DMA_InitTypeDef DMA_InitStructure;
	
    DMA_DeInit(DMA_CHx);
    DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;
    DMA_InitStructure.DMA_MemoryBaseAddr = cmar;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = cndtr;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA_CHx, &DMA_InitStructure);

    DMA_Cmd(DMA_CHx, ENABLE);
}
/******************************************************************
 * @brief   DMA_Enable
 * @input   DMA_CHx :
 *          CNDTR   :
 * @return  none
******************************************************************/
void DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u16 CNDTR)
{
    DMA_CHx->CCR&=~(1<<0);      
    DMA_CHx->CNDTR = CNDTR;     
    DMA_CHx->CCR|=1<<0;          
}




/******************************************************************
 * @brief  485 Sender Function
 * @input  none
 * @return  none
******************************************************************/
void usart1_send_data(unsigned char* data , unsigned char len)
{
	DMA_ClearFlag(DMA1_FLAG_TC2);                   
	DMA_Cmd(DMA1_Channel2, DISABLE);               
	memcpy(modbus_management.sendBuffer,data,len);
	DMA_Enable(DMA1_Channel2,len);
}

/******************************************************************
 * @brief  Serial transaction functions
 * @input  none
 * @return  none
******************************************************************/
void USART1_IRQHandler(void)
{
	volatile unsigned char temp;   // 临时变量，用于辅助清除串口空闲中断标志
  
	if(USART_GetFlagStatus(USART1,USART_FLAG_IDLE) != RESET)   // 判断 USART1 是否产生了空闲中断（通常表示一帧数据接收结束）
	{

		DMA_Cmd(DMA1_Channel3,DISABLE);   // 先关闭 DMA1 通道3（接收 DMA），冻结当前接收现场，避免长度继续变化
    
		modbus_management.receive_length = USART1_RECEIVE_BUFFER_SIZE - DMA_GetCurrDataCounter(DMA1_Channel3); 
    // 计算本次实际收到的数据长度：
    // 已接收长度 = 接收缓冲区总长度 - DMA 当前剩余未传输数量
    
    DMA_Enable(DMA1_Channel3,USART1_RECEIVE_BUFFER_SIZE);   // 重新装载接收 DMA 长度，并重新开启 DMA，准备接收下一帧数据
    
		USART_ClearITPendingBit(USART1,USART_IT_IDLE);   // 清除 USART1 的空闲中断挂起标志
    
		temp = USART1 -> ISR;   // 读取状态寄存器（配合下一句一起完成某些标志清除）
		temp = USART1 -> RDR;   // 读取接收数据寄存器，彻底清除空闲中断相关状态

	  if(modbus_management.receive_length > 5)   // 如果本次接收到的数据长度大于 5，认为是一帧有效数据
		{
			rs485_receive_data_handler();   // 调用 RS485 接收数据处理函数，对这一帧进行协议解析
		}
		
	}
  
  USART_ClearITPendingBit(USART1,USART_IT_IDLE);   // 再次清除空闲中断挂起标志，防止标志残留
  
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE) == SET)    // 判断是否发生溢出错误（ORE：接收过载/溢出）
	{
		USART_ClearFlag(USART1,USART_FLAG_ORE);   // 清除溢出错误标志，避免影响后续接收
	}
}


