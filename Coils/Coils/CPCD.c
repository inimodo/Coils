#define _CRT_SECURE_NO_WARNINGS
#define _CPCD_FILE_WRITEMODE	"w"
#define _CPCD_FILE_NAME_RGB	"export.ppm"
#define _CPCD_FILE_NAME_GRA	"export.pgm"
#define _CPCD_FILE_PGM		"P2\n"
#define _CPCD_FILE_PPM		"P3\n"
#define _CPCD_FILE_S			"255\n"
#define _CPCD_FILE_WH		"%hu  %hu\n"
#define _CPCD_FILE_COLOR		"%hu  %hu  %hu    "
#define _CPCD_FILE_GRAY		"%hu  "
#define _CPCD_FILE_BRK		"\n"
#define _CPCD_WINDOW_CLASS	"CPCD"
#define _CPCD_WINDOW_CONSOLE "CON", "w", stdout
#define _CPCD_WINDOW_DEFAULT "> Initialized window and console...\n\n"
#define _CPCD_WINDOW_WITITLE	""
#define _CPCD_WINDOW_EHWND	"window handle damaged"
#define _CPCD_WINDOW_EREG	"windows registration failed"

#include "CPCD.h"

extern void _CPCD_INIT();

static _CPCD_DATA_WNDC wc_window;
static _CPCD_DATA_HWND hw_handle;
static _CPCD_DATA_MSG Msg;
static _CPCD_INT ui_frame = 0;
static _CPCD_DATA_HDC hdc_frame;

long __stdcall WndProc(_CPCD_DATA_HWND hwnds, _CPCD_UINT UI_MSG, _CPCD_DATA_WPARAM wParam, _CPCD_DATA_LPARAM lParam) {


	#ifdef _CPCD_EVENTUSAGE 
	_CPCD_PROCESSEVENT(_CPCD_ELEMENT_F(), UI_MSG);
	#endif

	if (UI_MSG == WM_CLOSE)DestroyWindow(hwnds);
	else if (UI_MSG == WM_DESTROY)PostQuitMessage(0);
	else return DefWindowProc(hwnds, UI_MSG, wParam, lParam);
	return 0;
}
int __stdcall WinMain(_CPCD_DATA_HINTS hInstance, _CPCD_DATA_HINTS hPrevInstance, _CPCD_DATA_LPSTR lpCmdLine, int nCmdShow) {

	#ifdef _CPCD_SHOWCONSOL
	AllocConsole();
	freopen(_CPCD_WINDOW_CONSOLE);
	printf(_CPCD_WINDOW_DEFAULT);
	#endif

	wc_window.hbrBackground = (_CPCD_DATA_HBRUSH)(2);
	wc_window.cbSize = sizeof(_CPCD_DATA_WNDC);
	wc_window.lpfnWndProc = WndProc;
	wc_window.hInstance = hInstance;
	wc_window.lpszClassName = _CPCD_WINDOW_CLASS;
	wc_window.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);


	if (!RegisterClassEx(&wc_window))return 0;
	
	hw_handle = CreateWindowEx(
		WS_EX_CLIENTEDGE, 
		_CPCD_WINDOW_CLASS, 
		_CPCD_WINDOW_WINAME, 
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		_CPCD_WINDOW_WIDTH, 
		_CPCD_WINDOW_HEIGHT, 
		NULL, 
		NULL, 
		hInstance, 
		NULL
	);

	_CPCD_INIT();

	if (hw_handle == NULL)return 0;
	if (_CPCD_MAIN_F(hw_handle) != 0)return 0;

	ShowWindow(hw_handle, nCmdShow);
	UpdateWindow(hw_handle);
	hdc_frame = GetDC(hw_handle);


	_CDD_DATA_TIME t_pasttime = clock();

	while (GetMessage(&Msg, NULL, 0, 0) > 0) {

		#ifdef _CPCD_SINGLEFRAME
		if (ui_frame == 0)
		#endif 
		if (_CPCD_UPDATE_F(ui_frame, clock() - t_pasttime, Msg) != 0)return 0;

		t_pasttime = clock();
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
		ui_frame++;
	}
	_CPCD_END_F(Msg.wParam, ui_frame);
	ReleaseDC(NULL, hdc_frame);
	return Msg.wParam;
}

