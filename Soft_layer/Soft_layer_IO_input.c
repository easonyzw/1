#include "soft_layer_IO_input.h"

sIO_parameter IO_parameter[16];

/******************************************************************
 * @brief  digital input data initialize
 * @input  none
 * @return  none
******************************************************************/
void digital_input_initialization(void)
{
  unsigned char i;
		
	for(i=0;i<16;i++)
	{
			IO_parameter[i].filter=1;
	}
	
}


/******************************************************************
 * @brief  Port LED 
 * @input  none
 * @return  none
******************************************************************/
void IO_state_update(void)                          
{

	 if(CH0 == SET)   // 当前检测到 CH0 是高电平
    {
        // ===== 第二步：滤波时间是否达到阈值 =====
        if(timer_counter.IO_filter[0] >= IO_parameter[0].filter) // 是否已经稳定了一段时间（例如 ≥5ms）
        { 
            // ===== 第三步：再次读取确认（防抖） =====
            if(CH0 == SET)
            {
                // ===== 确认稳定为高电平 =====
                LED0_ON;                // 点亮 LED
                IO_Input_L |= 0x01;     // 设置输入状态 bit0 = 1
            }
            else
            {
                // ===== 出现抖动（刚才是高，现在变低）=====
                LED0_OFF;
                IO_Input_L &= 0xFE;     // 清 bit0
            }

            // ===== 第四步：本轮滤波完成，计时清零 =====
            timer_counter.IO_filter[0] = 0;
        }
    }
    else   // 当前检测到 CH0 是低电平
    {
        // ===== 同样流程，只是针对“低电平确认” =====
        if(timer_counter.IO_filter[0] >= IO_parameter[0].filter)
        { 
            // 再确认一次
            if(CH0 == RESET)
            {
                // ===== 确认稳定为低电平 =====
                LED0_OFF;               
                IO_Input_L &= 0xFE;     // 清 bit0
            }
            else
            {
                // ===== 出现抖动（刚才是低，现在变高）=====
                LED0_ON;                
                IO_Input_L |= 0x01;     // 设置 bit0
            }

            // ===== 本轮滤波结束，重新开始计时 =====
            timer_counter.IO_filter[0] = 0;
        }     
    }
    
    
    
    if(CH1==SET)
		{
      if(timer_counter.IO_filter[1]>=IO_parameter[1].filter)
      { 
        if(CH1==SET)
        {
          LED1_ON;
          IO_Input_L|=0x02;                  
        }
        else
        {
          LED1_OFF;
          IO_Input_L&=0xFD;
        }
        timer_counter.IO_filter[1]=0;
      }
		}
    else
    {
      
      if(timer_counter.IO_filter[1]>=IO_parameter[1].filter)
      { 
        if(CH1==RESET)
        {
          LED1_OFF;
          IO_Input_L&=0xFD;                  
        }
        else
        {
          LED1_ON;
          IO_Input_L|=0x02;
        }
        timer_counter.IO_filter[1]=0;
      }     
    }
    
    if(CH2==SET)
		{
      if(timer_counter.IO_filter[2]>=IO_parameter[2].filter)
      { 
        if(CH2==SET)
        {
          LED2_ON;
          IO_Input_L|=0x04;                  
        }
        else
        {
          LED2_OFF;
          IO_Input_L&=0xFB;
        }
        timer_counter.IO_filter[2]=0;
      }
		}
    else
    {
      
      if(timer_counter.IO_filter[2]>=IO_parameter[2].filter)
      { 
        if(CH2==RESET)
        {
          LED2_OFF;
          IO_Input_L&=0xFB;                  
        }
        else
        {
          LED2_ON;
          IO_Input_L|=0x04;
        }
        timer_counter.IO_filter[2]=0;
      }     
    }
    
    
    if(CH3==SET)
		{
      if(timer_counter.IO_filter[3]>=IO_parameter[3].filter)
      { 
        if(CH3==SET)
        {
          LED3_ON;
          IO_Input_L|=0x08;                  
        }
        else
        {
          LED3_OFF;
          IO_Input_L&=0xF7;
        }
        timer_counter.IO_filter[3]=0;
      }
		}
    else
    {
      
      if(timer_counter.IO_filter[3]>=IO_parameter[3].filter)
      { 
        if(CH3==RESET)
        {
          LED3_OFF;
          IO_Input_L&=0xF7;                  
        }
        else
        {
          LED3_ON;
          IO_Input_L|=0x08;
        }
        timer_counter.IO_filter[3]=0;
      }     
    }
    
    
    if(CH4==SET)
		{
      if(timer_counter.IO_filter[4]>=IO_parameter[4].filter)
      { 
        if(CH4==SET)
        {
          LED4_ON;
          IO_Input_L|=0x10;                  
        }
        else
        {
          LED4_OFF;
          IO_Input_L&=0xEF;
        }
        timer_counter.IO_filter[4]=0;
      }
		}
    else
    {
      
      if(timer_counter.IO_filter[4]>=IO_parameter[4].filter)
      { 
        if(CH4==RESET)
        {
          LED4_OFF;
          IO_Input_L&=0xEF;                  
        }
        else
        {
          LED4_ON;
          IO_Input_L|=0x10;
        }
        timer_counter.IO_filter[4]=0;
      }     
    }

    if(CH5==SET)
		{
      if(timer_counter.IO_filter[5]>=IO_parameter[5].filter)
      { 
        if(CH5==SET)
        {
          LED5_ON;
          IO_Input_L|=0x20;                  
        }
        else
        {
          LED5_OFF;
          IO_Input_L&=0xDF;
        }
        timer_counter.IO_filter[5]=0;
      }
		}
    else
    {
      
      if(timer_counter.IO_filter[5]>=IO_parameter[5].filter)
      { 
        if(CH5==RESET)
        {
          LED5_OFF;
          IO_Input_L&=0xDF;                  
        }
        else
        {
          LED5_ON;
          IO_Input_L|=0x20;
        }
        timer_counter.IO_filter[5]=0;
      }     
    }
    
    
    if(CH6==SET)
		{
      if(timer_counter.IO_filter[6]>=IO_parameter[6].filter)
      { 
        if(CH6==SET)
        {
          LED6_ON;
          IO_Input_L|=0x40;                  
        }
        else
        {
          LED6_OFF;
          IO_Input_L&=0xBF;
        }
        timer_counter.IO_filter[6]=0;
      }
		}
    else
    {
      
      if(timer_counter.IO_filter[6]>=IO_parameter[6].filter)
      { 
        if(CH6==RESET)
        {
          LED6_OFF;
          IO_Input_L&=0xBF;                  
        }
        else
        {
          LED6_ON;
          IO_Input_L|=0x40;
        }
        timer_counter.IO_filter[6]=0;
      }     
    }
    
    
    if(CH7==SET)
		{
      if(timer_counter.IO_filter[7]>=IO_parameter[7].filter)
      { 
        if(CH7==SET)
        {
          LED7_ON;
          IO_Input_L|=0x80;                  
        }
        else
        {
          LED7_OFF;
          IO_Input_L&=0x7F;
        }
        timer_counter.IO_filter[7]=0;
      }
		}
    else
    {
      
      if(timer_counter.IO_filter[7]>=IO_parameter[7].filter)
      { 
        if(CH7==RESET)
        {
          LED7_OFF;
          IO_Input_L&=0x7F;                  
        }
        else
        {
          LED7_ON;
          IO_Input_L|=0x80;
        }
        timer_counter.IO_filter[7]=0;
      }     
    }
    
    
    if(CH8==SET)
		{
      if(timer_counter.IO_filter[8]>=IO_parameter[8].filter)
      { 
        if(CH8==SET)
        {
          LED8_ON;
          IO_Input_H|=0x01;                  
        }
        else
        {
          LED8_OFF;
          IO_Input_H&=0xFE;
        }
        timer_counter.IO_filter[8]=0;
      }
		}
    else
    {
      
      if(timer_counter.IO_filter[8]>=IO_parameter[8].filter)
      { 
        if(CH8==RESET)
        {
          LED8_OFF;
          IO_Input_H&=0xFE;                  
        }
        else
        {
          LED8_ON;
          IO_Input_H|=0x01;
        }
        timer_counter.IO_filter[8]=0;
      }     
    }
    
    if(CH9==SET)
		{
      if(timer_counter.IO_filter[9]>=IO_parameter[9].filter)
      { 
        if(CH9==SET)
        {
          LED9_ON;
          IO_Input_H|=0x02;                  
        }
        else
        {
          LED9_OFF;
          IO_Input_H&=0xFD;
        }
        timer_counter.IO_filter[9]=0;
      }
		}
    else
    {
      
      if(timer_counter.IO_filter[9]>=IO_parameter[9].filter)
      { 
        if(CH9==RESET)
        {
          LED9_OFF;
          IO_Input_H&=0xFD;                  
        }
        else
        {
          LED9_ON;
          IO_Input_H|=0x02;
        }
        timer_counter.IO_filter[9]=0;
      }     
    }
    
    
    
    if(CH10==SET)
		{
      if(timer_counter.IO_filter[10]>=IO_parameter[10].filter)
      { 
        if(CH10==SET)
        {
          LED10_ON;
          IO_Input_H|=0x04;                  
        }
        else
        {
          LED10_OFF;
          IO_Input_H&=0xFB;
        }
        timer_counter.IO_filter[10]=0;
      }
		}
    else
    {
      
      if(timer_counter.IO_filter[10]>=IO_parameter[10].filter)
      { 
        if(CH10==RESET)
        {
          LED10_OFF;
          IO_Input_H&=0xFB;                  
        }
        else
        {
          LED10_ON;
          IO_Input_H|=0x04;
        }
        timer_counter.IO_filter[10]=0;
      }     
    }
    
    
    if(CH11==SET)
		{
      if(timer_counter.IO_filter[11]>=IO_parameter[11].filter)
      { 
        if(CH11==SET)
        {
          LED11_ON;
          IO_Input_H|=0x08;                  
        }
        else
        {
          LED11_OFF;
          IO_Input_H&=0xF7;
        }
        timer_counter.IO_filter[11]=0;
      }
		}
    else
    {
      
      if(timer_counter.IO_filter[11]>=IO_parameter[11].filter)
      { 
        if(CH11==RESET)
        {
          LED11_OFF;
          IO_Input_H&=0xF7;                  
        }
        else
        {
          LED11_ON;
          IO_Input_H|=0x08;
        }
        timer_counter.IO_filter[11]=0;
      }     
    }
    
    
    
    if(CH12==SET)
		{
      if(timer_counter.IO_filter[12]>=IO_parameter[12].filter)
      { 
        if(CH12==SET)
        {
          LED12_ON;
          IO_Input_H|=0x10;                  
        }
        else
        {
          LED12_OFF;
          IO_Input_H&=0xEF;
        }
        timer_counter.IO_filter[12]=0;
      }
		}
    else
    {
      
      if(timer_counter.IO_filter[12]>=IO_parameter[12].filter)
      { 
        if(CH12==RESET)
        {
          LED12_OFF;
          IO_Input_H&=0xEF;                  
        }
        else
        {
          LED12_ON;
          IO_Input_H|=0x10;
        }
        timer_counter.IO_filter[12]=0;
      }     
    }
    
    
    
    if(CH13==SET)
		{
      if(timer_counter.IO_filter[13]>=IO_parameter[13].filter)
      { 
        if(CH13==SET)
        {
          LED13_ON;
          IO_Input_H|=0x20;                  
        }
        else
        {
          LED13_OFF;
          IO_Input_H&=0xDF;
        }
        timer_counter.IO_filter[13]=0;
      }
		}
    else
    {
      
      if(timer_counter.IO_filter[13]>=IO_parameter[13].filter)
      { 
        if(CH13==RESET)
        {
          LED13_OFF;
          IO_Input_H&=0xDF;                  
        }
        else
        {
          LED13_ON;
          IO_Input_H|=0x20;
        }
        timer_counter.IO_filter[13]=0;
      }     
    }
    
    
    if(CH14==SET)
		{
      if(timer_counter.IO_filter[14]>=IO_parameter[14].filter)
      { 
        if(CH14==SET)
        {
          LED14_ON;
          IO_Input_H|=0x40;                  
        }
        else
        {
          LED14_OFF;
          IO_Input_H&=0xBF;
        }
        timer_counter.IO_filter[14]=0;
      }
		}
    else
    {
      
      if(timer_counter.IO_filter[14]>=IO_parameter[14].filter)
      { 
        if(CH14==RESET)
        {
          LED14_OFF;
          IO_Input_H&=0xBF;                  
        }
        else
        {
          LED14_ON;
          IO_Input_H|=0x40;
        }
        timer_counter.IO_filter[14]=0;
      }     
    }
    
    
    if(CH15==SET)
		{
      if(timer_counter.IO_filter[15]>=IO_parameter[15].filter)
      { 
        if(CH15==SET)
        {
          LED15_ON;
          IO_Input_H|=0x80;                  
        }
        else
        {
          LED15_OFF;
          IO_Input_H&=0x7F;
        }
        timer_counter.IO_filter[15]=0;
      }
		}
    else
    {
      
      if(timer_counter.IO_filter[15]>=IO_parameter[15].filter)
      { 
        if(CH15==RESET)
        {
          LED15_OFF;
          IO_Input_H&=0x7F;                  
        }
        else
        {
          LED15_ON;
          IO_Input_H|=0x80;
        }
        timer_counter.IO_filter[15]=0;
      }     
    }
   
		module_show_status_running();

}

