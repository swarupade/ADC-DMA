/**
* @file main.c
*
* This file contains the peak measurement application
*
* @author Steve and Swarupa
* @date Dec 8, 2018
*
*/

//----------------------------------------
//         Includes
//----------------------------------------
#include "main.h"

//----------------------------------------
//			Functions
//----------------------------------------
//*****************************************************************************
// Name        : main
//
// Description : main function
//
// Arguments   : None
//
// return      : unused
//
//****************************************************************************/
int main(void)
{
	TOP_FLAG = 1;
	DOWN_FLAG = 0;


	//UART0 configuration
	uartinit();

	//-------------------------------------------------------

	//pointer variable for accessing the DMA destination array
	int16_t *pointer_dest;

	PRINT("Welcome to signal measurement application!!!\r\n");

	//--------------------------------------------------------


	//------------For Oscilloscope reading-------------------
	//Clock for PORT C
	SIM_BASE_PTR->SCGC5 |= SIM_SCGC5_PORTC_MASK;

	//Alternate function selection of PORT C PTC0
	PORTC_PCR0 |= PORT_PCR_MUX(0x1);

	// selecting as output pin
	PTC->PDDR |= (1<<0);

	//-------------------------------------------------------

	//Flag for DMA0 Interrupt
	FLAG_BUFFER_FULL = 0;


	//ADC0 configuration
	adc0_config();

	//DMA
	DMA_Configuration();

	//for debug purpose
	int16_t adc_output;


	while(1)
	{


		if(FLAG_BUFFER_FULL == 1)
		{
			//for tracking the DMA fetch number
			int count;

#ifdef DEBUG
			adc_output = ADC0_RA;

			memset(to_print,0,sizeof(to_print));
			sprintf(to_print,"ADC output %d \r\n",adc_output);
			PRINT(to_print);

#endif

			if((TOP_FLAG == 1) && (DOWN_FLAG ==0))
			{
				ptr_DB = &destination[0];
				TOP_FLAG = 0;
				DOWN_FLAG = 1;

			}
			else if((DOWN_FLAG == 1) && (TOP_FLAG ==0))
			{
				ptr_DB = &destination[32];
				DOWN_FLAG = 0;
				TOP_FLAG = 1;

			}

			memset(to_print,0,sizeof(to_print));
			sprintf(to_print,"TOP FLAG %d, DOWN_FLAG %d\r\n",TOP_FLAG, DOWN_FLAG);
			PRINT(to_print);

			//Displaying the buffer values transfered from DMA
			for(count = 0,pointer_dest = ptr_DB; count<BUFFER_ELTS;count++,pointer_dest++)
			{
				memset(to_print,0,sizeof(to_print));
				sprintf(to_print,"ADC result %d , add %lx\r\n",*pointer_dest, pointer_dest);
				PRINT(to_print);
			}

			//find the max value of the elements in the buffer
			max_value = max();
			memset(to_print,0,sizeof(to_print));
			sprintf(to_print,"Max value of this cycle %d\r\n",max_value);
			PRINT(to_print);

			//peak value measurement
			peak_value = peak_value_cal();
			memset(to_print,0,sizeof(to_print));
			sprintf(to_print,"Peak value = %f\r\n",peak_value);
			PRINT(to_print);

			//dbFS value for the corresponding peak value
			decibel();

			//clearing the flag
			FLAG_BUFFER_FULL = 0;

			/* Enable DMA Interrupt */
			NVIC_EnableIRQ(DMA0_IRQn);


		}

	}

    return 0;
}

//*****************************************************************************
// Name        : max
//
// Description : function to find the maximum value of the elements in the buffer
//
// Arguments   : None
//
// return      : max value
//
//****************************************************************************/
int16_t max()
{
	//initial lowest possible value
	int16_t max = ADC_MIN_VALUE;

	//finding maximum
	int element;
	for(element = 0; element<BUFFER_ELTS;element++)
	{
		if(destination[element] > max)
			max = destination[element];
	}
	return max;
}

//*****************************************************************************
// Name        : peak_value_cal
//
// Description : function to find the peak value
//
// Arguments   : None
//
// return      : peak value
//
//****************************************************************************/
double peak_value_cal()
{
	//to store the peak and previous peak value
	static double curr_max, prev_max;

	double decay_value = 0;

	//count variable
	static int count = 0;

	//initializing the curr_max and prev_max
	if(count == 0)
	{
		prev_max = abs(max_value);
		curr_max = abs(max_value);
	}


	else
	{
		prev_max = curr_max;
		curr_max = abs(max_value) ;
	}



	//to reload count
	//count used only for initializing the variables
	if(count == 9000)
			count = 1;

	//return the max value of the cycle as the peak value
	//when current cycle maximum is >= previous cycle maximum
	if(curr_max>=prev_max)
	{
		peak_value = curr_max;
	}

	//Peak value = alpha*Previous Peak
	//when current cycle maximum is < previous cycle maximum
	else
	{
		decay_value = (prev_max*ALPHA);
		if(decay_value < curr_max)
			peak_value = curr_max;
		else
		{
			peak_value = decay_value;
			curr_max = peak_value;
		}

	}

	memset(to_print,0,sizeof(to_print));
	sprintf(to_print,"count %d, prev_peak  = %f, decay_peak %f\r\n",count,prev_max,decay_value);
	PRINT(to_print);
	count++;

	return peak_value;
}
//*****************************************************************************
// Name        : decibel
//
// Description : function to find DBFS
//
// Arguments   : None
//
// return      : none
//
//****************************************************************************/
void decibel()
{
	int rows;

	//finding the size of lookup table
	rows = (sizeof(mapping) / sizeof(mapping[0]) );

	int row_no = 0;

	for(row_no =0; row_no<rows;row_no++)
	{
		if((abs(peak_value) <= mapping[row_no].peak_high) && (abs(peak_value) >= mapping[row_no].peak_low))
		{
			memset(to_print,0,sizeof(to_print));
			sprintf(to_print,"DBFS = %f\r\n",(mapping[row_no].dbFS));
			PRINT(to_print);
		}
	}
}