_CPCD_MAP _CPCD_CREATE(_CPCD_INT USI_WIDTH, _CPCD_INT USI_HEIGTH)
{
	_CPCD_MAP _CPP_CREATE;
	_CPP_CREATE.USI_HEIGHT = USI_HEIGTH;
	_CPP_CREATE.USI_WIDTH = USI_WIDTH;
	_CPP_CREATE.C_COLOR = (_CPCD_COLOR**)malloc(USI_HEIGTH * sizeof(_CPCD_COLOR*));
	for (_CPCD_INT usi_row = 0; usi_row < USI_HEIGTH; usi_row++)
		_CPP_CREATE.C_COLOR[usi_row] = (_CPCD_COLOR*)malloc(USI_WIDTH * sizeof(_CPCD_COLOR));
	return _CPP_CREATE;
}
_CPCD_SPRITE _CPCD_SPR_CREATE(_CPCD_INT USI_WIDTH, _CPCD_INT USI_HEIGHT, _CPCD_VECTOR* V_POSITION)
{
	_CPCD_SPRITE _CPCD_SPR_CREATE;
	_CPCD_SPR_CREATE.USI_HEIGHT = USI_HEIGHT;
	_CPCD_SPR_CREATE.USI_WIDTH = USI_WIDTH;
	_CPCD_SPR_CREATE.C_COLOR = (_CPCD_COLOR**)malloc(USI_HEIGHT * sizeof(_CPCD_COLOR*));
	for (_CPCD_INT usi_row = 0; usi_row < USI_HEIGHT; usi_row++)
		_CPCD_SPR_CREATE.C_COLOR[usi_row] = (_CPCD_COLOR*)malloc(USI_WIDTH * sizeof(_CPCD_COLOR));
	if (V_POSITION != _CPCD_NULL) {
		_CPCD_SPR_CREATE.V_POSITION = *V_POSITION;
	}
	return _CPCD_SPR_CREATE;
}

void _CPCD_CLEAR(_CPCD_MAP* CPCD_CANVAS, _CPCD_COLOR	C_COLOR)
{
	for (_CPCD_INT X = 0; X <CPCD_CANVAS->USI_HEIGHT; X++)
	{
		for (_CPCD_INT Y = 0; Y < CPCD_CANVAS->USI_WIDTH; Y++)
		{
			CPCD_CANVAS->C_COLOR[X][Y] = C_COLOR;
		}
	}
}
void _CPCD_DELETE(_CPCD_MAP* CPCD_CANVAS)
{
	for (_CPCD_INT usi_row = 0; usi_row < CPCD_CANVAS->USI_HEIGHT; usi_row++)
		free(CPCD_CANVAS->C_COLOR[usi_row]);
	free(CPCD_CANVAS->C_COLOR);
}
void _CPCD_DRAWLINE(_CPCD_MAP* 	CPCD_CANVAS, _CPCD_VECTOR	V_LEFT, _CPCD_VECTOR	V_RIGHT, _CPCD_COLOR C_COLOR)
{
	_CPCD_INT usi_deltax = V_LEFT.X - V_RIGHT.X;
	_CPCD_INT usi_deltay = V_LEFT.Y - V_RIGHT.Y;

	float f_render = 0,f_quality=0 ;

#ifdef _CPCD_USEDYNAMICQUALITY 
	if(usi_deltax == 0.f) f_quality = 1.f / (float)usi_deltay;
	else f_quality = 1.f / (float)usi_deltax;

	if (f_quality < 0.f)f_quality *= -1;
#else 
	f_quality = _CPCD_QUALITY;
#endif 

	for (f_render = 1; f_render > 0; f_render -= f_quality)
	{
		_CPCD_VECTOR v_deltavec = { usi_deltax*f_render + V_RIGHT.X,usi_deltay*f_render + V_RIGHT.Y };
		_CPCD_SETPIXEL(CPCD_CANVAS, v_deltavec, C_COLOR);
	}
}

void _CPCD_DRAWRECT(_CPCD_MAP* CPCD_CANVAS, _CPCD_VECTOR	V_LEFTUPPER, _CPCD_VECTOR	V_RIGHTBOT, _CPCD_COLOR		C_COLOR, _CPCD_DATA_DRAWMODE DM_MODE)
{
	if (DM_MODE == _CPCD_DM_FILLED)
	{
		for (_CPCD_INT i = 0; i < V_RIGHTBOT.X - V_LEFTUPPER.X; i++)
		{
			_CPCD_DRAWLINE(CPCD_CANVAS, (_CPCD_VECTOR) { V_LEFTUPPER.X + i, V_LEFTUPPER.Y }, (_CPCD_VECTOR) { V_LEFTUPPER.X + i, V_RIGHTBOT.Y }, C_COLOR);
		}
	}
	else if (DM_MODE == _CPCD_DM_OUTLINE)
	{
		_CPCD_DRAWLINE(CPCD_CANVAS, V_LEFTUPPER, (_CPCD_VECTOR) { V_LEFTUPPER.X, V_RIGHTBOT.Y }, C_COLOR);
		_CPCD_DRAWLINE(CPCD_CANVAS, V_LEFTUPPER, (_CPCD_VECTOR) { V_RIGHTBOT.X, V_LEFTUPPER.Y }, C_COLOR);
		_CPCD_DRAWLINE(CPCD_CANVAS, V_RIGHTBOT, (_CPCD_VECTOR) { V_LEFTUPPER.X, V_RIGHTBOT.Y }, C_COLOR);
		_CPCD_DRAWLINE(CPCD_CANVAS, V_RIGHTBOT, (_CPCD_VECTOR) { V_RIGHTBOT.X, V_LEFTUPPER.Y }, C_COLOR);
	}
}
void _CPCD_DRAWTRI(_CPCD_MAP* CPCD_CANVAS, _CPCD_VECTOR V_ONE, _CPCD_VECTOR V_TWO, _CPCD_VECTOR V_THREE, _CPCD_COLOR	C_COLOR, _CPCD_DATA_DRAWMODE DM_MODE)
{
	if (DM_MODE == _CPCD_DM_FILLED)
	{
		_CPCD_INT usi_deltax = V_ONE.X - V_TWO.X;
		_CPCD_INT usi_deltay = V_ONE.Y - V_TWO.Y;
		for (float f_render = 1; f_render > 0; f_render -= _CPCD_QUALITY)
		{
			_CPCD_VECTOR v_deltavec = { usi_deltax*f_render + V_TWO.X,usi_deltay*f_render + V_TWO.Y };
			_CPCD_DRAWLINE(CPCD_CANVAS, V_THREE, v_deltavec, C_COLOR);
		}
	}
	else if (DM_MODE == _CPCD_DM_OUTLINE)
	{
		_CPCD_DRAWLINE(CPCD_CANVAS, V_ONE, V_TWO, C_COLOR);
		_CPCD_DRAWLINE(CPCD_CANVAS, V_TWO, V_THREE, C_COLOR);
		_CPCD_DRAWLINE(CPCD_CANVAS, V_THREE, V_ONE, C_COLOR);
	}
}

