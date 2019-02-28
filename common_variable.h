/**
* @file common_variable.h
* @brief contains the variables used across other files
*
*
* @author Steve and Swarupa
* @date Nov 7, 2018
*
*/
//***********************************************************************************
// Include files
//***********************************************************************************
#ifndef _COMMON_VARIABLE_H_
#define  _COMMON_VARIABLE_H_
#include <stdint.h>


//------------------------------------------------------------------------
// For debugging

//#define DEBUG
//------------------------------------------------------------------------


// Print function

#define PRINT send_to_console_str

//***********************************************************************************
// Globals
//***********************************************************************************

//Error handling enums
typedef enum status_t
{
	NULL_PTR = -5,
	OVERFLOW = -4,
	EMPTY = -3,
	BUFFER_NOT_INITIALISED = -2,
	ERROR = -1,
	SUCCESS = 1,
}status;

#endif
