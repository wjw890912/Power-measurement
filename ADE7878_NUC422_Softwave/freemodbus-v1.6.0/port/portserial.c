/*
 * FreeModbus Libary: NUC442 Port
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.60 2013/08/13 15:07:05 Armink $
 */

#include "port.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "NUC472_442.h"
/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR(void);
static void prvvUARTRxISR(void);
/* ----------------------- Start implementation -----------------------------*/

void vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
	if (xRxEnable) //�������߹رս����ж�
	{
	    
		SLAVE_RS485_RECEIVE_MODE;
	UART_EnableInt(UART3, UART_INTEN_RDAIEN_Msk);
  		Sysdelay(50);//1ms
	}
	else
	{
		
	   SLAVE_RS485_SEND_MODE;	
	UART_DisableInt(UART3, UART_INTEN_RDAIEN_Msk);
		  Sysdelay(50);//1ms

	}
	if (xTxEnable)	//�������߹رշ����ж�
	{

	UART_EnableInt(UART3,  UART_INTEN_THREIEN_Msk);
	

	}
	else
	{
	

  UART_DisableInt(UART3, UART_INTEN_THREIEN_Msk);

	}
}


  	
    
void vMBPortClose(void)
{



}
//Ĭ��һ���ӻ� ����1 �����ʿ�����  ��ż���������
BOOL xMBPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits,
		eMBParity eParity)
{

		 /* Enable IP clock */
    CLK_EnableModuleClock(UART3_MODULE);
    /* Select IP clock source */
    CLK_SetModuleClock(UART3_MODULE, CLK_CLKSEL1_UARTSEL_HIRC , CLK_CLKDIV0_UART(1));
   /* Set GPD multi-function pins for UART3 RXD and TXD */
    SYS->GPD_MFPL |= SYS_GPD_MFPL_PD4MFP_UART3_RXD | SYS_GPD_MFPL_PD5MFP_UART3_TXD  ;
	GPIO_SetMode(PD, 5, GPIO_MODE_OUTPUT);
	GPIO_SetMode(PD, 4, GPIO_MODE_INPUT);
   // UART_Open(UART3, ulBaudRate);
    UART_DisableFlowCtrl(UART3);
	UART_SetLine_Config(UART3, ulBaudRate, UART_WORD_LEN_8,UART_PARITY_NONE,UART_STOP_BIT_1);
	  	
	//����485���ͺͽ���ģʽ
//    TODO   ��ʱ��дB13 ��֮����������ʱ���޸�
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);

	ENTER_CRITICAL_SECTION(); //��ȫ���ж�

  	UART_EnableInt(UART3, UART_INTEN_RDAIEN_Msk );
	NVIC_SetPriority(UART3_IRQn, 0 );
   	NVIC_EnableIRQ(UART3_IRQn);	
	EXIT_CRITICAL_SECTION(); //��ȫ���ж�		 
	return TRUE;
}

BOOL xMBPortSerialPutByte(CHAR ucByte)//дһ���ֽڵ�Ӳ��
{

   UART_WRITE(UART3, ucByte);
	return TRUE;
}

BOOL xMBPortSerialGetByte(CHAR * pucByte) //��һ���Լ���Ӳ��
{	   
	*pucByte = UART_READ(UART3);
	return TRUE;
}

/* 
 * Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
void prvvUARTTxReadyISR(void)
{
	pxMBFrameCBTransmitterEmpty();
}

/* 
 * Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
void prvvUARTRxISR(void)
{
	pxMBFrameCBByteReceived();
}



/*******************************************************************************
 * Function Name  : USART1_IRQHandler
 * Description    : This function handles USART1 global interrupt request.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/


void UART3_IRQHandler(void)
{
    uint32_t u32IntSts= UART3->INTSTS;
	char u;
	  
	if(u32IntSts & UART_INTSTS_RDAINT_Msk)
	 {
		
        
		   
	prvvUARTRxISR();
	UART_ClearIntFlag(UART3 , UART_INTEN_RDAIEN_Msk);
	
     }
	 else
	 if(u32IntSts & UART_INTSTS_THREINT_Msk)
	 
	  {

	  	prvvUARTTxReadyISR();
	    UART_ClearIntFlag(UART3, UART_INTSTS_THREINT_Msk);
	 }
	 else
	 {
	 
	  	UART_ClearIntFlag(UART3 , UART_INTSTS_BUFERRINT_Msk|UART_INTSTS_MODEMINT_Msk|UART_INTSTS_RXTOINT_Msk);
	 }	  
	
}	