_CPCD_COLOR	_CPCD_GETPIXEL(_CPCD_MAP* CPCD_CANVAS, _CPCD_VECTOR V_POINT) {
	return CPCD_CANVAS->C_COLOR[V_POINT.Y][V_POINT.X];
}
void _CPCD_SETPIXEL(_CPCD_MAP* CPCD_CANVAS, _CPCD_VECTOR V_POINT, _CPCD_COLOR	C_COLOR) {
	if (V_POINT.X >= 0 && V_POINT.Y >= 0 && V_POINT.X < CPCD_CANVAS->USI_WIDTH && V_POINT.Y < CPCD_CANVAS->USI_HEIGHT) {
		CPCD_CANVAS->C_COLOR[V_POINT.Y][V_POINT.X] = C_COLOR;
	}
}
void _CPCD_DRAWBUFFER(_CPCD_MAP* CPCD_CANVAS) {
	_CPCD_DATA_COLR *col_buffer = (_CPCD_DATA_COLR*)malloc(CPCD_CANVAS->USI_HEIGHT * CPCD_CANVAS->USI_WIDTH * sizeof(_CPCD_DATA_COLR));
	for (_CPCD_UINT x = 0, c = 0; x < CPCD_CANVAS->USI_HEIGHT; x++)
		for (_CPCD_UINT y = 0; y < CPCD_CANVAS->USI_WIDTH; y++) {
#ifdef _CPCD_FULLRGB
			col_buffer[c] = RGB(CPCD_CANVAS->C_COLOR[x][y].B, CPCD_CANVAS->C_COLOR[x][y].G, CPCD_CANVAS->C_COLOR[x][y].R);
#else
			col_buffer[c] = RGB(CPCD_CANVAS->C_COLOR[x][y], CPCD_CANVAS->C_COLOR[x][y], CPCD_CANVAS->C_COLOR[x][y]);
#endif	
			c++;
		}
	_CPCD_DATA_HBITM bmp_map = CreateBitmap(CPCD_CANVAS->USI_WIDTH, CPCD_CANVAS->USI_HEIGHT, 1, 8 * 4, (void*)col_buffer);
	_CPCD_DATA_HDC hdc_src = CreateCompatibleDC(hdc_frame);
	SelectObject(hdc_src, bmp_map);
	BitBlt(hdc_frame, 0, 0, CPCD_CANVAS->USI_WIDTH, CPCD_CANVAS->USI_HEIGHT, hdc_src, 0, 0, SRCCOPY);
	free(col_buffer);
	DeleteObject(bmp_map);
	DeleteDC(hdc_src);
}


