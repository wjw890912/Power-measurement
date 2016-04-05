/*
 * FreeModbus Libary: STM32 Port
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
 * File: $Id: porttimer.c,v 1.60 2013/08/13 15:07:05 Armink $
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "NUC472_442.h"
/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR(void);

/* ----------------------- Start implementation -----------------------------*/
BOOL xMBPortTimersInit(USHORT usTim1Timerout50us)
{

	uint16_t PrescalerValue = 0;

   CLK_EnableModuleClock(TMR0_MODULE);
   CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0SEL_PCLK, 0);
    // Set timer frequency to 20KHZ	 (50us)
    TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, usTim1Timerout50us);

    // Enable timer interrupt
    TIMER_EnableInt(TIMER0);
	NVIC_SetPriority(TMR0_IRQn, 10 ); //定时器的中断优先级应该比串口的低
    NVIC_EnableIRQ(TMR0_IRQn);

	TIMER_SET_PRESCALE_VALUE(TIMER0, 0);
    // Stop Timer 0
    TIMER_Stop(TIMER0);
	
	return TRUE;
}

void vMBPortTimersEnable()
{

	TIMER_SET_PRESCALE_VALUE(TIMER0, 0);
	TIMER_Start(TIMER0); 
}

void vMBPortTimersDisable()
{
	 TIMER_Stop(TIMER0);
}

void prvvTIMERExpiredISR(void)
{
	(void) pxMBPortCBTimerExpired();
}


void TMR0_IRQHandler(void)
{

    // clear timer interrupt flag
    TIMER_ClearIntFlag(TIMER0);
	prvvTIMERExpiredISR();
}

