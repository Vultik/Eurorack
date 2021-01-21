#include <stdio.h>
#include <stdlib.h>
#include "dsp.h"
#include "tim.h"
#include "math.h"

#define SAMPLES_LENGTH			200
#define ADC_CHANNELS			6
#define ADC_BUFFER_SIZE			SAMPLES_LENGTH*ADC_CHANNELS
#define ADC_BUFFER_HALF_SIZE	(SAMPLES_LENGTH/2)*ADC_CHANNELS
#define DAC_BUFFER_1_SIZE		SAMPLES_LENGTH
#define DAC_BUFFER_1_HALF_SIZE	SAMPLES_LENGTH/2
#define DAC_BUFFER_2_SIZE		SAMPLES_LENGTH
#define DAC_BUFFER_2_HALF_SIZE	SAMPLES_LENGTH/2




uint16_t sin_wave[200] = {
	2048,2110,2173,2236,2298,2360,2422,2484,2545,2605,2666,2725,2784,2842
	,2899,2955,3011,3066,3119,3172,3223,3273,3322,3370,3417,3462,3505,3548
	,3589,3628,3666,3702,3736,3769,3800,3830,3857,3883,3907,3929,3950,3968
	,3985,3999,4012,4023,4032,4039,4044,4047,4048,4047,4044,4039,4032,4023
	,4012,3999,3985,3968,3950,3929,3907,3883,3857,3830,3800,3769,3736,3702
	,3666,3628,3589,3548,3505,3462,3417,3370,3322,3273,3223,3172,3119,3066
	,3011,2955,2899,2842,2784,2725,2666,2605,2545,2484,2422,2360,2298,2236
	,2173,2110,2048,1985,1922,1859,1797,1735,1673,1611,1550,1490,1429,1370
	,1311,1253,1196,1140,1084,1029,976,923,872,822,773,725,678,633
	,590,547,506,467,429,393,359,326,295,265,238,212,188,166
	,145,127,110,96,83,72,63,56,51,48,48,48,51,56
	,63,72,83,96,110,127,145,166,188,212,238,265,295,326
	,359,393,429,467,506,547,590,633,678,725,773,822,872,923
	,976,1029,1084,1140,1196,1253,1311,1370,1429,1490,1550,1611,1673,1735
	,1797,1859,1922,1985
};

uint16_t triangle_wave[200] = {
	39,79,118,158,198,237,277,316,356,396,435,475,514,554
	,594,633,673,712,752,792,831,871,910,950,990,1029,1069,1108
	,1148,1188,1227,1267,1306,1346,1386,1425,1465,1504,1544,1584,1623,1663
	,1702,1742,1782,1821,1861,1900,1940,1980,2019,2059,2099,2138,2178,2217
	,2257,2297,2336,2376,2415,2455,2495,2534,2574,2613,2653,2693,2732,2772
	,2811,2851,2891,2930,2970,3009,3049,3089,3128,3168,3207,3247,3287,3326
	,3366,3405,3445,3485,3524,3564,3603,3643,3683,3722,3762,3801,3841,3881
	,3920,3960,4000,3960,3920,3881,3841,3801,3762,3722,3683,3643,3603,3564
	,3524,3485,3445,3405,3366,3326,3287,3247,3207,3168,3128,3089,3049,3009
	,2970,2930,2891,2851,2811,2772,2732,2693,2653,2613,2574,2534,2495,2455
	,2415,2376,2336,2297,2257,2217,2178,2138,2099,2059,2019,1980,1940,1900
	,1861,1821,1782,1742,1702,1663,1623,1584,1544,1504,1465,1425,1386,1346
	,1306,1267,1227,1188,1148,1108,1069,1029,990,950,910,871,831,792
	,752,712,673,633,594,554,514,475,435,396,356,316,277,237
	,198,158,118,79
};

uint16_t square_wave[200] = {
	4048,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048
	,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048
	,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048
	,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048
	,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048
	,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048
	,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048,4048
	,4048,4048,4048,48,48,48,48,48,48,48,48,48,48,48
	,48,48,48,48,48,48,48,48,48,48,48,48,48,48
	,48,48,48,48,48,48,48,48,48,48,48,48,48,48
	,48,48,48,48,48,48,48,48,48,48,48,48,48,48
	,48,48,48,48,48,48,48,48,48,48,48,48,48,48
	,48,48,48,48,48,48,48,48,48,48,48,48,48,48
	,48,48,48,48,48,48,48,48,48,48,48,48,48,48
	,48,48,48,48
};