void _CPCD_DRAWSPRITE(_CPCD_MAP * CPCD_CANVAS, _CPCD_SPRITE SPR_SPRITE, _CPCD_DATA_DRAWMODE DM_MODE, _CPCD_DATA_DRAWMODE DM_VALUE)
{
	_CPCD_COLOR	c_color_over, c_color_avr;
	for (_CPCD_UINT x = 0, c = 0; x < SPR_SPRITE.USI_HEIGHT; x++)
		for (_CPCD_UINT y = 0; y < SPR_SPRITE.USI_WIDTH; y++) {

			switch (DM_MODE)
			{
			case _CPCD_SBM_OVERRIDE:
				_CPCD_SETPIXEL(CPCD_CANVAS, (_CPCD_VECTOR) { y + SPR_SPRITE.V_POSITION.X, x + SPR_SPRITE.V_POSITION.Y }, SPR_SPRITE.C_COLOR[x][y]);
				break;
			case _CPCD_SBM_COMPINE:
				c_color_over = _CPCD_GETPIXEL(CPCD_CANVAS, (_CPCD_VECTOR) { y + SPR_SPRITE.V_POSITION.X, x + SPR_SPRITE.V_POSITION.Y });
#ifdef _CPCD_FULLRGB
				_CPCD_COLOR	c_color_avr_over;
				c_color_avr_over.R = (SPR_SPRITE.C_COLOR[x][y].R + c_color_over.R) / 2;
				c_color_avr_over.G = (SPR_SPRITE.C_COLOR[x][y].G + c_color_over.G) / 2;
				c_color_avr_over.B = (SPR_SPRITE.C_COLOR[x][y].B + c_color_over.B) / 2;
				_CPCD_SETPIXEL(CPCD_CANVAS, (_CPCD_VECTOR) { y + SPR_SPRITE.V_POSITION.X, x + SPR_SPRITE.V_POSITION.Y }, c_color_avr_over);
#else
				if (c_color_over == 0)c_color_over = 1;
				_CPCD_SETPIXEL(CPCD_CANVAS, (_CPCD_VECTOR) { y + SPR_SPRITE.V_POSITION.X, x + SPR_SPRITE.V_POSITION.Y }, (SPR_SPRITE.C_COLOR[x][y] + c_color_over) / 2);
#endif
				break;
			case _CPCD_SBM_BLEND:
				c_color_over = _CPCD_GETPIXEL(CPCD_CANVAS, (_CPCD_VECTOR) { y + SPR_SPRITE.V_POSITION.X, x + SPR_SPRITE.V_POSITION.Y });
#ifdef _CPCD_FULLRGB
				c_color_avr.R = c_color_over.R;
				c_color_avr.G = c_color_over.G;
				c_color_avr.B = c_color_over.B;
				switch (DM_VALUE)
				{
				case _CPCD_C_RED:
					c_color_avr.R = SPR_SPRITE.C_COLOR[x][y].R;
					break;
				case _CPCD_C_GREEN:
					c_color_avr.G = SPR_SPRITE.C_COLOR[x][y].G;
					break;
				case _CPCD_C_BLUE:
					c_color_avr.B = SPR_SPRITE.C_COLOR[x][y].B;
					break;
				case _CPCD_C_REDGREEN:
					c_color_avr.R = SPR_SPRITE.C_COLOR[x][y].R;
					c_color_avr.G = SPR_SPRITE.C_COLOR[x][y].G;
					break;
				case _CPCD_C_REDBLUE:
					c_color_avr.R = SPR_SPRITE.C_COLOR[x][y].R;
					c_color_avr.B = SPR_SPRITE.C_COLOR[x][y].B;
					break;
				case _CPCD_C_BLUEGRN:
					c_color_avr.B = SPR_SPRITE.C_COLOR[x][y].B;
					c_color_avr.G = SPR_SPRITE.C_COLOR[x][y].G;
					break;
				}
				_CPCD_SETPIXEL(CPCD_CANVAS, (_CPCD_VECTOR) { y + SPR_SPRITE.V_POSITION.X, x + SPR_SPRITE.V_POSITION.Y }, c_color_avr);
#else 
				return;
#endif
				break;
			case _CPCD_SBM_ADD:
				c_color_over = _CPCD_GETPIXEL(CPCD_CANVAS, (_CPCD_VECTOR) { y + SPR_SPRITE.V_POSITION.X, x + SPR_SPRITE.V_POSITION.Y });
#ifdef _CPCD_FULLRGB
				_CPCD_COLOR	c_color_avr;
				c_color_avr.R = (SPR_SPRITE.C_COLOR[x][y].R + c_color_over.R) % 255;
				c_color_avr.G = (SPR_SPRITE.C_COLOR[x][y].G + c_color_over.G) % 255;
				c_color_avr.B = (SPR_SPRITE.C_COLOR[x][y].B + c_color_over.B) % 255;
				_CPCD_SETPIXEL(CPCD_CANVAS, (_CPCD_VECTOR) { y + SPR_SPRITE.V_POSITION.X, x + SPR_SPRITE.V_POSITION.Y }, c_color_avr);
#else
				if (c_color == 0)c_color = 1;
				_CPCD_SETPIXEL(CPCD_CANVAS, (_CPCD_VECTOR) { y + SPR_SPRITE.V_POSITION.X, x + SPR_SPRITE.V_POSITION.Y }, (SPR_SPRITE.C_COLOR[x][y] + c_color) % 255);
#endif
				break;
			}
		}
}

