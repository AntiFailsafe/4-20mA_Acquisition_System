#include "ads8864.h"
#include "arm_math.h"
#include "gpio.h"
#include "tim.h"

int adc_rawdata[ADC_SAMPLE_SIZE] = {0};
uint8_t adc_data[BUFFER_SIZE] = {0};
int adc_mean[1] = {0};

uint16_t adc_counter;
uint8_t adc_conv_flag;

static void DIN_Set(void) 
{
	LL_GPIO_SetOutputPin(DIN_GPIO_Port, DIN_Pin);
}

static void Convst_Set(uint16_t pulse) 
{
	WRITE_REG(TIM15->CCR2, pulse);
}

static void Convst_Reset(void) 
{
	WRITE_REG(TIM15->CCR2, 0);
}


void Mean_filter(void)
{
		arm_mean_q31(adc_rawdata,ADC_SAMPLE_SIZE,&adc_mean[0]);
}

/**
  * @brief  获得ADS8864的采样结果
  * @note   将此函数放入stm32f3xx_it.c文件中的 （LL库）
  *         void EXTI2_TSC_IRQHandler(void)
  * @retval none
  */

void ADS8864_ReadValue(void)
{
	if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_2) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_2);//清除中断标志
		
		LL_EXTI_DisableFallingTrig_0_31(LL_EXTI_LINE_2);//关闭下降沿触发
		LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_2);//关闭中断
		Convst_Reset();//关闭片选信号
		TIM15->CNT = 0;
		
		if( HAL_SPI_Receive(&ADS8864_SPI_Port, adc_data, 2, 0x0F) == HAL_OK)
		{

			adc_rawdata[adc_counter] = ((adc_data[0] << 8) | adc_data[1]) & 0xFFFFFFFF; //补符号位
			
			adc_counter++;
			
			if (adc_counter == (ADC_SAMPLE_SIZE) )
			{
				adc_counter = 0;
			}
			
			LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_2);//开启下降沿触发
			LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_2);//开启中断
			
			Convst_Set(50);//开启片选信号
		}
	}

}

/**
  * @brief  ADS8864初始化函数
  * @note   采用三线制SPI，片选信号脉冲宽度695ns
  * @retval none
  */
void ads8864_Init(void)
{
	DIN_Set();
	//TIM15 计数频率为400KHz，周期2.5us
	//Counter值为179，片选信号脉冲宽度为694.44ns
	//采样率 66.6KHz
	Convst_Set(50);
	
	//延时20ms
	LL_mDelay(20);
}

