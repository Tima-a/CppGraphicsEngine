typedef unsigned int uint32;
static float render_scale = 0.001f;
static float gravity = -9.81f;
static float Gravitational_constant = 0.000000000016f;
float pi = 3.14f;
float d_pi = 6.28f;
const int MAX_STORAGE_SIZE = 5000;
//Pixel size presets
const float SUB_PIXEL_SIZE = 0.1f; // 1 coordinate unit equals one ten pixels
const float PIXEL_SIZE_DEF = 1.0f; // 1 coordinate unit equals one pixel
const float PIXEL_SIZE_MAX = 1.0f;
const float PIXEL_SIZE_MIN = 0.1f;
const float PIXEL_SIZE_TRIANGLE = 1.0f;
const float VECTOR_HQ = 0.1f; // 1 coordinate unit equals ten pixel
const float VECTOR_MQ = 0.5f; // 1 coordinate unit equals five pixel
const float VECTOR_LQ = 1.0f; // 1 coordinate unit equals one pixel
const float PIXEL_SIZE_STLN = 1.0f;
#define MAX_FUNCTION_SIZE 100
#define MAX_PIXEL_QUANTITY = 1000*1000
//Pixel size presets
struct Colors
{
	uint32 white = 0xffffff;
	uint32 green = 0x00ff00;
	uint32 red = 0xff0000;
	uint32 blue = 0x0000ff;
	uint32 yellow = 0xffff00;
	uint32 black = 0;
};
Colors colors;
static enum pivot
{
	center,
	startpos,
	endpos,
	around_point
};
static enum rotation_direction_
{
	clockwise,
	counter_clockwise
};
static enum FunctionType
{
	no_function,
	straight,
	reverse_proportion,
	proportion,
	linear,
	hyperbola,
	power_of_x_function,
	sqrt_x_function,
};
class Vector2f
{
public:
	float x;
	float y;
	Vector2f(float x_, float y_)
	{
		x = x_;
		y = y_;
	}
	Vector2f()
	{

	}
	~Vector2f()
	{


	}
	Vector2f(Vector2f& vx)
	{
		x = vx.x;
		y = vx.y;
	}
};
class Vector2i
{
public:
	int x;
	int y;
	Vector2i(int x_, int y_)
	{
		x = x_;
		y = y_;
	}
	Vector2i()
	{

	}
	~Vector2i()
	{


	}
	Vector2i(Vector2i& vx)
	{
		x = vx.x;
		y = vx.y;
	}
};
class Color32
{
public:
	float red = 255.0f;
	float green = 255.0f;
	float blue = 255.0f;
	float alpha = 255.0f;
	Color32(float red_, float green_, float blue_, float alpha_)
	{
		red = red_;
		green = green_;
		blue = blue_;
		alpha = alpha_;
	}
	Color32()
	{

	}
	~Color32()
	{

	}
	Color32(Color32& col_)
	{
		red = col_.red;
		green = col_.green;
		blue = col_.blue;
		alpha = col_.alpha;
	}
	inline uint32 get_color()
	{
		return rgba(red, green, blue, alpha);
	}
};
class VectorTransform
{
public:
	class Position
	{
	public:
		float x1;
		float y1;
		float x2;
		float y2;
	};
	Position position;
	class Rotation
	{
	public:
		float angle = 0.0f;
		pivot pivot_rotation = center;
		Vector2f pivot_point;
		rotation_direction_ rotation_direction = clockwise;
	};
	Rotation rotation;
	class Size
	{
	public:
		int thickness_l = 1; //thickness left
		int thickness_r = 1; //thickness right
	};
	Size size;
	//Calculate Vector's magnitude
	float magnitude()
	{
		float adx = fabs(position.x2 - position.x1);
		float ady = fabs(position.y2 - position.y1);
		return sqrt(adx * adx + ady * ady);
	}
	inline void Rotate(float angle_, pivot rpivot, rotation_direction_ rotation_direction__ = clockwise)
	{
		/*How Rotate function works?
		All Rotate() functions work by using rotate_point() function with the inputted angle and setting pixels to new positions outputted from the rotate_point() function.
		rotate_point() function rotates pixel around inputted origin's x and y.*/
		rotation.pivot_rotation = rpivot;
		rotation.rotation_direction = rotation_direction__;
		int m = angle_ / 360.0f;
		if (angle_ >= 360.0f * (m))
		{
			angle_ = (angle_ - (360.0f * m));
		}
		else if (angle_ < 0.0f + (360 * (m - 1)))
		{
			angle_ = (360.0f * m) - fabs(angle_);
		}
		angle_ = fabs(angle_);
		if (rotation.pivot_rotation == center)
		{
			if (rotation_direction__ == clockwise)
			{
				angle_ = -1.0f * angle_;
			}
			int i = 0;
			if ((int)angle_ >= 180)
			{
				i = angle_ - 180;
			}
			else
			{
				i = 180 + angle_;
			}
			Vector2f_r p1 = rotate_point(position.x2, position.y2, (position.x1 + position.x2) / 2.0f, (position.y1 + position.y2) / 2.0f, angle_);
			Vector2f_r p2 = rotate_point(position.x2, position.y2, (position.x1 + position.x2) / 2.0f, (position.y1 + position.y2) / 2.0f, i);
			position.x1 = p1.x;
			position.y1 = p1.y;
			position.x2 = p2.x;
			position.y2 = p2.y;
			rotation.angle = angle_;
		}
		else if (rotation.pivot_rotation == startpos)
		{
			if (rotation_direction__ == clockwise)
			{
				angle_ = -1.0f * angle_;
			}
			Vector2f_r p1 = rotate_point(position.x2, position.y2, position.x1, position.y1, angle_);
			position.x2 = p1.x;
			position.y2 = p1.y;
			rotation.angle = angle_;
		}
		if (rotation.pivot_rotation == endpos)
		{
			if (rotation_direction__ == clockwise)
			{
				angle_ = -1.0f * angle_;
			}
			Vector2f_r p1 = rotate_point(position.x1, position.y1, position.x2, position.y2, angle_);
			position.x1 = p1.x;
			position.y1 = p1.y;
			rotation.angle = angle_;
		}
		if (rotation.pivot_rotation == around_point)
		{
			if (rotation_direction__ == clockwise)
			{
				angle_ = -1.0f * angle_;
			}
			Vector2f_r rpa1 = rotate_point(position.x1, position.y1, rotation.pivot_point.x, rotation.pivot_point.y, angle_);
			Vector2f_r rpa2 = rotate_point(position.x2, position.y2, rotation.pivot_point.x, rotation.pivot_point.y, angle_);
			position.x1 = rpa1.x;
			position.y1 = rpa1.y;
			position.x2 = rpa2.x;
			position.y2 = rpa2.y;
			rotation.angle = angle_;
		}
	}
};
//Set window screen color
//inline static void update_screen(uint32 color, WINDOW win) // processing screen visibility. This function is done to place update_screen function to the bottom of the code.
//{
//	screen.update_screen_bool[win.wnd_param] = true;
//	screen.scr_refresh_color[win.wnd_param] = color;
//}
int get_window_upper_height(WINDOW& window)
{
	float x_pixel_size = PIXEL_SIZE_DEF * _win.win_height[window.wnd_param] * render_scale;
	float y_pixel_size = PIXEL_SIZE_DEF * render_scale * render_scale;
	float x = window.width / 2.0f;
	float y = window.height;
	x *= _win.win_height[window.wnd_param] * render_scale;
	y *= _win.win_height[window.wnd_param] * render_scale;
	x += (float)_win.win_width[window.wnd_param] / 2.0f;
	y += (float)_win.win_height[window.wnd_param] / 2.0f;

	// Change to pixels
	int x0 = (int)x - (int)x_pixel_size;
	int x1 = (int)x + (int)x_pixel_size;
	int y0 = (int)y - (int)y_pixel_size;
	int y1 = (int)y + (int)y_pixel_size;

	x0 = clamp(0, x0, _win.win_width[window.wnd_param]);
	x1 = clamp(0, x1, _win.win_width[window.wnd_param]);
	y0 = clamp(0, y0, _win.win_height[window.wnd_param]);
	y1 = clamp(0, y1, _win.win_height[window.wnd_param]);
	uint32* pixel = (uint32*)_win.win_memory[window.wnd_param] + x0 + y0 * _win.win_width[window.wnd_param];
	int i = (int)pixel;
	int w = i - 4294967295;
	if (w > 0)
	{
		//exception
		int q = 0;
	}
	return w;
}
inline static void refresh_screen(uint32 color, WINDOW& window_)
{
	uint32* pixel = (uint32*)_win.win_memory[window_.wnd_param];
	for (int y = 0; y < window_.height; y++)
	{
		for (int x = 0; x < window_.width; x++)
		{
			*pixel++ = color;
		}
	}
}
//Draw pixels on x, y
inline static void draw_pixel(float x, float y, uint32 color, WINDOW& window)
{
	if ((x >= 0.0f && x < _win.win_width_px[window.wnd_param]) && (y > 0.0f && y < _win.win_height_px[window.wnd_param]))
	{
		y += 40.0f; //fixing some bugs
		uint32* pixel = (uint32*)_win.win_memory[window.wnd_param] + (int)x + (int)y * _win.win_width[window.wnd_param];
		*pixel++ = color;
	}
}
namespace ellipse2d
{
	class ellipsef
	{
	private:
		Vector2f half_ellpse[1800];
		inline float fast_trunc_ellipse_function(float num)
		{
			return int(num * 10.0f) / 10.0f;
		}
		inline void draw_ellipse(float x, float y, float ra, float rb, uint32 color, bool filled, WINDOW win_)
		{
			float get_angle = 0.0f;
			float j = rb; //temp_j is pre-cycle y value to check if y to place vectors is changed; j is by default is the highest point of the circle
			int px_amount = 0;
			int px_circum = 0;
			if (end_circum > start_circum)
			{
				get_angle = start_circum * 0.1f;
				for (int i = start_circum; i < end_circum; i++)
				{
					int b = 0;
					float temp_j = 0.0f;
					float fx2 = 0.0f;
					float x2 = 0.0f;
					float y2 = 0.0f;
					float circle_trigonometric_var = get_angle * pi / 180.0f;
					x2 = ra * fast_sin(circle_trigonometric_var);
					y2 = rb * fast_cos(circle_trigonometric_var);
					if (filled == true)
					{
						fx2 = fast_trunc_ellipse_function(fabs(x2)); //rounds to tenths(because of coordinate unit and pixel proportion which is 0.1f) here to place certain number of pixels for every line to fill circle
						temp_j = y2; //temp_j is temperate y value to check y current cycle status
					}
					if (filled == false)
					{
						if (visible)
						{
							draw_pixel(x2 + x, y2 + y, color, win_);
						}
					}
					if (filled == true)
					{
						if (fast_trunc_ellipse_function(temp_j) != j)
						{
							j = fast_trunc_ellipse_function(temp_j);
							// bug with lack of pixels in center
							//if (i >= 900 && i <= 1000)
							//{
							//	vector2d::fvector v1(fx2 + x, y, x - fx2, y, color, true); v1.draw();
							//	draw_pixel(v1.x2, v1.y1, color);
							//	draw_pixel(v1.x2, v1.y1 + 0.3f, color);
							//	draw_pixel(v1.x2, v1.y1 + 0.2f, color);
							//	draw_pixel(v1.x2, v1.y1 + 0.1f, color);
							//	draw_pixel(v1.x2, v1.y1 - 0.1f, color);
							//	draw_pixel(v1.x2, v1.y1 - 0.2f, color);
							//	draw_pixel(v1.x2, v1.y1 - 0.3f, color);
							//}
							int k = abs(1800 - end_circum); // if end_circum is 2100 and ellipse must be filled it must fill right part of the circle only the when i > 1800 - |1800-end_circum| and i < 1800.
							if (start_circum > 1800 && i < 1800 + k && i > 1800 - (start_circum - 1800))
							{
								for (int q = 0; q < fabs(fx2); q++)
								{
									if (visible)
									{
										draw_pixel((fx2 - x) - q, y + y2, color, win_);
									}
									//vector2d::fvector v1(fx2 - x, y + y2, x - fx2, y + y2, color, true); v1.draw();
								}
							}
							else if (i >= 1800)
							{
								for (int q = 0; q < fabs(fx2); q++)
								{
									if (visible)
									{
										draw_pixel((x - fx2) + q, y + y2, color, win_);
									}
									//vector2d::fvector v1(x - fx2, y + y2, x, y + y2, color, true); v1.draw();
								}
							}
							else if (i <= 1800)
							{
								for (int q = 0; q < fabs(fx2); q++)
								{
									if (visible)
									{
										draw_pixel((fx2 + x) - q, y + y2, color, win_);
									}
									//vector2d::fvector v1(x + fx2, y + y2, x, y + y2, color, true); v1.draw();
								}
							}
						}
						else
						{
							if (visible)
							{
								draw_pixel(x2 + x, y2 + y, color, win_);
							}
						}
					}
					get_angle += 0.1f;
				}
			}
			else
			{
				get_angle = start_circum * 0.1f;
				for (int i = start_circum; i > end_circum; i--)
				{
					int b = 0;
					float temp_j = 0.0f;
					float fx2 = 0.0f;
					float x2 = 0.0f;
					float y2 = 0.0f;
					float circle_trigonometric_var = get_angle * pi / 180.0f;
					x2 = ra * fast_sin(circle_trigonometric_var);
					y2 = rb * fast_cos(circle_trigonometric_var);
					if (filled == true)
					{
						fx2 = fast_trunc_ellipse_function(fabs(x2)); //rounds to tenths(because of coordinate unit and pixel proportion which is 0.1f) here to place certain number of pixels for every line to fill circle
						temp_j = y2; //temp_j is temperate y value to check y current cycle status
					}
					if (filled == false)
					{
						if (visible)
						{
							draw_pixel(x2 + x, y2 + y, color, win_);
						}
					}
					if (filled == true)
					{
						if (fast_trunc_ellipse_function(temp_j) != j)
						{
							j = fast_trunc_ellipse_function(temp_j);
							// bug with lack of pixels in center
							//if (i >= 900 && i <= 1000)
							//{
							//	vector2d::fvector v1(fx2 + x, y, x - fx2, y, color, true); v1.draw();
							//	draw_pixel(v1.x2, v1.y1, color);
							//	draw_pixel(v1.x2, v1.y1 + 0.3f, color);
							//	draw_pixel(v1.x2, v1.y1 + 0.2f, color);
							//	draw_pixel(v1.x2, v1.y1 + 0.1f, color);
							//	draw_pixel(v1.x2, v1.y1 - 0.1f, color);
							//	draw_pixel(v1.x2, v1.y1 - 0.2f, color);
							//	draw_pixel(v1.x2, v1.y1 - 0.3f, color);
							//}
							int k = abs(1800 - end_circum); // if end_circum is 2100 and ellipse must be filled it must fill right part of the circle only the when i > 1800 - |1800-end_circum| and i < 1800.
							if (start_circum > 1800 && i < 1800 + k && i > 1800 - (start_circum - 1800))
							{
								for (int q = 0; q < fabs(fx2); q++)
								{
									if (visible)
									{
										draw_pixel((fx2 - x) - q, y + y2, color, win_);
									}
									//vector2d::fvector v1(fx2 - x, y + y2, x - fx2, y + y2, color, true); v1.draw();
								}
							}
							else if (i >= 1800)
							{
								for (int q = 0; q < fabs(fx2); q++)
								{
									if (visible)
									{
										draw_pixel((x - fx2) + q, y + y2, color, win_);
									}
									//vector2d::fvector v1(x - fx2, y + y2, x, y + y2, color, true); v1.draw();
								}
							}
							else if (i <= 1800)
							{
								for (int q = 0; q < fabs(fx2); q++)
								{
									if (visible)
									{
										draw_pixel((fx2 + x) - q, y + y2, color, win_);
									}
									//vector2d::fvector v1(x + fx2, y + y2, x, y + y2, color, true); v1.draw();
								}
							}
						}
						else
						{
							if (visible)
							{
								draw_pixel(x2 + x, y2 + y, color, win_);
							}
						}
					}
					get_angle -= 0.1f;
				}
			}
		}
	public:
		float x = 0.0f;
		float y = 0.0f;
		float radiusa = 0.0f;
		float radiusb = 0.0f;
		int start_circum = 0;
		int end_circum = 3600;
		bool visible = true;
		bool filled = false;
		uint32 color = colors.red;
		WINDOW window;
		ellipsef(WINDOW window_, float x_, float y_, float radiusa_, float radiusb_, uint32 color_, int start_circum_, int end_circum_, bool filled_, bool visible_ = true)
		{
			window = window_;
			x = x_;
			y = y_;
			radiusa = radiusa_;
			radiusb = radiusb_;
			filled = filled_;
			color = color_;
			start_circum = start_circum_;
			end_circum = end_circum_;
			visible = visible_;
		}
		ellipsef(WINDOW window_, Vector2f& pos_, float radiusa_, float radiusb_, uint32 color_, bool filled_, bool visible_ = true)
		{
			window = window_;
			x = pos_.x;
			y = pos_.y;
			radiusa = radiusa_;
			radiusb = radiusb_;
			filled = filled_;
			color = color_;
			start_circum = 0;
			end_circum = 3600;
			visible = visible_;
		}
		ellipsef(WINDOW window_, Vector2f& pos_, float radiusa_, float radiusb_, uint32 color_, int start_circum_, int end_circum_, bool filled_, bool visible_ = true)
		{
			window = window_;
			x = pos_.x;
			y = pos_.y;
			radiusa = radiusa_;
			radiusb = radiusb_;
			filled = filled_;
			color = color_;
			start_circum = start_circum_;
			end_circum = end_circum_;
			visible = visible_;
		}
		ellipsef()
		{

		}
		//Calculate circle's size
		int perimeter()
		{
			float h = pow((radiusa - radiusb), 2) / pow((radiusa + radiusb), 2);
			float p = (pi * radiusa) + (pi * radiusb) * (1.0f + ((3.0f * h) / 10.0f + sqrt(4.0f - 3.0f * h)));
			return p / 2.0f;
		}
		int area()
		{
			return pi * radiusa * radiusb;
		}
		inline void draw()
		{
			draw_ellipse(x, y, radiusa, radiusb, color, filled, window);
		}
	};
	class ellipsed
	{
	private:
		inline float fast_trunc_ellipse_function(float num)
		{
			return int(num * 10.0f) / 10.0f;
		}
		inline void draw_ellipse(float x, float y, float ra, float rb, uint32 color, bool filled, WINDOW win_)
		{
			float get_angle = 0.0f;
			float j = rb; //temp_j is pre-cycle y value to check if y to place vectors is changed; j is by default is the highest point of the circle
			int px_amount = 0;
			int px_circum = 0;
			if (end_circum > start_circum)
			{
				get_angle = start_circum * 0.1f;
				for (int i = start_circum; i < end_circum; i++)
				{
					int b = 0;
					float temp_j = 0.0f;
					float fx2 = 0.0f;
					float x2 = 0.0f;
					float y2 = 0.0f;
					float circle_trigonometric_var = get_angle * pi / 180.0f;
					x2 = ra * fast_sin(circle_trigonometric_var);
					y2 = rb * fast_cos(circle_trigonometric_var);
					if (filled == true)
					{
						fx2 = fast_trunc_ellipse_function(fabs(x2)); //rounds to tenths(because of coordinate unit and pixel proportion which is 0.1f) here to place certain number of pixels for every line to fill circle
						temp_j = y2; //temp_j is temperate y value to check y current cycle status
					}
					if (filled == false)
					{
						if (visible)
						{
							draw_pixel(x2 + x, y2 + y, color, win_);
							matrix_pixels_pos[i].x = x2 + x;
							matrix_pixels_pos[i].y = y2 + y;
						}
					}
					if (filled == true)
					{
						if (fast_trunc_ellipse_function(temp_j) != j)
						{
							j = fast_trunc_ellipse_function(temp_j);
							// bug with lack of pixels in center
							//if (i >= 900 && i <= 1000)
							//{
							//	vector2d::fvector v1(fx2 + x, y, x - fx2, y, color, true); v1.draw();
							//	draw_pixel(v1.x2, v1.y1, color);
							//	draw_pixel(v1.x2, v1.y1 + 0.3f, color);
							//	draw_pixel(v1.x2, v1.y1 + 0.2f, color);
							//	draw_pixel(v1.x2, v1.y1 + 0.1f, color);
							//	draw_pixel(v1.x2, v1.y1 - 0.1f, color);
							//	draw_pixel(v1.x2, v1.y1 - 0.2f, color);
							//	draw_pixel(v1.x2, v1.y1 - 0.3f, color);
							//}
							int k = abs(1800 - end_circum); // if end_circum is 2100 and ellipse must be filled it must fill right part of the circle only the when i > 1800 - |1800-end_circum| and i < 1800.
							if (start_circum > 1800 && i < 1800 + k && i > 1800 - (start_circum - 1800))
							{
								for (int q = 0; q < fabs(fx2); q++)
								{
									if (visible)
									{
										draw_pixel((fx2 - x) - q, y + y2, color, win_);
									}
									//vector2d::fvector v1(fx2 - x, y + y2, x - fx2, y + y2, color, true); v1.draw();
								}
							}
							else if (i >= 1800)
							{
								for (int q = 0; q < fabs(fx2); q++)
								{
									if (visible)
									{
										draw_pixel((x - fx2) + q, y + y2, color, win_);
									}
									//vector2d::fvector v1(x - fx2, y + y2, x, y + y2, color, true); v1.draw();
								}
							}
							else if (i <= 1800)
							{
								for (int q = 0; q < fabs(fx2); q++)
								{
									if (visible)
									{
										draw_pixel((fx2 + x) - q, y + y2, color, win_);
									}
									//vector2d::fvector v1(x + fx2, y + y2, x, y + y2, color, true); v1.draw();
								}
							}
							matrix_pixels_pos[i].x = x2 + x;
							matrix_pixels_pos[i].y = y2 + y;
						}
						else
						{
							if (visible)
							{
								draw_pixel(x2 + x, y2 + y, color, win_);
								matrix_pixels_pos[i].x = x2 + x;
								matrix_pixels_pos[i].y = y2 + y;
							}
						}
					}
					get_angle += 0.1f;
				}
			}
			else
			{
				get_angle = start_circum * 0.1f;
				for (int i = start_circum; i > end_circum; i--)
				{
					int b = 0;
					float temp_j = 0.0f;
					float fx2 = 0.0f;
					float x2 = 0.0f;
					float y2 = 0.0f;
					float circle_trigonometric_var = get_angle * pi / 180.0f;
					x2 = ra * fast_sin(circle_trigonometric_var);
					y2 = rb * fast_cos(circle_trigonometric_var);
					if (filled == true)
					{
						fx2 = fast_trunc_ellipse_function(fabs(x2)); //rounds to tenths(because of coordinate unit and pixel proportion which is 0.1f) here to place certain number of pixels for every line to fill circle
						temp_j = y2; //temp_j is temperate y value to check y current cycle status
					}
					if (filled == false)
					{
						if (visible)
						{
							draw_pixel(x2 + x, y2 + y, color, win_);
							matrix_pixels_pos[i].x = x2 + x;
							matrix_pixels_pos[i].y = y2 + y;
						}
					}
					if (filled == true)
					{
						if (fast_trunc_ellipse_function(temp_j) != j)
						{
							j = fast_trunc_ellipse_function(temp_j);
							// bug with lack of pixels in center
							//if (i >= 900 && i <= 1000)
							//{
							//	vector2d::fvector v1(fx2 + x, y, x - fx2, y, color, true); v1.draw();
							//	draw_pixel(v1.x2, v1.y1, color);
							//	draw_pixel(v1.x2, v1.y1 + 0.3f, color);
							//	draw_pixel(v1.x2, v1.y1 + 0.2f, color);
							//	draw_pixel(v1.x2, v1.y1 + 0.1f, color);
							//	draw_pixel(v1.x2, v1.y1 - 0.1f, color);
							//	draw_pixel(v1.x2, v1.y1 - 0.2f, color);
							//	draw_pixel(v1.x2, v1.y1 - 0.3f, color);
							//}
							int k = abs(1800 - end_circum); // if end_circum is 2100 and ellipse must be filled it must fill right part of the circle only the when i > 1800 - |1800-end_circum| and i < 1800.
							if (start_circum > 1800 && i < 1800 + k && i > 1800 - (start_circum - 1800))
							{
								for (int q = 0; q < fabs(fx2); q++)
								{
									if (visible)
									{
										draw_pixel((fx2 - x) - q, y + y2, color, win_);
									}
									//vector2d::fvector v1(fx2 - x, y + y2, x - fx2, y + y2, color, true); v1.draw();
								}
							}
							else if (i >= 1800)
							{
								for (int q = 0; q < fabs(fx2); q++)
								{
									if (visible)
									{
										draw_pixel((x - fx2) + q, y + y2, color, win_);
									}
									//vector2d::fvector v1(x - fx2, y + y2, x, y + y2, color, true); v1.draw();
								}
							}
							else if (i <= 1800)
							{
								for (int q = 0; q < fabs(fx2); q++)
								{
									if (visible)
									{
										draw_pixel((fx2 + x) - q, y + y2, color, win_);
									}
									//vector2d::fvector v1(x + fx2, y + y2, x, y + y2, color, true); v1.draw();
								}
							}
							matrix_pixels_pos[i].x = x2 + x;
							matrix_pixels_pos[i].y = y2 + y;
						}
						else
						{
							if (visible)
							{
								draw_pixel(x2 + x, y2 + y, color, win_);
								matrix_pixels_pos[i].x = x2 + x;
								matrix_pixels_pos[i].y = y2 + y;
							}
						}
					}
					get_angle -= 0.1f;
				}
			}
		}
	public:
		float x = 0.0f;
		float y = 0.0f;
		float radiusa = 0.0f;
		float radiusb = 0.0f;
		bool visible = true;
		Vector2f matrix_pixels_pos[3600];
		bool filled = false;
		uint32 color = colors.red;
		int start_circum = 0;
		int end_circum = 3600;
		WINDOW window;
		ellipsed(WINDOW _win_, float x_, float y_, float radiusa_, float radiusb_, uint32 color_, int start_circum_, int end_circum_, bool filled_, bool visible_ = true)
		{
			window = _win_;
			x = x_;
			y = y_;
			radiusa = radiusa_;
			radiusb = radiusb_;
			filled = filled_;
			color = color_;
			start_circum = start_circum_;
			end_circum = end_circum_;
			visible = visible_;
		}
		ellipsed(WINDOW _win_, Vector2f& pos_, float radiusa_, float radiusb_, uint32 color_, bool filled_, bool visible_ = true)
		{
			window = _win_;
			x = pos_.x;
			y = pos_.y;
			radiusa = radiusa_;
			radiusb = radiusb_;
			filled = filled_;
			color = color_;
			start_circum = 0;
			end_circum = 3600;
			visible = visible_;
		}
		ellipsed(WINDOW _win_, Vector2f& pos_, float radiusa_, float radiusb_, uint32 color_, int start_circum_, int end_circum_, bool filled_, bool visible_ = true)
		{
			window = _win_;
			x = pos_.x;
			y = pos_.y;
			radiusa = radiusa_;
			radiusb = radiusb_;
			filled = filled_;
			color = color_;
			start_circum = start_circum_;
			end_circum = end_circum_;
			visible = visible_;
		}
		ellipsed()
		{

		}
		//Calculate circle's size
		int perimeter()
		{
			float h = pow((radiusa - radiusb), 2) / pow((radiusa + radiusb), 2);
			float p = (pi * radiusa) + (pi * radiusb) * (1.0f + ((3.0f * h) / 10.0f + sqrt(4.0f - 3.0f * h)));
			return p / 2.0f;
		}
		int area()
		{
			return pi * radiusa * radiusb;
		}
		inline void draw()
		{
			draw_ellipse(x, y, radiusa, radiusb, color, filled, window);
		}
	};
}
namespace vector2d
{
	class fvector
	{
	private:
		inline void check(float x1, float x2, float y1, float y2, float slope_x_modf, float slope_y_modf, float& slope_x_incr, float& slope_y_incr, bool directions[4])
		{
			float differencex = 0.0f;
			float differencey = 0.0f;
			if (x2 > x1)
			{
				if (directions[0] == true || directions[2] == true)
				{
					differencex = x2 - (x1 + slope_x_modf);
				}
				if (directions[1] == true || directions[3] == true)
				{
					differencex = x2 - (x1 - slope_x_modf);
				}
			}
			if (x1 > x2)
			{
				if (directions[0] == true || directions[2] == true)
				{
					differencex = x1 - (x2 + slope_x_modf);
				}
				if (directions[1] == true || directions[3] == true)
				{
					differencex = x1 - (x2 - slope_x_modf);
				}
			}
			if (y2 > y1)
			{
				if (directions[0] == true || directions[1] == true)
				{
					differencey = y2 - (y1 + slope_y_modf);
				}
				if (directions[2] == true || directions[3] == true)
				{
					differencey = y2 - (y1 - slope_y_modf);
				}
			}
			if (y1 > y2)
			{
				if (directions[0] == true || directions[1] == true)
				{
					differencey = y1 - (y2 + slope_y_modf);
				}
				if (directions[2] == true || directions[3] == true)
				{
					differencey = y1 - (y2 - slope_y_modf);
				}
			}
			if (differencex < 0.0f)
			{
				slope_x_incr = 0.0f;
			}
			if (differencey < 0.0f)
			{
				slope_y_incr = 0.0f;
			}
		}
		inline void draw_vector(float x1, float y1, float x2, float y2, uint32 color)
		{
			bool x_y_outweight = false, xy_swap = false; // if x2>y2, false, y2>x2, true; if x2&&y2 < x1&&y1 swap them
			bool straight_line_drawing = false;
			bool draw_one_pixel = false;
			int straight_line_type = -1;
			float g = 0.0f;
			float p = 0.0f;
			float q = 0.0f;
			//0 - up
			//1 - down
			//2 - right
			//3 - left
			//if (x1 == y1 && x1 == x2 && x2 != y2 && neg_or_pos_num(y2)) //right
			//{
			//	straight_line_drawing = true;
			//	straight_line_type = 0;
			//}
			//else if (x1 == y1 && x1 == x2 && x2 != y2 && !neg_or_pos_num(y2)) //left
			//{
			//	straight_line_drawing = true;
			//	straight_line_type = 1;
			//}
			//else if (x1 == y1 && x1 == y2 && y2 != x2 && neg_or_pos_num(x2)) //up
			//{
			//	straight_line_drawing = true;
			//	straight_line_type = 2;
			//}
			//else if (x1 == y1 && x1 == y2 && y2 != x2 && !neg_or_pos_num(x2)) //down
			//{
			//	straight_line_drawing = true;
			//	straight_line_type = 3;
			//}
			if (x1 == x2 && y2 > y1)
			{
				straight_line_drawing = true;
				straight_line_type = 0;
			}
			if (x1 == x2 && y1 > y2)
			{
				straight_line_drawing = true;
				straight_line_type = 1;
			}
			if (y1 == y2 && x2 > x1)
			{
				straight_line_drawing = true;
				straight_line_type = 2;
			}
			if (y1 == y2 && x1 > x2)
			{
				straight_line_drawing = true;
				straight_line_type = 3;
			}
			if (x1 == x2 && y1 == y2)
			{
				draw_pixel(x2, y2, color, window);
				draw_one_pixel = true;
			}
			float adx = fabs(x2 - x1);
			float ady = fabs(y2 - y1);
			float slope_x_modf = 0.0f, slope_y_modf = 0.0f;
			if (highlight_vertexes)
			{
				for (int i = 0; i < 5; i++)
				{
					draw_pixel(x1, y1 + i * 10.0f, highlightColor, window);
					draw_pixel(x2, y2 + i * 10.0f, highlightColor, window);
				}
			}
			if (straight_line_drawing == false && draw_one_pixel == false)
			{
				//fabs(x1) > fabs(x2) && fabs(y1) > fabs(y2) ? xy_swap = true : xy_swap = false;
				//if (xy_swap == true)
				//{
				//	swap(x2, x1);
				//	swap(y2, y1);
				//	//swapping function
				//}
				bool directions[4]
				{
					false, // up_right
					false, // up_left
					false, // down_right
					false // down_left
				};
				float dx = x2 - x1;
				float dy = y2 - y1;
				dx > 0.0f && dy > 0.0f ? directions[0] = true : directions[0] = false; // if (dx > 0.0f && dy > 0.0f){directions[0]=true} else {directions=false}
				dx < 0.0f && dy > 0.0f ? directions[1] = true : directions[1] = false;
				dx > 0.0f && dy < 0.0f ? directions[2] = true : directions[2] = false;
				dx < 0.0f && dy < 0.0f ? directions[3] = true : directions[3] = false;
				float slope_x = 1.0f, slope_y = 1.0f;
				int start_x = 0, start_y = 0;
				adx > ady ? slope_x = make_float_divisible(fabs(adx), fabs(ady)) * 2.0f : slope_y = make_float_divisible(fabs(ady), fabs(adx)) * 2.0f;
				adx > ady ? x_y_outweight = false : x_y_outweight = true;
				adx > ady ? start_x = 1 : start_y = 1;
				float cl_x_add = ceil(slope_x);
				float cl_y_add = ceil(slope_y);
				bool break_loop = false;
				int direction_index = -1;
				float direction_modifier_x = 0.0f;
				float direction_modifier_y = 0.0f;
				for (int j = 0; j < 4; j++)
				{
					if (directions[j])
					{
						if (j == 0 || j == 2)
						{
							direction_modifier_x = 1.0f;
						}
						else if (j == 1 || j == 3)
						{
							direction_modifier_x = -1.0f;
						}
						if (j == 0 || j == 1)
						{
							direction_modifier_y = 1.0f;
						}
						else if (j == 2 || j == 3)
						{
							direction_modifier_y = -1.0f;
						}
					}
				}
				int max_pixels = 0;
				if (adx > ady)
				{
					max_pixels = adx * 10.0f;
				}
				else if (ady > adx)
				{
					max_pixels = ady * 10.0f;
				}
				else if (adx == ady)
				{
					max_pixels = ady * 10.0f; // or max_pixels = ((y2 - y1) * 100) - 1; there is no matter
				}
				for (int i = 0; i < int(max_pixels); i++)
				{
					if (break_loop) { break; pixelQuantity.px_quantity = i; }
					bool x_cycle = false, y_cycle = false;
					i % 2 == start_y ? x_cycle = true : x_cycle = false;
					i % 2 == start_x ? y_cycle = true : y_cycle = false;
					if (x_cycle == true)
					{
						if (x_y_outweight == false)
						{
							for (int x = 0; x < cl_x_add; x++)
							{
								float slope_x_incr = vectorQuality * direction_modifier_x, slope_y_incr = vectorQuality * direction_modifier_y;
								if (check_if_float_has_digits((double)slope_x) == false)// floor(fabs(slope_y)) will detect the last loop and that's why there is ceil in for-i loop statement.
								{ //if statement checks the last loop of for-i loop because only last pixel's size must be changed in x_cycle or y_cycle
									if (x == floor(fabs(slope_x)))
									{
										slope_x_incr = (fabs(slope_x) - floor(fabs(slope_x))) * vectorQuality * direction_modifier_x;
										//p += fabs(slope_x_incr);
										//g += fabs(slope_x_incr);
									}
									else if (x != floor(fabs(slope_x)))// if the loop is not the last then it doesn't change pixels' size.
									{
										slope_x_incr = vectorQuality * direction_modifier_x;
										//p+= fabs(slope_x_incr);
										//g += fabs(slope_x_incr);
									}
								}
								p++;
								g++;
								check(x1, x2, y1, y2, slope_x_modf, slope_y_modf, slope_x_incr, slope_y_incr, directions);
								if (slope_x_incr == 0.0f && slope_y_incr == 0.0f)
								{
									break_loop = true;
									break;
								}
								slope_x_modf += slope_x_incr;
								if (visible)
								{
									draw_pixel(x1 + slope_x_modf, y1 + slope_y_modf, color, window);
								}
							}
						}
						else if (x_y_outweight == true)
						{
							for (int x = 0; x < 2; x++)
							{
								float slope_x_incr = vectorQuality * direction_modifier_x, slope_y_incr = vectorQuality * direction_modifier_y;
								if (visible)
								{
									draw_pixel(x1 + slope_x_modf, y1 + slope_y_modf, color, window);
								}
								check(x1, x2, y1, y2, slope_x_modf, slope_y_modf, slope_x_incr, slope_y_incr, directions);
								//p += fabs(slope_x_incr);
								//g += fabs(slope_x_incr);
								p++;
								g++;
								if (slope_x_incr == 0.0f && slope_y_incr == 0.0f)
								{
									break_loop = true;
									break;
								}
								slope_x_modf += slope_x_incr;
							}
						}
					}
					if (y_cycle == true)
					{
						if (x_y_outweight == true)
						{
							for (int y = 0; y < cl_y_add; y++)
							{
								float slope_x_incr = vectorQuality * direction_modifier_y, slope_y_incr = vectorQuality * direction_modifier_y;
								if (check_if_float_has_digits(slope_y) == false)
								{
									if (y == floor(fabs(slope_y))) // floor(fabs(slope_y)) will detect the last loop and that's why there is ceil in for-i loop statement.
									{ //if statement checks the last loop of for-i loop because only last pixel's size must be changed in x_cycle or y_cycle
										slope_y_incr = (fabs(slope_y) - floor(fabs(slope_y))) * vectorQuality * direction_modifier_y;
										//q += fabs(slope_y_incr);
										//g += fabs(slope_y_incr);
									}
									else if (y != floor(fabs(slope_y))) // if the loop is not the last then it doesn't change pixels' size.
									{
										slope_y_incr = vectorQuality * direction_modifier_y;
										//q += fabs(slope_y_incr);
										//g += fabs(slope_y_incr);
									}
								}
								check(x1, x2, y1, y2, slope_x_modf, slope_y_modf, slope_x_incr, slope_y_incr, directions);
								if (slope_x_incr == 0.0f && slope_y_incr == 0.0f)
								{
									break_loop = true;
									break;
								}
								slope_y_modf += slope_y_incr;
								q++;
								g++;
								if (visible)
								{
									draw_pixel(x1 + slope_x_modf, y1 + slope_y_modf, color, window);
								}
							}
						}
						else if (x_y_outweight == false)
						{
							for (int x = 0; x < 2; x++) //+2 because of < sign in for, and because when the first pixel generates at y1+slope_y_modf it will not be counted in for
							{
								float slope_x_incr = vectorQuality * direction_modifier_y, slope_y_incr = vectorQuality * direction_modifier_y;
								if (visible)
								{
									draw_pixel(x1 + slope_x_modf, y1 + slope_y_modf, color, window);
								}
								check(x1, x2, y1, y2, slope_x_modf, slope_y_modf, slope_x_incr, slope_y_incr, directions);
								q += fabs(slope_y_incr);
								g += fabs(slope_y_incr);
								if (slope_x_incr == 0.0f && slope_y_incr == 0.0f)
								{
									break_loop = true;
									break;
								}
								q++;
								g++;
								slope_y_modf += slope_y_incr;
							}
						}
					}
					float slope_x_incr = vectorQuality * direction_modifier_y, slope_y_incr = vectorQuality * direction_modifier_y;
					check(x1, x2, y1, y2, slope_x_modf, slope_y_modf, slope_x_incr, slope_y_incr, directions);
					if (slope_x_incr == 0.0f && slope_y_incr == 0.0f)
					{
						break_loop = true;
					}
				}
			}
			else if (straight_line_drawing == true)
			{
				int max_pixels = 0;
				if (straight_line_type == 0 || straight_line_type == 1)
				{
					max_pixels = ady;
				}
				else if (straight_line_type == 2 || straight_line_type == 3)
				{
					max_pixels = adx;
				}
				g = max_pixels;
				for (int x = 0; x < max_pixels; x++)
				{
					if (straight_line_type == 0) //down
					{
						float slope_y_incr = PIXEL_SIZE_STLN; float slope_x_incr = PIXEL_SIZE_STLN;
						if (visible)
						{
							draw_pixel(x1, y1 + slope_y_modf, color, window);
						}
						slope_y_modf += slope_y_incr;
						q = max_pixels;
					}
					else if (straight_line_type == 1) //up
					{
						float slope_y_incr = PIXEL_SIZE_STLN; float slope_x_incr = PIXEL_SIZE_STLN;
						if (visible)
						{
							draw_pixel(x1, y1 + slope_y_modf, color, window);
						}
						slope_y_modf -= slope_y_incr;
						q = max_pixels;
					}
					else if (straight_line_type == 2) //right
					{
						float slope_y_incr = PIXEL_SIZE_STLN; float slope_x_incr = PIXEL_SIZE_STLN;
						if (visible)
						{
							draw_pixel(x1 + slope_x_modf, y1, color, window);
						}
						slope_x_modf += slope_x_incr;
						p = max_pixels;
					}
					else if (straight_line_type == 3) //left
					{
						float slope_y_incr = PIXEL_SIZE_STLN; float slope_x_incr = PIXEL_SIZE_STLN;
						if (visible)
						{
							draw_pixel(x1 + slope_x_modf, y1, color, window);
						}
						slope_x_modf -= slope_x_incr;
						p = max_pixels;
					}
				}
			}
			pixelQuantity.px_quantity_x_cycle = p;
			pixelQuantity.px_quantity_y_cycle = q;
			pixelQuantity.px_quantity = p + q;
		}
	public:
		class pixelQuantity_
		{
		public:
			float px_quantity = 0.0f;
			float px_quantity_x_cycle = 0.0f;
			float px_quantity_y_cycle = 0.0f;
		};
		pixelQuantity_ pixelQuantity;
		VectorTransform transform;
		float vectorQuality = VECTOR_MQ;
		bool visible = true;
		bool highlight_vertexes = false;
		uint32 highlightColor = colors.yellow;
		uint32 color = colors.red;
		WINDOW window;
		//Draw 2d vector.
		fvector(WINDOW win, float x1_, float y1_, float x2_, float y2_, uint32 color_, bool visible_ = true)
		{
			window = win;
			transform.position.x1 = x1_;
			transform.position.y1 = y1_;
			transform.position.x2 = x2_;
			transform.position.y2 = y2_;
			color = color_;
			vectorQuality = VECTOR_MQ;
			highlight_vertexes = false;
			highlightColor = colors.white;
			visible = visible_;
		}
		fvector(WINDOW win, float x1_, float y1_, Vector2f& vx2, uint32 color_, bool visible_ = true)
		{
			window = win;
			transform.position.x1 = x1_;
			transform.position.y1 = y1_;
			transform.position.x2 = vx2.x;
			transform.position.y2 = vx2.y;
			color = color_;
			vectorQuality = VECTOR_MQ;
			highlight_vertexes = false;
			highlightColor = colors.white;
			visible = visible_;
		}
		fvector(WINDOW win, Vector2f& vx1, float x2_, float y2_, uint32 color_, bool visible_ = true)
		{
			window = win;
			transform.position.x1 = vx1.x;
			transform.position.y1 = vx1.y;
			transform.position.x2 = x2_;
			transform.position.y2 = y2_;
			color = color_;
			vectorQuality = VECTOR_MQ;
			highlight_vertexes = false;
			highlightColor = colors.white;
			visible = visible_;
		}
		fvector(WINDOW win, Vector2f& vx1, Vector2f& vx2, uint32 color_, bool visible_ = true)
		{
			window = win;
			transform.position.x1 = vx1.x;
			transform.position.y1 = vx1.y;
			transform.position.x2 = vx2.x;
			transform.position.y2 = vx2.y;
			color = color_;
			vectorQuality = VECTOR_MQ;
			highlight_vertexes = false;
			highlightColor = colors.white;
			visible = visible_;
		}
		fvector(WINDOW win, VectorTransform& VectorTransform_, uint32 color_, bool visible_ = true)
		{
			window = win;
			transform.position.x1 = VectorTransform_.position.x1;
			transform.position.y1 = VectorTransform_.position.y1;
			transform.position.x2 = VectorTransform_.position.x2;
			transform.position.y2 = VectorTransform_.position.y2;
			color = color_;
			vectorQuality = VECTOR_MQ;
			highlight_vertexes = false;
			highlightColor = colors.white;
			visible = visible_;
		}
		fvector()
		{

		}
		inline void add_vector_vertex(float x2_, float y2_)
		{
			transform.position.x2 = x2_;
			transform.position.y2 = y2_;
			draw();
		}
		//Draw function
		inline void draw()
		{
			for (int i = 0; i < transform.size.thickness_l + transform.size.thickness_r + 1; i++) // +1 because thickness_l and thickness_r are by default zero and by default normal vector must be build and therefore to start for-loop with 1 loop.
			{
				//this if-statements are to merge two expected for-loops(one for-loop is for left thickness and the second for-loop is for right thickness) to one for-loop
				if (i < transform.size.thickness_l) //this is to draw left thickness
				{
					draw_vector(transform.position.x1 - (0.1f * i), transform.position.y1 + (0.1f * i), transform.position.x2 - (0.1f * i), transform.position.y2 + (0.1f * i), color);
				}
				if (i > transform.size.thickness_l && i < transform.size.thickness_r + transform.size.thickness_r)
				{
					draw_vector(transform.position.x1 + (0.1f * (i - transform.size.thickness_l)), transform.position.y1 - (0.1f * (i - transform.size.thickness_l)), transform.position.x2 + (0.1f * (i - transform.size.thickness_l)), transform.position.y2 - (0.1f * (i - transform.size.thickness_l)), color);
				}
			}
		}
	};
	class dvector
	{
	private:
		inline void check(float x1, float x2, float y1, float y2, float slope_x_modf, float slope_y_modf, float& slope_x_incr, float& slope_y_incr, bool directions[4])
		{
			float differencex = 0.0f;
			float differencey = 0.0f;
			if (x2 > x1)
			{
				if (directions[0] == true || directions[2] == true)
				{
					differencex = x2 - (x1 + slope_x_modf);
				}
				if (directions[1] == true || directions[3] == true)
				{
					differencex = x2 - (x1 - slope_x_modf);
				}
			}
			if (x1 > x2)
			{
				if (directions[0] == true || directions[2] == true)
				{
					differencex = x1 - (x2 + slope_x_modf);
				}
				if (directions[1] == true || directions[3] == true)
				{
					differencex = x1 - (x2 - slope_x_modf);
				}
			}
			if (y2 > y1)
			{
				if (directions[0] == true || directions[1] == true)
				{
					differencey = y2 - (y1 + slope_y_modf);
				}
				if (directions[2] == true || directions[3] == true)
				{
					differencey = y2 - (y1 - slope_y_modf);
				}
			}
			if (y1 > y2)
			{
				if (directions[0] == true || directions[1] == true)
				{
					differencey = y1 - (y2 + slope_y_modf);
				}
				if (directions[2] == true || directions[3] == true)
				{
					differencey = y1 - (y2 - slope_y_modf);
				}
			}
			if (differencex < 0.0f)
			{
				slope_x_incr = 0.0f;
			}
			if (differencey < 0.0f)
			{
				slope_y_incr = 0.0f;
			}
		}
		inline void draw_vector(float x1, float y1, float x2, float y2, uint32 color, bool save_pixels_matrix, bool save_pixels_matrix_x_cycle, bool save_pixels_matrix_y_cycle, bool rotate_around_call = false)
		{
			bool x_y_outweight = false, xy_swap = false; // if x2>y2, false, y2>x2, true; if x2&&y2 < x1&&y1 swap them
			bool straight_line_drawing = false;
			bool draw_just_pixel = false;
			int straight_line_type = -1;
			//0 - up
			//1 - down
			//2 - right
			//3 - left
			int g = 0;
			int p = 0;
			int q = 0;
			if (x1 == y1 && x1 == x2 && x2 != y2 && neg_or_pos_num(y2)) //right
			{
				straight_line_drawing = true;
				straight_line_type = 0;
			}
			else if (x1 == y1 && x1 == x2 && x2 != y2 && !neg_or_pos_num(y2)) //left
			{
				straight_line_drawing = true;
				straight_line_type = 1;
			}
			else if (x1 == y1 && x1 == y2 && y2 != x2 && neg_or_pos_num(x2)) //up
			{
				straight_line_drawing = true;
				straight_line_type = 2;
			}
			else if (x1 == y1 && x1 == y2 && y2 != x2 && !neg_or_pos_num(x2)) //down
			{
				straight_line_drawing = true;
				straight_line_type = 3;
			}
			else if (x1 == x2 && y2 > y1)
			{
				straight_line_drawing = true;
				straight_line_type = 0;
			}
			else if (x1 == x2 && y1 > y2)
			{
				straight_line_drawing = true;
				straight_line_type = 1;
			}
			else if (y1 == y2 && x2 > x1)
			{
				straight_line_drawing = true;
				straight_line_type = 2;
			}
			else if (y1 == y2 && x1 > x2)
			{
				straight_line_drawing = true;
				straight_line_type = 3;
			}
			else if (x1 == x2 && y1 == y2)
			{
				if (visible)
				{
					draw_pixel(x2, y2, color, window);
				}
				if (save_pixels_matrix == true)
				{
					matrix_pixels[g].x = x2;
					matrix_pixels[g].y = y2;
				}
				if (save_pixels_matrix_x_cycle == true || save_pixels_matrix_y_cycle == true)
				{
					matrix_pixels_x_cycle[p].x = x2;
					matrix_pixels_x_cycle[p].y = y2;
					matrix_pixels_y_cycle[q].x = x2;
					matrix_pixels_y_cycle[q].y = y2;
				}
				g += 1;
				p += 1;
				q += 1;
				draw_just_pixel = true;
			}
			float dx = x2 - x1;
			float dy = y2 - y1;
			float adx = fabs(dx);
			float ady = fabs(dy);
			float slope_x_modf = 0.0f, slope_y_modf = 0.0f;
			//for (int i = 0; i < 5; i++)
			//{
			//	draw_pixel(x1, y1 + i,rgb(255, 255, 0));
			//	draw_pixel(x2, y2 + i, rgb(255, 255, 0));
			//}
			//draw_pixel(0.0f, 0.0f, rgb(255, 255, 0));
			float slope_x = 1.0f, slope_y = 1.0f;
			int ipx = 0;
			if (straight_line_drawing == false && draw_just_pixel == false)
			{
				int max_pixels = 0;
				//fabs(x1) > fabs(x2) && fabs(y1) > fabs(y2) ? xy_swap = true : xy_swap = false;
				//if (xy_swap == true)
				//{
				//	swap(x2, x1);
				//	swap(y2, y1);
				//	//swapping function
				//}
				bool directions[4]
				{
					false, // up_right
					false, // up_left
					false, // down_right
					false // down_left
				};
				dx > 0.0f && dy > 0.0f ? directions[0] = true : directions[0] = false; // if (dx > 0.0f && dy > 0.0f){directions[0]=true} else {directions=false}
				dx < 0.0f && dy > 0.0f ? directions[1] = true : directions[1] = false;
				dx > 0.0f && dy < 0.0f ? directions[2] = true : directions[2] = false;
				dx < 0.0f && dy < 0.0f ? directions[3] = true : directions[3] = false;
				int start_x = 0, start_y = 0;
				adx > ady ? slope_x = make_float_divisible(adx, ady) * 2.0f : slope_y = make_float_divisible(ady, adx) * 2.0f;
				adx > ady ? x_y_outweight = false : x_y_outweight = true;
				adx > ady ? start_x = 1 : start_y = 1;
				float cl_x_add = ceil(slope_x);
				float cl_y_add = ceil(slope_y);
				px_quantity_x_cycle_each = floor(slope_x);
				px_quantity_y_cycle_each = floor(slope_y);
				bool break_loop = false;
				int direction_index = -1;
				float direction_modifier_x = 0.0f;
				float direction_modifier_y = 0.0f;
				for (int j = 0; j < 4; j++)
				{
					if (directions[j])
					{
						if (j == 0 || j == 2)
						{
							direction_modifier_x = 1.0f;
						}
						else if (j == 1 || j == 3)
						{
							direction_modifier_x = -1.0f;
						}
						if (j == 0 || j == 1)
						{
							direction_modifier_y = 1.0f;
						}
						else if (j == 2 || j == 3)
						{
							direction_modifier_y = -1.0f;
						}
					}
				}
				if (adx > ady)
				{
					max_pixels = adx * 10.0f;
				}
				else if (ady > adx)
				{
					max_pixels = ady * 10.0f;
				}
				else if (adx == ady)
				{
					max_pixels = adx * 10.0f; // or max_pixels = ((y2 - y1) * 100) - 1; there is no matter
				}
				for (int i = 0; i < int(max_pixels); i++)
				{
					if (break_loop) { break; }
					bool x_cycle = false, y_cycle = false;
					i % 2 == start_y ? x_cycle = true : x_cycle = false;
					i % 2 == start_x ? y_cycle = true : y_cycle = false;
					if (x_cycle == true)
					{
						if (x_y_outweight == false)
						{
							for (int x = 0; x < cl_x_add; x++)
							{
								float slope_x_incr = vectorQuality * direction_modifier_x, slope_y_incr = vectorQuality * direction_modifier_x;
								if (check_if_float_has_digits(slope_x) == false)// floor(fabs(slope_y)) will detect the last loop and that's why there is ceil in for-i loop statement.
								{ //if statement checks the last loop of for-i loop because only last pixel's size must be changed in x_cycle or y_cycle
									if (x == cl_x_add - 1)
									{
										slope_x_incr = (fabs(slope_x) - floor(fabs(slope_x))) * vectorQuality * direction_modifier_x;
									}
									else if (x != floor(fabs(slope_x)))// if the loop is not the last then it doesn't change pixels' size.
									{
										slope_x_incr = vectorQuality * direction_modifier_x;
									}
								}
								check(x1, x2, y1, y2, slope_x_modf, slope_y_modf, slope_x_incr, slope_y_incr, directions);
								if (slope_x_incr == 0.0f && slope_y_incr == 0.0f)
								{
									break_loop = true;
									break;
								}
								slope_x_modf += slope_x_incr;
								if (visible)
								{
									float x_ = x1 + slope_x_modf;
									float y_ = y1 + slope_y_modf;
									draw_pixel(x_, y_, color, window);
								}
								if (save_pixels_matrix == true)
								{
									matrix_pixels[g].x = x1 + slope_x_modf;
									matrix_pixels[g].y = y1 + slope_y_modf;
								}
								g++;
								if (save_pixels_matrix_x_cycle == true)
								{
									matrix_pixels_x_cycle[p].x = x1 + slope_x_modf;
									matrix_pixels_x_cycle[p].y = y1 + slope_y_modf;
								}
								p++;
							}
						}
						else if (x_y_outweight == true)
						{
							for (int x = 0; x < 2; x++)
							{
								float slope_x_incr = vectorQuality * direction_modifier_x, slope_y_incr = vectorQuality * direction_modifier_x;
								if (visible)
								{
									float x_ = x1 + slope_x_modf;
									float y_ = y1 + slope_y_modf;
									draw_pixel(x_, y_, color, window);
								}
								if (save_pixels_matrix == true)
								{
									matrix_pixels[g].x = x1 + slope_x_modf;
									matrix_pixels[g].y = y1 + slope_y_modf;
								}
								g++;
								if (save_pixels_matrix_x_cycle == true)
								{
									matrix_pixels_x_cycle[p].x = x1 + slope_x_modf;
									matrix_pixels_x_cycle[p].y = y1 + slope_y_modf;
								}
								p++;
								check(x1, x2, y1, y2, slope_x_modf, slope_y_modf, slope_x_incr, slope_y_incr, directions);
								if (slope_x_incr == 0.0f && slope_y_incr == 0.0f)
								{
									break_loop = true;
									break;
								}
								slope_x_modf += slope_x_incr;
							}
						}
					}
					if (y_cycle == true)
					{
						if (x_y_outweight == true)
						{
							for (int y = 0; y < cl_y_add; y++)
							{
								float slope_x_incr = vectorQuality * direction_modifier_y, slope_y_incr = vectorQuality * direction_modifier_y;
								if (check_if_float_has_digits(slope_y) == false)
								{
									if (y == cl_y_add - 1) // floor(fabs(slope_y)) will detect the last loop and that's why there is ceil in for-i loop statement.
									{ //if statement checks the last loop of for-i loop because only last pixel's size must be changed in x_cycle or y_cycle
										slope_y_incr = (fabs(slope_y) - floor(fabs(slope_y))) * vectorQuality * direction_modifier_y;
									}
									else if (y != floor(fabs(slope_y))) // if the loop is not the last then it doesn't change pixels' size.
									{
										slope_y_incr = vectorQuality * direction_modifier_y;
									}
								}
								check(x1, x2, y1, y2, slope_x_modf, slope_y_modf, slope_x_incr, slope_y_incr, directions);
								if (slope_x_incr == 0.0f && slope_y_incr == 0.0f)
								{
									px_quantity = i;
									break_loop = true;
									break;
								}
								slope_y_modf += slope_y_incr;
								if (visible)
								{
									float x_ = x1 + slope_x_modf;
									float y_ = y1 + slope_y_modf;
									draw_pixel(x_, y_, color, window);
								}
								if (save_pixels_matrix == true)
								{
									matrix_pixels[g].x = x1 + slope_x_modf;
									matrix_pixels[g].y = y1 + slope_y_modf;
								}
								g++;
								if (save_pixels_matrix_y_cycle == true)
								{
									matrix_pixels_y_cycle[q].x = x1 + slope_x_modf;
									matrix_pixels_y_cycle[q].y = y1 + slope_y_modf;
								}
								q++;
							}
						}
						else if (x_y_outweight == false)
						{
							for (int x = 0; x < 2; x++) //+2 because of < sign in for, and because when the first pixel generates at y1+slope_y_modf it will not be counted in for
							{
								float slope_x_incr = vectorQuality * direction_modifier_y, slope_y_incr = vectorQuality * direction_modifier_y;
								if (visible)
								{
									float x_ = x1 + slope_x_modf;
									float y_ = y1 + slope_y_modf;
									draw_pixel(x_, y_, color, window);
								}
								if (save_pixels_matrix == true)
								{
									matrix_pixels[g].x = x1 + slope_x_modf;
									matrix_pixels[g].y = y1 + slope_y_modf;
								}
								g++;
								if (save_pixels_matrix_y_cycle == true)
								{
									matrix_pixels_y_cycle[q].x = x1 + slope_x_modf;
									matrix_pixels_y_cycle[q].y = y1 + slope_y_modf;
								}
								q++;
								if (visible)
								{
									check(x1, x2, y1, y2, slope_x_modf, slope_y_modf, slope_x_incr, slope_y_incr, directions);
								}
								if (slope_x_incr == 0.0f && slope_y_incr == 0.0f)
								{
									break_loop = true;
									break;
								}
								slope_y_modf += slope_y_incr;
							}
						}
					}
					float slope_x_incr = vectorQuality * direction_modifier_y, slope_y_incr = vectorQuality * direction_modifier_y;
					check(x1, x2, y1, y2, slope_x_modf, slope_y_modf, slope_x_incr, slope_y_incr, directions);
					if (slope_x_incr == 0.0f && slope_y_incr == 0.0f)
					{
						break_loop = true;
						break;
					}
				}
			}
			else if (straight_line_drawing == true)
			{
				int max_pixels = 0;
				if (straight_line_type == 0 || straight_line_type == 1)
				{
					max_pixels = ady;
				}
				else if (straight_line_type == 2 || straight_line_type == 3)
				{
					max_pixels = adx;
				}
				ipx = max_pixels;
				for (int x = 0; x < max_pixels; x++)
				{
					if (straight_line_type == 0) //down
					{
						float slope_y_incr = PIXEL_SIZE_STLN;
						if (visible)
						{
							float x_ = x1;
							float y_ = y1 + slope_y_modf;
							draw_pixel(x_, y_, color, window);
						}
						if (save_pixels_matrix == true)
						{
							matrix_pixels[g].x = x1;
							matrix_pixels[g].y = y1 + slope_y_modf;
						}
						if (save_pixels_matrix_y_cycle == true)
						{
							matrix_pixels_y_cycle[q].x = x1;
							matrix_pixels_y_cycle[q].y = y1 + slope_y_modf;
						}
						q += 1;
						g += 1;
						slope_y_modf += slope_y_incr;
					}
					else if (straight_line_type == 1) //up
					{
						float slope_y_incr = PIXEL_SIZE_STLN;
						if (visible)
						{
							float x_ = x1;
							float y_ = y1 + slope_y_modf;
							draw_pixel(x_, y_, color, window);
						}
						if (save_pixels_matrix == true)
						{
							matrix_pixels[g].x = x1;
							matrix_pixels[g].y = y1 + slope_y_modf;
						}
						if (save_pixels_matrix_y_cycle == true)
						{
							matrix_pixels_y_cycle[q].x = x1;
							matrix_pixels_y_cycle[q].y = y1 + slope_y_modf;
						}
						q += 1;
						g += 1;
						slope_y_modf -= slope_y_incr;
					}
					else if (straight_line_type == 2) //right
					{
						float slope_x_incr = PIXEL_SIZE_STLN;
						if (visible)
						{
							float x_ = x1 + slope_x_modf;
							float y_ = y1;
							draw_pixel(x_, y_, color, window);
						}
						if (save_pixels_matrix == true)
						{
							matrix_pixels[g].x = x1 + slope_x_modf;
							matrix_pixels[g].y = y1;
						}
						if (save_pixels_matrix_x_cycle == true)
						{
							matrix_pixels_x_cycle[p].x = x1 + slope_x_modf;
							matrix_pixels_x_cycle[p].y = y1;
						}
						p += 1;
						g += 1;
						slope_x_modf += slope_x_incr;
						if (x >= max_pixels)
						{
							slope_x_incr = 0.0f;
							slope_x_modf = 0.0f;
							break;
						}
					}
					else if (straight_line_type == 3) //left
					{
						float slope_x_incr = PIXEL_SIZE_STLN;
						if (visible)
						{
							float x_ = x1 + slope_x_modf;
							float y_ = y1;
							draw_pixel(x_, y_, color, window);
						}
						if (save_pixels_matrix == true)
						{
							matrix_pixels[g].x = x1 + slope_x_modf;
							matrix_pixels[g].y = y1;
						}
						if (save_pixels_matrix_x_cycle == true)
						{
							matrix_pixels_x_cycle[p].x = x1 + slope_x_modf;
							matrix_pixels_x_cycle[p].y = y1;
						}
						p += 1;
						g += 1;
						slope_x_modf -= slope_x_incr;
						if (x >= max_pixels)
						{
							slope_x_incr = 0.0f;
							slope_x_modf = 0.0f;
							break;
						}
					}
				}
			}
			px_quantity_x_cycle = p;
			px_quantity_y_cycle = q;
			px_quantity = p + q;
		}
	public:
		int px_quantity = 0;
		int px_quantity_x_cycle = 0;
		int px_quantity_y_cycle = 0;
		Vector2f matrix_pixels[MAX_STORAGE_SIZE];
		Vector2f matrix_pixels_x_cycle[MAX_STORAGE_SIZE];
		Vector2f matrix_pixels_y_cycle[MAX_STORAGE_SIZE];
		float x1 = 0.0f;
		float y1 = 0.0f;
		float x2 = 0.0f;
		float y2 = 0.0f;
		float angle = 0.0f;
		float vectorQuality = VECTOR_MQ;
		uint32 color = colors.red;
		bool visible = true;
		bool save_pixels_position = false;
		bool save_pixels_position_x_cycle = false;
		bool save_pixels_position_y_cycle = false;
		int px_quantity_x_cycle_each = 0;
		int px_quantity_y_cycle_each = 0;
		pivot pivot_rotation = center;
		rotation_direction_ rotation_direction = clockwise;
		Vector2f pivot_point;
		bool rotate_around_call_ = false;
		int thickness_l = 1; //thickness left
		int thickness_r = 1; //thickness right
		WINDOW window;
		//Draw 2d vector.
		dvector(WINDOW win, float x1_, float y1_, float x2_, float y2_, uint32 color_, bool visible_ = true)
		{
			window = win;
			x1 = x1_;
			y1 = y1_;
			x2 = x2_;
			y2 = y2_;
			color = color_;
			vectorQuality = VECTOR_MQ;
			visible = visible_;
			pivot_point.x = 0.0f; pivot_point.y = 0.0f;
		}
		dvector(WINDOW win, float x1_, float y1_, Vector2f& vx2, uint32 color_, bool visible_ = true)
		{
			window = win;
			x1 = x1_;
			y1 = y1_;
			x2 = vx2.x;
			y2 = vx2.y;
			color = color_;
			vectorQuality = VECTOR_MQ;
			visible = visible_;
			pivot_point.x = 0.0f; pivot_point.y = 0.0f;
		}
		dvector(WINDOW win, Vector2f& vx1, float x2_, float y2_, uint32 color_, bool visible_ = true)
		{
			window = win;
			x1 = vx1.x;
			y1 = vx1.y;
			x2 = x2_;
			y2 = y2_;
			color = color_;
			vectorQuality = VECTOR_MQ;
			visible = visible_;
			pivot_point.x = 0.0f; pivot_point.y = 0.0f;
		}
		dvector(WINDOW win, Vector2f& vx1, Vector2f& vx2, uint32 color_, bool visible_ = true)
		{
			window = win;
			x1 = vx1.x;
			y1 = vx1.y;
			x2 = vx2.x;
			y2 = vx2.y;
			color = color_;
			vectorQuality = VECTOR_MQ;
			visible = visible_;
			pivot_point.x = 0.0f; pivot_point.y = 0.0f;
		}
		dvector(WINDOW win, float x1_, float y1_, float x2_, float y2_, uint32 color_, int thickness_l_, int thickness_r_, bool save_pixels_position_, bool save_pixels_position_x_cycle_, bool save_pixels_position_y_cycle_, bool visible_ = true)
		{
			window = win;
			x1 = x1_;
			y1 = y1_;
			x2 = x2_;
			y2 = y2_;
			color = color_;
			save_pixels_position = save_pixels_position_;
			save_pixels_position_x_cycle = save_pixels_position_x_cycle_;
			save_pixels_position_y_cycle = save_pixels_position_y_cycle_;
			vectorQuality = VECTOR_MQ;
			thickness_l = thickness_l_;
			thickness_r = thickness_r_;
			visible = visible_;
			pivot_point.x = 0.0f; pivot_point.y = 0.0f;
		}
		dvector(WINDOW win, Vector2f& vx1, Vector2f& vx2, uint32 color_, int thickness_l_, int thickness_r_, bool save_pixels_position_, bool save_pixels_position_x_cycle_, bool save_pixels_position_y_cycle_, bool visible_ = true)
		{
			window = win;
			x1 = vx1.x;
			y1 = vx1.y;
			x2 = vx2.x;
			y2 = vx2.y;
			color = color_;
			save_pixels_position = save_pixels_position_;
			save_pixels_position_x_cycle = save_pixels_position_x_cycle_;
			save_pixels_position_y_cycle = save_pixels_position_y_cycle_;
			vectorQuality = VECTOR_MQ;
			thickness_l = thickness_l_;
			thickness_r = thickness_r_;
			visible = visible_;
			pivot_point.x = 0.0f; pivot_point.y = 0.0f;
		}
		dvector(WINDOW win, Vector2f& vx1, float x2_, float y2_, uint32 color_, int thickness_l_, int thickness_r_, bool save_pixels_position_, bool save_pixels_position_x_cycle_, bool save_pixels_position_y_cycle_, bool visible_ = true)
		{
			window = win;
			x1 = vx1.x;
			y1 = vx1.y;;
			x2 = x2_;
			y2 = y2_;
			color = color_;
			save_pixels_position = save_pixels_position_;
			save_pixels_position_x_cycle = save_pixels_position_x_cycle_;
			save_pixels_position_y_cycle = save_pixels_position_y_cycle_;
			vectorQuality = VECTOR_MQ;
			thickness_l = thickness_l_;
			thickness_r = thickness_r_;
			visible = visible_;
			pivot_point.x = 0.0f; pivot_point.y = 0.0f;
		}
		dvector(WINDOW win, float x1_, float y1_, Vector2f& vx2, uint32 color_, int thickness_l_, int thickness_r_, bool save_pixels_position_, bool save_pixels_position_x_cycle_, bool save_pixels_position_y_cycle_, bool visible_ = true)
		{
			window = win;
			x1 = x1_;
			y1 = y1_;
			x2 = vx2.x;
			y2 = vx2.y;
			color = color_;
			save_pixels_position = save_pixels_position_;
			save_pixels_position_x_cycle = save_pixels_position_x_cycle_;
			save_pixels_position_y_cycle = save_pixels_position_y_cycle_;
			vectorQuality = VECTOR_MQ;
			thickness_l = thickness_l_;
			thickness_r = thickness_r_;
			visible = visible_;
			pivot_point.x = 0.0f; pivot_point.y = 0.0f;
		}
		dvector()
		{

		}
		int magnitude()
		{
			return px_quantity;
		}
		inline void Rotate(float angle_, pivot rpivot, rotation_direction_ rotation_direction__ = clockwise)
		{
			/*How Rotate function works?
			All Rotate() functions work by using rotate_point() function with the inputted angle and setting pixels to new positions outputted from the rotate_point() function.
			rotate_point() function rotates pixel around inputted origin's x and y.*/
			pivot_rotation = rpivot;
			rotation_direction = rotation_direction__;
			int m = angle_ / 360.0f;
			if (angle_ >= 360.0f * (m))
			{
				angle_ = (angle_ - (360.0f * m));
			}
			else if (angle_ < 0.0f + (360 * (m - 1)))
			{
				angle_ = (360.0f * m) - fabs(angle_);
			}
			angle_ = fabs(angle_);
			if (pivot_rotation == center)
			{
				if (rotation_direction__ == clockwise)
				{
					angle_ = -1.0f * angle_;
				}
				int i = 0;
				if ((int)angle_ >= 180)
				{
					i = angle_ - 180;
				}
				else
				{
					i = 180 + angle_;
				}
				Vector2f_r p1 = rotate_point(x2, y2, (x1 + x2) / 2.0f, (y1 + y2) / 2.0f, angle_);
				Vector2f_r p2 = rotate_point(x2, y2, (x1 + x2) / 2.0f, (y1 + y2) / 2.0f, i);
				x1 = p1.x;
				y1 = p1.y;
				x2 = p2.x;
				y2 = p2.y;
				angle = angle_;
			}
			else if (pivot_rotation == startpos)
			{
				if (rotation_direction__ == clockwise)
				{
					angle_ = -1.0f * angle_;
				}
				Vector2f_r p1 = rotate_point(x2, y2, x1, y1, angle_);
				x2 = p1.x;
				y2 = p1.y;
				angle = angle_;
			}
			if (pivot_rotation == endpos)
			{
				if (rotation_direction__ == clockwise)
				{
					angle_ = -1.0f * angle_;
				}
				Vector2f_r p1 = rotate_point(x1, y1, x2, y2, angle_);
				x1 = p1.x;
				y1 = p1.y;
				angle = angle_;
			}
			if (pivot_rotation == around_point)
			{
				if (rotation_direction__ == clockwise)
				{
					angle_ = -1.0f * angle_;
				}
				Vector2f_r rpa1 = rotate_point(x1, y1, pivot_point.x, pivot_point.y, angle_);
				Vector2f_r rpa2 = rotate_point(x2, y2, pivot_point.x, pivot_point.y, angle_);
				x1 = rpa1.x;
				y1 = rpa1.y;
				x2 = rpa2.x;
				y2 = rpa2.y;
				rotate_around_call_ = true;
				angle = angle_;
			}
		}
		//Calculate Vector's angle
		float get_angle()
		{
			return angle;
		}
		inline void create_projection(bool vh, bool ur, static int color) // vh - false - x, true - y; vh - true, if ur - false - left, ur - true - right; vh - false, if ur - false - bootom, if ur - true - top 
		{
			if (vh == false)
			{
				if (ur == false)
				{
					vector2d::fvector vprojectionx(window, x1, y1, x2, y1, color);
				}
				if (ur == true)
				{
					vector2d::fvector vprojectionx(window, x1, y2, x2, y2, color);
				}
			}
			if (vh == true)
			{
				if (ur == false)
				{
					vector2d::fvector vprojectiony(window, x1, y1, x1, y2, color);
				}
				if (ur == true)
				{
					vector2d::fvector vprojectiony(window, x2, y1, x2, y2, color);
				}
			}
		}
		//Draw function
		inline void draw()
		{
			for (int i = 0; i < thickness_l + thickness_r + 1; i++) // +1 because thickness_l and thickness_r are by default zero and by default normal vector must be build and therefore to start for-loop with 1 loop.
			{
				//this if-statements are to merge two expected for-loops(one for-loop is for left thickness and the second for-loop is for right thickness) to one for-loop
				if (i < thickness_l) //this is to draw left thickness
				{
					draw_vector(x1 - (0.1f * i), y1 + (0.1f * i), x2 - (0.1f * i), y2 + (0.1f * i), color, save_pixels_position, save_pixels_position_x_cycle, save_pixels_position_y_cycle, rotate_around_call_);
				}
				if (i > thickness_l && i < thickness_r + thickness_r)
				{
					draw_vector(x1 + (0.1f * (i - thickness_l)), y1 - (0.1f * (i - thickness_l)), x2 + (0.1f * (i - thickness_l)), y2 - (0.1f * (i - thickness_l)), color, save_pixels_position, save_pixels_position_x_cycle, save_pixels_position_y_cycle, rotate_around_call_);
				}
			}
		}
	};
}
namespace rectangle
{
	class rectf
	{
	public:
		float x = 0.0f;
		float y = 0.0f;
		float width = 0.0f;
		float height = 0.0f;
		uint32 color = colors.red;
		float angle = 0.0f;
		pivot transformation_pivot = startpos;
		pivot rotation_pivot = center;
		Vector2f rotation_point;
		bool visible = true;
		bool filled = true;
		bool rotate_bool = false;
		rotation_direction_ rotation_direction = clockwise;
		WINDOW window;
		float gt = 0.0f;
		//AirResistanceType_ ar_type = AR_DEFAULT;
		float ar_drag = 0.24f;
		float density = 0.0f; // no meaning for now

