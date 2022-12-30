#ifndef __ADS8864_H
#define __ADS8864_H

#include "main.h"
#include "spi.h"

#define BUFFER_SIZE 2
#define ADC_SAMPLE_SIZE 32

#define ADS8864_SPI_Port hspi2
extern SPI_HandleTypeDef ADS8864_SPI_Port;


void Mean_filter(void);
void ADS8864_IRQ(void);

void ADS8864_ReadValue(void);
void ads8864_Init(void);


#endif
