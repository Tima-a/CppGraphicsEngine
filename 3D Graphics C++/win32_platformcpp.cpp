#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <windows.h>
#include "utility.cpp"
#include <cmath>
#define delta_time_equalization 1000000
typedef unsigned int uint32;
static bool running = true;
struct Screen
{
	bool update_screen = false;
	unsigned int scr_refresh_color = 0;
};
Screen screen;
struct Render
{
	void* memory;
	int width, height;
	BITMAPINFO bitmap_info;
};
Render render;
LPCTSTR window_name = "C++ Graphics";
HWND hWnd = FindWindow("Game Window Class", window_name);
static struct gTime
{
	float delta_time;
};
gTime gtime;
//#include <vector>
//template <class T>
//std::vector<T> name;
#include "renderering.cpp"
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
		RECT rect;
		GetClientRect(hwnd, &rect);
		render.width = rect.right - rect.left;
		render.height = rect.bottom - rect.top;
		int size = render.width * render.height * sizeof(unsigned int);
		if (render.memory)
		{
			VirtualFree(render.memory, 0, MEM_RELEASE);
		}
		render.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		render.bitmap_info.bmiHeader.biSize = sizeof(render.bitmap_info.bmiHeader);
		render.bitmap_info.bmiHeader.biWidth = render.width;
		render.bitmap_info.bmiHeader.biHeight = render.height;
		render.bitmap_info.bmiHeader.biPlanes = 1;
		render.bitmap_info.bmiHeader.biBitCount = 32;
		render.bitmap_info.bmiHeader.biCompression = BI_RGB;
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
	HWND window = CreateWindow(window_class.lpszClassName, window_name, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1000, 600, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);

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
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		//Simulate
		refresh_screen(screen.scr_refresh_color);
		simulategame();
		//Creating Window
		StretchDIBits(hdc, 0, 0, render.width, render.height, 0, 0, render.width, render.height, render.memory, &render.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

		LARGE_INTEGER frame_end_time;
		QueryPerformanceCounter(&frame_end_time);
		gtime.delta_time = ((float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / perfomance_frequency) * delta_time_equalization; //delta time in 1s
		frame_begin_time = frame_end_time;
	}
}