uint16_t sawtooth_wave[200] = {
	48,67,87,107,127,147,167,187,207,227,247,267,287,307
	,327,347,367,387,407,427,447,467,487,507,527,547,567,587
	,607,627,647,667,687,707,727,747,767,787,807,827,847,867
	,887,907,927,947,967,987,1007,1027,1047,1067,1087,1107,1127,1147
	,1167,1187,1207,1227,1247,1267,1287,1307,1327,1347,1367,1387,1407,1427
	,1447,1467,1487,1507,1527,1547,1567,1587,1607,1627,1647,1667,1687,1707
	,1727,1748,1767,1787,1807,1827,1847,1867,1887,1907,1927,1948,1967,1987
	,2007,2027,2047,2067,2087,2107,2127,2147,2167,2187,2207,2227,2247,2267
	,2287,2307,2327,2347,2367,2387,2407,2427,2447,2467,2487,2507,2527,2547
	,2567,2587,2607,2627,2647,2667,2687,2707,2727,2747,2767,2787,2807,2828
	,2847,2867,2887,2907,2927,2947,2967,2987,3007,3027,3047,3067,3087,3107
	,3127,3147,3167,3187,3207,3227,3247,3267,3287,3307,3327,3348,3367,3387
	,3407,3427,3448,3467,3487,3507,3527,3547,3567,3587,3607,3627,3647,3667
	,3687,3707,3727,3747,3767,3787,3807,3827,3848,3867,3887,3907,3927,3947
	,3967,3987,4007,4027
};



//Variables globales
float num_ech = 200;
const float note[96] = {
	32.7, 34.65, 36.71, 38.89, 41.2, 43.65, 46.25, 49, 51.91, 55, 58.27, 61.74,
	65.41, 69.30, 73.42, 77.78, 82.41, 87.31, 92.5, 98, 103.83, 110, 116.54, 123.47,
	130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185, 196, 207.65, 220, 233.08, 246.94,
	261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392, 415.3, 440, 466.16, 493.88,
	523.25, 554.37, 587.33, 622.25, 659.26, 698.46, 739.99, 783.99, 830.61, 880, 932.33, 987.77,
	1046.5, 1108.73, 1174.66, 1244.51, 1318.51, 1396.91, 1479.98, 1567.98, 1661.22, 1760, 1864.66, 1975.53,
	2093, 2217.46, 2349.32, 2489.02, 2637.02, 2793.83, 2959.96, 3135.96, 3322.44, 3520, 3729.31, 3951.07,
	4186.01, 4434.92, 4698.64, 4978.03, 5274.04, 5587.65, 5919.91, 6271.93, 6644.88, 7040, 7458.62, 7902.13
};

uint32_t timer_clock = 80000000U;


volatile uint16_t* in_buffer_ptr_adc;
volatile uint16_t* in_buffer_ptr_1;
volatile uint16_t* in_buffer_ptr_2;
volatile uint16_t* in_buffer_ptr_3;
volatile uint16_t* in_buffer_ptr_4;
volatile uint16_t* out_buffer_1_ptr;
volatile uint16_t* out_buffer_2_ptr;

/*	CV_0 -> PA0 -> CHANNEL_5 -> RANK_2
 * 	CV_1 -> PA1 -> CHANNEL_6 -> RANK_3
 *	CV_2 -> PA6 -> CHANNEL_11 -> RANK_4
 *	CV_3 -> PB0 -> CHANNEL_15 -> RANK_6
 *	IN_L -> PA7 -> CHANNEL_12 -> RANK_5
 *	IN_R -> PA3 -> CHANNEL_8 -> RANK_1
 * */
// adc_buffer[6n] -> ADC_RANK_1
// adc_buffer[6n+1] -> ADC_RANK_2

uint16_t adc_buffer[ADC_BUFFER_SIZE];
uint16_t dac_buffer_1[DAC_BUFFER_1_SIZE];
uint16_t dac_buffer_2[DAC_BUFFER_2_SIZE];

float sin_amplitude = 0;
float triangle_amplitude = 0;
float square_amplitude = 0;
float saw_amplitude = 0;


//Variables externes
extern float x;
extern float y;
extern uint8_t note_index;
extern ADC_HandleTypeDef hadc1;
extern DAC_HandleTypeDef hdac1;
extern uint8_t start_dsp;

float get_note_frequency(uint8_t desired_note_index){
	return note[desired_note_index] ;
}


void changePitch(uint8_t desired_note_index){
	float desired_frequency = get_note_frequency(desired_note_index) * num_ech;
	//Calcul du Counter Period de TIM2

	TIM2->ARR = (int)(timer_clock/desired_frequency -1);
	TIM2->CNT = 0;
}

void change_direct_frequency(float desired_frequency){
	TIM2->ARR = (int)(timer_clock/desired_frequency -1);
	TIM2->CNT = 0;
}