		//Draw rectangle
		rectf(WINDOW win, float x_, float y_, float width_, float height_, uint32 color_, bool filled_ = true, bool visible_ = true)
		{
			window = win;
			x = x_;
			y = y_;
			width = width_;
			height = height_;
			color = color_;
			filled = filled_;
			rotation_point.x = x;
			rotation_point.y = y;
			visible = visible_;
		}
		rectf(WINDOW win, Vector2f& pos_, float width_, float height_, uint32 color_, bool filled_ = true, bool visible_ = true)
		{
			window = win;
			x = pos_.x;
			y = pos_.y;
			width = width_;
			height = height_;
			color = color_;
			filled = filled_;
			rotation_point.x = x;
			rotation_point.y = y;
			visible = visible_;
		}
		rectf(WINDOW win, float x_, float y_, Vector2f& size, uint32 color_, bool filled_ = true, bool visible_ = true)
		{
			window = win;
			x = x_;
			y = y_;
			width = size.x;
			height = size.y;
			color = color_;
			filled = filled_;
			rotation_point.x = x;
			rotation_point.y = y;
			visible = visible_;
		}
		rectf(WINDOW win, Vector2f& pos_, Vector2f& size, uint32 color_, bool filled_ = true, bool visible_ = true)
		{
			window = win;
			x = pos_.x;
			y = pos_.y;
			width = size.x;
			height = size.y;
			color = color_;
			filled = filled_;
			rotation_point.x = x;
			rotation_point.y = y;
			visible = visible_;
		}
		rectf()
		{

		}
		~rectf()
		{

		}
		//Calculate rectangle's perimeter
		float perimeter()
		{
			return (2.0f * width) + (2.0f * height);
		}
		//Calculate rectangle's area
		float area()
		{
			return width * height;
		}
		inline void Rotate(float angle_, pivot rotation_pivot_, rotation_direction_ rotation_direction__ = clockwise)
		{
			/*How Rotate function works?
			All Rotate() functions work by using rotate_point() function with the inputted angle and setting pixels to new positions outputted from the rotate_point() function.
			rotate_point() function rotates pixel around inputted origin's x and y.*/
			angle = angle_;
			rotation_pivot = rotation_pivot_;
			rotation_direction = rotation_direction__;
			rotate_bool = true;
		}
		//Draw function
		inline void draw()
		{
			if (visible)
			{
				draw_rect(x, y, width, height, color, filled, rotate_bool, rotation_direction);
			}
		}
	private:
		inline void draw_rect(float x, float y, float width, float height, uint32 color, bool filled, bool rotate_call_ = false, rotation_direction_ rotation_direction__ = clockwise)
		{
			if (rotation_pivot == center)
			{
				rotation_point.x = x;
				rotation_point.y = y;
			}
			else if (rotation_pivot == startpos)
			{
				rotation_point.x = x - width / 2.0f;
				rotation_point.y = y - height / 2.0f;
			}
			else if (rotation_pivot == endpos)
			{
				rotation_point.x = x + width / 2.0f;
				rotation_point.y = y + height / 2.0f;
			}
			if (rotation_direction__ == clockwise)
			{
				angle = -1.0f * angle;
			}
			if (!filled && rotate_call_)
			{
				float x_ = x - width / 2.0f;
				float y_ = y - height / 2.0f;
				Vector2f_r p1 = rotate_point(x_, y_, x, y, angle);
				Vector2f_r p2 = rotate_point(x_, y + height / 2.0f, x, y, angle);
				Vector2f_r p3 = rotate_point(x + width / 2.0f, y_, x, y, angle);
				Vector2f_r p4 = rotate_point(x + width / 2.0f, y + height / 2.0f, x, y, angle);
				vector2d::fvector v1(window, p1.x, p1.y, p2.x, p2.y, color, true); v1.draw();
				vector2d::fvector v2(window, p1.x, p1.y, p3.x, p3.y, color, true); v2.draw();
				vector2d::fvector v3(window, p3.x, p3.y, p4.x, p4.y, color, true); v3.draw();
				vector2d::fvector v4(window, p2.x, p2.y, p4.x, p4.y, color, true); v4.draw();
			}
			if (filled && rotate_call_)
			{
				float sb = 0.0f;
				if (width >= height)
				{
					sb = width;
				}
				else
				{
					sb = height;
				}
				for (int i = 0; i < sb; i++)
				{
					for (int d = 0; d < height; d++)
					{
						Vector2f_r px1 = rotate_point(x - width / 2.0f + i, y - height / 2.0f + d, rotation_point.x, rotation_point.y, angle);
						draw_pixel(px1.x, px1.y, color, window);
					}
				}
			}
			if (!rotate_call_ && filled)
			{
				float sb = 0.0f;
				if (width >= height)
				{
					sb = width;
				}
				else
				{
					sb = height;
				}
				for (int i = 0; i < sb; i++)
				{
					vector2d::dvector v1(window, x - width / 2.0f + i, y - height / 2.0f, x - width / 2.0f + i, y + height / 2.0f, color, true);
					v1.draw();
				}
			}
			if (!rotate_call_ && !filled)
			{
				vector2d::fvector v1(window, x - width / 2.0f, y - height / 2.0f, x - width / 2.0f, y + height / 2.0f, color, visible); v1.draw();
				vector2d::fvector v2(window, x - width / 2.0f, y - height / 2.0f, x + width / 2.0f, y - height / 2.0f, color, visible); v2.draw();
				vector2d::fvector v3(window, x - width / 2.0f, y + height / 2.0f, x + width / 2.0f, y + height / 2.0f, color, visible); v3.draw();
				vector2d::fvector v4(window, x + width / 2.0f, y - height / 2.0f, x + width / 2.0f, y + height / 2.0f, color, visible); v4.draw();
			}
		}
	};
}

