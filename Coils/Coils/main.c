#include"CPCD.h"
#include "incoil.h"

#include <stdio.h>
#include <stdlib.h>

#define _P_SIZE 79
#define _P_WIDTH (_P_SIZE * 16)
#define _P_HEIGHT (_P_SIZE * 9)

#define _P_BUTTON_HIGHTOFFSET 40
#define _P_BUTTON_WIDTHHEIGHT 20


static _CPCD_SPRITE s_graphwindow;
static _CPCD_MAP m_map;


static _CPCD_ELEMENT_LIST el_list;
static _CPCD_ELEMENT_FIELD  f_field;
#define _P_INCREMENT_ELEMENTS 2
static _CPCD_ELEMENT *  b_buttonlist;


const _CPCD_ELEMENTSTYLE es_styletest = {
	{ { 255,255,255 },{ 66, 134, 244 },{ 255,255,255 } },
{ 33,33,33 }
};


void catchclick(_CPCD_EVENT e_event);
void catchhover(_CPCD_EVENT e_event);
void catchtrigger();
void editclick(_CPCD_SPRITE * s_look);
void edithover(_CPCD_SPRITE * s_look);
void editidle(_CPCD_SPRITE * s_look);

void initelements() {
	b_buttonlist = (_CPCD_ELEMENT*)malloc(sizeof(_CPCD_ELEMENT)*_P_INCREMENT_ELEMENTS * 2);

	f_field.F_EVENT_CLICKED = catchclick;
	f_field.F_EVENT_HOVER = catchhover;
	f_field.F_EVENT_STYLE_CLICKED = editclick;
	f_field.F_EVENT_STYLE_HOVER = edithover;
	f_field.F_EVENT_STYLE_IDLE = editidle;

	el_list.F_TRIGGER = catchtrigger;
	el_list.USI_ELEMENTS = 4;
	el_list.E_ELEMENTS = malloc(sizeof(_CPCD_ELEMENT)*4);
	for (int i_setpos_index = 0; i_setpos_index < _P_INCREMENT_ELEMENTS*2; i_setpos_index+=2)
	{

		b_buttonlist[i_setpos_index].S_DISPLAY = _CPCD_SPR_CREATE(_P_BUTTON_WIDTHHEIGHT, _P_BUTTON_WIDTHHEIGHT, NULL);
		b_buttonlist[i_setpos_index].V_ELEMENT = &f_field;
		b_buttonlist[i_setpos_index].USI_HEIGHT = _P_BUTTON_WIDTHHEIGHT;
		b_buttonlist[i_setpos_index].USI_WIDTH = _P_BUTTON_WIDTHHEIGHT;
		b_buttonlist[i_setpos_index].USI_ELEMENT = _CPCD_EI_FIELD;
		b_buttonlist[i_setpos_index].V_POSITION = (_CPCD_VECTOR) { _P_BUTTON_HIGHTOFFSET + (_P_BUTTON_HIGHTOFFSET*0), _P_BUTTON_HIGHTOFFSET + (_P_BUTTON_HIGHTOFFSET*(i_setpos_index/2)) };

		el_list.E_ELEMENTS[i_setpos_index] = b_buttonlist[i_setpos_index];


		b_buttonlist[i_setpos_index + 1].S_DISPLAY = _CPCD_SPR_CREATE(_P_BUTTON_WIDTHHEIGHT, _P_BUTTON_WIDTHHEIGHT, NULL);
		b_buttonlist[i_setpos_index + 1].V_ELEMENT = &f_field;
		b_buttonlist[i_setpos_index + 1].USI_HEIGHT = _P_BUTTON_WIDTHHEIGHT;
		b_buttonlist[i_setpos_index + 1].USI_WIDTH = _P_BUTTON_WIDTHHEIGHT;
		b_buttonlist[i_setpos_index + 1].USI_ELEMENT = _CPCD_EI_FIELD;
		b_buttonlist[i_setpos_index + 1].V_POSITION = (_CPCD_VECTOR) { _P_BUTTON_HIGHTOFFSET + (_P_BUTTON_HIGHTOFFSET*2), _P_BUTTON_HIGHTOFFSET + (_P_BUTTON_HIGHTOFFSET*(i_setpos_index / 2)) };

		el_list.E_ELEMENTS[i_setpos_index+1] = b_buttonlist[i_setpos_index+1];

	}


}


_CPCD_MAIN{
	//

m_map = _CPCD_CREATE(_P_WIDTH,_P_HEIGHT);
s_graphwindow = _CPCD_SPR_CREATE(_P_WIDTH,_P_HEIGHT,NULL);
s_graphwindow.V_POSITION = _CPCD_LEFTUP;

return 0;
}

_CPCD_UPDATE{

	if(ui_frame  == 0)initelements();

	_CPCD_CLEAR(&m_map,(_CPCD_COLOR) { 0,33,0 });
_CPCD_CLEAR(&s_graphwindow,(_CPCD_COLOR) { 55,0,0 });


_CPCD_DRAWSPRITE(&m_map,s_graphwindow,_CPCD_SBM_OVERRIDE,0);

catchtrigger();

_CPCD_DRAWBUFFER(&m_map);
return 0;
}

_CPCD_END{
	_CPCD_DELETE(&m_map);
}

_CPCD_EVENTS{

	return &el_list;
}

void catchclick(_CPCD_EVENT e_event) {
	printf("TRIGGER %u\n", e_event.USI_INDEX);

}
void catchhover(_CPCD_EVENT e_event) {
	printf("TRIGGER %u\n", e_event.USI_INDEX);

}
void catchtrigger() {
	for (int i_index_element = 0; i_index_element < el_list.USI_ELEMENTS; i_index_element++)
	{
		_CPCD_ELEMENTRENDER(&m_map, es_styletest, el_list.E_ELEMENTS[i_index_element]);
	}
	_CPCD_DRAWBUFFER(&m_map);


}

void editclick(_CPCD_SPRITE * s_look) {
	_CPCD_CLEAR(s_look, (_CPCD_COLOR) { 0, 255, 20 });

}
void edithover(_CPCD_SPRITE * s_look) {
	_CPCD_CLEAR(s_look, (_CPCD_COLOR) { 0, 80, 80 });

}
void editidle(_CPCD_SPRITE * s_look) {
	_CPCD_CLEAR(s_look, (_CPCD_COLOR) { 0, 33, 33 });
}