void _CPCD_CPUSHADER(_CPCD_MAP * CPCD_CANVAS, _CPCD_COLOR(*cpu_shader)(_CPCD_COLOR, _CPCD_VECTOR))
{
	for (_CPCD_UINT x = 0, c = 0; x < CPCD_CANVAS->USI_HEIGHT; x++)
		for (_CPCD_UINT y = 0; y < CPCD_CANVAS->USI_WIDTH; y++) {
			CPCD_CANVAS->C_COLOR[x][y] = cpu_shader(CPCD_CANVAS->C_COLOR[x][y], (_CPCD_VECTOR) { x, y });
		}
}
_CPCD_VECGRAPHIC _CPCD_VG_CREATE(_CPCD_UNIONTRANS* T_TRANFORM)
{
	_CPCD_VECGRAPHIC _CPCD_VG_CREATE;
	_CPCD_VG_CREATE.USI_SIZE = 0;
	if (T_TRANFORM != _CPCD_NULL)_CPCD_VG_CREATE.T_TRANSFORM = *T_TRANFORM;
	else {
		_CPCD_VG_CREATE.T_TRANSFORM.USI_ROTATION = 0;
		_CPCD_VG_CREATE.T_TRANSFORM.V_POSITION.X = 0;
		_CPCD_VG_CREATE.T_TRANSFORM.V_POSITION.Y = 0;
		_CPCD_VG_CREATE.T_TRANSFORM.V_SCALE.X = 1;
		_CPCD_VG_CREATE.T_TRANSFORM.V_SCALE.Y = 1;
	}
	return _CPCD_VG_CREATE;
}
void _CPCD_VG_PUSHLINE(_CPCD_VECGRAPHIC * VG_GRAPHIC, _CPCD_UNIONLINE L_LINE)
{
	_CPCD_UNIONLINE* L_LINES;
	if (VG_GRAPHIC->USI_SIZE != 0) {
		VG_GRAPHIC->USI_SIZE++;
		L_LINES = VG_GRAPHIC->L_LINES;
		VG_GRAPHIC->L_LINES = malloc(sizeof(_CPCD_UNIONLINE)*VG_GRAPHIC->USI_SIZE);
		for (_CPCD_INT i = 0; i < VG_GRAPHIC->USI_SIZE; i++) {
			if (i == VG_GRAPHIC->USI_SIZE - 1)
				VG_GRAPHIC->L_LINES[i] = L_LINE;
			else
				VG_GRAPHIC->L_LINES[i] = L_LINES[i];
		}
	}
	else {
		VG_GRAPHIC->USI_SIZE++;
		VG_GRAPHIC->L_LINES = (_CPCD_UNIONLINE*)malloc(sizeof(_CPCD_UNIONLINE)*VG_GRAPHIC->USI_SIZE);
		VG_GRAPHIC->L_LINES[0] = L_LINE;
	}
}
void _CPCD_VG_DRAW(_CPCD_MAP* CPCD_CANVAS, _CPCD_VECGRAPHIC VG_GRAPHIC)
{
	for (_CPCD_INT i = 0; i < VG_GRAPHIC.USI_SIZE; i++)
	{
		switch (VG_GRAPHIC.L_LINES[i].USI_MODE)
		{
		case _CPCD_DM_LINE:
			_CPCD_DRAWLINE(CPCD_CANVAS,
				_CPCD_VECADD(_CPCD_VECMULTI(VG_GRAPHIC.L_LINES[i].V_POS_ONE, VG_GRAPHIC.T_TRANSFORM.V_SCALE), VG_GRAPHIC.T_TRANSFORM.V_POSITION),
				_CPCD_VECADD(_CPCD_VECMULTI(VG_GRAPHIC.L_LINES[i].V_POS_TWO, VG_GRAPHIC.T_TRANSFORM.V_SCALE), VG_GRAPHIC.T_TRANSFORM.V_POSITION),
				VG_GRAPHIC.L_LINES[i].C_COLOR
			);
			break;
		case _CPCD_DM_TRI:
			_CPCD_DRAWTRI(CPCD_CANVAS,
				_CPCD_VECADD(_CPCD_VECMULTI(VG_GRAPHIC.L_LINES[i].V_POS_ONE, VG_GRAPHIC.T_TRANSFORM.V_SCALE), VG_GRAPHIC.T_TRANSFORM.V_POSITION),
				_CPCD_VECADD(_CPCD_VECMULTI(VG_GRAPHIC.L_LINES[i].V_POS_TWO, VG_GRAPHIC.T_TRANSFORM.V_SCALE), VG_GRAPHIC.T_TRANSFORM.V_POSITION),
				_CPCD_VECADD(_CPCD_VECMULTI(VG_GRAPHIC.L_LINES[i + 1].V_POS_TWO, VG_GRAPHIC.T_TRANSFORM.V_SCALE), VG_GRAPHIC.T_TRANSFORM.V_POSITION),
				VG_GRAPHIC.L_LINES[i].C_COLOR,
				VG_GRAPHIC.L_LINES[i].USI_DRAWMODE);
			i++;
			break;
		case _CPCD_DM_RECT:
			_CPCD_DRAWRECT(CPCD_CANVAS,
				_CPCD_VECADD(_CPCD_VECMULTI(VG_GRAPHIC.L_LINES[i].V_POS_ONE, VG_GRAPHIC.T_TRANSFORM.V_SCALE), VG_GRAPHIC.T_TRANSFORM.V_POSITION),
				_CPCD_VECADD(_CPCD_VECMULTI(VG_GRAPHIC.L_LINES[i].V_POS_TWO, VG_GRAPHIC.T_TRANSFORM.V_SCALE), VG_GRAPHIC.T_TRANSFORM.V_POSITION),
				VG_GRAPHIC.L_LINES[i].C_COLOR,
				VG_GRAPHIC.L_LINES[i].USI_DRAWMODE);
			break;
		}
	}
}
void _CPCD_VG_DRAWLINE(_CPCD_VECGRAPHIC * VG_GRAPHIC, _CPCD_VECTOR V_LEFT, _CPCD_VECTOR V_RIGH, _CPCD_COLOR C_COLOR) {
	_CPCD_VG_PUSHLINE(VG_GRAPHIC, (_CPCD_UNIONLINE) { V_LEFT, V_RIGH, C_COLOR, _CPCD_DM_LINE, 0 });
}
void _CPCD_VG_DRAWRECT(_CPCD_VECGRAPHIC * VG_GRAPHIC, _CPCD_VECTOR V_LEFTUPPER, _CPCD_VECTOR V_RIGHTBOT, _CPCD_COLOR C_COLOR, _CPCD_DATA_DRAWMODE DM_MODE) {
	_CPCD_VG_PUSHLINE(VG_GRAPHIC, (_CPCD_UNIONLINE) { V_LEFTUPPER, V_RIGHTBOT, C_COLOR, _CPCD_DM_RECT, DM_MODE });
}
void _CPCD_VG_DRAWTRI(_CPCD_VECGRAPHIC * VG_GRAPHIC, _CPCD_VECTOR V_ONE, _CPCD_VECTOR V_TWO, _CPCD_VECTOR V_THREE, _CPCD_COLOR C_COLOR, _CPCD_DATA_DRAWMODE DM_MODE) {
	_CPCD_VG_PUSHLINE(VG_GRAPHIC, (_CPCD_UNIONLINE) { V_ONE, V_TWO, C_COLOR, _CPCD_DM_TRI, DM_MODE });
	_CPCD_VG_PUSHLINE(VG_GRAPHIC, (_CPCD_UNIONLINE) { V_TWO, V_THREE, C_COLOR, _CPCD_DM_TRI, DM_MODE });
}
_CPCD_COLOR _CPCD_COLORSUB(_CPCD_COLOR C_ONE, _CPCD_COLOR C_TWO) {
#ifdef _CPCD_FULLRGB
	return (_CPCD_COLOR) {
		(C_ONE.R - C_TWO.R) % 255,
			(C_ONE.G - C_TWO.G) % 255,
			(C_ONE.B - C_TWO.B) % 255
	};
#else			 
	return C_ONE - C_TWO;
#endif
}
_CPCD_COLOR _CPCD_COLORDIV(_CPCD_COLOR C_ONE, _CPCD_COLOR C_TWO) {
#ifdef _CPCD_FULLRGB
	return (_CPCD_COLOR) {
		(C_ONE.R / C_TWO.R) % 255,
			(C_ONE.G / C_TWO.G) % 255,
			(C_ONE.B / C_TWO.B) % 255
	};
#else			 
	return C_ONE / C_TWO;
#endif
}
_CPCD_COLOR _CPCD_COLORMULTI(_CPCD_COLOR C_ONE, _CPCD_COLOR C_TWO) {
#ifdef _CPCD_FULLRGB
	return (_CPCD_COLOR) {
		(C_ONE.R * C_TWO.R) % 255,
			(C_ONE.G * C_TWO.G) % 255,
			(C_ONE.B * C_TWO.B) % 255
	};
#else
	return C_ONE * C_TWO;
#endif
}
_CPCD_COLOR _CPCD_COLORADD(_CPCD_COLOR C_ONE, _CPCD_COLOR C_TWO) {
#ifdef _CPCD_FULLRGB
	return (_CPCD_COLOR) {
		(C_ONE.R + C_TWO.R) % 255,
			(C_ONE.G + C_TWO.G) % 255,
			(C_ONE.B + C_TWO.B) % 255
	};
#else
	return C_ONE + C_TWO;
#endif
}
int _CPCD_COLORCMP(_CPCD_COLOR C_ONE, _CPCD_COLOR C_TWO)
{
#ifdef _CPCD_FULLRGB
	if (C_ONE.R == C_TWO.R &&
		C_ONE.G == C_TWO.G &&
		C_ONE.B == C_TWO.B)return 1;
#else
	if (C_ONE == C_TWO)return 1;
#endif
	return 0;
}
int _CPCD_VECCMP(_CPCD_VECTOR V_ONE, _CPCD_VECTOR V_TWO) {
	if (V_ONE.X == V_TWO.X &&V_ONE.Y == V_TWO.Y)return 1;
	return 0;
}
_CPCD_VECTOR _CPCD_VECADD(_CPCD_VECTOR V_ONE, _CPCD_VECTOR V_TWO) {
	return (_CPCD_VECTOR) { V_ONE.X + V_TWO.X, V_ONE.Y + V_TWO.Y };
}
_CPCD_VECTOR _CPCD_VECSUB(_CPCD_VECTOR V_ONE, _CPCD_VECTOR V_TWO) {
	return (_CPCD_VECTOR) { V_ONE.X - V_TWO.X, V_ONE.Y - V_TWO.Y };
}
_CPCD_VECTOR _CPCD_VECMULTI(_CPCD_VECTOR V_ONE, _CPCD_VECTOR V_TWO) {
	return (_CPCD_VECTOR) { V_ONE.X * V_TWO.X, V_ONE.Y * V_TWO.Y };
}
_CPCD_VECTOR _CPCD_VECDIV(_CPCD_VECTOR V_ONE, _CPCD_VECTOR V_TWO) {
	return (_CPCD_VECTOR) { V_ONE.X / V_TWO.X, V_ONE.Y / V_TWO.Y };
}
_CPCD_VECTOR _CPCD_CURSORPOS(){
	POINT p_pos;
	GetCursorPos(&p_pos);
	ScreenToClient(hw_handle, &p_pos);
	return (_CPCD_VECTOR) { p_pos.x, p_pos.y};
}
void _CPCD_DRAWSTRING(_CPCD_MAP* CPCD_CANVAS, _CPCD_STRING C_TEXT, _CPCD_INT	USI_LENGTH, _CPCD_VECTOR V_POINT, _CPCD_INT I_SCALE, _CPCD_COLOR	C_COLOR){
	_CPCD_VECTOR v_cursor = V_POINT;
	_CPCD_INT i_cursor = 0;
	for (_CPCD_INT i = 0; i < USI_LENGTH; i++){
		i_cursor = (i*((_CPCD_CHARWIDTH*I_SCALE) + (_CPCD_SPACING* I_SCALE)));
		for (_CPCD_INT i_heigth = 0; i_heigth < _CPCD_CHARHEIGTH*I_SCALE; i_heigth++) {
			for (_CPCD_INT i_width = 0; i_width < _CPCD_CHARWIDTH*I_SCALE; i_width++){
				v_cursor.X = i_cursor + V_POINT.X+ i_width;
				v_cursor.Y = V_POINT.Y+ i_heigth;
				if (C_CHARLIST[C_TEXT[i]].C_VALUE[((i_heigth/ I_SCALE )*_CPCD_CHARWIDTH )+ (i_width/ I_SCALE)] == _CPCD_CHARTRUE) {
					_CPCD_SETPIXEL(CPCD_CANVAS, v_cursor, C_COLOR);
				}
				else {
				}
			}
		}
	}
}
void _CPCD_PROCESSEVENT(_CPCD_ELEMENT_LIST * EL_LIST, _CPCD_UINT UI_MSG) {
	if (EL_LIST == _CPCD_NULL || (*EL_LIST).E_ELEMENTS == 0)return;
	_CPCD_EVENT e_event;
	_CPCD_INT i_refresh = 0;
	for (_CPCD_INT i_index = 0; i_index < (*EL_LIST).USI_ELEMENTS; i_index++) {
		if ((*EL_LIST).E_ELEMENTS[i_index].USI_ELEMENT == _CPCD_EI_EMPTY) continue;
		
		e_event.USI_ELEMENT = (*EL_LIST).E_ELEMENTS[i_index].USI_ELEMENT;
		e_event.USI_INDEX = i_index;
		e_event.UI_MSG = UI_MSG;

		_CPCD_INT i_width = (*EL_LIST).E_ELEMENTS[i_index].USI_WIDTH;
		_CPCD_INT i_height = (*EL_LIST).E_ELEMENTS[i_index].USI_HEIGHT;

		_CPCD_ELEMENT_FIELD e_field = *(_CPCD_ELEMENT_FIELD*)(*EL_LIST).E_ELEMENTS[i_index].V_ELEMENT;
		
		switch (UI_MSG)
		{
		case WM_LBUTTONDOWN:				
			if (_CPCD_INREAGION(_CPCD_CURSORPOS(), i_width, i_height, (*EL_LIST).E_ELEMENTS[i_index].V_POSITION)) {

				if ((*EL_LIST).E_ELEMENTS[i_index].USI_ELEMENT == _CPCD_EI_FIELD) {
					e_field.F_EVENT_CLICKED(e_event);
					e_field.F_EVENT_STYLE_CLICKED(&((*EL_LIST).E_ELEMENTS[i_index].S_DISPLAY));			
				}
				i_refresh = 1;
			}
			break;
		case WM_MOUSEMOVE: 
		case WM_LBUTTONUP:
			if (_CPCD_INREAGION(_CPCD_CURSORPOS(), i_width, i_height, (*EL_LIST).E_ELEMENTS[i_index].V_POSITION)) {

				if ((*EL_LIST).E_ELEMENTS[i_index].USI_ELEMENT == _CPCD_EI_FIELD) {
					e_field.F_EVENT_HOVER(e_event);
					e_field.F_EVENT_STYLE_HOVER(&((*EL_LIST).E_ELEMENTS[i_index].S_DISPLAY));
				}
				i_refresh = 1;
			}
			else {
				if (UI_MSG != (*EL_LIST).E_ELEMENTS[i_index].USI_STATE && UI_MSG == WM_MOUSEMOVE) {

					if ((*EL_LIST).E_ELEMENTS[i_index].USI_ELEMENT == _CPCD_EI_FIELD) {
						e_field.F_EVENT_STYLE_IDLE(&((*EL_LIST).E_ELEMENTS[i_index].S_DISPLAY));
					}
					i_refresh = 1;
				}	
			}		
			break;
		}
		(*EL_LIST).E_ELEMENTS[i_index].USI_STATE = UI_MSG;
	}

	if (i_refresh) {
		(*EL_LIST).F_TRIGGER();
	}
	
}
void _CPCD_PUSHELEMENT(_CPCD_ELEMENT_LIST * EL_LIST, _CPCD_ELEMENT E_ELEMENT ) {

	if ((*EL_LIST).USI_ELEMENTS <= 0) {
		(*EL_LIST).USI_ELEMENTS = 1;
		(*EL_LIST).E_ELEMENTS = malloc(sizeof(_CPCD_INT));
		(*EL_LIST).E_ELEMENTS[0] = E_ELEMENT;
	}
	else {
		(*EL_LIST).USI_ELEMENTS += 1;
	}
	_CPCD_ELEMENT_LIST el_item;
	el_item.USI_ELEMENTS = (*EL_LIST).USI_ELEMENTS;
	el_item.E_ELEMENTS = malloc(sizeof(_CPCD_ELEMENT)*(*EL_LIST).USI_ELEMENTS);

	for (_CPCD_INT i_index = 0; i_index < (*EL_LIST).USI_ELEMENTS-1; i_index++){
		el_item.E_ELEMENTS[i_index] = (*EL_LIST).E_ELEMENTS[i_index];
	}
	el_item.E_ELEMENTS[(*EL_LIST).USI_ELEMENTS - 1] = E_ELEMENT;

	(*EL_LIST).E_ELEMENTS = malloc(sizeof(_CPCD_ELEMENT)*(*EL_LIST).USI_ELEMENTS);;

	for (_CPCD_INT i_index = 0; i_index < (*EL_LIST).USI_ELEMENTS; i_index++){
		(*EL_LIST).E_ELEMENTS[i_index] = el_item.E_ELEMENTS[i_index];
	}

	free(el_item.E_ELEMENTS);
}

