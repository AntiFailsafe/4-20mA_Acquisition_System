#ifndef __CAN_BSP_H__
#define __CAN_BSP_H__
#include "main.h"

void CAN_Init(CAN_HandleTypeDef *hcan);
uint8_t CAN_SendData(CAN_HandleTypeDef *hcan, uint8_t *pData, uint16_t ID);
#endif