void dsp_init(){
	HAL_ADC_Start_DMA(&hadc1 , (uint32_t *) adc_buffer, ADC_BUFFER_SIZE);
	//  HAL_Delay(1);
	HAL_DAC_Start_DMA(&hdac1,DAC_CHANNEL_1 , (uint32_t *) dac_buffer_1, DAC_BUFFER_1_SIZE, DAC_ALIGN_12B_R);
	HAL_DAC_Start_DMA(&hdac1,DAC_CHANNEL_2 , (uint32_t *) dac_buffer_2, DAC_BUFFER_2_SIZE, DAC_ALIGN_12B_R);
	HAL_TIM_Base_Start(&htim2);
}

void processDSP(){
	__NOP();

	//Get pitch from CV0 & CV1 input
	float CV0_average =0;
	float CV1_average =0;
	float CV2_average =0;
	for(uint8_t i=0;i!=10;i++){
		CV0_average +=adc_buffer[6*i+1];
		CV1_average +=adc_buffer[6*i+2];
		CV2_average +=adc_buffer[6*i+3];
	}

	//Get average
	CV0_average = 2048-CV0_average/10;
	CV1_average = 2048-CV1_average/10;
	CV2_average = 2048-CV2_average/10;

	//Convert from 0->3.3V (0->4096) to -10 +10 (-2048->2048)input
	/*CV0_average = CV0_average;
	CV1_average = CV1_average;
	CV2_average = CV2_average;*/

	//CVx value between -10 to 10 (float)
	CV0_average = CV0_average *(10.0/1100.0);
	CV1_average = CV1_average *(10.0/2048.0);
	CV2_average = CV2_average *(10.0/2048.0);

	//Change pitch
	float cv_frequency = get_note_frequency(note_index)*pow(2,CV0_average);
	changePitch(note_index);

	/*if(note_index < 0){
		note_index = 0;
	}
	if(note_index >95){
		note_index = 95;
	}*/

	change_direct_frequency(200*cv_frequency);
	//Change x and y
	float x_cv = CV1_average/10.0;
	float y_cv = CV2_average/10.0;
	//x= x + x_cv;
	//y= y + y_cv;

	if(x_cv <0){x_cv =0;}
	if(x_cv >1){x_cv =1;}
	if(y_cv <0){y_cv =0;}
	if(y_cv >1){y_cv =1;}

	//Get amplitude of wave
	sin_amplitude = (1-(x+x_cv))*(1-(y+y_cv));
	triangle_amplitude = (x+x_cv)*(1-(y+y_cv));
	square_amplitude = (1-(x+x_cv))*(y+y_cv);
	saw_amplitude = (x+x_cv)*(y+y_cv);

	//For DAC 2 (OUT_1)
	for(uint16_t i=0; i!= SAMPLES_LENGTH/2; i++){
		out_buffer_2_ptr[i] = sin_amplitude*in_buffer_ptr_1[i] + triangle_amplitude*in_buffer_ptr_2[i]+ square_amplitude*in_buffer_ptr_3[i]+in_buffer_ptr_4[i]*saw_amplitude;
	}

	//For DAC 1 (OUT_2)
	/*for(uint16_t i=0; i!= SAMPLES_LENGTH/2; i++){
		out_buffer_2_ptr[i] = in_buffer_ptr_1[6*i];
		out_buffer_2_ptr[i] = in_buffer_ptr_1[i];
	}*/
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc){
	__NOP();
	//First half ot the buffer is full
	in_buffer_ptr_adc =  &adc_buffer[0];
	in_buffer_ptr_1 =  &sin_wave[0];
	in_buffer_ptr_2 =  &triangle_wave[0];
	in_buffer_ptr_3 =  &square_wave[0];
	in_buffer_ptr_4 =  &sawtooth_wave[0];
	out_buffer_1_ptr = &dac_buffer_1[DAC_BUFFER_1_HALF_SIZE];
	out_buffer_2_ptr = &dac_buffer_2[DAC_BUFFER_2_HALF_SIZE];
	start_dsp = 1;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	__NOP();
	//The buffer is now full
	in_buffer_ptr_adc = &adc_buffer[ADC_BUFFER_HALF_SIZE];
	in_buffer_ptr_1 = &sin_wave[100];
	in_buffer_ptr_2 =  &triangle_wave[100];
	in_buffer_ptr_3 =  &square_wave[100];
	in_buffer_ptr_4 =  &sawtooth_wave[100];
	out_buffer_1_ptr = &dac_buffer_1[0];
	out_buffer_2_ptr = &dac_buffer_2[0];
	start_dsp = 1;
}
