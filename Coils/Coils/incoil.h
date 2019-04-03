#pragma once

#define _C_POINT c_point
typedef struct {
	double d_t;
	double d_i;
	double d_u;
	double d_ul;
	double d_ur;
}_C_POINT;

#define _C_PERIOD c_period
typedef struct {
	float f_time;
	float f_voltage;
}_C_PERIOD;

#define _C_SAMPLE c_sample 
typedef struct {
	float f_resistor;
	float f_inductor;
	int i_samples;
	_C_PERIOD * cp_samples;
	
}_C_SAMPLE;

#define _P_FILE_HEADER "[R %f #L %f #P %d]\n"
#define _P_FILE_BODY "(V %f |T %f / %f)\n"
int getperiodinfo(char* c_file, _C_SAMPLE* cs_samples);
int getdata_a(_C_SAMPLE cs_sample, unsigned short us_periods, unsigned short us_upscale, _C_POINT ** p_dataset, int * i_setlength);