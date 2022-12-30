#include "can.h"
#include "can_bsp.h"
#include "main.h"

/*初始化CAN*/
void CAN_Init(CAN_HandleTypeDef *hcan_Cur)
{
    CAN_FilterTypeDef sFilterConfig;
    //HAL_StatusTypeDef HAL_Status;
    if (hcan_Cur->Instance == hcan.Instance)
    {
        sFilterConfig.FilterBank = 1;                     //过滤器组
        sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK; //CAN_FILTERMODE_IDLIST  CAN_FILTERMODE_IDMASK
        sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
        sFilterConfig.FilterIdHigh = 0x0000; //filter id
        sFilterConfig.FilterIdLow = 0x0000;
        sFilterConfig.FilterMaskIdHigh = 0x0000;
        sFilterConfig.FilterMaskIdLow = 0x0000;
        sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0; //用FIFO接收
        sFilterConfig.FilterActivation = ENABLE;
        sFilterConfig.SlaveStartFilterBank = 14;

        HAL_CAN_ConfigFilter(hcan_Cur, &sFilterConfig);
        HAL_CAN_Start(hcan_Cur); //开启CAN
    }
    
}



/*CAN发送函数*/
uint8_t CAN_SendData(CAN_HandleTypeDef *hcan_Cur, uint8_t *pData, uint16_t ID)
{
    HAL_StatusTypeDef HAL_RetVal = HAL_ERROR;
    uint8_t FreeTxNum = 0;
    CAN_TxHeaderTypeDef TxMessage;

    TxMessage.StdId = ID;
    TxMessage.DLC = 8;  /*默认一帧传输长度为8*/
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.RTR = CAN_RTR_DATA;

    FreeTxNum = HAL_CAN_GetTxMailboxesFreeLevel(hcan_Cur);

    while (FreeTxNum == 0) //等待空邮箱，可能会卡死在这里（小BUG）
    {
        FreeTxNum = HAL_CAN_GetTxMailboxesFreeLevel(hcan_Cur);
    }

    HAL_RetVal = HAL_CAN_AddTxMessage(hcan_Cur, &TxMessage, pData, (uint32_t *)CAN_TX_MAILBOX1);

    if (HAL_RetVal != HAL_OK)
    {
        return 2;
    }

    return 0;
}
