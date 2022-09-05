#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//Multiple Windows
#include <windows.h>
#include "utility.cpp"
#include <cmath>
#define MAX_WINDOW_QUANTITY 100
#define delta_time_equalization 1000000
static bool running = true;
int win_count = 0;
//HWND hWnd = FindWindow("Game Window Class", window_name);

static struct gTime
{
	float delta_time;
};
gTime gtime;
struct _Win
{
	void* win_memory[MAX_WINDOW_QUANTITY];
	int win_width[MAX_WINDOW_QUANTITY];
	int win_height[MAX_WINDOW_QUANTITY];
	BITMAPINFO bitmap_info[MAX_WINDOW_QUANTITY];
	LPCSTR window_name[MAX_WINDOW_QUANTITY];
	HWND windows[MAX_WINDOW_QUANTITY];
	HDC hdcs[MAX_WINDOW_QUANTITY];
	HINSTANCE hInstance_;
};
_Win _win;
int size[MAX_WINDOW_QUANTITY];
static class Window
{
public:
	float x_pixel_size;
	float y_pixel_size;
	float render_scale_height;
	float screen_half_width;
	float screen_half_height;
	Window()
	{
		//win_count++;
	}
	Window(int width, int height, LPCSTR win_name)
	{
		_win.win_width[win_count] = width;
		_win.win_height[win_count] = height;
		_win.window_name[win_count] = win_name;
		win_count++;
		InitializeWinBitmapMemory();
		InitializeWindow();
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
	void InitializeWindow()
	{
		/*LPCSTR lpszClassName = "Game Wnidow Class";
		for (int i = 0; i < win_count; i++)
		{
			_win.windows[i] = CreateWindow(lpszClassName, _win.window_name[i], WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0,0, _win.win_width[i], _win.win_height[i], 0, 0, _win.hInstance_[i], 0);
			_win.hdcs[i] = GetDC(_win.windows[i]);
		}*/
	}
};

//#include <vector>
//template <class T>
//std::vector<T> name;
#include "rendering.cpp"
#include <time.h>
#include "game.cpp"

LRESULT CALLBACK Window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//This is function which sends a message when something important is happened(input, minimize, maximize, size changed
	LRESULT result = 0;
	switch (uMsg)
	{
	case WM_CLOSE:
	case WM_DESTROY:
	{
		running = false;
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
	//Creating Window(or Window class)
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW || CS_VREDRAW; //CS_HREDRAW - Horizontal; CS_VREDRAW - Vertical 
	window_class.lpszClassName = "Game Window Class";
	window_class.lpfnWndProc = Window_callback;

	//Register Class
	RegisterClass(&window_class);
	for (int i = 0; i < win_count; i++)
	{
		_win.windows[i] = CreateWindow(window_class.lpszClassName, _win.window_name[i], WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, _win.win_width[i], _win.win_height[i], 0, 0, hInstance, 0);
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

	while (running)
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
		//Creating Window
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
