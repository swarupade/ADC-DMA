/*
 * main.h
 *
 *  Created on: Dec 9, 2018
 *      Author: Steve
 */

#ifndef CODE_INCLUDE_MAIN_H_
#define CODE_INCLUDE_MAIN_H_

//***********************************************************************************
// Include files
//***********************************************************************************
#include "MKL25Z4.h"
#include "common_variable.h"
#include "dma.h"
#include "uart.h"
#include "adc.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//***********************************************************************************
// MACROS
//***********************************************************************************
#define ALPHA (0.9)
#define ADC_MIN_VALUE (-32768)
#define BUFFER_ELTS (32)

//***********************************************************************************
//  Function prototypes
//***********************************************************************************
double peak_value_cal();
int16_t max();
void display();
void decibel();

//***********************************************************************************
// Globals
//***********************************************************************************
int16_t max_value;
char to_print[100];
double peak_value;
int16_t destination[64];
int16_t *ptr_DB;

int8_t FLAG_BUFFER_FULL;
int8_t TOP_FLAG;
int8_t DOWN_FLAG;

//creating a structure for lookup table
struct raw_adc
{
	double peak_high;
	double peak_low;
    double dbFS;
};

//Lookup table for dbFS
struct raw_adc mapping[] =
{
		{32768, 32569, 0.000},
		{32568, 32369, -0.05},
		{32368, 32169, -0.10},
		{32168, 31969, -0.16},
		{31968, 31769, -0.21},
		{31768, 31569, -0.26},
		{31568, 31369, -0.32},
		{31368, 31169, -0.37},
		{31168, 30969, -0.43},
		{30968, 30769, -0.49},
		{30768, 30569, -0.54},
		{30568, 30369, -0.60},
		{30368, 30169, -0.66},
		{30168, 29969, -0.71},
		{29968, 29769, -0.77},
		{29768, 29569, -0.83},
		{29568, 29369, -0.89},
		{29368, 29169, -0.95},
		{29168, 28969, -1.01},
		{28968, 28769, -1.07},
		{28768, 28569, -1.13},
		{28568, 28369, -1.19},
		{28368, 28169, -1.25},
		{28168, 27969, -1.31},
		{27968, 27769, -1.37},
		{27768, 27569, -1.43},
		{27568, 27369, -1.50},
		{27368, 27169, -1.56},
		{27168, 26969, -1.62},
		{26968, 26769, -1.69},
		{26768, 26569, -1.75},
		{26568, 26369, -1.82},
		{26368, 26169, -1.88},
		{26168, 25969, -1.95},
		{25968, 25769, -2.02},
		{25768, 25569, -2.08},
		{25568, 25369, -2.15},
		{25368, 25169, -2.22},
		{25168, 24969, -2.29},
		{24968, 24769, -2.36},
		{24768, 24569, -2.43},
		{24568, 24369, -2.50},
		{24368, 24169, -2.57},
		{24168, 23969, -2.64},
		{23968, 23769, -2.71},
		{23768, 23569, -2.78},
		{23568, 23369, -2.86},
		{23368, 23169, -2.93},
		{23168, 22969, -3.01},
		{22968, 22769, -3.08},
		{22768, 22569, -3.16},
		{22568, 22369, -3.23},
		{22368, 22169, -3.31},
		{22168, 21969, -3.39},
		{21968, 21769, -3.47},
		{21768, 21569, -3.55},
		{21568, 21369, -3.63},
		{21368, 21169, -3.71},
		{21168, 20969, -3.79},
		{20968, 20769, -3.87},
		{20768, 20569, -3.96},
		{20568, 20369, -4.04},
		{20368, 20169, -4.13},
		{20168, 19969, -4.21},
		{19968, 19769, -4.30},
		{19768, 19569, -4.38},
		{19568, 19369, -4.47},
		{19368, 19169, -4.56},
		{19168, 18969, -4.65},
		{18968, 18769, -4.74},
		{18768, 18569, -4.84},
		{18568, 18369, -4.93},
		{18368, 18169, -5.02},
		{18168, 17969, -5.12},
		{17968, 17769, -5.21},
		{17768, 17569, -5.31},
		{17568, 17369, -5.41},
		{17368, 17169, -5.51},
		{17168, 16969, -5.61},
		{16968, 16769, -5.71},
		{16768, 16569, -5.81},
		{16568, 16369, -5.92},
		{16368, 16169, -6.02},
		{16168, 15969, -6.13},
		{15968, 15769, -6.24},
		{15768, 15569, -6.35},
		{15568, 15369, -6.46},
		{15368, 15169, -6.57},
		{15168, 14969, -6.69},
		{14968, 14769, -6.80},
		{14768, 14569, -6.92},
		{14568, 14369, -7.04},
		{14368, 14169, -7.16},
		{14168, 13969, -7.28},
		{13968, 13769, -7.40},
		{13768, 13569, -7.53},
		{13568, 13369, -7.65},
		{13368, 13169, -7.78},
		{13168, 12969, -7.91},
		{12968, 12769, -8.05},
		{12768, 12569, -8.18},
		{12568, 12369, -8.32},
		{12368, 12169, -8.46},
		{12168, 11969, -8.60},
		{11968, 11769, -8.74},
		{11768, 11569, -8.89},
		{11568, 11369, -9.04},
		{11368, 11169, -9.19},
		{11168, 10969, -9.34},
		{10968, 10769, -9.50},
		{10768, 10569, -9.66},
		{10568, 10369, -9.82},
		{10368, 10169, -9.99},
		{10168,  9969, -10.16},
		{ 9968,  9769, -10.33},
		{ 9768,  9569, -10.51},
		{ 9568,  9369, -10.69},
		{ 9368,  9169, -10.87},
		{ 9168,  8969, -11.06},
		{ 8968,  8769, -11.25},
		{ 8768,  8569, -11.45},
		{ 8568,  8369, -11.65},
		{ 8368,  8169, -11.85},
		{ 8168,  7969, -12.06},
		{ 7968,  7769, -12.28},
		{ 7768,  7569, -12.50},
		{ 7568,  7369, -12.72},
		{ 7368,  7169, -12.96},
		{ 7168,  6969, -13.20},
		{ 6968,  6769, -13.44},
		{ 6768,  6569, -13.69},
		{ 6568,  6369, -13.96},
		{ 6368,  6169, -14.22},
		{ 6168,  5969, -14.50},
		{ 5968,  5769, -14.79},
		{ 5768,  5569, -15.08},
		{ 5568,  5369, -15.39},
		{ 5368,  5169, -15.71},
		{ 5168,  4969, -16.04},
		{ 4968,  4769, -16.38},
		{ 4768,  4569, -16.74},
		{ 4568,  4369, -17.11},
		{ 4368,  4169, -17.50},
		{ 4168,  3969, -17.91},
		{ 3968,  3769, -18.33},
		{ 3768,  3569, -18.78},
		{ 3568,  3369, -19.26},
		{ 3368,  3169, -19.76},
		{ 3168,  2969, -20.29},
		{ 2968,  2769, -20.85},
		{ 2768,  2569, -21.46},
		{ 2568,  2369, -22.11},
		{ 2368,  2169, -22.82},
		{ 2168,  1969, -23.58},
		{ 1968,  1769, -24.42},
		{ 1768,  1569, -25.35},
		{ 1568,  1369, -26.40},
		{ 1368,  1169, -27.58},
		{ 1168,   969, -28.96},
		{  968,   769, -30.59},
		{  768,   569, -32.60},
		{  568,   369, -35.22},
		{  368,   169, -38.99},
		{  168,   0, -45.80}
};


#endif /* CODE_INCLUDE_MAIN_H_ */