void _CPCD_ELEMENTRENDER(_CPCD_MAP* CPCD_CANVAS, _CPCD_ELEMENTSTYLE ES_STYLE, _CPCD_ELEMENT E_ELEMENT) {
	_CPCD_INT i_width = E_ELEMENT.S_DISPLAY.USI_WIDTH+ _CPCD_PIXELQUALITY*2, i_height = E_ELEMENT.S_DISPLAY.USI_HEIGHT+ _CPCD_PIXELQUALITY*2;

	for (_CPCD_INT i_index = 0; i_index < _CPCD_PIXELQUALITY; i_index++){
		_CPCD_DRAWRECT(CPCD_CANVAS,
			_CPCD_VECADD(E_ELEMENT.V_POSITION, (_CPCD_VECTOR) { i_index, i_index}),
			_CPCD_VECADD(E_ELEMENT.V_POSITION, (_CPCD_VECTOR) { i_width-i_index, i_height-i_index}),
			ES_STYLE.C_DIVIDER[i_index],_CPCD_DM_OUTLINE);
	}
	
	_CPCD_DRAWRECT(CPCD_CANVAS, 
		_CPCD_VECADD(E_ELEMENT.V_POSITION, (_CPCD_VECTOR) { _CPCD_PIXELQUALITY, _CPCD_PIXELQUALITY}),
		_CPCD_VECADD(E_ELEMENT.V_POSITION, (_CPCD_VECTOR) { i_width - _CPCD_PIXELQUALITY+1, i_height - _CPCD_PIXELQUALITY+1}),
		ES_STYLE.C_MIDDLE, _CPCD_DM_FILLED);

	for (_CPCD_INT i_x = 0; i_x < E_ELEMENT.S_DISPLAY.USI_WIDTH; i_x++)
	{
		for (_CPCD_INT i_y = 0; i_y < E_ELEMENT.S_DISPLAY.USI_HEIGHT; i_y++)
		{
			_CPCD_SETPIXEL(CPCD_CANVAS, 
				_CPCD_VECADD(E_ELEMENT.V_POSITION, (_CPCD_VECTOR) { i_x+ _CPCD_PIXELQUALITY, i_y+ _CPCD_PIXELQUALITY}), 
				_CPCD_GETPIXEL(&(E_ELEMENT.S_DISPLAY),(_CPCD_VECTOR) { i_x , i_y }));
		}
	}
}

_CPCD_ELEMENT _CPCD_FILLELEMENT(_CPCD_INT USI_ELEMENT, _CPCD_VECTOR V_POSITION, void * V_ELEMENT){
	_CPCD_ELEMENT e_element;
	e_element.USI_ELEMENT = USI_ELEMENT;
	e_element.V_POSITION = V_POSITION;
	e_element.V_ELEMENT = V_ELEMENT;
	return e_element;
}

int _CPCD_INREAGION(_CPCD_VECTOR V_POS, _CPCD_INT USI_WIDTH, _CPCD_INT USI_HEIGHT, _CPCD_VECTOR V_ANCHOR){
	if (V_ANCHOR.X <= V_POS.X && (V_ANCHOR.X + USI_WIDTH + _CPCD_PIXELQUALITY) >= V_POS.X
		&& V_ANCHOR.Y <= V_POS.Y && (V_ANCHOR.Y + USI_HEIGHT + _CPCD_PIXELQUALITY) >= V_POS.Y)return 1;
	return 0;
}