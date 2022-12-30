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
  * @brief  ���ADS8864�Ĳ������
  * @note   ���˺�������stm32f3xx_it.c�ļ��е� ��LL�⣩
  *         void EXTI2_TSC_IRQHandler(void)
  * @retval none
  */

void ADS8864_ReadValue(void)
{
	if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_2) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_2);//����жϱ�־
		
		LL_EXTI_DisableFallingTrig_0_31(LL_EXTI_LINE_2);//�ر��½��ش���
		LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_2);//�ر��ж�
		Convst_Reset();//�ر�Ƭѡ�ź�
		TIM15->CNT = 0;
		
		if( HAL_SPI_Receive(&ADS8864_SPI_Port, adc_data, 2, 0x0F) == HAL_OK)
		{

			adc_rawdata[adc_counter] = ((adc_data[0] << 8) | adc_data[1]) & 0xFFFFFFFF; //������λ
			
			adc_counter++;
			
			if (adc_counter == (ADC_SAMPLE_SIZE) )
			{
				adc_counter = 0;
			}
			
			LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_2);//�����½��ش���
			LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_2);//�����ж�
			
			Convst_Set(50);//����Ƭѡ�ź�
		}
	}

}

/**
  * @brief  ADS8864��ʼ������
  * @note   ����������SPI��Ƭѡ�ź�������695ns
  * @retval none
  */
void ads8864_Init(void)
{
	DIN_Set();
	//TIM15 ����Ƶ��Ϊ400KHz������2.5us
	//CounterֵΪ179��Ƭѡ�ź�������Ϊ694.44ns
	//������ 66.6KHz
	Convst_Set(50);
	
	//��ʱ20ms
	LL_mDelay(20);
}

