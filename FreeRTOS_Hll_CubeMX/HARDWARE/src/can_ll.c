/*
 * @Description: 
 * @LastEditTime: 2022-12-27 18:14:50
 */
#include "can_ll.h"

/**
 * @description: CAN1筛选器配置
 * @return {*}
 * @LastEditTime: Do not edit
 * @param {CAN_HandleTypeDef*} hcan
 */
void CAN1_FILTER_CONFIG(CAN_HandleTypeDef* hcan)
{
	CAN_FilterTypeDef CAN_FilterConfigStructure;		
	CAN_FilterConfigStructure.FilterBank = 0;
	CAN_FilterConfigStructure.FilterMode = CAN_FILTERMODE_IDMASK;
	CAN_FilterConfigStructure.FilterScale = CAN_FILTERSCALE_32BIT;
	CAN_FilterConfigStructure.FilterIdHigh = 0x7FFE;//ID1高16位
	CAN_FilterConfigStructure.FilterIdLow = 0x0000;//ID1低16位
	CAN_FilterConfigStructure.FilterMaskIdHigh = 0x0000;//ID1掩码高16位
	CAN_FilterConfigStructure.FilterMaskIdLow = 0x0000;//ID1掩码低16位
	CAN_FilterConfigStructure.FilterFIFOAssignment = CAN_FilterFIFO0;
	CAN_FilterConfigStructure.SlaveStartFilterBank = 14;//筛选器组<15则给can1,>15给can2
	CAN_FilterConfigStructure.FilterActivation = ENABLE;
		
	HAL_CAN_ConfigFilter(&hcan1,&CAN_FilterConfigStructure);
	HAL_CAN_Start(&hcan1);
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);/* 开启PENDING */
}

/**
 * @description: CAN1中断回调
 * @return {*}
 * @LastEditTime: Do not edit
 * @param {CAN_HandleTypeDef} *hcan
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if(hcan->Instance==CAN1)
	{

	}  
	else if(hcan->Instance==CAN2)
	{

	}
}




