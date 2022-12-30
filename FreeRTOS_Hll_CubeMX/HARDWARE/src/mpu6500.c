/*
 * @Description: 
 * @Author: YiPei_Fang
 * @Date: 2022-10-03 18:45:10
 */
#include "mpu6500.h"



//// tx:�����͵�ֵ
//// rx:���յ���ֵ
//int spi_transmit_receive(uint8_t tx, uint8_t *rx)
//{
//	uint8_t state = 0;
//	uint32_t timeout_cnt;
//	static const uint32_t timeout_cnt_num = 10000;
//	
//	// Wait until TXE flag is set to send data 
//	timeout_cnt = 0;
//	while(!LL_SPI_IsActiveFlag_TXE(SPI5)){
//		timeout_cnt ++;
//		if(timeout_cnt > timeout_cnt_num){
//			state = 1;
//			break;
//		} 
//	}
//	// Transmit data in 8 Bit mode
//	LL_SPI_TransmitData8(SPI5, tx);
//	
//	// Check BSY flag 
//	timeout_cnt = 0;
//	while(LL_SPI_IsActiveFlag_BSY(SPI5)){
//		timeout_cnt ++;
//		if(timeout_cnt > timeout_cnt_num){
//			state = 2;
//			break;
//		}
//	}
//	
//	// Check RXNE flag 
//	timeout_cnt = 0;
//	while(!LL_SPI_IsActiveFlag_RXNE(SPI5)){
//		timeout_cnt ++;
//		if(timeout_cnt > timeout_cnt_num){
//			state = 3;
//			break;
//		}
//	}
//	
//	// Read 16-Bits in the data register
//	*rx = LL_SPI_ReceiveData8(SPI5);
//	
//	return state;
//}


/*
 * ��������SPI1_Read_Write_Byte
 * ����  ����дһ���ֽ�
 * ����  ��TxData:Ҫд����ֽ�
 * ���  ����ȡ�����ֽ�
 */ 
uint8_t SPI5_Read_Write_Byte(uint8_t TxData)
{		
	uint8_t retry = 0;				 	
	while (LL_SPI_IsActiveFlag_TXE(SPI5) == RESET) 	//���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	{
		retry++;
		if(retry > 250)	return 0;
	}			  
	LL_SPI_TransmitData8(SPI5, TxData); 																//ͨ������SPIx����һ������
	retry = 0;

	while (LL_SPI_IsActiveFlag_RXNE(SPI5) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	{
		retry++;
		if(retry > 250) return 0;
	}	  						    
	return LL_SPI_ReceiveData8(SPI5); 															//����ͨ��SPIx������յ�����					    
}