/******************************************************************
 * @brief  LED status of station number waiting
 * @input  none
 * @return  none
******************************************************************/
void module_show_status(void)
{
  if(timer_counter.LED_ms>500)
  {
    LED_all_close();
    timer_counter.LED_ms=0;
  }
  else if(timer_counter.LED_ms>250)
  {    
    LED_all_open();
  }
}

void module_show_status_running(void)
{
  if((IO_Input_L!=0)||(IO_Input_H!=0))
  {
    return;
  }
  
  if(timer_counter.LED_ms>5200)
  {
    LED_all_close();
    timer_counter.LED_ms=0;
  }
  else if(timer_counter.LED_ms>5000)
  {    
    LED_all_open();
  }
}

void LED_all_open(void)
{
  PAout(7)=0;
  PAout(6)=0;
  PAout(5)=0;
  PAout(4)=0;
  PAout(3)=0;
  PAout(2)=0;
  PAout(1)=0;
  PAout(0)=0;
  PBout(0)=0;
  PBout(1)=0;
  PBout(2)=0;
  PBout(10)=0;
  PBout(11)=0;
  PCout(2)=0;
  PCout(3)=0;
  PCout(4)=0;

	
}
void LED_all_close(void)
{
	PAout(7)=1;
  PAout(6)=1;
  PAout(5)=1;
  PAout(4)=1;
  PAout(3)=1;
  PAout(2)=1;
  PAout(1)=1;
  PAout(0)=1;
  PBout(0)=1;
  PBout(1)=1;
  PBout(2)=1;
  PBout(10)=1;
  PBout(11)=1;
  PCout(2)=1;
  PCout(3)=1;
  PCout(4)=1;
}