inline static void draw_coordinate_grid(uint32 color, WINDOW window)
{
	vector2d::fvector v1(window, 0, _win.win_height_px[window.wnd_param]/2, _win.win_width_px[window.wnd_param], _win.win_height_px[window.wnd_param]/2, color, true); v1.draw();
	vector2d::fvector v2(window, _win.win_width_px[window.wnd_param]/2, 0.0f, _win.win_width_px[window.wnd_param]/2, _win.win_height_px[window.wnd_param], color, true); v2.draw();
}

namespace cube
{
	class CUBE
	{
	private:
		void draw_cube(float x, float y, float a, int r, int g, int b) // size of each side of a cube
		{
			vector2d::fvector v1(window, x, y, x - (a / 2.0f), y - (a / 2.0f), rgb(r - 10, g - 10, b - 10));
			vector2d::fvector v2(window, x - (a / 2.0f), y - (a / 2.0f), x + (a / 2.0f), y - (a / 2.0f), rgb(r, g, b)); v2.draw();
			vector2d::fvector v3(window, x - (a / 2.0f), y - (a / 2.0f), x - (a / 2.0f), y + (a / 2.0f), rgb(r, g, b)); v3.draw();
			vector2d::fvector v4(window, x - (a / 2.0f), y + (a / 2.0f), x + (a / 2.0f), y + (a / 2.0f), rgb(r, g, b)); v4.draw();
			vector2d::fvector v5(window, x + (a / 2.0f), y - (a / 2.0f), x + (a / 2.0f), y + (a / 2.0f), rgb(r, g, b)); v5.draw();
			vector2d::fvector v6(window, x, y, x, y + a, rgb(r - 10, g - 10, b - 10)); v6.draw();
			vector2d::fvector v7(window, x, y + a, x + a, y + a, rgb(r, g, b)); v7.draw();
			vector2d::fvector v8(window, x, y + a, x - (a / 2.0f), y + (a / 2.0f), rgb(r, g, b)); v8.draw();
			vector2d::fvector v9(window, x, y, x + a, y, rgb(r - 10, g - 10, b - 10)); v9.draw();
			vector2d::fvector v10(window, x + a, y + a, x + (a / 2.0f), y + (a / 2.0f), rgb(r, g, b)); v10.draw();
			vector2d::fvector v11(window, x + (a / 2.0f), y - (a / 2.0f), x + a, y, rgb(r, g, b)); v11.draw();
			vector2d::fvector v12(window, x + a, y + a, x + a, y, rgb(r, g, b)); v12.draw();
			//fix bug with drawing last line
		}
	public:
		float x = 0.0f;
		float y = 0.0f;
		float a = 0.0f;
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		int px_cube = 0;
		WINDOW window;
		CUBE(WINDOW win, float x_, float y_, float a_, float r_, float g_, float b_, bool visible = true)
		{
			x = x_;
			y = y_;
			a = a_;
			r = r_;
			g = g_;
			b = b_;
			if (visible == true)
			{
				draw_cube(x_, y_, a_, r_, g_, b_);
			}
		}
		CUBE(WINDOW win, Vector2f& vx1, float a_, float r_, float g_, float b_, bool visible = true)
		{
			x = vx1.x;
			y = vx1.y;
			a = a_;
			r = r_;
			g = g_;
			b = b_;
			if (visible == true)
			{
				draw_cube(vx1.x, vx1.y, a_, r_, g_, b_);
			}
		}
		CUBE()
		{

		}
		~CUBE()
		{

		}
		float perimeter()
		{
			return 6.0f * a;
		}
		float area()
		{
			return a * a * a;
		}
		void draw()
		{
			draw_cube(x, y, a, r, g, b);
		}
	};
}
namespace triangle2d
{
	class triangle
	{
	private:
		inline void draw_triangle(float a, float b, float c, float h_apex, float x, float y, uint32 color, bool filled, bool rotate_call_ = false)
		{
			if (rotate_call_)
			{
				main_dvector.visible = false;
				main_fvectors[0].visible = false;
				main_fvectors[1].visible = false;
				main_dvector.Rotate(angle, rotation_pivot, rotation_direction);
				main_fvectors[0].transform.Rotate(angle, rotation_pivot, rotation_direction);
				main_fvectors[1].transform.Rotate(angle, around_point, rotation_direction);
			}
			main_dvector.draw(); //c-line
			main_fvectors[0].draw(); //a-line
			main_fvectors[1].draw(); //b-line
			if (filled)
			{
				Vector2f initial_point;
				initial_point.x = x;
				initial_point.y = y + h_apex/2.0f;
				for (int i = 0; i < main_dvector.px_quantity; i++)
				{
					vector2d::fvector v4(window, main_fvectors[0].transform.position.x2, main_fvectors[0].transform.position.y2, main_dvector.matrix_pixels[i].x, main_dvector.matrix_pixels[i].y, colors.white, true);
					v4.transform.rotation.pivot_point.x = main_dvector.x1;
					v4.transform.rotation.pivot_point.y = main_dvector.y1;
					if (rotate_call_)
					{
						v4.transform.Rotate(angle, around_point, rotation_direction);
					}
					v4.draw();
				}
			}
		}
	public:
		vector2d::dvector main_dvector;
		vector2d::fvector main_fvectors[2];
		float a = 0.0f;
		float b = 0.0f;
		float c = 0.0f;
		float h_apex = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		bool visible = true;
		uint32 color = colors.red;
		bool filled = false;
		float angle = 0.0f;
		pivot rotation_pivot = center;
		rotation_direction_ rotation_direction = clockwise;
		bool rotate_call = false;
		WINDOW window;
		//Draw triangle funciton. Exception: a + b > c && a + c > b && b + c > a and a,b,c,h > 0
		triangle(WINDOW win, float a_, float b_, float c_, float h_apex_, float x_, float y_, uint32 color_, bool filled_, bool visible = true)
		{
			window = win;
			a = a_;
			b = b_;
			c = c_;
			h_apex = h_apex_;
			x = x_;
			y = y_;
			color = color_;
			filled = filled_;
			InitializeMainVectors();
		}
		triangle()
		{

		}
		void Rotate(float angle_, pivot rotation_pivot_ = center, rotation_direction_ rotation_direction__ = clockwise)
		{
			rotate_call = true;
			angle = angle_;
			rotation_pivot = rotation_pivot_;
			rotation_direction = rotation_direction__;
		}
		void InitializeMainVectors()
		{
			vector2d::dvector v1(window, x - c / 2.0f, y, x + c / 2.0f, y, color, false, 1, 1, true, true, true);
			vector2d::fvector v2(window, x - c / 2.0f, y, (x - c / 2.0f) + a, y + h_apex, color, false); //a-line
			vector2d::fvector v3(window, x + c / 2.0f, y, main_fvectors[0].transform.position.x2, main_fvectors[0].transform.position.y2, color, false);  //b-line
			if (a >= b && a < c && b < c)
			{
				float abx = a - b;
				v2.transform.position.x1 = x - c / 2.0f; v2.transform.position.y1 = y; v2.transform.position.x2 = x + abx; v2.transform.position.y2 = y + h_apex; v2.color = color;
				v3.transform.position.x1 = x + c / 2.0f; v3.transform.position.y1 = y; v3.transform.position.x2 = v2.transform.position.x2; v3.transform.position.y2 = v2.transform.position.y2;  v3.color = color;
			}
			if (a < b && a < c && b < c)
			{
				float abx = b - a;
				v2.transform.position.x1 = x - c / 2.0f; v2.transform.position.y1 = y; v2.transform.position.x2 = x - abx; v2.transform.position.y2 = y + h_apex; v2.color = color;
				v3.transform.position.x1 = x + c / 2.0f; v3.transform.position.y1 = y; v3.transform.position.x2 = v2.transform.position.x2; v3.transform.position.y2 = v2.transform.position.y2;  v3.color = color;
			}
			if (b > c && a < c && a < b)
			{
				v3.transform.position.x2 = (x + c / 2.0f) - b; v3.transform.position.y2 = y + h_apex;
				v2.transform.position.x2 = v3.transform.position.x2; v2.transform.position.y2 = v3.transform.position.y2;
			}
			main_dvector = v1;
			main_fvectors[0] = v2;
			main_fvectors[1] = v3;
			main_fvectors[1].transform.rotation.pivot_point.x = v1.x1;
			main_fvectors[1].transform.rotation.pivot_point.y = v1.y1;
		}
		//Calculate triangle's perimeter
		int perimeter()
		{
			return a + b + c;
		}
		//Calculate triangle's area 
		int area()
		{
			return (h_apex * b) / 2.0f;
		}
		inline void draw()
		{
			if (visible)
			{
				draw_triangle(a, b, c, h_apex, x, y, color, filled, rotate_call);
			}
		}
	};
}
namespace text2d
{
	class text
	{
	private:
		Vector2f* next_pos = new Vector2f(0.0f, 0.0f);
		inline void draw_char(const char ch, Vector2f& position)
		{
			float temp_ch_width = ch_width;
			float temp_ch_height = ch_height;
			if (ch == 'a' || ch == 'b' || ch == 'c' || ch == 'd' || ch == 'e' || ch == 'f' || ch == 'g' || ch == 'h' || ch == 'i' || ch == 'j' || ch == 'k' || ch == 'l' || ch == 'm' || ch == 'n' || ch == 'o' || ch == 'p' || ch == 'q' || ch == 'r' || ch == 's' || ch == 't' || ch == 'u' || ch == 'v' || ch == 'w' || ch == 'x' || ch == 'y' || ch == 'z')
			{ // change width of small characters variable
				ch_width /= 0.6f;
				ch_height /= 0.6f;
			}
			if (ch == 'A')
			{
				vector2d::dvector v1(window, position.x, position.y, position.x + ch_width / 2.0f, position.y + ch_height, color, true); v1.save_pixels_position = true; v1.save_pixels_position_x_cycle = true; v1.save_pixels_position_y_cycle = true; v1.draw(); // left line
				vector2d::dvector v2(window, position.x + ch_width, position.y, v1.x2, v1.y2, color, true); v1.save_pixels_position = true; v1.save_pixels_position_x_cycle = true; v1.save_pixels_position_y_cycle = true; v2.draw(); // right line
				vector2d::fvector v3(window, v1.matrix_pixels_y_cycle[int(v1.px_quantity_y_cycle / 2.0f) - 2].x, v1.matrix_pixels_y_cycle[int(v1.px_quantity_y_cycle / 2.0f) - 1].y, v2.matrix_pixels_y_cycle[int(v2.px_quantity_y_cycle / 2.0f) - 1].x, v1.matrix_pixels_y_cycle[int(v1.px_quantity_y_cycle / 2.0f) - 1].y, color, true); v3.draw(); // middle line
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'B')
			{
				vector2d::dvector v1(window, position.x, position.y, position.x, position.y + ch_height, color, true); v1.save_pixels_position = true; v1.save_pixels_position_x_cycle = true; v1.save_pixels_position_y_cycle = true; v1.draw(); // straight line
				ellipse2d::ellipsef e1(window, position.x, position.y + (ch_height / 4.0f), ch_width / 1.5f, ch_height / 4.0f, color, 0, 1800, false, true); e1.draw(); // bottom ellipse
				ellipse2d::ellipsef e2(window, position.x, position.y + (ch_height / 1.33f), ch_width / 1.5f, ch_height / 4.0f, color, 0, 1800, false, true); e2.draw(); // top ellipse
				next_pos = new Vector2f(position.x + ch_width / 1.5f, position.y);
			}
			if (ch == 'C')
			{
				ellipse2d::ellipsef e1(window, position.x + ch_width / 2.0f, position.y + ch_height / 2.0f, ch_width / 2.0f, ch_height / 2.0f, color, 1400, 3600, false, true); e1.draw();
				ellipse2d::ellipsef e2(window, position.x + ch_width / 2.0f, position.y + ch_height / 2.0f, ch_width / 2.0f, ch_height / 2.0f, color, 0, 400, false, true); e1.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.23f, position.y + ch_height / 2.0f);
			}
			if (ch == 'D')
			{
				vector2d::dvector v1(window, position.x, position.y, position.x, position.y + ch_height, color, true); v1.save_pixels_position = true; v1.save_pixels_position_x_cycle = true; v1.save_pixels_position_y_cycle = true; v1.draw(); // straight line
				ellipse2d::ellipsef e1(window, position.x, position.y + ch_height / 2.0f, ch_width / 1.5f, ch_height / 2.0f, color, 0, 1800, false, true); e1.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.5f, position.y);
			}
			if (ch == 'E')
			{
				vector2d::fvector v1(window, position.x, position.y, position.x, position.y + ch_height, color, true); v1.draw(); // straight line
				vector2d::fvector v2(window, position.x, position.y + ch_height, position.x + ch_width, position.y + ch_height, color, true); v2.draw(); // top line
				vector2d::fvector v3(window, position.x, position.y + ch_height / 2.0f, position.x + ch_width, position.y + ch_height / 2.0f, color, true); v3.draw(); // middle line
				vector2d::fvector v4(window, position.x, position.y, position.x + ch_width, position.y, color, true); v4.draw(); // bottom line
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'F')
			{
				vector2d::fvector v1(window, position.x, position.y, position.x, position.y + ch_height, color, true); v1.draw(); // straight line
				vector2d::fvector v2(window, position.x, position.y + ch_height, position.x + ch_width, position.y + ch_height, color, true); v2.draw(); // top line
				vector2d::fvector v3(window, position.x, position.y + ch_height / 1.5f, position.x + ch_width, position.y + ch_height / 1.5f, color, true); v3.draw(); // middle line
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'G')
			{
				ellipse2d::ellipsef e1(window, position.x + ch_width, position.y + ch_height / 2.0f, ch_width, ch_height / 2.0f, color, 1800, 3600, false, true); e1.draw();
				vector2d::fvector v1(window, position.x + ch_width, position.y, position.x + ch_width, position.y + ch_height / 2.0f, color, true); v1.draw();
				vector2d::fvector v2(window, v1.transform.position.x2, v1.transform.position.y2, position.x + ch_width / 2.0f, position.y + ch_height / 2.0f, color, true); v2.draw();
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'H')
			{
				vector2d::fvector v1(window, position.x, position.y, position.x, position.y + ch_height, color, true); v1.draw();
				vector2d::fvector v2(window, position.x + ch_width / 1.5f, position.y, position.x + ch_width / 1.5f, position.y + ch_height, color, true); v2.draw();
				vector2d::fvector v3(window, position.x, position.y + ch_height / 2.0f, position.x + ch_width / 1.5f, position.y + ch_height / 2.0f, color, true); v3.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.5f, position.y);
			}
			if (ch == 'I') // Remember about the bug with I
			{
				vector2d::fvector v1(window, position.x, position.y, position.x, position.y + ch_height, color, true); v1.draw();
				next_pos = new Vector2f(position.x + 0.1f, position.y);
			}
			if (ch == 'J')
			{
				ellipse2d::ellipsef e1(window, position.x + ch_width / 2.0f, position.y + ch_height / 2.0f, ch_width / 2.0f, ch_height / 2.0f, color, 1100, 2500, false, true); e1.draw();
				vector2d::fvector v1(window, position.x + ch_width - 0.1f, position.y + ch_height / 2.9f, position.x + ch_width - 0.1f, position.y + ch_height / 1.0f, color, true); v1.draw();
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'K')
			{
				vector2d::fvector v1(window, position.x, position.y, position.x, position.y + ch_height, color, true); v1.draw();
				vector2d::fvector v2(window, position.x, position.y + ch_height / 2.0f, position.x + ch_width, position.y + ch_height, color, true); v2.draw();
				vector2d::fvector v3(window, position.x, position.y + ch_height / 2.0f, position.x + ch_width, position.y, color, true); v3.draw();
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'L')
			{
				vector2d::fvector v1(window, position.x, position.y, position.x, position.y + ch_height, color, true); v1.draw();
				vector2d::fvector v2(window, position.x, position.y, position.x + ch_width / 1.2f, position.y, color, true); v2.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.5f, position.y);
			}
			if (ch == 'M')
			{
				vector2d::fvector v1(window, position.x, position.y, position.x, position.y + ch_height, color, true); v1.draw();
				vector2d::fvector v2(window, position.x, position.y + ch_height, position.x + ch_width / 2.0f, position.y, color, true); v2.draw();
				vector2d::fvector v3(window, v2.transform.position.x2, v2.transform.position.y2, position.x + ch_width, position.y + ch_height, color, true); v3.draw();
				vector2d::fvector v4(window, position.x + ch_width, position.y, position.x + ch_width, position.y + ch_height, color, true); v4.draw();
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'N')
			{
				vector2d::fvector v1(window, position.x, position.y, position.x, position.y + ch_height, color, true); v1.draw();
				vector2d::fvector v2(window, position.x, position.y + ch_height, position.x + ch_width / 1.5f, position.y, color, true); v2.draw();
				vector2d::fvector v4(window, position.x + ch_width / 1.5f, position.y, position.x + ch_width / 1.5f, position.y + ch_height, color, true); v4.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.5f, position.y);
			}
			if (ch == 'O')
			{
				ellipse2d::ellipsef e1(window, position.x + ch_width / 2.0f, position.y + ch_height / 2.0f, ch_width / 2.0f, ch_height / 2.0f, color, 0, 3600, false, true); e1.draw();
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'P')
			{
				vector2d::fvector v1(window, position.x, position.y, position.x, position.y + ch_height, color, true); v1.draw(); // straight line
				ellipse2d::ellipsef e2(window, position.x, position.y + (ch_height / 1.33f), ch_width / 1.5f, ch_height / 4.0f, color, 0, 1800, false, true); e2.draw(); // top ellipse
				next_pos = new Vector2f(position.x + ch_width / 1.5f, position.y);
			}
			if (ch == 'Q')
			{
				ellipse2d::ellipsef e1(window, position.x + ch_width / 2.0f, position.y + ch_height / 2.0f, ch_width / 2.0f, ch_height / 2.0f, color, 0, 3600, false, true); e1.draw();
				vector2d::fvector v1(window, position.x + ch_width / 2.0f, position.y, position.x + ch_width / 1.6f, position.y - ch_height / 6.0f, color, true); v1.draw();
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'R')
			{
				vector2d::fvector v1(window, position.x, position.y, position.x, position.y + ch_height, color, true); v1.draw(); // straight line
				ellipse2d::ellipsef e2(window, position.x, position.y + (ch_height / 1.33f), ch_width, ch_height / 4.0f, color, 0, 1800, false, true); e2.draw(); // top ellipse
				vector2d::fvector v2(window, position.x, position.y + ch_height / 2.0f, position.x + ch_width, position.y, color, true); v2.draw(); // straight line
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'S')
			{
				ellipse2d::ellipsef e1(window, position.x, position.y + ch_height / 4.0f, ch_width, ch_height / 4.0f, color, 300, 1800, false, true); e1.draw(); // bottom ellipse
				ellipse2d::ellipsef e2(window, position.x + ch_width, position.y + ch_height / 1.33f, ch_width, ch_height / 4.0f, color, 2100, 3600, false, true); e2.draw(); // top ellipse
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'T')
			{
				vector2d::fvector v1(window, position.x + ch_width / 2.0f, position.y, position.x + ch_width / 2.0f, position.y + ch_height, color, true); v1.draw();
				vector2d::fvector v2(window, position.x - ch_width / 8.0f, position.y + ch_height, position.x + ch_width + ch_width / 8.0f, position.y + ch_height, color, true); v2.draw();
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'U')
			{
				ellipse2d::ellipsef e1(window, position.x + ch_width / 2.0f, position.y + ch_height / 1.6f, ch_width / 3.0f, ch_height / 1.6f, color, 3000, 600, false, true); e1.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.2f, position.y);
			}
			if (ch == 'V')
			{
				vector2d::fvector v1(window, position.x, position.y + ch_height, position.x + ch_width / 2.0f, position.y, color, true); v1.draw();
				vector2d::fvector v2(window, v1.transform.position.x2, v1.transform.position.y2, position.x + ch_width, position.y + ch_height, color, true); v2.draw();
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'W')
			{
				vector2d::fvector v1(window, position.x, position.y + ch_height, position.x + ch_width / 4.0f, position.y, color, true); v1.draw();
				vector2d::fvector v2(window, v1.transform.position.x2, v1.transform.position.y2, position.x + ch_width / 2.0f, position.y + ch_height, color, true); v2.draw();
				vector2d::fvector v3(window, v2.transform.position.x2, v2.transform.position.y2, position.x + ch_width / 1.33f, position.y, color, true); v3.draw();
				vector2d::fvector v4(window, v3.transform.position.x2, v3.transform.position.y2, position.x + ch_width, position.y + ch_height, color, true); v4.draw();
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'X')
			{
				vector2d::fvector v1(window, position.x, position.y + ch_height, position.x + ch_width, position.y, color, true); v1.draw();
				vector2d::fvector v2(window, position.x, position.y, position.x + ch_width, position.y + ch_height, color, true); v2.draw();
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'Y')
			{
				vector2d::fvector v1(window, position.x, position.y + ch_height, position.x + ch_width / 2.0f, position.y + ch_height / 2.0f, color, true); v1.draw();
				vector2d::fvector v2(window, v1.transform.position.x2, v1.transform.position.y2, position.x + ch_width, position.y + ch_height, color, true); v2.draw();
				vector2d::fvector v3(window, v1.transform.position.x2, v1.transform.position.y2, v1.transform.position.x2, position.y, color, true); v3.draw();
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'Z')
			{
				vector2d::fvector v1(window, position.x, position.y + ch_height, position.x + ch_width, position.y + ch_height, color, true); v1.draw();
				vector2d::fvector v2(window, v1.transform.position.x2, v1.transform.position.y2, position.x, position.y, color, true); v2.draw();
				vector2d::fvector v3(window, v2.transform.position.x2, v2.transform.position.y2, position.x + ch_width, position.y, color, true); v3.draw();
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == ' ')
			{
				next_pos = new Vector2f(position.x + space_width, position.y);
			}
			if (ch == 'a')
			{
				ellipse2d::ellipsef e1(window, position.x + ch_width / 8.0f, position.y + ch_height / 5.0f, ch_width / 5.0f, ch_height / 5.0f, color, 0, 3600, false, true); e1.draw();
				ellipse2d::ellipsef e2(window, position.x + (ch_width / 4.5f) + (ch_width / 2.0f), position.y + ch_height / 4.0f, ch_width / 2.4f, ch_height / 4.0f, color, 2900, 2100, false, true); e2.draw();
				next_pos = new Vector2f(position.x + (ch_width / 4.5f) + (ch_width / 5.0f), position.y);
			}
			if (ch == 'b')
			{
				vector2d::fvector v1(window, position.x, position.y, position.x, position.y + ch_height / 2.5f, color); v1.draw();	// straight line
				ellipse2d::ellipsef e1(window, position.x, position.y + (ch_height / 8.0f), ch_width / 3.0f, ch_height / 8.0f, color, 0, 1800, false, true); e1.draw(); // bottom ellipse
				next_pos = new Vector2f(position.x + ch_width / 3.0f, position.y);
			}
			if (ch == 'c')
			{
				ellipse2d::ellipsef e1(window, position.x + ch_width / 5.0f, position.y + ch_height / 5.0f, ch_width / 5.0f, ch_height / 5.0f, color, 1400, 3600, false, true); e1.draw();
				ellipse2d::ellipsef e2(window, position.x + ch_width / 5.0f, position.y + ch_height / 5.0f, ch_width / 5.0f, ch_height / 5.0f, color, 0, 400, false, true); e2.draw();
				next_pos = new Vector2f(position.x + ch_width / 5.0f + ch_width * 0.05f, position.y);
			}
			if (ch == 'd')
			{
				vector2d::fvector v1(window, position.x + ch_width / 6.0f, position.y, position.x + ch_width / 6.0f, position.y + ch_height / 2.2f, color); v1.draw(); // straight line
				ellipse2d::ellipsef e1(window, position.x + ch_width / 6.0f, position.y + (ch_height / 8.0f), ch_width / 4.0f, ch_height / 8.0f, color, 1800, 3600, false, true); e1.draw(); // bottom ellipse
				next_pos = new Vector2f(position.x + ch_width / 6.0f, position.y);
			}
			if (ch == 'e')
			{
				ellipse2d::ellipsef e1(window, position.x + ch_width / 5.0f, position.y + (ch_height / 5.0f), ch_width / 5.0f, ch_height / 5.0f, color, 1500, 3600, false, true); e1.draw(); // bottom ellipse
				ellipse2d::ellipsef e2(window, position.x + ch_width / 5.0f, position.y + (ch_height / 5.0f), ch_width / 5.0f, ch_height / 5.0f, color, 0, 700, false, true); e2.draw(); // second ellipse
				vector2d::fvector v1(window, position.x + ch_width / 5.0f + ch_width / 5.0f * fast_sin(69.9f * pi / 180.0f), position.y + (ch_height / 5.0f) + ch_height / 5.0f * fast_cos(69.9f * pi / 180.0f), position.x + ch_width / 5.0f + ch_width / 5.0f * fast_sin(300.0f * pi / 180.0f), position.y + (ch_height / 5.0f) + ch_height / 5.0f * fast_cos(69.9f * pi / 180.0f), color, true); v1.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.5f, position.y);
			}
			if (ch == 'f')
			{
				float avg = (position.x + ch_width / 4.0f) - position.x;
				vector2d::fvector v1(window, position.x + ch_width / 12.0f, position.y, position.x + ch_width / 12.0f, position.y + ch_height / 3.5f, color, true); v1.draw(); // straight line
				vector2d::fvector v2(window, position.x, position.y + ch_height / 3.5f, position.x + ch_width / 4.0f, position.y + ch_height / 3.5f, color, true); v2.draw(); // top line
				ellipse2d::ellipsef e1(window, position.x + avg * 1.5f, position.y + ch_height / 3.5f, ch_width / 4.0f, ch_height / 8.0f, color, 2700, 3500, false, true); e1.draw();
				next_pos = new Vector2f(position.x + ch_width / 8.0f, position.y);
			}
			if (ch == 'g')
			{
				vector2d::fvector v1(window, position.x + ch_width / 8.0f + ch_width / 5.3f, position.y, position.x + ch_width / 8.0f + ch_width / 6.3f, position.y + ch_height / 2.5f, color, true); v1.draw();
				ellipse2d::ellipsef e1(window, position.x + ch_width / (7.535f * 1.2f), position.y + ch_height / 4.5f, ch_width / 5.4f, ch_height / 6.5f, color, 0, 3600, false, true); e1.draw();
				ellipse2d::ellipsef e2(window, position.x + ch_width / (7.47f * 1.2f), position.y + ch_height / 25.0f, ch_width / 4.05f, ch_height / 4.75f, color, 700, 2100, false, true); e2.draw();
				//tail -- ellipse2d::ELLIPSE e3(position.x + ch_width+ch_width/7.4f, position.y + ch_height / 2.4f, ch_width / 4.0f, ch_height / 5.0f, color, 2700, 3600, false, true); v1.draw();
				next_pos = new Vector2f(position.x + ch_width / 5.0f + ch_width / 5.3f, position.y);
			}
			if (ch == 'h')
			{
				vector2d::fvector v1(window, position.x, position.y, position.x, position.y + ch_height / 1.6f, color, true); v1.draw();
				vector2d::fvector v2(window, position.x + ch_width / 4.0f - 0.1f, position.y, position.x + ch_width / 4.0f - 0.1f, position.y + ch_height / 4.0f, color, true); v2.draw();
				ellipse2d::ellipsef e1(window, position.x + ch_width / 8.0f, position.y + ch_height / 8.0f, ch_width / 8.0f, ch_height / 4.0f, color, 3000, 3600, false, true); e1.draw();
				ellipse2d::ellipsef e2(window, position.x + ch_width / 8.0f, position.y + ch_height / 8.0f, ch_width / 8.0f, ch_height / 4.0f, color, 0, 600, false, true); e2.draw();
				next_pos = new Vector2f(position.x + ch_width / 3.0f - 0.1f, position.y);
			}
			if (ch == 'i')
			{
				vector2d::fvector v1(window, position.x, position.y, position.x, position.y + ch_height / 3.0f, color, true); v1.draw();
				draw_pixel(position.x, position.y + ch_height / 2.4f, color, window);
				next_pos = new Vector2f(position.x + 0.1f, position.y);
			}
			if (ch == 'j')
			{
				ellipse2d::ellipsef e1(window, position.x - ch_width / 4.0f, position.y + ch_height / 3.0f, ch_width / 3.0f, ch_height / 1.5f, color, 900, 1700, false, true); e1.draw();
				//vector2d::fvector v1(position.x + ch_width / 8.0f-0.2f, position.y + ch_height / 8.0f - 0.8f, position.x + ch_width / 4.0f-0.2f, position.y + ch_height / 1.8f, color, true); v1.draw();
				draw_pixel(position.x + ch_width / 8.0f - 0.3f, position.y + ch_height / 2.0f, color, window);
				next_pos = new Vector2f(position.x + ch_width / 8.0f - 0.3f, position.y);
			}
			if (ch == 'k')
			{
				vector2d::fvector v1(window, position.x, position.y, position.x, position.y + ch_height / 2.5f, color, true); v1.draw();
				vector2d::fvector v2(window, position.x, position.y + ch_height / 4.0f, position.x + ch_width / 4.0f, position.y + ch_height / 2.5f, color, true); v2.draw();
				vector2d::fvector v3(window, position.x, position.y + ch_height / 4.0f, position.x + ch_width / 3.0f, position.y, color, true); v3.draw();
				next_pos = new Vector2f(position.x + ch_width / 3.0f, position.y);
			}
			if (ch == 'l')
			{
				vector2d::fvector v1(window, position.x, position.y, position.x, position.y + ch_height / 1.7f, color, true); v1.draw();
				next_pos = new Vector2f(position.x + 0.2f, position.y);
			}
			if (ch == 'm')
			{
				vector2d::fvector v1(window, position.x, position.y, position.x, position.y + ch_height / 2.5f, color, true); v1.draw();
				vector2d::fvector v2(window, position.x + ch_width / 4.0f, position.y, position.x + ch_width / 4.0f - 0.1f, position.y + ch_height / 4.0f, color, true); v2.draw();
				vector2d::fvector v3(window, position.x + ch_width / 2.0f - 0.1f, position.y, position.x + ch_width / 2.0f - 0.1f, position.y + ch_height / 4.0f, color, true); v3.draw();
				ellipse2d::ellipsef e1(window, position.x + ch_width / 8.2f, position.y + ch_height / 8.0f, ch_width / 8.0f, ch_height / 4.0f, color, 3000, 3600, false, true); e1.draw();
				ellipse2d::ellipsef e2(window, position.x + ch_width / 8.2f, position.y + ch_height / 8.0f, ch_width / 8.0f, ch_height / 4.0f, color, 0, 600, false, true); e2.draw();
				ellipse2d::ellipsef e3(window, position.x + ch_width / 2.76f, position.y + ch_height / 8.0f, ch_width / 8.0f, ch_height / 4.0f, color, 3000, 3600, false, true); e3.draw();
				ellipse2d::ellipsef e4(window, position.x + ch_width / 2.76f, position.y + ch_height / 8.0f, ch_width / 7.8f, ch_height / 4.0f, color, 0, 600, false, true); e4.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.8f - 0.1f, position.y);
			}
			if (ch == 'n')
			{
				vector2d::fvector v1(window, position.x, position.y, position.x, position.y + ch_height / 2.5f, color, true); v1.draw();
				vector2d::fvector v2(window, position.x + ch_width / 4.0f - 0.1f, position.y, position.x + ch_width / 4.0f - 0.1f, position.y + ch_height / 4.0f, color, true); v2.draw();
				ellipse2d::ellipsef e1(window, position.x + ch_width / 8.0f, position.y + ch_height / 8.0f, ch_width / 8.0f, ch_height / 4.0f, color, 3000, 3600, false, true); e1.draw();
				ellipse2d::ellipsef e2(window, position.x + ch_width / 8.0f, position.y + ch_height / 8.0f, ch_width / 8.0f, ch_height / 4.0f, color, 0, 600, false, true); e2.draw();
				next_pos = new Vector2f(position.x + ch_width / 3.0f - 0.1f, position.y);
			}
			if (ch == 'o')
			{
				ellipse2d::ellipsef e1(window, position.x + ch_width / 5.0f, position.y + ch_height / 5.0f, ch_width / 5.0f, ch_height / 5.0f, color, 0, 3600, false, true); e1.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.5f, position.y);
			}
			if (ch == 'p')
			{
				vector2d::fvector v1(window, position.x, position.y - ch_height / 4.0f, position.x, position.y + ch_height / 2.0f, color, true); v1.draw(); // straight line
				ellipse2d::ellipsef e1(window, position.x, position.y + (ch_height / 4.0f), ch_width / 3.0f, ch_height / 8.0f, color, 0, 1800, false, true); e1.draw(); // bottom ellipse
				next_pos = new Vector2f(position.x + ch_width / 2.0f, position.y);
			}
			if (ch == 'q')
			{
				vector2d::fvector v1(window, position.x + ch_width / 2.0f, position.y - ch_height / 4.0f, position.x + ch_width / 2.0f, position.y + ch_height / 2.0f, color, true); v1.draw(); // straight line
				ellipse2d::ellipsef e1(window, position.x + ch_width / 2.0f, position.y + (ch_height / 4.0f), ch_width / 2.0f, ch_height / 8.0f, color, 1800, 3600, false, true); e1.draw(); // bottom ellipse
				next_pos = new Vector2f(position.x + ch_width / 2.0f, position.y);
			}
			if (ch == 'r')
			{
				vector2d::fvector v1(window, position.x, position.y, position.x, position.y + ch_height / 2.5f, color, true); v1.draw();
				ellipse2d::ellipsef e1(window, position.x + ch_width / 6.0f, position.y + ch_height / 8.0f, ch_width / 6.0f, ch_height / 4.0f, color, 3000, 3600, false, true); e1.draw();
				ellipse2d::ellipsef e2(window, position.x + ch_width / 6.0f, position.y + ch_height / 8.0f, ch_width / 6.0f, ch_height / 4.0f, color, 0, 600, false, true); e2.draw();
				next_pos = new Vector2f(position.x + ch_width / 4.0f, position.y);
			}
			if (ch == 's')
			{
				ellipse2d::ellipsef e1(window, position.x, position.y + ch_height / 12.0f, ch_width / 3.0f, ch_height / 12.0f, color, 300, 1800, false, true); e1.draw(); // bottom ellipse
				ellipse2d::ellipsef e2(window, position.x + ch_width / 3.0f, position.y + ch_height / 3.99f, ch_width / 3.0f, ch_height / 12.0f, color, 2100, 3600, false, true); e2.draw(); // top ellipse
				next_pos = new Vector2f(position.x + ch_width / 3.0f, position.y);
			}
			if (ch == 't')
			{
				float avg = (position.x + ch_width / 4.0f) - position.x;
				vector2d::fvector v1(window, position.x + ch_width / 11.0f, position.y + ch_height / 9.14f, position.x + ch_width / 12.0f, position.y + ch_height / 2.0f, color, true); v1.draw(); // straight line
				vector2d::fvector v2(window, position.x, position.y + ch_height / 4.0f, position.x + ch_width / 4.0f, position.y + ch_height / 3.5f, color, true); v2.draw(); // top line
				ellipse2d::ellipsef e1(window, position.x + avg * 1.5f, position.y + ch_height / 10.0f, ch_width / 4.0f, ch_height / 8.0f, color, 2700, 2100, false, true); e1.draw();
				//vector2d::fvector v3(position.x, position.y + ch_height / 1.5f, position.x + ch_width, position.y + ch_height / 1.5f, color, true); v1.draw(); // middle line
				next_pos = new Vector2f(position.x + ch_width / 4.0f, position.y);
			}
			if (ch == 'u')
			{
				ellipse2d::ellipsef e1(window, position.x + ch_width / 6.0f, position.y + ch_height / 4.8f, ch_width / 6.0f, ch_height / 4.8f, color, 3000, 600, false, true); e1.draw();
				ellipse2d::ellipsef e2(window, position.x + (ch_width / 4.5f) + (ch_width / 1.8f), position.y + ch_height / 4.0f, ch_width / 2.4f, ch_height / 4.0f, color, 2700, 2200, false, true); e2.draw();
				next_pos = new Vector2f(position.x + (ch_width / 4.5f) + (ch_width / 4.5f), position.y);
			}
			if (ch == 'v')
			{
				vector2d::fvector v1(window, position.x, position.y + ch_height / 3.0f, position.x + ch_width / 4.0f, position.y, color, true); v1.draw();
				vector2d::fvector v2(window, v1.transform.position.x2, v1.transform.position.y2, position.x + ch_width / 2.0f, position.y + ch_height / 3.0f, color, true); v2.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.0f, position.y);
			}
			if (ch == 'w')
			{
				vector2d::fvector v1(window, position.x, position.y + ch_height / 3.0f, position.x + ch_width / 8.0f, position.y, color, true); v1.draw();
				vector2d::fvector v2(window, v1.transform.position.x2, v1.transform.position.y2, position.x + ch_width / 4.0f, position.y + ch_height / 3.0f, color, true); v2.draw();
				vector2d::fvector v3(window, v2.transform.position.x2, v2.transform.position.y2, position.x + ch_width / 2.66f, position.y, color, true); v3.draw();
				vector2d::fvector v4(window, v3.transform.position.x2, v3.transform.position.y2, position.x + ch_width / 2.0f, position.y + ch_height / 3.0f, color, true); v4.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.0f, position.y);
			}
			if (ch == 'x')
			{
				vector2d::fvector v1(window, position.x, position.y + ch_height / 3.0f, position.x + ch_width / 2.0f, position.y, color, true); v1.draw();
				vector2d::fvector v2(window, position.x, position.y, position.x + ch_width / 2.0f, position.y + ch_height / 3.0f, color, true); v2.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.0f, position.y);
			}
			if (ch == 'y')
			{
				vector2d::fvector v1(window, position.x, position.y + ch_height / 3.0f, position.x + ch_width / 4.0f, position.y, color, true); v1.draw();
				vector2d::fvector v2(window, v1.transform.position.x2, v1.transform.position.y2, position.x + ch_width / 2.0f, position.y + ch_height / 3.0f, color, true); v2.draw();
				ellipse2d::ellipsef e1(window, position.x + ch_width / 14.0f, position.y + ch_height / 3.0f, ch_width / 4.0f, ch_height / 1.5f, color, 1200, 1800, false, true); e1.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.0f, position.y);
			}
			if (ch == 'z')
			{
				vector2d::fvector v1(window, position.x, position.y + ch_height / 3.0f, position.x + ch_width / 3.0f, position.y + ch_height / 3.0f, color, true); v1.draw();
				vector2d::fvector v2(window, v1.transform.position.x2, v1.transform.position.y2, position.x, position.y, color, true); v2.draw();
				vector2d::fvector v3(window, v2.transform.position.x2, v2.transform.position.y2, position.x + ch_width / 3.0f, position.y, color, true); v3.draw();
				next_pos = new Vector2f(position.x + ch_width / 3.0f, position.y);
			}
			if (ch == '1')
			{
				vector2d::fvector v1(window, position.x, position.y, position.x, position.y + ch_height, color, true); v1.draw();
				vector2d::fvector v2(window, position.x - ch_width / 7.0f, position.y + ch_height / 1.3f, position.x, position.y + ch_height, color, true); v2.draw();
				next_pos = new Vector2f(position.x + ch_width / 7.0f, position.y);
			}
			if (ch == '2')
			{
				ellipse2d::ellipsef e1(window, position.x, position.y + ch_height / 1.9f, ch_width / 2.0f, ch_height / 2.0f, color, 0, 1800, false, true); e1.draw();
				vector2d::fvector v1(window, position.x, position.y, position.x + ch_width / 1.2f, position.y, color, true); v1.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.2f, position.y);
			}
			if (ch == '3')
			{
				ellipse2d::ellipsef e1(window, position.x, position.y + ch_height / 1.3f, ch_width / 2.0f, ch_height / 4.0f, color, 0, 1800, false, true); e1.draw();
				ellipse2d::ellipsef e2(window, position.x, position.y + ch_height / 3.5f, ch_width / 2.0f, ch_height / 4.0f, color, 0, 1800, false, true); e2.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.0f, position.y);
			}
			if (ch == '4')
			{
				vector2d::fvector v1(window, position.x, position.y + ch_height / 2.9f, position.x + ch_width / 1.5f, position.y + ch_height, color, true); v1.draw();
				vector2d::fvector v2(window, position.x, position.y + ch_height / 2.9f, position.x + ch_width / 1.5f, position.y + ch_height / 2.9f, color, true); v2.draw();
				vector2d::fvector v3(window, position.x + ch_width / 1.5f, position.y, position.x + ch_width / 1.5f, position.y + ch_height, color, true); v3.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.3f, position.y);
			}
			if (ch == '5')
			{
				vector2d::fvector v1(window, position.x, position.y + ch_height / 3.5f + ch_height / 4.0f, position.x, position.y + ch_height, color, true); v1.draw();
				vector2d::fvector v2(window, position.x, position.y + ch_height, position.x + ch_width / 2.0f, position.y + ch_height, color, true); v2.draw();
				ellipse2d::ellipsef e1(window, position.x, position.y + ch_height / 3.5f, ch_width / 2.0f, ch_height / 4.0f, color, 0, 1800, false, true); e1.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.0f, position.y);
			}
			if (ch == '6')
			{
				ellipse2d::ellipsef e1(window, position.x + ch_width / 4.0f, position.y + ch_height / 4.0f, ch_width / 4.0f, ch_height / 3.5f, color, 0, 3600, false, true); e1.draw();
				ellipse2d::ellipsef e2(window, position.x + ch_width / 4.0f, position.y + ch_height / 3.0f, ch_width / 4.0f, ch_height / 1.5f, color, 0, 400, false, true); e2.draw();
				ellipse2d::ellipsef e3(window, position.x + ch_width / 4.0f, position.y + ch_height / 3.0f, ch_width / 4.0f, ch_height / 1.5f, color, 2700, 3600, false, true); e3.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.5f, position.y);
			}
			if (ch == '7')
			{
				vector2d::fvector v1(window, position.x, position.y + ch_height, position.x + ch_width / 2.0f, position.y + ch_height, color, true); v1.draw();
				vector2d::fvector v2(window, position.x, position.y, position.x + ch_width / 2.0f, position.y + ch_height, color, true); v2.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.0f, position.y);
			}
			if (ch == '8')
			{
				ellipse2d::ellipsef e1(window, position.x + ch_width / 4.0f, position.y + ch_height / 1.3f, ch_width / 3.75f, ch_height / 3.75f, color, 0, 3600, false, true); e1.draw();
				ellipse2d::ellipsef e2(window, position.x + ch_width / 4.0f, position.y + ch_height / 4.0f, ch_width / 3.75f, ch_height / 3.75f, color, 0, 3600, false, true); e2.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.0f, position.y);
			}
			if (ch == '9')
			{
				ellipse2d::ellipsef e1(window, position.x + ch_width / 4.0f, position.y + ch_height / 1.3f, ch_width / 3.5f, ch_height / 3.5f, color, 0, 3600, false, true); e1.draw();
				ellipse2d::ellipsef e2(window, position.x + ch_width / 4.0f, position.y + ch_height / 1.5f, ch_width / 3.5f, ch_height / 1.5f, color, 900, 1800, false, true); e2.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.0f, position.y);
			}
			if (ch == '0')
			{
				ellipse2d::ellipsef e1(window, position.x + ch_width / 5.0f, position.y + ch_height / 2.0f, ch_width / 3.0f, ch_height / 2.0f, color, 0, 3600, false, true); e1.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.1f, position.y);
			}
			if (ch == '.')
			{
				ellipse2d::ellipsef e1(window, position.x + ch_width, position.y, ch_width / 10.0f, ch_height / 10.0f, color, 0, 3600, true, true); e1.draw();
				next_pos = new Vector2f(position.x + ch_width / 24.0f, position.y);
			}
			if (ch == '-')
			{
				vector2d::fvector v1(window, position.x, position.y + ch_height / 2.0f, position.x + ch_width / 1.5f, position.y + ch_height / 2.0f, color, true); v1.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.3f, position.y);
			}
			if (ch == '+')
			{
				vector2d::fvector v1(window, position.x, position.y + ch_height / 2.0f, position.x + ch_width / 1.5f, position.y + ch_height / 2.0f, color, true); v1.draw();
				vector2d::fvector v2(window, position.x + ch_width / 2.0f, position.y + ch_height, position.x + ch_width / 2.0f, position.y, color, true); v2.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.3f, position.y);
			}
			if (ch == '=')
			{
				vector2d::fvector v1(window, position.x, position.y + ch_height / 1.5f, position.x + ch_width / 1.5f, position.y + ch_height / 1.5f, color, true); v1.draw();
				vector2d::fvector v2(window, position.x, position.y + ch_height / 3.0f, position.x + ch_width / 1.5f, position.y + ch_height / 3.0f, color, true); v2.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.4f, position.y);
			}
			if (ch == '!')
			{
				draw_pixel(position.x, position.y, color, window);
				vector2d::fvector v1(window, position.x, position.y + ch_height / 4.0f, position.x, position.y + ch_height, color, true); v1.draw();
				next_pos = new Vector2f(position.x + 0.1f, position.y);
			}
			if (ch == '<')
			{
				vector2d::fvector v1(window, position.x, position.y + ch_height / 2.0f, position.x + ch_width / 1.5f, position.y + ch_height, color, true); v1.draw();
				vector2d::fvector v2(window, position.x, position.y + ch_height / 2.0f, position.x + ch_width / 1.5f, position.y, color, true); v2.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.4f, position.y);
			}
			if (ch == '>')
			{
				vector2d::fvector v1(window, position.x, position.y + ch_height, position.x + ch_width / 1.5f, position.y + ch_height / 2.0f, color, true); v1.draw();
				vector2d::fvector v2(window, position.x, position.y, position.x + ch_width / 1.5f, position.y + ch_height / 2.0f, color, true); v2.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.4f, position.y);
			}
			if (ch == ',')
			{
				ellipse2d::ellipsef e1(window, position.x - ch_width / 36.0f, position.y - ch_width / 30.0f, ch_width / 24.0f, ch_height / 4.0f, color, 0, 2100, false, true); e1.draw();
				next_pos = new Vector2f(position.x + ch_width / 24.0f, position.y);
			}
			if (ch == '?')
			{
				draw_pixel(position.x, position.y, color, window);
				vector2d::fvector v1(window, position.x, position.y + ch_height / 3.5f, position.x, position.y + ch_height / 2.0f, color, true); v1.draw();
				ellipse2d::ellipsef e1(window, position.x, position.y + ch_height / 1.33f, ch_width / 4.0f, ch_height / 4.0f, color, 0, 1800, false, true); e1.draw();
				ellipse2d::ellipsef e2(window, position.x, position.y + ch_height / 1.33f, ch_width / 4.0f, ch_height / 4.0f, color, 3300, 3600, false, true); e2.draw();
				next_pos = new Vector2f(position.x + ch_width / 4.0f, position.y);
			}
			if (ch == '/')
			{
				vector2d::fvector v1(window, position.x, position.y, position.x + ch_width / 2.0f, position.y + ch_height, color, true); v1.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.0f, position.y);
			}
			if (ch == '|')
			{
				vector2d::fvector v1(window, position.x, position.y, position.x, position.y + ch_height, color, true); v1.draw();
				next_pos = new Vector2f(position.x + 0.1f, position.y);
			}
			if (ch == '(')
			{
				ellipse2d::ellipsef e1(window, position.x + ch_width / 4.0f, position.y + ch_height / 2.0f, ch_width / 4.0f, ch_height / 2.0f, color, 1800, 3600, false, true); e1.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.0f, position.y);
			}
			if (ch == ')')
			{
				ellipse2d::ellipsef e1(window, position.x, position.y + ch_height / 2.0f, ch_width / 4.0f, ch_height / 2.0f, color, 0, 1800, false, true); e1.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.0f, position.y);
			}
			if (ch == '_')
			{
				vector2d::fvector v1(window, position.x, position.y, position.x + ch_width / 1.5f, position.y, color, true); v1.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.5f, position.y);
			}
			if (ch == ':')
			{
				ellipse2d::ellipsef e1(window, position.x, position.y + ch_height / 1.7f, ch_width / 32.0f, ch_height / 32.0f, color, 0, 3600, true, true); e1.draw();
				ellipse2d::ellipsef e2(window, position.x, position.y + ch_height / 7.0f, ch_width / 32.0f, ch_height / 32.0f, color, 0, 3600, true, true); e2.draw();
				next_pos = new Vector2f(position.x + ch_width / 32.0f, position.y);
			}
			if (ch == '[')
			{
				vector2d::fvector v1(window, position.x, position.y, position.x, position.y + ch_height, color, true); v1.draw();
				vector2d::fvector v2(window, position.x, position.y + ch_height, position.x + ch_width / 3.0f, position.y + ch_height, color, true); v2.draw();
				vector2d::fvector v3(window, position.x, position.y, position.x + ch_width / 3.0f, position.y, color, true); v3.draw();
				next_pos = new Vector2f(position.x + ch_width / 3.0f, position.y);
			}
			if (ch == ']')
			{
				vector2d::fvector v1(window, position.x + ch_width / 3.0f, position.y, position.x + ch_width / 3.0f, position.y + ch_height, color, true); v1.draw();
				vector2d::fvector v2(window, position.x, position.y + ch_height, position.x + ch_width / 3.0f, position.y + ch_height, color, true); v2.draw();
				vector2d::fvector v3(window, position.x, position.y, position.x + ch_width / 3.0f, position.y, color, true); v3.draw();
				next_pos = new Vector2f(position.x + ch_width / 3.0f, position.y);
			}
			ch_width = temp_ch_width;
			ch_height = temp_ch_height;
		}
	public:
		const char* content = "";
		float x = 0.0f;
		float y = 0.0f;
		float ch_spacing = 0.0f;
		float ch_width = 0.0f;
		float ch_height = 0.0f;
		float space_width = 0.0f;
		uint32 color = colors.red;
		bool visible = true;
		WINDOW window;
		text(WINDOW win, const char* txt, float x_, float y_, float ch_width_, float ch_height_, float ch_space_, uint32 color_, bool visible_ = true)
		{
			window = win;
			x = x_; y = y_;
			content = txt;
			ch_spacing = ch_space_;
			ch_width = ch_width_;
			ch_height = ch_height_;
			space_width = ch_width / 3.0f;
			color = color_;
			visible = visible_;
			next_pos = new Vector2f(x_, y_);
		}
		text(WINDOW win, const char* txt, Vector2f pos_, float ch_width_, float ch_height_, float ch_space_, uint32 color_, bool visible_ = true)
		{
			window = win;
			x = pos_.x;
			y = pos_.y;
			content = txt;
			ch_spacing = ch_space_;
			ch_width = ch_width_;
			ch_height = ch_height_;
			space_width = ch_width / 3.0f;
			color = color_;
			visible = visible_;
			next_pos = new Vector2f(pos_.x, pos_.y);
		}
		//think about default constructor for text
		text()
		{

		}
		~text()
		{

		}
		//should be private void
		inline void draw()
		{
			if (visible)
			{
				//draw text
				Vector2f* offset_pos;
				for (int i = 0; i < get_text_last_index(content); i++)
				{
					if (i > 0) // because it automatically aadds ch_spacing to the first letter
					{
						offset_pos = new Vector2f((next_pos->x + ch_spacing), y);
					}
					else
					{
						offset_pos = new Vector2f(x, y);
					}
					draw_char(content[i], *offset_pos);
				}
			}
		}
	};
}
class Texture
{
public:
	const char* path = "";
	bool visible = true;
	int texture_width = 0;
	int texture_height = 0;
	int rgb_channels = 3;
	float opacity = 255.0f;
	Texture(const char* path_)
	{
		path = path_;
		visible = true;
		stbi_load(path, &texture_width, &texture_height, &rgb_channels, 0);
	}
	Texture()
	{

	}
};
class Sprite
{
private:
	inline uint32 ProcessImageColor(uint32& r1, uint32& g1, uint32& b1)
	{
		uint32 col = 0;
		if (redfilter == false && greenfilter == false && bluefilter == false && grayscalefilter == false)
		{
			col = rgba(r1, g1, b1, opacity);
		}
		if (grayscalefilter == true)
		{
			col = rgba((r1 + b1 + g1) / 3, (r1 + b1 + g1) / 3, (r1 + b1 + g1) / 3, opacity);
		}
		if (redfilter == true)
		{
			r1 += filterIntensity_red;
			col = rgba(r1, g1, b1, opacity);
		}
		if (greenfilter == true)
		{
			g1 += filterIntensity_green;
			col = rgba(r1, g1, b1, opacity);
		}
		if (bluefilter == true)
		{
			b1 += filterIntensity_blue;
			col = rgba(r1, g1, b1, opacity);
		}
		return col;
	}
public:
	const char* texture_path = "";
	float x = 0.0f;
	float y = 0.0f;
	float area_ = 0.0f;
	bool visible = true;
	int texture_original_width = 0;
	int texture_original_height = 0;
	float texture_width = 0.0f;
	float texture_height = 0.0f;
	float opacity = 255.0f;
	bool redfilter = false;
	bool greenfilter = false;
	bool bluefilter = false;
	bool grayscalefilter = false;
	uint32 filterIntensity_red = 50;
	uint32 filterIntensity_green = 50;
	uint32 filterIntensity_blue = 50;
	int rgb_channels = 3;
	bool rotate_bool = false;
	float angle = 0.0f;
	pivot rotation_pivot = center;
	rotation_direction_ rotation_direction = clockwise;
	WINDOW window;
	Sprite(WINDOW win, Texture& texture_, float x_, float y_, float img_width_, float img_height_, bool visible_)
	{
		window = win;
		texture_path = texture_.path;
		opacity = texture_.opacity;
		x = x_;
		y = y_;
		texture_width = img_width_;
		texture_height = img_height_;
		visible = visible_;
	}
	Sprite(WINDOW win, Texture& texture_, Vector2f& pos_, float img_width_, float img_height_, bool visible_)
	{
		window = win;
		texture_path = texture_.path;
		opacity = texture_.opacity;
		x = pos_.x;
		y = pos_.y;
		texture_width = img_width_;
		texture_height = img_height_;
		visible = visible_;
	}
	Sprite(WINDOW win, const char* path_, float x_, float y_, float img_width_, float img_height_, bool visible_)
	{
		window = win;
		texture_path = path_;
		x = x_;
		y = y_;
		texture_width = img_width_;
		texture_height = img_height_;
		visible = visible_;
	}
	Sprite(WINDOW win, const char* path_, Vector2f& pos_, float img_width_, float img_height_, bool visible_)
	{
		window = win;
		texture_path = path_;
		x = pos_.x;
		y = pos_.y;
		texture_width = img_width_;
		texture_height = img_height_;
		visible = visible_;
	}
	Sprite()
	{

	}
	inline void Rotate(float angle_, pivot rotation_pivot_, rotation_direction_ rotation_direction__)
	{
		/*How Rotate function works?
		All Rotate() functions work by using rotate_point() function with the inputted angle and setting pixels to new positions outputted from the rotate_point() function.
		rotate_point() function rotates pixel around inputted origin's x and y.*/
		angle = angle_;
		rotation_pivot = rotation_pivot_;
		rotation_direction = rotation_direction__;
		rotate_bool = true;
	}
	inline void draw_sprite()
	{
		if (visible)
		{
			/*Resizing Algorithm:
			Algorithm works by decreasing or increasing distance between pixels depending on texture_width and texture_height.
			If texture_width and texture_height are 0.5 then distance between pixels will be 1*0.5 = 0.5. Consequently, image becomes smaller in size.
			But when expanding image's size algorithm draws additional pixels to fill empty pixels. For example, texture_width = 2 and texture_height = 2,
			then distance between pixels will be 1*2=2, and one pixel will be missing every time pixel is built. To solve this issue algorithm draws additional pixel every time pixel is built,
			distance from previous pixel equals to texture_width-PIXEL_SIZE_MAX and same with y. */
			unsigned char* data = stbi_load(texture_path, &texture_original_width, &texture_original_height, &rgb_channels, 0);
			int ipx = 0;
			float y_ = y + (texture_original_height / 2.0f) * PIXEL_SIZE_MAX * texture_height; // to achieve the center pivot of the picture
			float x_ = x - (texture_original_width / 2.0f) * PIXEL_SIZE_MAX * texture_width; // to achieve the center pivot of the picture
			float angle_ = angle;
			float rot_x = x;
			float rot_y = y;
			if (rotation_direction == clockwise)
			{
				angle_ = -1.0f * angle_;
			}
			if (rotation_pivot == startpos)
			{
				rot_x = x - (texture_original_width / 2.0f) * texture_width;
				rot_y = y - (texture_original_height / 2.0f) * texture_height;
			}
			else if (rotation_pivot == endpos)
			{
				rot_x = x + (texture_original_width / 2.0f) * texture_width;
				rot_y = y + (texture_original_height / 2.0f) * texture_height;
			}
			bool image_expand_error = false;
			float image_expand_error_fx_x = 0.0f;
			float image_expand_error_fx_y = 0.0f;
			if (texture_width - PIXEL_SIZE_MAX > 0.0f)
			{
				image_expand_error = true;
				image_expand_error_fx_x = texture_width - PIXEL_SIZE_MAX;
			}
			if (texture_height - PIXEL_SIZE_MAX > 0.0f)
			{
				image_expand_error = true;
				image_expand_error_fx_y = texture_height - PIXEL_SIZE_MAX;
			}
			area_ = texture_original_height * texture_original_width;
			if (data != nullptr && texture_original_height > 0 && texture_original_width > 0)
			{
				for (int i = 0; i < texture_original_height; i++)
				{
					for (int j = 0; j < texture_original_width; j++)
					{
						unsigned int r1 = static_cast<unsigned int>(data[ipx]);
						unsigned int g1 = static_cast<unsigned int>(data[ipx + 1]);
						unsigned int b1 = static_cast<unsigned int>(data[ipx + 2]);
						uint32 col = ProcessImageColor(r1, g1, b1);
						if (!rotate_bool)
						{
							draw_pixel(x_ + (j * PIXEL_SIZE_MAX * texture_width), (y_ - i * PIXEL_SIZE_MAX * texture_height), col, window);
							if (image_expand_error)
							{
								draw_pixel(x_ + (j * PIXEL_SIZE_MAX * texture_width) + image_expand_error_fx_x, (y_ - i * PIXEL_SIZE_MAX * texture_height) + image_expand_error_fx_y, col, window);
							}
						}
						else if (rotate_bool)
						{
							Vector2f_r p1 = rotate_point(x_ + (j * PIXEL_SIZE_MAX * texture_width), (y_ - i * PIXEL_SIZE_MAX * texture_height), rot_x, rot_y, angle_);
							draw_pixel(p1.x, p1.y, col, window);
							if (image_expand_error)
							{
								Vector2f_r p1 = rotate_point(x_ + (j * PIXEL_SIZE_MAX * texture_width) + image_expand_error_fx_x, (y_ - i * PIXEL_SIZE_MAX * texture_height) + image_expand_error_fx_y, rot_x, rot_y, angle_);
								draw_pixel(p1.x, p1.y, col, window);
							}
						}
						ipx += 3;
						//std::cout << index << " pixel: RGB " << static_cast<int>(data[index]) << " " << static_cast<int>(data[index + 1]) << " " << static_cast<int>(data[index + 2]) << std::endl;
					}
				}
			}
			stbi_image_free(data);
		}
	}
};
namespace graphs
{
	class function
	{
	public:
		float y = 0.0f;
		float m = 0.0f;
		float x = 0.0f;
		float c = 0.0f;
		const char* content = "";
		uint32 color = 0;
		bool visible = false;
		FunctionType functionType = no_function;
		WINDOW window;
		function(WINDOW window_, const char* f_equation_, uint32 color_, bool visible_)
		{
			window = window_;
			content = f_equation_;
			color = color_;
			visible = visible_;
			GetEquationVariables();
		}
		function(WINDOW window_, float m_, float c_, uint32 color_, bool visible_)
		{
			window = window_;
			m = m_;
			c = c_;
			color = color_;
			visible = visible_;
		}
		function()
		{

		}
		inline void GetEquationVariables()
		{
			int equal_sign_n = 0;
			int x_sign_n = 0;
			int c_sign_n = 0;
			bool function_type[4] = 
			{ 
				false, // if function has x(made for straight lines)
				false, // if function has c
				false, // if funciton has power sign (^)
				false // if function has division sign (made for hyperbola)
			};

			//y=mx+c
			for (int i = 0; i < MAX_FUNCTION_SIZE; i++)
			{
				if (content[i] == '=')
				{
					equal_sign_n = i;
					break;
				}
				else
				{
					//exception
				}
			}
			if (equal_sign_n == 1)
			{
				//finding m
				for (int z = 0; z < MAX_FUNCTION_SIZE; z++)
				{
					if (content[z] == 'x')
					{
						function_type[0] = true;
						x_sign_n = z;
						bool negative_m = false;
						int diff_m = x_sign_n - (equal_sign_n + 1);
						char m_ch[30] = "";
						bool rational_m = false;
						int dot_diff = 0;
						//int mq = get_quantity_of_digits(m, false) + get_quantity_of_digits(m, true);
						for (int u = equal_sign_n + 1; u < equal_sign_n + 1 + diff_m; u++)
						{
							if (content[u] != '-' && content[u] != '.')
							{
								const char* proceeded_int_ch = proceed_int_ch(content[u]);
								append_chars(m_ch, proceeded_int_ch);
							}
							else if (content[u] == '.')
							{
								rational_m = true;
								dot_diff = z - u;
							}
							else if (content[u] == '-')
							{
								negative_m = true;
							}
						}
						m = cnstchtoint(m_ch);
						if (negative_m)
						{
							m *= -1.0f;
						}
						if (rational_m)
						{
							m /= pow(10, dot_diff - 1);
						}
						break;
					}
					else
					{
						//exception
					}
				}
				//finding c
				if (function_type[0] == true)
				{
					for (int z = 0; z < MAX_FUNCTION_SIZE; z++)
					{
						if ((content[z] == '+' || content[z] == '-') && content[z - 1] == 'x')
						{
							function_type[1] = true;
							c_sign_n = z;
							int diff_c = (get_text_last_index(content) - 1) - z;
							char c_ch[30] = "";
							int cq = get_quantity_of_digits(c, false) + get_quantity_of_digits(c, true);
							bool rational_c = false;
							int dot_diff = 0;
							bool negative_c = false;
							//int mq = get_quantity_of_digits(m, false) + get_quantity_of_digits(m, true);
							for (int u = z; u < z + 1 + diff_c; u++)
							{
								if (content[u] != '-' && content[u] != '.' && content[u] != '+')
								{
									const char* proceeded_int_ch = proceed_int_ch(content[u]);
									append_chars(c_ch, proceeded_int_ch);
								}
								else if (content[u] == '.')
								{
									rational_c = true;
									dot_diff = z - u;
								}
								else if (content[u] == '-')
								{
									negative_c = true;
								}
							}
							c = cnstchtoint(c_ch);
							if (negative_c)
							{
								c *= -1.0f;
							}
							if (rational_c)
							{
								c /= pow(10, dot_diff - 1);
							}
							break;
						}
						else
						{
							//exception
						}
					}
				}
				else if (function_type[0] == false)
				{
					int d = get_text_last_index(content);
					bool negative_c = false;
					char c_ch[30] = "";
					for (int i = equal_sign_n + 1; i < d; i++)
					{
						if (content[i] != '-')
						{
							negative_c = true;
						}
						const char* proceeded_int_ch = proceed_int_ch(content[i]);
						append_chars(c_ch, proceeded_int_ch);
					}
					c = cnstchtoint(c_ch);
					if (negative_c)
					{
						c *= -1.0f;
					}
				}
			}
			else
			{
				//simplify equation
			}
			if (function_type[0] && !function_type[1] && !function_type[2] && !function_type[3])
			{
				functionType = proportion;
			}
			else if (!function_type[0] && !function_type[1] && !function_type[2] && !function_type[3])
			{
				functionType = straight;
			}
			else if (function_type[0] && function_type[1] && !function_type[2] && !function_type[3])
			{
				functionType = linear;
			}
		}
		inline const char* proceed_int_ch(char n)
		{
			const char* int_cnst_chars[10] =
			{
				"0",
				"1",
				"2",
				"3",
				"4",
				"5",
				"6",
				"7",
				"8",
				"9",
			};
			char int_chars[10] =
			{
				'0',
				'1',
				'2',
				'3',
				'4',
				'5',
				'6',
				'7',
				'8',
				'9',
			};
			for (int i = 0; i < 10; i++)
			{
				if (n == int_chars[i])
				{
					return int_cnst_chars[i];
				}
			}
		}
	    inline void draw()
		{
			if (visible)
			{
				if (functionType == linear)
				{
					float xp = 0.0f;
					float yp = 0.0f;
					float xn = 0.0f;
					float yn = 0.0f;
					//vector2d::fvector v1(window, _win.win_width_px[window.wnd_param] / 2, y_max - 50, x_when_y_equals_max + _win.win_width_px[window.wnd_param] / 2, y_max - 50, color, true); v1.draw();
					//First for-loop draws only positive part of the line and second for-loop draws only negative part not including c
					m += 0.15f; // fixing an interesting bug.
					for (float i = 0.0f; i < 1000.0f; i += 0.1f)
					{
						xp = i;
						yp = m * xp;
						draw_pixel(xp+_win.win_width_px[window.wnd_param]/2, yp+ _win.win_height_px[window.wnd_param]/2+c, color, window);
						if (xp+ _win.win_width_px[window.wnd_param]/2 >= _win.win_width_px[window.wnd_param] || xp + _win.win_width_px[window.wnd_param] / 2 < 0)
						{
							break;
						}
						if (yp+ _win.win_height_px[window.wnd_param]/2 + c >= _win.win_height_px[window.wnd_param] || yp + _win.win_height_px[window.wnd_param] / 2 + c < 0)
						{
							break;
						}
					}
					for (float d = 0.0f; d > -1000.0f; d -= 0.1f)
					{
						xn = d;
						yn = m * xn;
						draw_pixel(xn + _win.win_width_px[window.wnd_param] / 2, yn + _win.win_height_px[window.wnd_param] / 2+c, color, window);
						if (xn + _win.win_width_px[window.wnd_param] / 2 >= _win.win_width_px[window.wnd_param] || xn + _win.win_width_px[window.wnd_param] / 2 < 0)
						{
							break;
						}
						if (yn + _win.win_height_px[window.wnd_param] / 2 + c>= _win.win_height_px[window.wnd_param] || yn + _win.win_height_px[window.wnd_param] / 2 + c < 0)
						{
							break;
						}
					}
					//if ((m > 1.0f && m > 0.0f) || (m < -1.0f && m < 0.0f))
					//{
					//	m += 0.15f;
					//	float x1 = 0.0f;
					//	for (float i = 0; i < 1000; i += 0.1f)
					//	{
					//		if (fabs(m) * i >= _win.win_height_px[window.wnd_param])
					//		{
					//			x1 = i;
					//			break;
					//		}
					//	}
					//	for (float k = 0; k < 1000; k += 0.1f)
					//	{
					//		float y_ = fabs(m) * k;
					//		float y_max = 0.0f;
					//		if (m * k >= _win.win_height_px[window.wnd_param])
					//		{
					//			break;
					//		}
					//		float r = k - ((x1) / 2.0f);
					//		float q = 0.0f;
					//		q = (_win.win_width_px[window.wnd_param] / 2) + r - c; // do not know why - c
					//		draw_pixel(q, y_, colors.white, window);
					//		//So, to make line intersect exactly with the origin algorithm calculates (the maximum x 2 and divides it by 2) + (window width / 2) to get the appropriate x and y of each pixel. 
					//		//Then, actually, when modifiying y-intercept in linear function it seems as you just alter x inversed. For instance, if c > 0 we decrease x by c and vice versa, it is easy and could be proved by any graph calculator.
					//		//And thus, algorithm works perfectly!
					//	}
					//}
					//if ((m < 1.0f && m > 0.0f) || (m < 0.0f && m > -1.0f))
					//{
					//	float i = 1.0f / fabs(m);
					//	float j = _win.win_height_px[window.wnd_param] / 2;
					//	float j_ = j * (1 - m);
					//	float j_diff_ = j - j_;
					//	vector2d::fvector v1(window, 0.0f, j_ + c, _win.win_width_px[window.wnd_param], _win.win_height_px[window.wnd_param] / 2 + j_diff_ + c, color, true);
					//	v1.draw();
					//}
				}
				if (functionType == straight)
				{
					int u = 0;
				}
			}
		}
	};
}
