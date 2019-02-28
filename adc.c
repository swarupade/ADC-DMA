/**
* @file adc.c
*
* This file contains the ADC configuration
*
* @author Steve and Swarupa
* @date Dec 8, 2018
*
*/
//----------------------------------------
//         Includes
//----------------------------------------
#include "adc.h"

//*****************************************************************************
// Name        : adc0_config
//
// Description : ADC0 configuration
//
// Arguments   : None
//
// return      : status
//
//****************************************************************************/
status adc0_config()
{
	//------------clock enable--------//

	//enable clock for PORT E - input pins PTE20, PTE21 / pin 13, 14
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;

	//enable clock for ADC0
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;



	//----------ADC0_SC2-----------//
	//enable DMA
	ADC0_SC2 |= ADC_SC2_DMAEN_MASK;


	//--------ADC0_CFG1-----------//
	// 16 bit conversion
	ADC0_CFG1 |= ADC_CFG1_MODE(0x03);



	//Start ADC0 calibration
	ADC0_SC3 |= ADC_SC3_CAL_MASK;


	//hardware average enabled
	ADC0_SC3 |= ADC_SC3_AVGE_MASK;


	//continuous mode
	ADC0_SC3 |= ADC_SC3_ADCO_MASK;



	//Wait till calibration ends
	while(ADC0_SC3 & ADC_SC3_CAL_MASK);


	//Check if the calibration failed
	if(ADC0_SC3 & ADC_SC3_CALF_MASK)
	{
		PRINT("Calibration error\r\n");
		return ERROR;
	}




	//enable conversion complete interrupt
	//Differential mode enable
	//Input channel select
	ADC0_SC1A = (ADC_SC1_AIEN_MASK | ADC_SC1_DIFF_MASK | ADC_SC1_ADCH(0x0));


	//Wait till conversion complete flag is set
	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK));

	return SUCCESS;
}

