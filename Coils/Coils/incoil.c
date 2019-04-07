#include "incoil.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <stdio.h>
#include <stdlib.h>




int getperiodinfo(char* c_file, _C_SAMPLE* cs_samples) {
	FILE * f_filestream;
	fopen_s(&f_filestream, c_file,"r");
	if (f_filestream == NULL) return 0;

	fscanf_s(f_filestream, _P_FILE_HEADER,&(cs_samples->f_resistor), &(cs_samples->f_inductor), &(cs_samples->i_samples));


	if ((*cs_samples).cp_samples != NULL)free((*cs_samples).cp_samples);
	(*cs_samples).cp_samples = (_C_PERIOD*)malloc(sizeof(_C_PERIOD)*(cs_samples->i_samples));

	float f_div;
	for (int i_cpindex = 0,i_return = 0; i_cpindex<(cs_samples->i_samples); i_cpindex++)
	{
		i_return=fscanf_s(f_filestream, _P_FILE_BODY, &((*cs_samples).cp_samples[i_cpindex].f_voltage), &((*cs_samples).cp_samples[i_cpindex].f_time), &f_div);
		(*cs_samples).cp_samples[i_cpindex].f_time /= f_div;
		if (i_return== EOF)break;

	}

	fclose(f_filestream);

	return 1;
}
int getdata_a(_C_SAMPLE cs_sample, unsigned short us_periods,unsigned short us_upscale, _C_POINT ** p_dataset,int * i_setlength ) {
	(*i_setlength) = cs_sample.i_samples*(us_upscale + 2);
	if ((*p_dataset) != NULL)free(*p_dataset);
	(*p_dataset) = (_C_POINT*)malloc(sizeof(_C_POINT)*(*i_setlength));

	const register int i_bitsperbeat = (us_upscale + 2);
	const register float f_tau = cs_sample.f_inductor/cs_sample.f_resistor;
	register float f_imaxvalue = 0, f_imaxvaluedelta = 0;

	register float f_imaxvalueoffset = 0;

	register float f_time = 0, f_timedelta = 0;
	register float f_dir = -1;

	for (int i_index = 0, i_offset = 0, i_timeindex; i_index < cs_sample.i_samples; i_index++)
	{
		i_offset = i_index * i_bitsperbeat;
		f_imaxvalue = cs_sample.cp_samples[i_index].f_voltage / cs_sample.f_resistor;

		f_imaxvaluedelta = f_imaxvalue;

		if (f_dir == -1) {
			f_dir = 1;
			f_imaxvaluedelta = 0;
		}
		else {
			if (cs_sample.cp_samples[i_index - 1].f_voltage > cs_sample.cp_samples[i_index].f_voltage)f_dir = 0;
			if (cs_sample.cp_samples[i_index - 1].f_voltage < cs_sample.cp_samples[i_index].f_voltage)f_dir = 1;
		}

		for (i_timeindex = 0; i_timeindex < i_bitsperbeat; i_timeindex++)
		{
			f_time = ((float)i_timeindex / ((float)i_bitsperbeat - 1.f))*cs_sample.cp_samples[i_index].f_time;

			if (f_dir == 1) {
				(*p_dataset)[i_offset + i_timeindex].d_i = (double)(1.0f - pow(M_E, (-f_time / f_tau)));
				(*p_dataset)[i_offset + i_timeindex].d_i *= (f_imaxvalue - f_imaxvalueoffset);
				(*p_dataset)[i_offset + i_timeindex].d_i += f_imaxvalueoffset;
			}
			if (f_dir == 0) {
				(*p_dataset)[i_offset + i_timeindex].d_i = (double)(pow(M_E, (-f_time / f_tau)));
				(*p_dataset)[i_offset + i_timeindex].d_i *= (f_imaxvalueoffset - f_imaxvaluedelta);
				(*p_dataset)[i_offset + i_timeindex].d_i += f_imaxvaluedelta;
			}

			(*p_dataset)[i_offset + i_timeindex].d_ur = cs_sample.f_resistor*(*p_dataset)[i_offset + i_timeindex].d_i;
			(*p_dataset)[i_offset + i_timeindex].d_ul = cs_sample.cp_samples[i_index].f_voltage - (*p_dataset)[i_offset + i_timeindex].d_ur;


			(*p_dataset)[i_offset + i_timeindex].d_u = cs_sample.cp_samples[i_index].f_voltage;
			(*p_dataset)[i_offset + i_timeindex].d_t = f_time + f_timedelta;

		}
		f_imaxvalueoffset = (*p_dataset)[i_offset + i_timeindex - 1].d_i;
		f_timedelta += f_time;
	}
	
	return  0;
}
//printf("%f # %f\n\n", f_imaxvalueoffset, f_timedelta);
//printf("%0.1f]%d/%d->%lf # %f\n", f_dir,i_index, i_timeindex, (*p_dataset)[i_offset + i_timeindex].d_i, f_time);
