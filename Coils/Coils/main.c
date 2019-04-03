#include"CPCD.h"
#include "strmod.h"
#include "incoil.h"

#include <stdio.h>
#include <stdlib.h>

#define _P_BUTTON_HIGHTOFFSET 50
#define _P_BUTTON_PADDING 30
#define _P_BUTTON_WIDTHHEIGHT 20

#define _P_SIZE 79
#define _P_WIDTH (_P_SIZE * 16)
#define _P_HEIGHT (_P_SIZE * 9)
#define _P_HEIGHT_S (_P_HEIGHT-(_P_HEIGHT/4))



#define _P_GRAPHZERO	(2*_P_HEIGHT_S/4)


#define _P_GRAPHCOLOR_ZEROLINE (_CPCD_COLOR){200,200,200}
#define _P_GRAPHCOLOR_POINTS (_CPCD_COLOR){255,0,0}
#define _P_GRAPHCOLOR_BACKCOLOR (_CPCD_COLOR){0,0,0}

static unsigned short us_periods = 1;
static unsigned short us_upscale = 10;

static unsigned float us_draw_scale_y = 1000.f;
static unsigned float us_draw_scale_x = 3000.f;

static _C_SAMPLE cs_loadedsample;


static _CPCD_SPRITE s_graphwindow;
static _CPCD_MAP m_map;


static _CPCD_VECTOR * v_intexts;
static _CPCD_ELEMENT_LIST el_list;
static _CPCD_ELEMENT_FIELD  f_field;
#define _P_INCREMENT_ELEMENTS 2
static _CPCD_ELEMENT *  b_buttonlist;


const _CPCD_ELEMENTSTYLE es_styletest = {
	{ { 255,255,255 },{ 100, 100, 100 },{ 255,255,255 } },
{ 33,33,33 }
};

void catchclick(_CPCD_EVENT e_event);
void catchhover(_CPCD_EVENT e_event);
void catchtrigger();
void editclick(_CPCD_SPRITE * s_look);
void edithover(_CPCD_SPRITE * s_look);
void editidle(_CPCD_SPRITE * s_look);
void initelements();
int callupdate();

_CPCD_MAIN
{
	getperiodinfo("H:\\VisualStudio\\Projekts\\C\\Coils\\GIT\\Coils\\Coils\\Coils\\infofile.txt", &cs_loadedsample);

	initelements();
	m_map = _CPCD_CREATE(_P_WIDTH,_P_HEIGHT);
	s_graphwindow = _CPCD_SPR_CREATE(_P_WIDTH, _P_HEIGHT_S,NULL);
	s_graphwindow.V_POSITION = (_CPCD_VECTOR) {
		0, 
		_P_BUTTON_PADDING*2 + _P_BUTTON_WIDTHHEIGHT * 2+ _P_BUTTON_HIGHTOFFSET
	};
	return 0;
}

_CPCD_UPDATE
{
	_CPCD_CLEAR(&m_map,_P_GRAPHCOLOR_BACKCOLOR);

	callupdate();

	_CPCD_DRAWSPRITE(&m_map, s_graphwindow, _CPCD_SBM_OVERRIDE, 0);

	SM_ALIAS sm_string = SM_TOSTRING(us_periods);
	_CPCD_DRAWSTRING(&m_map, sm_string.c_text, sm_string.i_length, v_intexts[0], 4, (_CPCD_COLOR) { 255, 255, 255 });
	sm_string = SM_TOSTRING(us_upscale);
	_CPCD_DRAWSTRING(&m_map, sm_string.c_text, sm_string.i_length, v_intexts[2], 4, (_CPCD_COLOR) { 255, 255, 255 });
	sm_string = SM_SET("REPEATS");
	_CPCD_DRAWSTRING(&m_map, sm_string.c_text, sm_string.i_length, v_intexts[1], 4, (_CPCD_COLOR) { 255, 255, 255 });
	sm_string = SM_SET("SCALE");
	_CPCD_DRAWSTRING(&m_map, sm_string.c_text, sm_string.i_length, v_intexts[3], 4, (_CPCD_COLOR) { 255, 255, 255 });

	catchtrigger();

	_CPCD_DRAWBUFFER(&m_map);
	return 0;
}

_CPCD_END
{
	_CPCD_DELETE(&m_map);
	free(v_intexts);
	free(b_buttonlist);
}

_CPCD_EVENTS
{
	return &el_list;
}

void catchclick(_CPCD_EVENT e_event) 
{
	if (e_event.USI_INDEX == 1)us_periods++;
	if (e_event.USI_INDEX == 0 && us_periods>1)us_periods--;
	if (e_event.USI_INDEX == 2 && us_upscale>1)us_upscale--;
	if (e_event.USI_INDEX == 3)us_upscale++;
	callupdate();
}

void catchhover(_CPCD_EVENT e_event) {}

void catchtrigger()
{
	for (int i_index_element = 0; i_index_element < el_list.USI_ELEMENTS; i_index_element++)
	{
		_CPCD_ELEMENTRENDER(&m_map, es_styletest, el_list.E_ELEMENTS[i_index_element]);
	}
	_CPCD_DRAWBUFFER(&m_map);
}

