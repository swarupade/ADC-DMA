/**
* @file dma.c
*
* This file contains the DMA configuration
*
* @author Steve and Swarupa
* @date Dec 8, 2018
*
*/
//----------------------------------------
//         Includes
//----------------------------------------
#include "dma.h"

//*****************************************************************************
// Name        : DMA_Configuration
//
// Description : DMA configuration
//
// Arguments   : None
//
// return      : None
//
//****************************************************************************/
void DMA_Configuration()
{
	//enabling the clock of DMA
	SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;

	// enabling clock of DMA MUX
	SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;

	// Disabling DMA MUX
	DMAMUX0_CHCFG0 = 0x00;

	//source address from ADC0 data register
	DMA_SAR0 = (uint32_t)&ADC0_RA;


	//destination address is the start address of the buffer
	DMA_DAR0 = (uint32_t)(&destination[0]);

	//reads 64 bytes i.e 32 elements of 16 bits
	DMA_DSR_BCR0 = DMA_DSR_BCR_BCR(64);

	// read till BCR becomes 0- i.e after reading 64 bytes
	DMA_DCR0 |= DMA_DCR_CS(0);

	// data size of the source bus cycle
	// 16 bit data
	DMA_DCR0 |= DMA_DCR_SSIZE(2);

	// data size of the destination bus cycle
	// 16 bit data
	DMA_DCR0 |= DMA_DCR_DSIZE(2);

	//increment the destination after each 2 bytes fetch
	DMA_DCR0 |= DMA_DCR_DINC_MASK;

	// Enable DMA channel and source as ADC0
	DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(40);

	// enabling interrupt for DMA
	DMA_DCR0 |= DMA_DCR_EINT_MASK;

	/* Enable DMA Interrupt */
	NVIC_EnableIRQ(DMA0_IRQn);

	// Start DMA process
	DMA_DCR0 |= DMA_DCR_START_MASK;




}


//*****************************************************************************
// Name        : DMA0_IRQHandler
//
// Description : Interrupt Service routine of DMA
//
// Arguments   : None
//
// return      : None
//
//****************************************************************************/

//interrupt called on completion of transfer
void DMA0_IRQHandler(void)
{
	//output pin as high
	PTC->PTOR |=(1<<0);

	/* Enable DMA Interrupt */
	NVIC_DisableIRQ(DMA0_IRQn);

	PRINT("Entered ISR\r\n");
	if(DMA_DSR_BCR_DONE_MASK)
	{
		// Clear the clears all DMA status bits
		DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE(1);

		//source address from ADC0 data register
		DMA_SAR0 = (uint32_t)&ADC0_RA;

		memset(to_print,0,sizeof(to_print));
		sprintf(to_print,"TOP FLAG %d, DOWN_FLAG %d\r\n---------\r\n",TOP_FLAG, DOWN_FLAG);
		PRINT(to_print);

		if(TOP_FLAG == 1)
		{
			//destination address is the start address of the buffer
			DMA_DAR0 = (uint32_t)(&destination[0]);
		}
		else if(DOWN_FLAG == 1)
		{
			//destination address is the start address of the buffer
			DMA_DAR0 = (uint32_t)(&destination[32]);

		}

		//Reload BCR value for next operation
		DMA_DSR_BCR0 |= DMA_DSR_BCR_BCR(64);

		DMA_DCR0 |= DMA_DCR_START_MASK;

	}

	FLAG_BUFFER_FULL = 1;

}
