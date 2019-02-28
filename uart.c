/**
* @file uart.c
*
* This file contains function configuring, tranmitting and receiving part of UART0
*
* @author Steve and Swarupa
* @date Nov 24, 2018
*
*/

//***********************************************************************************
// Include files
//***********************************************************************************
#include "MKL25Z4.h"
#include "uart.h"
#include <stdlib.h>
#include <string.h>

//***********************************************************************************
// Function definition
//***********************************************************************************

//*****************************************************************************
// Name        : uartinit
//
// Description : Function to initiate UART0
//
// Arguments   : None
//
// return      : None
//
//****************************************************************************/
void uartinit()
{
			//for baud rate divisor
		    uint16_t baudmoddivisor;

		   //Set gate clock  for PORTA
			SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;

			//Enable clock for UART0
			SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;

			// Set the alternate function for PORTA as UART0
			PORTA_PCR1 |= PORT_PCR_MUX(0x2);
			PORTA_PCR2 |= PORT_PCR_MUX(0x2);

			//set clock source as MCGPLLCLK/2
			SIM_SOPT2 &= ~(SIM_SOPT2_PLLFLLSEL_MASK);

			SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1);
			//SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL(1);

			// Disable tx and rx before editing registers of UART0
			UART0_C2 |= (UART0_C2_TE(0) | UART0_C2_RE(0));


			//calculate the baud rate modulo divisor and set the baud rate
			UART0_BDH &= ~UART0_BDH_SBR_MASK;
			UART0_BDL &= ~UART0_BDL_SBR_MASK;

			//baud rate calculation
			baudmoddivisor= (uint16_t)((SystemCoreClock)/(UART0_BAUD_RATE *16));
			UART0_BDH |= UARTLP_BDH_SBR((baudmoddivisor >> 8)) ;
			UART0_BDL |= UARTLP_BDL_SBR(baudmoddivisor);

			//Selecting 8 bit data, No parity
			UART0_C1 |= UART0_C1_M(0) | UART_C1_PE(0);

			//selecting one stop bit
			UART0_BDH |= UART_BDH_SBNS(0);

			//Enabling the Transmitter enable and receiver enable bit
			UART0_C2 |= (UART0_C2_TE(1) | UART0_C2_RE(1));


}

//***********************************************************************************
// Function definition
//***********************************************************************************

//*****************************************************************************
// Name        : send_to_console_str
//
// Description : Function to transmit a string
//
// Arguments   : string to be transmitted
//
// return      : None
//
//****************************************************************************/
void send_to_console_str(char data[])
{
	for(int i =0; data[i] != '\0'; i++)
	{
		//polling for transmitting
		while(!(UART0_S1 & UART_S1_TDRE_MASK));
		UART0_D = (data[i]);
	}
}
