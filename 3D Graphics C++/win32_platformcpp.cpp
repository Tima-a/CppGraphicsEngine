#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <limits.h>
#include <windows.h>
#include "utility.cpp"
#include <cmath>
#define MAX_WINDOW_QUANTITY 100
#define delta_time_equalization 1000000
int win_count = 0;
static bool all_win_active = true;
const float window_pixel_coefficent = 2.5f; // Proportion of window's size in microsoft pixels with my pixels.
const float fixed_width = 0.0f;
const float fixed_height = 40.0f;
static struct gTime
{
	float delta_time = 0.01f;
};
gTime gtime;
struct Screen
{
	bool update_screen_bool[MAX_WINDOW_QUANTITY] = { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true };;
	uint32 scr_refresh_color[MAX_WINDOW_QUANTITY] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
};
Screen screen;
struct _Win
{
	void* win_memory[MAX_WINDOW_QUANTITY];
	int win_width[MAX_WINDOW_QUANTITY];
	int win_height[MAX_WINDOW_QUANTITY];
	int win_width_px[MAX_WINDOW_QUANTITY];
	int win_height_px[MAX_WINDOW_QUANTITY];
	int winx[MAX_WINDOW_QUANTITY];
	int winy[MAX_WINDOW_QUANTITY];
	BITMAPINFO bitmap_info[MAX_WINDOW_QUANTITY];
	LPCSTR window_name[MAX_WINDOW_QUANTITY];
	HWND windows[MAX_WINDOW_QUANTITY];
	HDC hdcs[MAX_WINDOW_QUANTITY];
	HINSTANCE hInstance_;
	bool win_can_be_closed[MAX_WINDOW_QUANTITY] = { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true };
};
_Win _win;
static bool visible_windows[MAX_WINDOW_QUANTITY] = { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true };
int size[MAX_WINDOW_QUANTITY];
static class WINDOW
{
public:
	int wnd_param = 0;
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;
	LPCSTR name = "";
	WINDOW()
	{
		//win_count++;
	}
	WINDOW(LPCSTR win_name, int x_, int y_, int width_, int height_)
	{
		x = x_;
		y = y_;
		width = width_;
		height = height_;
		name = win_name;
		_win.win_width[win_count] = width_;
		_win.win_height[win_count] = height_;
		_win.win_width_px[win_count] = width_ - 15;
		_win.win_height_px[win_count] = height_ - 41;
		_win.window_name[win_count] = win_name;
		_win.winx[win_count] = x_;
		_win.winy[win_count] = y_;
		wnd_param = win_count;
		win_count++;
		InitializeWinBitmapMemory();
	}
	void InitializeWinBitmapMemory()
	{
		int size[MAX_WINDOW_QUANTITY];
		for (int i = 0; i < win_count; i++)
		{
			size[i] = _win.win_width[i] * _win.win_height[i] * sizeof(unsigned int);

			if (_win.win_memory[i])
			{
				VirtualFree(_win.win_memory[i], 0, MEM_RELEASE);
			}
			_win.win_memory[i] = VirtualAlloc(0, size[i], MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			_win.bitmap_info[i].bmiHeader.biSize = sizeof(_win.bitmap_info[i].bmiHeader);
			_win.bitmap_info[i].bmiHeader.biWidth = _win.win_width[i];
			_win.bitmap_info[i].bmiHeader.biHeight = _win.win_height[i];
			_win.bitmap_info[i].bmiHeader.biPlanes = 1;
			_win.bitmap_info[i].bmiHeader.biBitCount = 32;
			_win.bitmap_info[i].bmiHeader.biCompression = BI_RGB;
		}
	}
	HWND GetWindowHandleByIndex()
	{
		return _win.windows[wnd_param];
	}
	void HideWindow()
	{
		ShowWindow(GetWindowHandleByIndex(), SW_HIDE);
	}
	void ActivateWindow()
	{
		ShowWindow(GetWindowHandleByIndex(), SW_SHOW);
	}
	void MinimizeWindow()
	{
		ShowWindow(GetWindowHandleByIndex(), SW_FORCEMINIMIZE);
	}
	void MaximizeWindow()
	{
		ShowWindow(GetWindowHandleByIndex(), SW_SHOWMAXIMIZED);
	}
	void RestoreWindowState()
	{
		ShowWindow(GetWindowHandleByIndex(), SW_SHOWDEFAULT);
	}
	//To change window's size use SetWindowPos in simulate_game() loop
	void change_win_x(int x_)
	{
		_win.winx[wnd_param] = x_;
		SetWindowPos(GetWindowHandleByIndex(), HWND_NOTOPMOST, x_, _win.winy[wnd_param], _win.win_width[wnd_param], _win.win_height[wnd_param], SWP_NOSIZE | SWP_NOZORDER);
		x = x_;
	}
	void change_win_y(int y_)
	{
		_win.winy[wnd_param] = y_;
		SetWindowPos(GetWindowHandleByIndex(), HWND_NOTOPMOST, _win.winx[wnd_param], y_, _win.win_width[wnd_param], _win.win_height[wnd_param], SWP_NOSIZE | SWP_NOZORDER);
		y = y_;
	}
	void change_win_name(LPCSTR name_)
	{
		_win.window_name[wnd_param] = name_;
		//SendMessage(_win.windows[], WM_SETTEXT, 0, name_);
		SetWindowTextA(_win.windows[wnd_param], name_);
		name = name_;
	}
	void WindowCanBeClosed(bool close_state)
	{
		_win.win_can_be_closed[wnd_param] = close_state;
	}
	void WindowDestroy()
	{
		DestroyWindow(_win.windows[wnd_param]);
	}
};

//#include <vector>
//template <class T>
//std::vector<T> name;
#include "rendering.cpp"
#include "physics.cpp"
#include <time.h>
#include "game.cpp"
//#include <atlstr.h>
//int FindWindowIndexByName(LPSTR win_name_)
//{
//	LPCSTR wnd_name = (LPCSTR)win_name_;
//	CStringA wnd_name_CString(wnd_name);
//	int i = 0;
//	for (i = 0; i < MAX_WINDOW_QUANTITY; i++)
//	{
//		CStringA win_class_name_CString(_win.window_name[i]);
//		if (wnd_name_CString == win_class_name_CString)
//		{
//			break;
//		}
//	}
//	return i;
//}
int FindWindowIndexByHWND(HWND win_hwnd)
{
	int i = 0;
	for (i = 0; i < win_count; i++)
	{
		if (_win.windows[i] == win_hwnd)
		{
			return i;
		}
	}
	return i;
}
LRESULT CALLBACK Window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//This is function which sends a message when something important is happened(input, minimize, maximize, size changed
	LRESULT result = 0;
	switch (uMsg)
	{
	case WM_CLOSE:
	case WM_DESTROY:
	{
		int q = FindWindowIndexByHWND(hwnd);
		if (_win.win_can_be_closed[q])
		{
			DestroyWindow(hwnd);
		}
		all_win_active = false;
		//LPSTR win_name = new CHAR[MAX_PATH]; //this is done because i get an error that win_name is uninitialized
		//GetWindowText(hwnd, win_name, 100);
		//int index = FindWindowIndexByName(win_name);
		//visible_windows[index] = false;
	}
	break;
	case WM_SIZE:
	{
		int size[MAX_WINDOW_QUANTITY];
		for (int i = 0; i < win_count; i++)
		{
			size[i] = _win.win_width[i] * _win.win_height[i] * sizeof(unsigned int);

			if (_win.win_memory[i])
			{
				VirtualFree(_win.win_memory[i], 0, MEM_RELEASE);
			}
			_win.win_memory[i] = VirtualAlloc(0, size[i], MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			_win.bitmap_info[i].bmiHeader.biSize = sizeof(_win.bitmap_info[i].bmiHeader);
			_win.bitmap_info[i].bmiHeader.biWidth = _win.win_width[i];
			_win.bitmap_info[i].bmiHeader.biHeight = _win.win_height[i];
			_win.bitmap_info[i].bmiHeader.biPlanes = 1;
			_win.bitmap_info[i].bmiHeader.biBitCount = 32;
			_win.bitmap_info[i].bmiHeader.biCompression = BI_RGB;
		}
	}
	default:
	{
		result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
	return result;
}
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//Creating WINDOW(or WINDOW class)
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW || CS_VREDRAW; //CS_HREDRAW - Horizontal; CS_VREDRAW - Vertical 
	window_class.lpszClassName = "Game WINDOW Class";
	window_class.lpfnWndProc = Window_callback;

	//Register Class
	RegisterClass(&window_class);
	for (int i = 0; i < win_count; i++)
	{
		_win.windows[i] = CreateWindow(window_class.lpszClassName, _win.window_name[i], WS_OVERLAPPEDWINDOW | WS_VISIBLE, _win.winx[i], _win.winy[i], _win.win_width[i], _win.win_height[i], 0, 0, hInstance, 0);
		_win.hdcs[i] = GetDC(_win.windows[i]);
	}
	float delta_time = 0.016666f;
	LARGE_INTEGER frame_begin_time;
	QueryPerformanceCounter(&frame_begin_time);

	float perfomance_frequency;
	{
		LARGE_INTEGER perfomance;
		QueryPerformanceCounter(&perfomance);
		perfomance_frequency = (float)perfomance.QuadPart;
	}

	while (all_win_active)
	{
		//Input
		MSG messages[MAX_WINDOW_QUANTITY];
		for (int j = 0; j < win_count; j++)
		{
			while (PeekMessage(&messages[j], _win.windows[j], 0, 0, PM_REMOVE))
			{
				TranslateMessage(&messages[j]);
				DispatchMessage(&messages[j]);
			}
		}
		//Simulate
		simulategame();
		//Creating WINDOW
		for (int z = 0; z < win_count; z++)
		{
			StretchDIBits(_win.hdcs[z], 0, 0, _win.win_width[z], _win.win_height[z], 0, 0, _win.win_width[z], _win.win_height[z], _win.win_memory[z], &_win.bitmap_info[z], DIB_RGB_COLORS, SRCCOPY);
		}
		LARGE_INTEGER frame_end_time;
		QueryPerformanceCounter(&frame_end_time);
		gtime.delta_time = ((float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / perfomance_frequency) * delta_time_equalization; //delta time in 1s
		frame_begin_time = frame_end_time;
	}
}