void initelements() 
{
	b_buttonlist = (_CPCD_ELEMENT*)malloc(sizeof(_CPCD_ELEMENT)*_P_INCREMENT_ELEMENTS * 2);
	v_intexts = (_CPCD_VECTOR*)malloc(sizeof(_CPCD_VECTOR)*_P_INCREMENT_ELEMENTS * 2);

	f_field.F_EVENT_CLICKED = catchclick;
	f_field.F_EVENT_HOVER = catchhover;
	f_field.F_EVENT_STYLE_CLICKED = editclick;
	f_field.F_EVENT_STYLE_HOVER = edithover;
	f_field.F_EVENT_STYLE_IDLE = editidle;

	el_list.F_TRIGGER = catchtrigger;
	el_list.USI_ELEMENTS = 4;
	el_list.E_ELEMENTS = malloc(sizeof(_CPCD_ELEMENT) * 4);

	for (int i_setpos_index = 0; i_setpos_index < _P_INCREMENT_ELEMENTS * 2; i_setpos_index += 2)
	{
		v_intexts[i_setpos_index ] = (_CPCD_VECTOR) { 
			_P_BUTTON_PADDING + (_P_BUTTON_HIGHTOFFSET * 1), 
				_P_BUTTON_PADDING + (_P_BUTTON_HIGHTOFFSET*(i_setpos_index / 2)) 
		};
		v_intexts[i_setpos_index +1] = (_CPCD_VECTOR) { 
			_P_BUTTON_PADDING + (_P_BUTTON_HIGHTOFFSET * 3), 
				_P_BUTTON_PADDING + (_P_BUTTON_HIGHTOFFSET*(i_setpos_index / 2)) 
		};

		b_buttonlist[i_setpos_index].S_DISPLAY = _CPCD_SPR_CREATE(_P_BUTTON_WIDTHHEIGHT, _P_BUTTON_WIDTHHEIGHT, NULL);
		b_buttonlist[i_setpos_index].V_ELEMENT = &f_field;
		b_buttonlist[i_setpos_index].USI_HEIGHT = _P_BUTTON_WIDTHHEIGHT;
		b_buttonlist[i_setpos_index].USI_WIDTH = _P_BUTTON_WIDTHHEIGHT;
		b_buttonlist[i_setpos_index].USI_ELEMENT = _CPCD_EI_FIELD;
		b_buttonlist[i_setpos_index].V_POSITION = (_CPCD_VECTOR) { 	
			_P_BUTTON_PADDING + (_P_BUTTON_HIGHTOFFSET * 0), 
			_P_BUTTON_PADDING + (_P_BUTTON_HIGHTOFFSET*(i_setpos_index / 2)) 
		};

		el_list.E_ELEMENTS[i_setpos_index] = b_buttonlist[i_setpos_index];

		b_buttonlist[i_setpos_index + 1].S_DISPLAY = _CPCD_SPR_CREATE(_P_BUTTON_WIDTHHEIGHT, _P_BUTTON_WIDTHHEIGHT, NULL);
		b_buttonlist[i_setpos_index + 1].V_ELEMENT = &f_field;
		b_buttonlist[i_setpos_index + 1].USI_HEIGHT = _P_BUTTON_WIDTHHEIGHT;
		b_buttonlist[i_setpos_index + 1].USI_WIDTH = _P_BUTTON_WIDTHHEIGHT;
		b_buttonlist[i_setpos_index + 1].USI_ELEMENT = _CPCD_EI_FIELD;
		b_buttonlist[i_setpos_index + 1].V_POSITION = (_CPCD_VECTOR) { 
			_P_BUTTON_PADDING + (_P_BUTTON_HIGHTOFFSET * 2), 
			_P_BUTTON_PADDING + (_P_BUTTON_HIGHTOFFSET*(i_setpos_index / 2)) 
		};

		el_list.E_ELEMENTS[i_setpos_index + 1] = b_buttonlist[i_setpos_index + 1];
	}
}

void editclick(_CPCD_SPRITE * s_look) 
{
	_CPCD_CLEAR(s_look, (_CPCD_COLOR) { 255, 255, 255 });

}

void edithover(_CPCD_SPRITE * s_look) 
{
	_CPCD_CLEAR(s_look, (_CPCD_COLOR) { 100, 100, 100 });

}

void editidle(_CPCD_SPRITE * s_look) 
{
	_CPCD_CLEAR(s_look, (_CPCD_COLOR) { 33, 33, 33 });
}

int callupdate() 
{
	printf("PING\n");
	_C_POINT * p_dataset = NULL;
	int i_datalength;

	getdata_a(cs_loadedsample, us_periods, us_upscale,&p_dataset,&i_datalength);
	
	_CPCD_CLEAR(&s_graphwindow, _P_GRAPHCOLOR_BACKCOLOR);


	_CPCD_DRAWLINE(&s_graphwindow, (_CPCD_VECTOR) {0, _P_GRAPHZERO}, (_CPCD_VECTOR) { _P_WIDTH, _P_GRAPHZERO}, _P_GRAPHCOLOR_ZEROLINE);
	 
	
	_CPCD_DRAWLINE(&s_graphwindow, (_CPCD_VECTOR) { _P_BUTTON_PADDING, 0}, (_CPCD_VECTOR) { _P_BUTTON_PADDING, _P_HEIGHT_S}, _P_GRAPHCOLOR_ZEROLINE);

	register _CPCD_VECTOR v_posvec,v_lastpos;

	for (int i_cdindex = 0; i_cdindex < i_datalength; i_cdindex++)
	{
		if (i_cdindex == 0)v_lastpos = (_CPCD_VECTOR) { _P_BUTTON_PADDING , _P_GRAPHZERO};

		v_posvec.X = (int)(p_dataset[i_cdindex].d_t*us_draw_scale_x);
		v_posvec.Y = -(int)(p_dataset[i_cdindex].d_i*us_draw_scale_y);

		v_posvec.X += _P_BUTTON_PADDING;
		v_posvec.Y += _P_GRAPHZERO;


		_CPCD_DRAWLINE(&s_graphwindow, v_posvec, v_lastpos, _P_GRAPHCOLOR_POINTS);

		v_lastpos = v_posvec;
	}

	return 0;
}