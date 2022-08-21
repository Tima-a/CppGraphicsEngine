typedef unsigned int uint32;
static double render_scale = 0.001f;
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
static enum TextureFilter { neutral, grayscale, redfilter, bluefilter, greenfilter };
static enum pivot { center, startpos, endpos };
//Set window screen color
inline static void update_screen(uint32 color) // processing screen visibility. This function is done to place update_screen function to the bottom of the code.
{
	screen.update_screen = true;
	screen.scr_refresh_color = color;
}
inline static void refresh_screen(uint32 color)
{
	if (screen.update_screen)
	{
		uint32* pixel = (uint32*)render.memory;
		for (int y = 0; y < render.height; y++)
		{
			for (int x = 0; x < render.width; x++)
			{
				*pixel++ = screen.scr_refresh_color;
			}
		}
		screen.update_screen = false;
	}
}
//Draw pixel on x, y positions
inline static void draw_pixel(float x, float y, uint32 color)
{
	float x_pixel_size = PIXEL_SIZE_DEF * render.height * render_scale;
	float y_pixel_size = PIXEL_SIZE_DEF * render_scale * render_scale;
	x *= render.height * render_scale;
	y *= render.height * render_scale;
	x += (float)render.width / 2.0f;
	y += (float)render.height / 2.0f;

	// Change to pixels
	int x0 = (int)x - (int)x_pixel_size;
	int x1 = (int)x + (int)x_pixel_size;
	int y0 = (int)y - (int)y_pixel_size;
	int y1 = (int)y + (int)y_pixel_size;

	x0 = clamp(0, x0, render.width);
	x1 = clamp(0, x1, render.width);
	y0 = clamp(0, y0, render.height);
	y1 = clamp(0, y1, render.height);

	uint32* pixel = (uint32*)render.memory + x0 + y0 * render.width;
	*pixel++ = color;
}
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
namespace rectangle
{
	class RECT
	{
	public:
		float x = 0.0f;
		float y = 0.0f;
		float a = 0.0f;
		float b = 0.0f;
		uint32 color = colors.red;
		bool visible = true;
		//Draw rectangle
		RECT(float x_, float y_, float a_, float b_, uint32 color_, bool visible_ = true)
		{
			x = x_;
			y = y_;
			a = a_;
			b = b_;
			color = color_;
			if (visible_)
			{
				visible = true;
				draw_rect(x_, y_, a_, b_, color_);
			}
		}
		RECT(Vector2f& pos_, float a_, float b_, uint32 color_, bool visible_ = true)
		{
			x = pos_.x;
			y = pos_.y;
			a = a_;
			b = b_;
			color = color_;
			if (visible_)
			{
				visible = true;
				draw_rect(pos_.x, pos_.y, a_, b_, color_);
			}
		}
		RECT()
		{

		}
		~RECT()
		{

		}
		//Calculate rectangle's perimeter
		float perimeter()
		{
			return (2.0f * a) + (2.0f * b);
		}
		//Calculate rectangle's area
		float area()
		{
			return x * y;
		}
		//Draw function
		inline void draw()
		{
			draw_rect(x, y, a, b, color);
		}
	private:
		inline void draw_rect(float x, float y, float half_size_x, float half_size_y, uint32 color)
		{
			x *= render.height * render_scale;
			y *= render.height * render_scale;
			half_size_x *= render.height * render_scale;
			half_size_y *= render.height * render_scale;

			x += (float)render.width / 2.0f;
			y += (float)render.height / 2.0f;

			// Change to pixels
			int x0 = (int)x - (int)half_size_x;
			int x1 = (int)x + (int)half_size_x;
			int y0 = (int)y - (int)half_size_y;
			int y1 = (int)y + (int)half_size_y;

			x0 = clamp(0, x0, render.width);
			x1 = clamp(0, x1, render.width);
			y0 = clamp(0, y0, render.height);
			y1 = clamp(0, y1, render.height);
			for (int y = y0; y < y1; y++)
			{
				uint32* pixel = (uint32*)render.memory + x0 + y * render.width;
				for (int x = x0; x < x1; x++)
				{
					*pixel++ = color;
				}
			}
		}
	};
}
namespace ellipse2d
{
	class ellipsef
	{
	private:
		inline float fast_trunc_ellipse_function(float num)
		{
			return int(num * 10.0f) / 10.0f;
		}
		inline void draw_ellipse(float x, float y, float ra, float rb, uint32 color, bool filled)
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
							draw_pixel(x2 + x, y2 + y, color);
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
										draw_pixel((fx2 - x) - q, y + y2, color);
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
										draw_pixel((x - fx2) + q, y + y2, color);
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
										draw_pixel((fx2 + x) - q, y + y2, color);
									}
									//vector2d::fvector v1(x + fx2, y + y2, x, y + y2, color, true); v1.draw();
								}
							}
						}
						else
						{
							if (visible)
							{
								draw_pixel(x2 + x, y2 + y, color);
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
							draw_pixel(x2 + x, y2 + y, color);
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
										draw_pixel((fx2 - x) - q, y + y2, color);
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
										draw_pixel((x - fx2) + q, y + y2, color);
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
										draw_pixel((fx2 + x) - q, y + y2, color);
									}
									//vector2d::fvector v1(x + fx2, y + y2, x, y + y2, color, true); v1.draw();
								}
							}
						}
						else
						{
							if (visible)
							{
								draw_pixel(x2 + x, y2 + y, color);
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
		bool filled = false;
		uint32 color = colors.red;
		int px_circum_ = 0;
		int px_area = 0;
		int start_circum = 0;
		int end_circum = 3600;
		ellipsef(float x_, float y_, float radiusa_, float radiusb_, uint32 color_, int start_circum_, int end_circum_, bool filled_, bool visible_ = true)
		{
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
		ellipsef()
		{

		}
		~ellipsef()
		{

		}
		//Calculate circle's size
		int circumference_px()
		{
			return 3600;
		}
		int area_px()
		{
			return px_area;
		}
		inline void draw()
		{
			draw_ellipse(x, y, radiusa, radiusb, color, filled);
		}
	};
	class ellipsed
	{
	private:
		inline float fast_trunc_ellipse_function(float num)
		{
			return int(num * 10.0f) / 10.0f;
		}
		inline void draw_ellipse(float x, float y, float ra, float rb, uint32 color, bool filled)
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
					//if (i <= 1800)
					//{
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
							draw_pixel(x2 + x, y2 + y, color);
						}
						matrix_pixels_pos[i].x = x2 + x;
						matrix_pixels_pos[i].y = y2 + y;
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
										draw_pixel((fx2 - x) - q, y + y2, color);
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
										draw_pixel((x - fx2) + q, y + y2, color);
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
										draw_pixel((fx2 + x) - q, y + y2, color);
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
								draw_pixel(x2 + x, y2 + y, color);
							}
							matrix_pixels_pos[i].x = x2 + x;
							matrix_pixels_pos[i].y = y2 + y;
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
							draw_pixel(x2 + x, y2 + y, color);
						}
						matrix_pixels_pos[i].x = x2 + x;
						matrix_pixels_pos[i].y = y2 + y;
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
										draw_pixel((fx2 - x) - q, y + y2, color);
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
										draw_pixel((x - fx2) + q, y + y2, color);
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
										draw_pixel((fx2 + x) - q, y + y2, color);
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
								draw_pixel(x2 + x, y2 + y, color);
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
		bool get_ellipse_pixel_matrix = false;
		uint32 color = colors.red;
		int px_circum_ = 0;
		int px_area = 0;
		int start_circum = 0;
		int end_circum = 3600;
		ellipsed(float x_, float y_, float radiusa_, float radiusb_, uint32 color_, int start_circum_, int end_circum_, bool filled_, bool visible_ = true)
		{
			x = x_;
			y = y_;
			radiusa = radiusa_;
			radiusb = radiusb_;
			filled = filled_;
			color = color_;
			start_circum = start_circum_;
			end_circum = end_circum_;
			get_ellipse_pixel_matrix = false;
			visible = visible_;
		}
		ellipsed()
		{

		}
		~ellipsed()
		{

		}
		//Calculate circle's size
		int circumference_px()
		{
			return 3600;
		}
		int area_px()
		{
			return px_area;
		}
		inline void draw()
		{
			draw_ellipse(x, y, radiusa, radiusb, color, filled);
		}
	};
}
namespace vector2d
{
	class fvector
	{
	private:
		inline void get_rotation_radius()
		{
			//Compute angle
			/*Firstly, algorithm works by drawing an ellipse with |x2-x1| and |y2-y1| radiuses. Then algorithm finds in ellipse's pixel matrix vertex with same x as x2(last not changed vector's pixel.x). Followingly,
			When algorithm found that vertex, it substracts vertex's y from |y2-y1| to get the difference between them. Then, it uplifts the |y2-y1| by the difference and then it checks if new ellipse's pixel
			matrix x and y are equal to x2 and y2 of new vector's x2 and y2.*/
			float ra = fabs(x2 - x1);
			float rb = fabs(y2 - y1);
			float temp_y2 = 0.0f;
			float diff = 0.0f;
			ellipse2d::ellipsed e1(x1, y1, ra * 2.0f, rb, color, 0, 3600, false, false); e1.draw();
			int q = 0;
			for (int k = 0; k < 3600; k++)
			{
				if (floor(e1.matrix_pixels_pos[k].x) == floor(x2 - 1.0f))
				{
					q = k;
					temp_y2 = e1.matrix_pixels_pos[k].y;
					diff = y2 - temp_y2;
					break;
				}
			}
			ellipse2d::ellipsed e2(x1, y1, ra * 2.0f, y2 + diff, color, 0, 3600, false, false); e2.draw();
			for (int i = 0; i < 3600; i++)
			{
				if (e2.matrix_pixels_pos[i].x == x2 && e2.matrix_pixels_pos[i].y == y2)
				{
					q = i;
					break;
				}
			}
			angle = (float)q / 10.0f;
		}
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
				draw_pixel(x2, y2, color);
				draw_one_pixel = true;
			}
			float adx = fabs(x2 - x1);
			float ady = fabs(y2 - y1);
			float slope_x_modf = 0.0f, slope_y_modf = 0.0f;
			if (highlight_vertexes)
			{
				for (int i = 0; i < 5; i++)
				{
					draw_pixel(x1, y1 + i * 10.0f, highlightColor);
					draw_pixel(x2, y2 + i * 10.0f, highlightColor);
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
					if (break_loop) { break; px_quantity = i; }
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
									draw_pixel(x1 + slope_x_modf, y1 + slope_y_modf, color);
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
									draw_pixel(x1 + slope_x_modf, y1 + slope_y_modf, color);
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
									draw_pixel(x1 + slope_x_modf, y1 + slope_y_modf, color);
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
									draw_pixel(x1 + slope_x_modf, y1 + slope_y_modf, color);
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
							draw_pixel(x1, y1 + slope_y_modf, color);
						}
						slope_y_modf += slope_y_incr;
						q = max_pixels;
					}
					else if (straight_line_type == 1) //up
					{
						float slope_y_incr = PIXEL_SIZE_STLN; float slope_x_incr = PIXEL_SIZE_STLN;
						if (visible)
						{
							draw_pixel(x1, y1 + slope_y_modf, color);
						}
						slope_y_modf -= slope_y_incr;
						q = max_pixels;
					}
					else if (straight_line_type == 2) //right
					{
						float slope_y_incr = PIXEL_SIZE_STLN; float slope_x_incr = PIXEL_SIZE_STLN;
						if (visible)
						{
							draw_pixel(x1 + slope_x_modf, y1, color);
						}
						slope_x_modf += slope_x_incr;
						p = max_pixels;
					}
					else if (straight_line_type == 3) //left
					{
						float slope_y_incr = PIXEL_SIZE_STLN; float slope_x_incr = PIXEL_SIZE_STLN;
						if (visible)
						{
							draw_pixel(x1 + slope_x_modf, y1, color);
						}
						slope_x_modf -= slope_x_incr;
						p = max_pixels;
					}
				}
			}
			px_quantity_x_cycle = p;
			px_quantity_y_cycle = q;
			px_quantity = p + q;
		}
	public:
		float px_quantity = 0.0f;
		float px_quantity_x_cycle = 0.0f;
		float px_quantity_y_cycle = 0.0f;
		float x1 = 0.0f;
		float y1 = 0.0f;
		float x2 = 0.0f;
		float y2 = 0.0f;
		float angle = 0.0f;
		float vectorQuality = VECTOR_MQ;
		bool visible = true;
		bool highlight_vertexes = false;
		uint32 highlightColor = colors.yellow;
		uint32 color = colors.red;
		int thickness_l = 1; //thickness left
		int thickness_r = 1; //thickness right
		//Draw 2d vector.
		fvector(float x1_, float y1_, float x2_, float y2_, uint32 color_, bool visible_ = true)
		{
			x1 = x1_;
			y1 = y1_;
			x2 = x2_;
			y2 = y2_;
			color = color_;
			vectorQuality = VECTOR_MQ;
			highlight_vertexes = false;
			highlightColor = colors.white;
			visible = visible_;
			get_rotation_radius();
		}
		fvector()
		{

		}
		~fvector()
		{

		}
		fvector(Vector2f& vx1, Vector2f& vx2, uint32 color_, bool visible_ = true)
		{
			x1 = vx1.x;
			y1 = vx1.y;;
			x2 = vx2.x;
			y2 = vx2.y;
			color = color_;
			highlight_vertexes = false;
			vectorQuality = VECTOR_MQ;
			highlightColor = colors.white;
			visible = visible_;
			get_rotation_radius();
		}
		//Calculate X Center of a vector
		float centerx()
		{
			return (x2 - x1) / 2.0f;
		}
		//Calculate Y Center of a vector
		float centery()
		{
			return (y2 - y1) / 2.0f;
		}
		//Calculate Vector's magnitude in pixels
		float magnitude()
		{
			return px_quantity;
		}
		inline void add_vector(float x2_, float y2_)
		{
			x2 = x2_;
			y2 = y2_;
			draw();
		}
		//inline void Rotate(float angle_)
		//{
		//	/*Firstly, algorithm gets |x2-x1| and |y2-y1| and checks which one is bigger and draws the ellipse with following radius which must be equal. Then algorithm finds in ellipse's pixel matrix vertex with same x as x2(last not changed vector's pixel.x). Followingly,
		//	When algorithm found that vertex, it substracts vertex's y from |y2-y1| to get the difference between them. Then, it uplifts the |y2-y1| by the difference and then it draws the new vector with changed rotation
		//	by getting the pixel matrix of the new ellipse with increasing angle by 10 index because that is how program draws ellipse.*/
		//	float ra = fabs(x2 - x1);
		//	float rb = fabs(y2 - y1);
		//	float r = 0.0f;
		//	if (ra >= rb)
		//	{
		//		r = ra;
		//	}
		//	if (ra < rb)
		//	{
		//		r = rb;
		//	}
		//	float temp_y2 = 0.0f;
		//	float diff = 0.0f;
		//	ellipse2d::ellipsed e1(x1, y1, rb, rb, color, 0, 3600, false, true); e1.draw();
		//	vector2d::fvector v1(x1, y1, x2, y2, color, true); v1.draw();
		//	int q = 0;
		//	for (int k = 0; k < 3600; k++)
		//	{
		//		if (floor(e1.matrix_pixels_pos[k].x) == floor(x2))
		//		{
		//			q = k;
		//			temp_y2 = e1.matrix_pixels_pos[k].y;
		//			diff = y2 - temp_y2;
		//			break;
		//		}
		//	}
		//	ellipse2d::ellipsed e2(x1, y1, (y2 + diff) - y1, (y2 + diff) - y1, colors.blue, 0, 3600, false, true); e2.draw();
		//	x2 = e2.matrix_pixels_pos[(int)angle_ * 10].x;
		//	y2 = e2.matrix_pixels_pos[(int)angle_ * 10].y;
		//	angle = angle_;
		//}
		//Calculate Vector's angle
		float get_angle()
		{
			return angle;
		}
		//Draw function
		inline void draw()
		{
			for (int i = 0; i < thickness_l + thickness_r + 1; i++) // +1 because thickness_l and thickness_r are by default zero and by default normal vector must be build and therefore to start for-loop with 1 loop.
			{
				//this if-statements are to merge two expected for-loops(one for-loop is for left thickness and the second for-loop is for right thickness) to one for-loop
				if (i < thickness_l) //this is to draw left thickness
				{
					draw_vector(x1 - (0.1f * i), y1 + (0.1f * i), x2 - (0.1f * i), y2 + (0.1f * i), color);
				}
				if (i > thickness_l && i < thickness_r + thickness_r)
				{
					draw_vector(x1 + (0.1f * (i - thickness_l)), y1 - (0.1f * (i - thickness_l)), x2 + (0.1f * (i - thickness_l)), y2 - (0.1f * (i - thickness_l)), color);
				}
			}
		}
	};
	class dvector
	{
	private:
		inline void get_rotation_radius(pivot pivot_)
		{
			//Compute angle
			/*Firstly, algorithm works by drawing an ellipse with |x2-x1| and |y2-y1| radiuses. Then algorithm finds in ellipse's pixel matrix vertex with same x as x2(last not changed vector's pixel.x). Followingly,
			When algorithm found that vertex, it substracts vertex's y from |y2-y1| to get the difference between them. Then, it uplifts the |y2-y1| by the difference and then it checks if new ellipse's pixel
			matrix x and y are equal to x2 and y2 of new vector's x2 and y2.*/
			float x1_ = x1;
			float y1_ = y1;
			if (pivot_ == center)
			{
				x1_ = (x1 + x2) / 2.0f;
				y1_ = (y1 + y2) / 2.0f;
			}
			float dx = x2 - x1_;
			float dy = y2 - y1_;
			float adx = fabs(x2 - x1_);
			float ady = fabs(y2 - y1_);
			float alpha = atan2(ady, adx); alpha *= 180.0f / pi;
			alpha = fabs(alpha);
			bool directions[4]
			{
				false, // up_right
				false, // up_left
				false, // down_right
				false // down_left
			};
			float direction_modifiers[4]
			{
				0,
				0,
				0,
				0,
			};
			dx > 0.0f && dy > 0.0f ? directions[0] = true : directions[0] = false; // if (dx > 0.0f && dy > 0.0f){directions[0]=true} else {directions=false}
			dx < 0.0f && dy > 0.0f ? directions[1] = true : directions[1] = false;
			dx > 0.0f && dy < 0.0f ? directions[2] = true : directions[2] = false;
			dx < 0.0f && dy < 0.0f ? directions[3] = true : directions[3] = false;
			for (int j = 0; j < 4; j++)
			{
				if (directions[j] == true)
				{
					if (j == 2)
					{
						alpha += 90;
					}
					if (j == 3)
					{
						alpha = 90 - alpha;
						alpha += 180;
					}
					if (j == 1)
					{
						alpha += 270;
					}
					if (j == 0)
					{
						alpha = 90 - alpha;
					}
				}
			}
			angle = alpha;
			float ra = fabs(x2 - x1_);
			float rb = fabs(y2 - y1_);
			float r = ra + rb;
			float error_distance = 9.5f;
			float range_x = 150.0f;
			float range_y = 50.0f;
			float error_distance_bug_fixer = 1.0f;
			float rdiv = 0.0f;
			float rmodf = 0.0f;
			ellipse2d::ellipsed e1(x1_, y1_, r, r, colors.red, 0, 3600, false, false); e1.draw();
			float x2__ = 0.0f;
			float y2__ = 0.0f;
			if (rb >= ra)
			{
				x2__ = e1.matrix_pixels_pos[0].x;
				y2__ = e1.matrix_pixels_pos[0].y;
			}
			else
			{
				x2__ = e1.matrix_pixels_pos[900].x;
				y2__ = 0.0f;
			}
			vector2d::fvector v2(x1_, y1_, x2__, y2__, colors.blue, false); v2.vectorQuality = VECTOR_LQ; v2.draw();
			vector2d::fvector d1(x1_, y1_, x2, y2, colors.white, false); d1.vectorQuality = VECTOR_LQ; d1.draw();
			if (ra == rb)
			{
				error_distance_bug_fixer = 1.5f;
			}
			if (ra > rb)
			{
				rdiv = ra / rb;
			}
			else if (ra <= rb)
			{
				rdiv = rb / ra;
			}
			if (rdiv >= 4.0f)
			{
				error_distance_bug_fixer = 0.5f;
			}
			if (ra > rb)
			{
				rmodf = (fabs(v2.px_quantity_x_cycle - d1.px_quantity_x_cycle)) / error_distance / error_distance_bug_fixer;
			}
			else if (ra <= rb)
			{
				rmodf = (fabs(v2.px_quantity_y_cycle - d1.px_quantity_y_cycle)) / error_distance / error_distance_bug_fixer;
			}
			bool found = false;
			for (int i = 0; i < 20; i++)
			{
				if (v2.px_quantity_x_cycle < d1.px_quantity_x_cycle)
				{
					break;
				}
				if (rb >= ra)
				{
					r -= rmodf;
				}
				else if (ra > rb)
				{
					r -= rmodf;
				}
				if ((angle * 10) - range_x < 0)
				{
					range_x = (angle * 10);
				}
				if ((angle * 10) + range_y > 3600)
				{
					range_y = 3600 - (angle * 10);
				}
				ellipse2d::ellipsed e2(x1_, y1_, r, r, colors.white, (angle * 10) - range_x, (angle * 10) + range_y, false, false); e2.draw();
				for (int k = (angle * 10) - range_x; k < (angle * 10) + range_y; k++)
				{
					bool x_range = in_range(e2.matrix_pixels_pos[k].x, x2, error_distance / 2.0f);
					bool y_range = in_range(e2.matrix_pixels_pos[k].y, y2, error_distance / 2.0f);
					//if ((x_range == true && y_range == true) || (ceil(e2.matrix_pixels_pos[k].x) == ceil(x2) && ceil(e2.matrix_pixels_pos[k].y) == ceil(y2)) || (floor(e2.matrix_pixels_pos[k].x) == ceil(x2) && floor(e2.matrix_pixels_pos[k].y) == floor(y2)))
					if (x_range == true && y_range == true)
					{
						angle = k + 20;
						found = true;
						break;
					}
				}
				//Computing angle
				if (found == true)
				{
					break;
				}
			}
			rotation_radius = r;
		}
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
		inline void draw_vector(float x1, float y1, float x2, float y2, uint32 color, bool save_pixels_matrix, bool save_pixels_matrix_x_cycle, bool save_pixels_matrix_y_cycle)
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
					draw_pixel(x2, y2, color);
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
				float triangle_exception_x = 0.0f;
				float triangle_exception_incr_x = 0.0f;
				float triangle_exception_y = 0.0f;
				float triangle_exception_incr_y = 0.0f;
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
									draw_pixel(x1 + slope_x_modf, y1 + slope_y_modf, color);
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
									draw_pixel(x1 + slope_x_modf, y1 + slope_y_modf, color);
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
									draw_pixel(x1 + slope_x_modf, y1 + slope_y_modf, color);
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
									draw_pixel(x1 + slope_x_modf, y1 + slope_y_modf, color);
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
							draw_pixel(x1, y1 + slope_y_modf, color);
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
							draw_pixel(x1, y1 + slope_y_modf, color);
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
							draw_pixel(x1 + slope_x_modf, y1, color);
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
							draw_pixel(x1 + slope_x_modf, y1, color);
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
		float rotation_radius = 0.0f;
		pivot pivot_rotation = center;
		int thickness_l = 1; //thickness left
		int thickness_r = 1; //thickness right
		//Draw 2d vector.
		dvector(float x1_, float y1_, float x2_, float y2_, uint32 color_, bool visible_ = true, int thickness_l_ = 1, int thickness_r_ = 1, bool save_pixels_position_ = false, bool save_pixels_position_x_cycle_ = false, bool save_pixels_position_y_cycle_ = false, pivot rpivot = center)
		{
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
			pivot_rotation = rpivot;
			get_rotation_radius(rpivot);
		}
		dvector(Vector2f& vx1, Vector2f& vx2, uint32 color_, bool visible_ = true, int thickness_l_ = 1, int thickness_r_ = 1, bool save_pixels_position_ = false, bool save_pixels_position_x_cycle_ = false, bool save_pixels_position_y_cycle_ = false, pivot rpivot = center)
		{
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
			pivot_rotation = rpivot;
			get_rotation_radius(rpivot);
		}
		dvector()
		{

		}
		~dvector()
		{

		}
		int magnitude()
		{
			return px_quantity;
		}
		inline void Rotate(float angle_)
		{
			int m = angle_ / 360.0f;
			if (angle_ >= 360.0f * (m))
			{
				angle_ = (angle_ - (360.0f * m));
			}
			else if (angle_ < 0.0f + (360 * (m - 1)))
			{
				angle_ = (360.0f * m) - fabs(angle_);
			}
			if (pivot_rotation == center)
			{
				int i = 0;
				if (((int)angle_ * 10) >= 1800)
				{
					i = angle_ * 10.0f - 1800;
				}
				else
				{
					i = 1800 + angle_ * 10.0f;
				}
				ellipse2d::ellipsed rotation_ellipse1((x1 + x2) / 2.0f, (y1 + y2) / 2.0f, rotation_radius, rotation_radius, colors.green, (int)angle_ * 10, (int)angle_ * 10 + 1, false, false);
				rotation_ellipse1.draw();
				ellipse2d::ellipsed rotation_ellipse2((x1 + x2) / 2.0f, (y1 + y2) / 2.0f, rotation_radius, rotation_radius, colors.green, i, i + 1, false, false);
				rotation_ellipse2.draw();
				x1 = rotation_ellipse2.matrix_pixels_pos[i].x;
				y1 = rotation_ellipse2.matrix_pixels_pos[i].y;
				x2 = rotation_ellipse1.matrix_pixels_pos[(int)angle_ * 10].x;
				y2 = rotation_ellipse1.matrix_pixels_pos[(int)angle_ * 10].y;
			}
			else if (pivot_rotation == startpos)
			{
				ellipse2d::ellipsed rotation_ellipse(x1, y1, rotation_radius, rotation_radius, colors.green, (int)angle_ * 10, (int)angle_ * 10 + 1, false, true);
				rotation_ellipse.draw();
				x2 = rotation_ellipse.matrix_pixels_pos[(int)angle_ * 10].x;
				y2 = rotation_ellipse.matrix_pixels_pos[(int)angle_ * 10].y;
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
					vector2d::fvector vprojectionx(x1, y1, x2, y1, color);
				}
				if (ur == true)
				{
					vector2d::fvector vprojectionx(x1, y2, x2, y2, color);
				}
			}
			if (vh == true)
			{
				if (ur == false)
				{
					vector2d::fvector vprojectiony(x1, y1, x1, y2, color);
				}
				if (ur == true)
				{
					vector2d::fvector vprojectiony(x2, y1, x2, y2, color);
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
					draw_vector(x1 - (0.1f * i), y1 + (0.1f * i), x2 - (0.1f * i), y2 + (0.1f * i), color, save_pixels_position, save_pixels_position_x_cycle, save_pixels_position_y_cycle);
				}
				if (i > thickness_l && i < thickness_r + thickness_r)
				{
					draw_vector(x1 + (0.1f * (i - thickness_l)), y1 - (0.1f * (i - thickness_l)), x2 + (0.1f * (i - thickness_l)), y2 - (0.1f * (i - thickness_l)), color, save_pixels_position, save_pixels_position_x_cycle, save_pixels_position_y_cycle);
				}
			}
		}
	};
}
inline static void draw_coordinate_grid(uint32 color)
{
	vector2d::fvector v1(-1500.0f, 0.0f, 1500.0f, 0.0f, color, true); v1.draw();
	vector2d::fvector v2(0.0f, -500.0f, 0.0f, 500.0f, color, true); v2.draw();
}

namespace cube
{
	class CUBE
	{
	private:
		void draw_cube(float x, float y, float a, int r, int g, int b) // size of each side of a cube
		{
			vector2d::fvector v1(x, y, x - (a / 2.0f), y - (a / 2.0f), rgb(r - 10, g - 10, b - 10)); v1.draw(); px_cube += v1.magnitude();
			vector2d::fvector v2(x - (a / 2.0f), y - (a / 2.0f), x + (a / 2.0f), y - (a / 2.0f), rgb(r, g, b)); v2.draw(); px_cube += v2.magnitude();
			vector2d::fvector v3(x - (a / 2.0f), y - (a / 2.0f), x - (a / 2.0f), y + (a / 2.0f), rgb(r, g, b)); v3.draw(); px_cube += v3.magnitude();
			vector2d::fvector v4(x - (a / 2.0f), y + (a / 2.0f), x + (a / 2.0f), y + (a / 2.0f), rgb(r, g, b)); v4.draw(); px_cube += v4.magnitude();
			vector2d::fvector v5(x + (a / 2.0f), y - (a / 2.0f), x + (a / 2.0f), y + (a / 2.0f), rgb(r, g, b)); v5.draw(); px_cube += v5.magnitude();
			vector2d::fvector v6(x, y, x, y + a, rgb(r - 10, g - 10, b - 10)); v6.draw(); px_cube += v6.magnitude();
			vector2d::fvector v7(x, y + a, x + a, y + a, rgb(r, g, b)); v7.draw(); px_cube += v7.magnitude();
			vector2d::fvector v8(x, y + a, x - (a / 2.0f), y + (a / 2.0f), rgb(r, g, b)); v8.draw(); px_cube += v8.magnitude();
			vector2d::fvector v9(x, y, x + a, y, rgb(r - 10, g - 10, b - 10)); v9.draw(); px_cube += v9.magnitude();
			vector2d::fvector v10(x + a, y + a, x + (a / 2.0f), y + (a / 2.0f), rgb(r, g, b)); v10.draw(); px_cube += v10.magnitude();
			vector2d::fvector v11(x + (a / 2.0f), y - (a / 2.0f), x + a, y, rgb(r, g, b)); v11.draw(); px_cube += v11.magnitude();
			vector2d::fvector v12(x + a, y + a, x + a, y, rgb(r, g, b)); v12.draw(); px_cube += v12.magnitude();
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
		CUBE(float x_, float y_, float a_, float r_, float g_, float b_, bool visible = true)
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
		CUBE()
		{

		}
		~CUBE()
		{

		}
		float perimeter()
		{
			return px_cube;
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
		inline void draw_triangle(float a, float b, float c, float h_apex, float x, float y, uint32 color, bool filled)
		{
			vector2d::dvector v1(x - c / 2.0f, y, x + c / 2.0f, y, color, true, 1, 1, true, true, true); v1.draw(); v1.draw(); //c-line
			vector2d::dvector v2(x - c / 2.0f, y, (x - c / 2.0f) + a, y + h_apex, color, false, 1, 1, true, true, true); v1.draw(); v1.draw(); //a-line
			vector2d::dvector v3(x + c / 2.0f, y, v2.x2, v2.y2, color, false, 1, 1, true, true, true); v1.draw(); v1.draw(); //b-line
			int fill_type = 0; //0 - a > c; 1 - b > c; 2 - a and b < c
			if (a >= b && a < c && b < c)
			{
				float abx = a - b;
				v2.x1 = x - c / 2.0f; v2.y1 = y; v2.x2 = x + abx; v2.y2 = y + h_apex; v2.color = color; v2.draw();
				v3.x1 = x + c / 2.0f; v3.y1 = y; v3.x2 = v2.x2; v3.y2 = v2.y2;  v3.color = color; v3.draw();
				fill_type = 1;
			}
			if (a < b && a < c && b < c)
			{
				float abx = b - a;
				v2.x1 = x - c / 2.0f; v2.y1 = y; v2.x2 = x - abx; v2.y2 = y + h_apex; v2.color = color; v2.draw();
				v3.x1 = x + c / 2.0f; v3.y1 = y; v3.x2 = v2.x2; v3.y2 = v2.y2;  v3.color = color; v3.draw();
				fill_type = 2;
			}
			if (a > c && b < c && a > b)
			{
				v2.draw();
				v3.draw();
				fill_type = 0;
			}
			if (b > c && a < c && a < b)
			{
				v3.x2 = (x + c / 2.0f) - b; v3.y2 = y + h_apex;
				v2.x2 = v3.x2; v2.y2 = v3.y2;
				v2.draw();
				v3.draw();
				fill_type = 4;
			}
			if (filled)
			{
				//Drawing triangle works simply by drawing the base line(c-line), then drawing a- or b-line depending on which is bigger and connecting them to each other.
				//Filling triangle algorithm works simply by connecting all c-line pixels x, y with a-line only y_cycle pixels x, y.
				float v1y = v1.matrix_pixels[0].y;
				if (filled)
				{
					if (fill_type == 1 || fill_type == 2)
					{
						for (int j = 0; j < v2.px_quantity_x_cycle; j++)
						{
							float mxp_v1 = v2.matrix_pixels_x_cycle[j].x; //mxp_v1 is the pixel's x of c- and a-line and they must be equal to build straight upward vector. Thus, here program gets one variable to not get it twice in x1 and x2
							vector2d::fvector v4(mxp_v1, v1y, mxp_v1, v2.matrix_pixels_x_cycle[j].y, color, true); v1.draw();
							//y1 is never changing because c is straight horizontal vector and there is no need to get it every time if it does not alter. Only y2 is a changing value of every a-line pixel's y.
						}
						for (int z = v2.px_quantity_x_cycle; z < v2.px_quantity_x_cycle + v3.px_quantity_x_cycle; z++)
						{
							int r = v3.px_quantity_x_cycle - (z - v2.px_quantity_x_cycle) - 1;
							float mxp_v1 = v3.matrix_pixels_x_cycle[r].x; //mxp_v1 is the pixel's x of c- and a-line and they must be equal to build straight upward vector. Thus, here program gets one variable to not get it twice in x1 and x2
							vector2d::fvector v4(mxp_v1, v1y, mxp_v1, v3.matrix_pixels_x_cycle[r].y, color, true); v1.draw();
						}
					}
					//if (fill_type == 0)
					//{
					//	int index_v3 = 0;
					//	int diff_ = (v1.px_quantity + v3.px_quantity_x_cycle) - v2.px_quantity_x_cycle;
					//	float ac = a - c;
					//	float ac_modf = (ac) + 1;
					//	float i_bug_fx = 1.0f;
					//	bool triangle_equillibrium_bug = false;
					//	float ac_modfA = ac / 2.0f;

					//	for (int z = 0; z < v2.px_quantity_x_cycle - 1; z++)
					//	{
					//		if (v2.matrix_pixels_x_cycle[z].x < v1.matrix_pixels[v1.px_quantity - 1].x)
					//		{
					//			float mxp_v1 = v2.matrix_pixels_x_cycle[z].x; //mxp_v1 is the pixel's x of c- and a-line and they must be equal to build straight upward vector. Thus, here program gets one variable to not get it twice in x1 and x2
					//			vector2d::VECTOR v4(mxp_v1, y, mxp_v1, v2.matrix_pixels_x_cycle[z].y, color, true); v1.draw();
					//			//y1 is never changing because c is straight horizontal vector and there is no need to get it every time if it does not alter. Only y2 is a changing value of every a-line pixel's y.
					//		}
					//		else
					//		{
					//			if (diff_ < 10 && diff_ > 0)
					//			{
					//				index_v3 = z;
					//				triangle_equillibrium_bug = true;
					//			}
					//			else
					//			{
					//				index_v3 = v1.px_quantity;
					//				triangle_equillibrium_bug = false;
					//			}
					//			break;
					//		}
					//	}
					//	if (triangle_equillibrium_bug)
					//	{
					//		for (int z = index_v3; z < v2.px_quantity_x_cycle - ac_modfA - 6; z++) // v3.px_quantity_x_cycle-2
					//		{
					//			float bug_fx;
					//			int r = z - index_v3;
					//			bug_fx = i_bug_fx - (r * 0.0005f);
					//			float mxp_v1 = v2.matrix_pixels_x_cycle[z].x; //mxp_v1 is the pixel's x of c- and a-line and they must be equal to build straight upward vector. Thus, here program gets one variable to not get it twice in x1 and x2
					//			vector2d::VECTOR v4(mxp_v1, v2.matrix_pixels_x_cycle[z].y - bug_fx, mxp_v1, v2.matrix_pixels_x_cycle[z].y, color, true); v1.draw();

					//			//y1 is never changing because c is straight horizontal vector and there is no need to get it every time if it does not alter. Only y2 is a changing value of every a-line pixel's y.
					//		}
					//		for (int z = index_v3; z < index_v3 + v3.px_quantity_x_cycle - ac_modfA; z++) // v3.px_quantity_x_cycle-2
					//		{
					//			int r = z - index_v3;
					//			float mxp_v1 = v3.matrix_pixels_x_cycle[r].x; //mxp_v1 is the pixel's x of c- and a-line and they must be equal to build straight upward vector. Thus, here program gets one variable to not get it twice in x1 and x2
					//			vector2d::VECTOR v4(mxp_v1, v3.matrix_pixels_x_cycle[r].y, mxp_v1, v2.matrix_pixels_x_cycle[z].y, color, true); v1.draw();

					//			//y1 is never changing because c is straight horizontal vector and there is no need to get it every time if it does not alter. Only y2 is a changing value of every a-line pixel's y.
					//		}
					//	}
					//	if (!triangle_equillibrium_bug)
					//	{
					//		for (int z = index_v3; z < index_v3 + v3.px_quantity_x_cycle - 2; z++) // v3.px_quantity_x_cycle-2
					//		{

					//			int r = z - index_v3;
					//			int w = z;
					//			float mxp_v1 = v3.matrix_pixels_x_cycle[r].x; //mxp_v1 is the pixel's x of c- and a-line and they must be equal to build straight upward vector. Thus, here program gets one variable to not get it twice in x1 and x2
					//			float ypos = 0.0f;
					//			ypos = v3.matrix_pixels_x_cycle[r].y;
					//			vector2d::VECTOR v4(mxp_v1, v3.matrix_pixels_x_cycle[r].y, mxp_v1, v2.matrix_pixels_x_cycle[w].y, color, true); v1.draw();

					//			//y1 is never changing because c is straight horizontal vector and there is no need to get it every time if it does not alter. Only y2 is a changing value of every a-line pixel's y.
					//		}
					//	}
					//}
					//if (fill_type == 4)
					//{
					//	int index_v3 = 0;
					//	int diff_ = (v1.px_quantity + v2.px_quantity_x_cycle) - v3.px_quantity_x_cycle;
					//	float bc = b - c;
					//	float bc_modf = (bc)+1;
					//	float i_bug_fx = 1.0f;
					//	bool triangle_equillibrium_bug = false;
					//	float bc_modfA = bc / 2.0f;

					//	for (int z = 0; z < v3.px_quantity_x_cycle - 1; z++)
					//	{
					//		if (v3.matrix_pixels_x_cycle[z].x > v1.matrix_pixels[0].x)
					//		{
					//			float mxp_v1 = v3.matrix_pixels_x_cycle[z].x; //mxp_v1 is the pixel's x of c- and a-line and they must be equal to build straight upward vector. Thus, here program gets one variable to not get it twice in x1 and x2
					//			vector2d::VECTOR v4(mxp_v1, y, mxp_v1, v3.matrix_pixels_x_cycle[z].y, color, true); v1.draw();
					//			//y1 is never changing because c is straight horizontal vector and there is no need to get it every time if it does not alter. Only y2 is a changing value of every a-line pixel's y.
					//		}
					//		else
					//		{
					//			if (diff_ < 10)
					//			{
					//				index_v3 = z;
					//				triangle_equillibrium_bug = true;
					//			}
					//			else
					//			{
					//				index_v3 = v1.px_quantity;
					//				triangle_equillibrium_bug = false;
					//			}
					//			break;
					//		}
					//	}
					//	if (triangle_equillibrium_bug)
					//	{
					//		for (int z = index_v3; z < v3.px_quantity_x_cycle - bc_modfA; z++) // v3.px_quantity_x_cycle-2
					//		{
					//			float bug_fx;
					//			int r = z - index_v3;
					//			bug_fx = i_bug_fx - (r * 0.0005f);
					//			float mxp_v1 = v3.matrix_pixels_x_cycle[z].x; //mxp_v1 is the pixel's x of c- and a-line and they must be equal to build straight upward vector. Thus, here program gets one variable to not get it twice in x1 and x2
					//			vector2d::VECTOR v4(mxp_v1, v3.matrix_pixels_x_cycle[z].y - bug_fx, mxp_v1, v3.matrix_pixels_x_cycle[z].y, color, true); v1.draw();

					//			//y1 is never changing because c is straight horizontal vector and there is no need to get it every time if it does not alter. Only y2 is a changing value of every a-line pixel's y.
					//		}
					//		for (int z = index_v3; z < index_v3 + v2.px_quantity_x_cycle - bc_modfA-3; z++) // v3.px_quantity_x_cycle-2
					//		{
					//			int r = z - index_v3;
					//			float mxp_v1 = v2.matrix_pixels_x_cycle[r].x; //mxp_v1 is the pixel's x of c- and a-line and they must be equal to build straight upward vector. Thus, here program gets one variable to not get it twice in x1 and x2
					//			vector2d::VECTOR v4(mxp_v1, v2.matrix_pixels_x_cycle[r].y, mxp_v1, v3.matrix_pixels_x_cycle[z].y, color, true); v1.draw();

					//			//y1 is never changing because c is straight horizontal vector and there is no need to get it every time if it does not alter. Only y2 is a changing value of every a-line pixel's y.
					//		}
					//	}
					//	if (!triangle_equillibrium_bug)
					//	{
					//		for (int z = index_v3; z < index_v3 + v2.px_quantity_x_cycle - 2; z++) // v3.px_quantity_x_cycle-2
					//		{

					//			int r = z - index_v3;
					//			int w = z;
					//			float mxp_v1 = v2.matrix_pixels_x_cycle[r].x; //mxp_v1 is the pixel's x of c- and a-line and they must be equal to build straight upward vector. Thus, here program gets one variable to not get it twice in x1 and x2
					//			vector2d::VECTOR v4(mxp_v1, v2.matrix_pixels_x_cycle[r].y, mxp_v1, v3.matrix_pixels_x_cycle[w].y, color, true); v1.draw();

					//			//y1 is never changing because c is straight horizontal vector and there is no need to get it every time if it does not alter. Only y2 is a changing value of every a-line pixel's y.
					//		}
					//	}
					//}
				}
			}
		}
	public:
		float a = 0.0f;
		float b = 0.0f;
		float c = 0.0f;
		float h_apex = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		char name[20];
		bool visible = true;
		uint32 color = colors.red;
		bool filled = false;
		//Draw triangle funciton. Exception: a + b > c && a + c > b && b + c > a and a,b,c,h > 0
		triangle(float a_, float b_, float c_, float h_apex_, float x_, float y_, uint32 color_, bool filled_, bool visible = true)
		{
			a = a_;
			b = b_;
			c = c_;
			h_apex = h_apex_;
			x = x_;
			y = y_;
			color = color_;
			filled = filled_;
			if (visible == true)
			{
				draw_triangle(a_, b_, c_, h_apex_, x_, y_, color_, filled_);
			}
		}
		triangle()
		{

		}
		~triangle()
		{

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
				draw_triangle(a, b, c, h_apex, x, y, color, filled);
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
	TextureFilter texturefilter = neutral;
	Texture(const char* path_)
	{
		path = path_;
		visible = true;
		stbi_load(path, &texture_width, &texture_height, &rgb_channels, 0);
	}
	Texture()
	{

	}
	~Texture()
	{

	}
};
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
				vector2d::dvector v1(position.x, position.y, position.x + ch_width / 2.0f, position.y + ch_height, color, true, 1, 1, true, true, true); v1.draw(); // left line
				vector2d::dvector v2(position.x + ch_width, position.y, v1.x2, v1.y2, color, true, 1, 1, true, true, true); v2.draw(); // right line
				vector2d::fvector v3(v1.matrix_pixels_y_cycle[int(v1.px_quantity_y_cycle / 2.0f) - 2].x, v1.matrix_pixels_y_cycle[int(v1.px_quantity_y_cycle / 2.0f) - 1].y, v2.matrix_pixels_y_cycle[int(v2.px_quantity_y_cycle / 2.0f) - 1].x, v1.matrix_pixels_y_cycle[int(v1.px_quantity_y_cycle / 2.0f) - 1].y, color, true); v3.draw(); // middle line
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'B')
			{
				vector2d::dvector v1(position.x, position.y, position.x, position.y + ch_height, color, true, 1, 1, true, true, true); v1.draw(); // straight line
				ellipse2d::ellipsef e1(position.x, position.y + (ch_height / 4.0f), ch_width / 1.5f, ch_height / 4.0f, color, 0, 1800, false, true); e1.draw(); // bottom ellipse
				ellipse2d::ellipsef e2(position.x, position.y + (ch_height / 1.33f), ch_width / 1.5f, ch_height / 4.0f, color, 0, 1800, false, true); e2.draw(); // top ellipse
				next_pos = new Vector2f(position.x + ch_width / 1.5f, position.y);
			}
			if (ch == 'C')
			{
				ellipse2d::ellipsef e1(position.x + ch_width / 2.0f, position.y + ch_height / 2.0f, ch_width / 2.0f, ch_height / 2.0f, color, 1400, 3600, false, true); e1.draw();
				ellipse2d::ellipsef e2(position.x + ch_width / 2.0f, position.y + ch_height / 2.0f, ch_width / 2.0f, ch_height / 2.0f, color, 0, 400, false, true); e1.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.23f, position.y + ch_height / 2.0f);
			}
			if (ch == 'D')
			{
				vector2d::dvector v1(position.x, position.y, position.x, position.y + ch_height, color, true, 1, 1, true, true, true); v1.draw(); v1.draw(); // straight line
				ellipse2d::ellipsef e1(position.x, position.y + ch_height / 2.0f, ch_width / 1.5f, ch_height / 2.0f, color, 0, 1800, false, true); e1.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.5f, position.y);
			}
			if (ch == 'E')
			{
				vector2d::fvector v1(position.x, position.y, position.x, position.y + ch_height, color, true); v1.draw(); // straight line
				vector2d::fvector v2(position.x, position.y + ch_height, position.x + ch_width, position.y + ch_height, color, true); v2.draw(); // top line
				vector2d::fvector v3(position.x, position.y + ch_height / 2.0f, position.x + ch_width, position.y + ch_height / 2.0f, color, true); v3.draw(); // middle line
				vector2d::fvector v4(position.x, position.y, position.x + ch_width, position.y, color, true); v4.draw(); // bottom line
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'F')
			{
				vector2d::fvector v1(position.x, position.y, position.x, position.y + ch_height, color, true); v1.draw(); // straight line
				vector2d::fvector v2(position.x, position.y + ch_height, position.x + ch_width, position.y + ch_height, color, true); v2.draw(); // top line
				vector2d::fvector v3(position.x, position.y + ch_height / 1.5f, position.x + ch_width, position.y + ch_height / 1.5f, color, true); v3.draw(); // middle line
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'G')
			{
				ellipse2d::ellipsef e1(position.x + ch_width, position.y + ch_height / 2.0f, ch_width, ch_height / 2.0f, color, 1800, 3600, false, true); e1.draw();
				vector2d::fvector v1(position.x + ch_width, position.y, position.x + ch_width, position.y + ch_height / 2.0f, color, true); v1.draw();
				vector2d::fvector v2(v1.x2, v1.y2, position.x + ch_width / 2.0f, position.y + ch_height / 2.0f, color, true); v2.draw();
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'H')
			{
				vector2d::fvector v1(position.x, position.y, position.x, position.y + ch_height, color, true); v1.draw();
				vector2d::fvector v2(position.x + ch_width / 1.5f, position.y, position.x + ch_width / 1.5f, position.y + ch_height, color, true); v2.draw();
				vector2d::fvector v3(position.x, position.y + ch_height / 2.0f, position.x + ch_width / 1.5f, position.y + ch_height / 2.0f, color, true); v3.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.5f, position.y);
			}
			if (ch == 'I') // Remember about the bug with I
			{
				vector2d::fvector v1(position.x, position.y, position.x, position.y + ch_height, color, true); v1.draw();
				next_pos = new Vector2f(position.x + 0.1f, position.y);
			}
			if (ch == 'J')
			{
				ellipse2d::ellipsef e1(position.x + ch_width / 2.0f, position.y + ch_height / 2.0f, ch_width / 2.0f, ch_height / 2.0f, color, 1100, 2500, false, true); e1.draw();
				vector2d::fvector v1(position.x + ch_width - 0.1f, position.y + ch_height / 2.9f, position.x + ch_width - 0.1f, position.y + ch_height / 1.0f, color, true); v1.draw();
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'K')
			{
				vector2d::fvector v1(position.x, position.y, position.x, position.y + ch_height, color, true); v1.draw();
				vector2d::fvector v2(position.x, position.y + ch_height / 2.0f, position.x + ch_width, position.y + ch_height, color, true); v2.draw();
				vector2d::fvector v3(position.x, position.y + ch_height / 2.0f, position.x + ch_width, position.y, color, true); v3.draw();
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'L')
			{
				vector2d::fvector v1(position.x, position.y, position.x, position.y + ch_height, color, true); v1.draw();
				vector2d::fvector v2(position.x, position.y, position.x + ch_width / 1.2f, position.y, color, true); v2.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.5f, position.y);
			}
			if (ch == 'M')
			{
				vector2d::fvector v1(position.x, position.y, position.x, position.y + ch_height, color, true); v1.draw();
				vector2d::fvector v2(position.x, position.y + ch_height, position.x + ch_width / 2.0f, position.y, color, true); v2.draw();
				vector2d::fvector v3(v2.x2, v2.y2, position.x + ch_width, position.y + ch_height, color, true); v3.draw();
				vector2d::fvector v4(position.x + ch_width, position.y, position.x + ch_width, position.y + ch_height, color, true); v4.draw();
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'N')
			{
				vector2d::fvector v1(position.x, position.y, position.x, position.y + ch_height, color, true); v1.draw();
				vector2d::fvector v2(position.x, position.y + ch_height, position.x + ch_width / 1.5f, position.y, color, true); v2.draw();
				vector2d::fvector v4(position.x + ch_width / 1.5f, position.y, position.x + ch_width / 1.5f, position.y + ch_height, color, true); v4.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.5f, position.y);
			}
			if (ch == 'O')
			{
				ellipse2d::ellipsef e1(position.x + ch_width / 2.0f, position.y + ch_height / 2.0f, ch_width / 2.0f, ch_height / 2.0f, color, 0, 3600, false, true); e1.draw();
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'P')
			{
				vector2d::fvector v1(position.x, position.y, position.x, position.y + ch_height, color, true); v1.draw(); // straight line
				ellipse2d::ellipsef e2(position.x, position.y + (ch_height / 1.33f), ch_width / 1.5f, ch_height / 4.0f, color, 0, 1800, false, true); e2.draw(); // top ellipse
				next_pos = new Vector2f(position.x + ch_width / 1.5f, position.y);
			}
			if (ch == 'Q')
			{
				ellipse2d::ellipsef e1(position.x + ch_width / 2.0f, position.y + ch_height / 2.0f, ch_width / 2.0f, ch_height / 2.0f, color, 0, 3600, false, true); e1.draw();
				vector2d::fvector v1(position.x + ch_width / 2.0f, position.y, position.x + ch_width / 1.6f, position.y - ch_height / 6.0f, color, true); v1.draw();
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'R')
			{
				vector2d::fvector v1(position.x, position.y, position.x, position.y + ch_height, color, true); v1.draw(); // straight line
				ellipse2d::ellipsef e2(position.x, position.y + (ch_height / 1.33f), ch_width, ch_height / 4.0f, color, 0, 1800, false, true); e2.draw(); // top ellipse
				vector2d::fvector v2(position.x, position.y + ch_height / 2.0f, position.x + ch_width, position.y, color, true); v2.draw(); // straight line
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'S')
			{
				ellipse2d::ellipsef e1(position.x, position.y + ch_height / 4.0f, ch_width, ch_height / 4.0f, color, 300, 1800, false, true); e1.draw(); // bottom ellipse
				ellipse2d::ellipsef e2(position.x + ch_width, position.y + ch_height / 1.33f, ch_width, ch_height / 4.0f, color, 2100, 3600, false, true); e2.draw(); // top ellipse
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'T')
			{
				vector2d::fvector v1(position.x + ch_width / 2.0f, position.y, position.x + ch_width / 2.0f, position.y + ch_height, color, true); v1.draw();
				vector2d::fvector v2(position.x - ch_width / 8.0f, position.y + ch_height, position.x + ch_width + ch_width / 8.0f, position.y + ch_height, color, true); v2.draw();
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'U')
			{
				ellipse2d::ellipsef e1(position.x + ch_width / 2.0f, position.y + ch_height / 1.6f, ch_width / 3.0f, ch_height / 1.6f, color, 3000, 600, false, true); e1.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.2f, position.y);
			}
			if (ch == 'V')
			{
				vector2d::fvector v1(position.x, position.y + ch_height, position.x + ch_width / 2.0f, position.y, color, true); v1.draw();
				vector2d::fvector v2(v1.x2, v1.y2, position.x + ch_width, position.y + ch_height, color, true); v2.draw();
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'W')
			{
				vector2d::fvector v1(position.x, position.y + ch_height, position.x + ch_width / 4.0f, position.y, color, true); v1.draw();
				vector2d::fvector v2(v1.x2, v1.y2, position.x + ch_width / 2.0f, position.y + ch_height, color, true); v2.draw();
				vector2d::fvector v3(v2.x2, v2.y2, position.x + ch_width / 1.33f, position.y, color, true); v3.draw();
				vector2d::fvector v4(v3.x2, v3.y2, position.x + ch_width, position.y + ch_height, color, true); v4.draw();
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'X')
			{
				vector2d::fvector v1(position.x, position.y + ch_height, position.x + ch_width, position.y, color, true); v1.draw();
				vector2d::fvector v2(position.x, position.y, position.x + ch_width, position.y + ch_height, color, true); v2.draw();
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'Y')
			{
				vector2d::fvector v1(position.x, position.y + ch_height, position.x + ch_width / 2.0f, position.y + ch_height / 2.0f, color, true); v1.draw();
				vector2d::fvector v2(v1.x2, v1.y2, position.x + ch_width, position.y + ch_height, color, true); v2.draw();
				vector2d::fvector v3(v1.x2, v1.y2, v1.x2, position.y, color, true); v3.draw();
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == 'Z')
			{
				vector2d::fvector v1(position.x, position.y + ch_height, position.x + ch_width, position.y + ch_height, color, true); v1.draw();
				vector2d::fvector v2(v1.x2, v1.y2, position.x, position.y, color, true); v2.draw();
				vector2d::fvector v3(v2.x2, v2.y2, position.x + ch_width, position.y, color, true); v3.draw();
				next_pos = new Vector2f(position.x + ch_width, position.y);
			}
			if (ch == ' ')
			{
				next_pos = new Vector2f(position.x + space_width, position.y);
			}
			if (ch == 'a')
			{
				ellipse2d::ellipsef e1(position.x + ch_width / 8.0f, position.y + ch_height / 5.0f, ch_width / 5.0f, ch_height / 5.0f, color, 0, 3600, false, true); e1.draw();
				ellipse2d::ellipsef e2(position.x + (ch_width / 4.5f) + (ch_width / 2.0f), position.y + ch_height / 4.0f, ch_width / 2.4f, ch_height / 4.0f, color, 2900, 2100, false, true); e2.draw();
				next_pos = new Vector2f(position.x + (ch_width / 4.5f) + (ch_width / 5.0f), position.y);
			}
			if (ch == 'b')
			{
				vector2d::fvector v1(position.x, position.y, position.x, position.y + ch_height / 2.5f, color); v1.draw();	// straight line
				ellipse2d::ellipsef e1(position.x, position.y + (ch_height / 8.0f), ch_width / 3.0f, ch_height / 8.0f, color, 0, 1800, false, true); e1.draw(); // bottom ellipse
				next_pos = new Vector2f(position.x + ch_width / 3.0f, position.y);
			}
			if (ch == 'c')
			{
				ellipse2d::ellipsef e1(position.x + ch_width / 5.0f, position.y + ch_height / 5.0f, ch_width / 5.0f, ch_height / 5.0f, color, 1400, 3600, false, true); e1.draw();
				ellipse2d::ellipsef e2(position.x + ch_width / 5.0f, position.y + ch_height / 5.0f, ch_width / 5.0f, ch_height / 5.0f, color, 0, 400, false, true); e2.draw();
				next_pos = new Vector2f(position.x + ch_width / 5.0f + ch_width * 0.05f, position.y);
			}
			if (ch == 'd')
			{
				vector2d::fvector v1(position.x + ch_width / 6.0f, position.y, position.x + ch_width / 6.0f, position.y + ch_height / 2.2f, color); v1.draw(); // straight line
				ellipse2d::ellipsef e1(position.x + ch_width / 6.0f, position.y + (ch_height / 8.0f), ch_width / 4.0f, ch_height / 8.0f, color, 1800, 3600, false, true); e1.draw(); // bottom ellipse
				next_pos = new Vector2f(position.x + ch_width / 6.0f, position.y);
			}
			if (ch == 'e')
			{
				ellipse2d::ellipsef e1(position.x + ch_width / 5.0f, position.y + (ch_height / 5.0f), ch_width / 5.0f, ch_height / 5.0f, color, 1500, 3600, false, true); e1.draw(); // bottom ellipse
				ellipse2d::ellipsef e2(position.x + ch_width / 5.0f, position.y + (ch_height / 5.0f), ch_width / 5.0f, ch_height / 5.0f, color, 0, 700, false, true); e2.draw(); // second ellipse
				vector2d::fvector v1(position.x + ch_width / 5.0f + ch_width / 5.0f * fast_sin(69.9f * pi / 180.0f), position.y + (ch_height / 5.0f) + ch_height / 5.0f * fast_cos(69.9f * pi / 180.0f), position.x + ch_width / 5.0f + ch_width / 5.0f * fast_sin(300.0f * pi / 180.0f), position.y + (ch_height / 5.0f) + ch_height / 5.0f * fast_cos(69.9f * pi / 180.0f), color, true); v1.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.5f, position.y);
			}
			if (ch == 'f')
			{
				float avg = (position.x + ch_width / 4.0f) - position.x;
				vector2d::fvector v1(position.x + ch_width / 12.0f, position.y, position.x + ch_width / 12.0f, position.y + ch_height / 3.5f, color, true); v1.draw(); // straight line
				vector2d::fvector v2(position.x, position.y + ch_height / 3.5f, position.x + ch_width / 4.0f, position.y + ch_height / 3.5f, color, true); v2.draw(); // top line
				ellipse2d::ellipsef e1(position.x + avg * 1.5f, position.y + ch_height / 3.5f, ch_width / 4.0f, ch_height / 8.0f, color, 2700, 3500, false, true); e1.draw();
				next_pos = new Vector2f(position.x + ch_width / 8.0f, position.y);
			}
			if (ch == 'g')
			{
				vector2d::fvector v1(position.x + ch_width / 8.0f + ch_width / 5.3f, position.y, position.x + ch_width / 8.0f + ch_width / 6.3f, position.y + ch_height / 2.5f, color, true); v1.draw();
				ellipse2d::ellipsef e1(position.x + ch_width / (7.535f * 1.2f), position.y + ch_height / 4.5f, ch_width / 5.4f, ch_height / 6.5f, color, 0, 3600, false, true); e1.draw();
				ellipse2d::ellipsef e2(position.x + ch_width / (7.47f * 1.2f), position.y + ch_height / 25.0f, ch_width / 4.05f, ch_height / 4.75f, color, 700, 2100, false, true); e2.draw();
				//tail -- ellipse2d::ELLIPSE e3(position.x + ch_width+ch_width/7.4f, position.y + ch_height / 2.4f, ch_width / 4.0f, ch_height / 5.0f, color, 2700, 3600, false, true); v1.draw();
				next_pos = new Vector2f(position.x + ch_width / 5.0f + ch_width / 5.3f, position.y);
			}
			if (ch == 'h')
			{
				vector2d::fvector v1(position.x, position.y, position.x, position.y + ch_height / 1.6f, color, true); v1.draw();
				vector2d::fvector v2(position.x + ch_width / 4.0f - 0.1f, position.y, position.x + ch_width / 4.0f - 0.1f, position.y + ch_height / 4.0f, color, true); v2.draw();
				ellipse2d::ellipsef e1(position.x + ch_width / 8.0f, position.y + ch_height / 8.0f, ch_width / 8.0f, ch_height / 4.0f, color, 3000, 3600, false, true); e1.draw();
				ellipse2d::ellipsef e2(position.x + ch_width / 8.0f, position.y + ch_height / 8.0f, ch_width / 8.0f, ch_height / 4.0f, color, 0, 600, false, true); e2.draw();
				next_pos = new Vector2f(position.x + ch_width / 3.0f - 0.1f, position.y);
			}
			if (ch == 'i')
			{
				vector2d::fvector v1(position.x, position.y, position.x, position.y + ch_height / 3.0f, color, true); v1.draw();
				draw_pixel(position.x, position.y + ch_height / 2.4f, color);
				next_pos = new Vector2f(position.x + 0.1f, position.y);
			}
			if (ch == 'j')
			{
				ellipse2d::ellipsef e1(position.x - ch_width / 4.0f, position.y + ch_height / 3.0f, ch_width / 3.0f, ch_height / 1.5f, color, 900, 1700, false, true); e1.draw();
				//vector2d::fvector v1(position.x + ch_width / 8.0f-0.2f, position.y + ch_height / 8.0f - 0.8f, position.x + ch_width / 4.0f-0.2f, position.y + ch_height / 1.8f, color, true); v1.draw();
				draw_pixel(position.x + ch_width / 8.0f - 0.3f, position.y + ch_height / 2.0f, color);
				next_pos = new Vector2f(position.x + ch_width / 8.0f - 0.3f, position.y);
			}
			if (ch == 'k')
			{
				vector2d::fvector v1(position.x, position.y, position.x, position.y + ch_height / 2.5f, color, true); v1.draw();
				vector2d::fvector v2(position.x, position.y + ch_height / 4.0f, position.x + ch_width / 4.0f, position.y + ch_height / 2.5f, color, true); v2.draw();
				vector2d::fvector v3(position.x, position.y + ch_height / 4.0f, position.x + ch_width / 3.0f, position.y, color, true); v3.draw();
				next_pos = new Vector2f(position.x + ch_width / 3.0f, position.y);
			}
			if (ch == 'l')
			{
				vector2d::fvector v1(position.x, position.y, position.x, position.y + ch_height / 1.7f, color, true); v1.draw();
				next_pos = new Vector2f(position.x + 0.2f, position.y);
			}
			if (ch == 'm')
			{
				vector2d::fvector v1(position.x, position.y, position.x, position.y + ch_height / 2.5f, color, true); v1.draw();
				vector2d::fvector v2(position.x + ch_width / 4.0f, position.y, position.x + ch_width / 4.0f - 0.1f, position.y + ch_height / 4.0f, color, true); v2.draw();
				vector2d::fvector v3(position.x + ch_width / 2.0f - 0.1f, position.y, position.x + ch_width / 2.0f - 0.1f, position.y + ch_height / 4.0f, color, true); v3.draw();
				ellipse2d::ellipsef e1(position.x + ch_width / 8.2f, position.y + ch_height / 8.0f, ch_width / 8.0f, ch_height / 4.0f, color, 3000, 3600, false, true); e1.draw();
				ellipse2d::ellipsef e2(position.x + ch_width / 8.2f, position.y + ch_height / 8.0f, ch_width / 8.0f, ch_height / 4.0f, color, 0, 600, false, true); e2.draw();
				ellipse2d::ellipsef e3(position.x + ch_width / 2.76f, position.y + ch_height / 8.0f, ch_width / 8.0f, ch_height / 4.0f, color, 3000, 3600, false, true); e3.draw();
				ellipse2d::ellipsef e4(position.x + ch_width / 2.76f, position.y + ch_height / 8.0f, ch_width / 7.8f, ch_height / 4.0f, color, 0, 600, false, true); e4.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.8f - 0.1f, position.y);
			}
			if (ch == 'n')
			{
				vector2d::fvector v1(position.x, position.y, position.x, position.y + ch_height / 2.5f, color, true); v1.draw();
				vector2d::fvector v2(position.x + ch_width / 4.0f - 0.1f, position.y, position.x + ch_width / 4.0f - 0.1f, position.y + ch_height / 4.0f, color, true); v2.draw();
				ellipse2d::ellipsef e1(position.x + ch_width / 8.0f, position.y + ch_height / 8.0f, ch_width / 8.0f, ch_height / 4.0f, color, 3000, 3600, false, true); e1.draw();
				ellipse2d::ellipsef e2(position.x + ch_width / 8.0f, position.y + ch_height / 8.0f, ch_width / 8.0f, ch_height / 4.0f, color, 0, 600, false, true); e2.draw();
				next_pos = new Vector2f(position.x + ch_width / 3.0f - 0.1f, position.y);
			}
			if (ch == 'o')
			{
				ellipse2d::ellipsef e1(position.x + ch_width / 5.0f, position.y + ch_height / 5.0f, ch_width / 5.0f, ch_height / 5.0f, color, 0, 3600, false, true); e1.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.5f, position.y);
			}
			if (ch == 'p')
			{
				vector2d::fvector v1(position.x, position.y - ch_height / 4.0f, position.x, position.y + ch_height / 2.0f, color, true); v1.draw(); // straight line
				ellipse2d::ellipsef e1(position.x, position.y + (ch_height / 4.0f), ch_width / 3.0f, ch_height / 8.0f, color, 0, 1800, false, true); e1.draw(); // bottom ellipse
				next_pos = new Vector2f(position.x + ch_width / 2.0f, position.y);
			}
			if (ch == 'q')
			{
				vector2d::fvector v1(position.x + ch_width / 2.0f, position.y - ch_height / 4.0f, position.x + ch_width / 2.0f, position.y + ch_height / 2.0f, color, true); v1.draw(); // straight line
				ellipse2d::ellipsef e1(position.x + ch_width / 2.0f, position.y + (ch_height / 4.0f), ch_width / 2.0f, ch_height / 8.0f, color, 1800, 3600, false, true); e1.draw(); // bottom ellipse
				next_pos = new Vector2f(position.x + ch_width / 2.0f, position.y);
			}
			if (ch == 'r')
			{
				vector2d::fvector v1(position.x, position.y, position.x, position.y + ch_height / 2.5f, color, true); v1.draw();
				ellipse2d::ellipsef e1(position.x + ch_width / 6.0f, position.y + ch_height / 8.0f, ch_width / 6.0f, ch_height / 4.0f, color, 3000, 3600, false, true); e1.draw();
				ellipse2d::ellipsef e2(position.x + ch_width / 6.0f, position.y + ch_height / 8.0f, ch_width / 6.0f, ch_height / 4.0f, color, 0, 600, false, true); e2.draw();
				next_pos = new Vector2f(position.x + ch_width / 4.0f, position.y);
			}
			if (ch == 's')
			{
				ellipse2d::ellipsef e1(position.x, position.y + ch_height / 12.0f, ch_width / 3.0f, ch_height / 12.0f, color, 300, 1800, false, true); e1.draw(); // bottom ellipse
				ellipse2d::ellipsef e2(position.x + ch_width / 3.0f, position.y + ch_height / 3.99f, ch_width / 3.0f, ch_height / 12.0f, color, 2100, 3600, false, true); e2.draw(); // top ellipse
				next_pos = new Vector2f(position.x + ch_width / 3.0f, position.y);
			}
			if (ch == 't')
			{
				float avg = (position.x + ch_width / 4.0f) - position.x;
				vector2d::fvector v1(position.x + ch_width / 11.0f, position.y + ch_height / 9.14f, position.x + ch_width / 12.0f, position.y + ch_height / 2.0f, color, true); v1.draw(); // straight line
				vector2d::fvector v2(position.x, position.y + ch_height / 4.0f, position.x + ch_width / 4.0f, position.y + ch_height / 3.5f, color, true); v2.draw(); // top line
				ellipse2d::ellipsef e1(position.x + avg * 1.5f, position.y + ch_height / 10.0f, ch_width / 4.0f, ch_height / 8.0f, color, 2700, 2100, false, true); e1.draw();
				//vector2d::fvector v3(position.x, position.y + ch_height / 1.5f, position.x + ch_width, position.y + ch_height / 1.5f, color, true); v1.draw(); // middle line
				next_pos = new Vector2f(position.x + ch_width / 4.0f, position.y);
			}
			if (ch == 'u')
			{
				ellipse2d::ellipsef e1(position.x + ch_width / 6.0f, position.y + ch_height / 4.8f, ch_width / 6.0f, ch_height / 4.8f, color, 3000, 600, false, true); e1.draw();
				ellipse2d::ellipsef e2(position.x + (ch_width / 4.5f) + (ch_width / 1.8f), position.y + ch_height / 4.0f, ch_width / 2.4f, ch_height / 4.0f, color, 2700, 2200, false, true); e2.draw();
				next_pos = new Vector2f(position.x + (ch_width / 4.5f) + (ch_width / 4.5f), position.y);
			}
			if (ch == 'v')
			{
				vector2d::fvector v1(position.x, position.y + ch_height / 3.0f, position.x + ch_width / 4.0f, position.y, color, true); v1.draw();
				vector2d::fvector v2(v1.x2, v1.y2, position.x + ch_width / 2.0f, position.y + ch_height / 3.0f, color, true); v2.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.0f, position.y);
			}
			if (ch == 'w')
			{
				vector2d::fvector v1(position.x, position.y + ch_height / 3.0f, position.x + ch_width / 8.0f, position.y, color, true); v1.draw();
				vector2d::fvector v2(v1.x2, v1.y2, position.x + ch_width / 4.0f, position.y + ch_height / 3.0f, color, true); v2.draw();
				vector2d::fvector v3(v2.x2, v2.y2, position.x + ch_width / 2.66f, position.y, color, true); v3.draw();
				vector2d::fvector v4(v3.x2, v3.y2, position.x + ch_width / 2.0f, position.y + ch_height / 3.0f, color, true); v4.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.0f, position.y);
			}
			if (ch == 'x')
			{
				vector2d::fvector v1(position.x, position.y + ch_height / 3.0f, position.x + ch_width / 2.0f, position.y, color, true); v1.draw();
				vector2d::fvector v2(position.x, position.y, position.x + ch_width / 2.0f, position.y + ch_height / 3.0f, color, true); v2.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.0f, position.y);
			}
			if (ch == 'y')
			{
				vector2d::fvector v1(position.x, position.y + ch_height / 3.0f, position.x + ch_width / 4.0f, position.y, color, true); v1.draw();
				vector2d::fvector v2(v1.x2, v1.y2, position.x + ch_width / 2.0f, position.y + ch_height / 3.0f, color, true); v2.draw();
				ellipse2d::ellipsef e1(position.x + ch_width / 14.0f, position.y + ch_height / 3.0f, ch_width / 4.0f, ch_height / 1.5f, color, 1200, 1800, false, true); e1.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.0f, position.y);
			}
			if (ch == 'z')
			{
				vector2d::fvector v1(position.x, position.y + ch_height / 3.0f, position.x + ch_width / 3.0f, position.y + ch_height / 3.0f, color, true); v1.draw();
				vector2d::fvector v2(v1.x2, v1.y2, position.x, position.y, color, true); v2.draw();
				vector2d::fvector v3(v2.x2, v2.y2, position.x + ch_width / 3.0f, position.y, color, true); v3.draw();
				next_pos = new Vector2f(position.x + ch_width / 3.0f, position.y);
			}
			if (ch == '1')
			{
				vector2d::fvector v1(position.x, position.y, position.x, position.y + ch_height, color, true); v1.draw();
				vector2d::fvector v2(position.x - ch_width / 7.0f, position.y + ch_height / 1.3f, position.x, position.y + ch_height, color, true); v2.draw();
				next_pos = new Vector2f(position.x + ch_width / 7.0f, position.y);
			}
			if (ch == '2')
			{
				ellipse2d::ellipsef e1(position.x, position.y + ch_height / 1.9f, ch_width / 2.0f, ch_height / 2.0f, color, 0, 1800, false, true); e1.draw();
				vector2d::fvector v1(position.x, position.y, position.x + ch_width / 1.2f, position.y, color, true); v1.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.2f, position.y);
			}
			if (ch == '3')
			{
				ellipse2d::ellipsef e1(position.x, position.y + ch_height / 1.3f, ch_width / 2.0f, ch_height / 4.0f, color, 0, 1800, false, true); e1.draw();
				ellipse2d::ellipsef e2(position.x, position.y + ch_height / 3.5f, ch_width / 2.0f, ch_height / 4.0f, color, 0, 1800, false, true); e2.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.0f, position.y);
			}
			if (ch == '4')
			{
				vector2d::fvector v1(position.x, position.y + ch_height / 2.9f, position.x + ch_width / 1.5f, position.y + ch_height, color, true); v1.draw();
				vector2d::fvector v2(position.x, position.y + ch_height / 2.9f, position.x + ch_width / 1.5f, position.y + ch_height / 2.9f, color, true); v2.draw();
				vector2d::fvector v3(position.x + ch_width / 1.5f, position.y, position.x + ch_width / 1.5f, position.y + ch_height, color, true); v3.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.3f, position.y);
			}
			if (ch == '5')
			{
				vector2d::fvector v1(position.x, position.y + ch_height / 3.5f + ch_height / 4.0f, position.x, position.y + ch_height, color, true); v1.draw();
				vector2d::fvector v2(position.x, position.y + ch_height, position.x + ch_width / 2.0f, position.y + ch_height, color, true); v2.draw();
				ellipse2d::ellipsef e1(position.x, position.y + ch_height / 3.5f, ch_width / 2.0f, ch_height / 4.0f, color, 0, 1800, false, true); e1.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.0f, position.y);
			}
			if (ch == '6')
			{
				ellipse2d::ellipsef e1(position.x + ch_width / 4.0f, position.y + ch_height / 4.0f, ch_width / 4.0f, ch_height / 3.5f, color, 0, 3600, false, true); e1.draw();
				ellipse2d::ellipsef e2(position.x + ch_width / 4.0f, position.y + ch_height / 3.0f, ch_width / 4.0f, ch_height / 1.5f, color, 0, 400, false, true); e2.draw();
				ellipse2d::ellipsef e3(position.x + ch_width / 4.0f, position.y + ch_height / 3.0f, ch_width / 4.0f, ch_height / 1.5f, color, 2700, 3600, false, true); e3.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.5f, position.y);
			}
			if (ch == '7')
			{
				vector2d::fvector v1(position.x, position.y + ch_height, position.x + ch_width / 2.0f, position.y + ch_height, color, true); v1.draw();
				vector2d::fvector v2(position.x, position.y, position.x + ch_width / 2.0f, position.y + ch_height, color, true); v2.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.0f, position.y);
			}
			if (ch == '8')
			{
				ellipse2d::ellipsef e1(position.x + ch_width / 4.0f, position.y + ch_height / 1.3f, ch_width / 3.75f, ch_height / 3.75f, color, 0, 3600, false, true); e1.draw();
				ellipse2d::ellipsef e2(position.x + ch_width / 4.0f, position.y + ch_height / 4.0f, ch_width / 3.75f, ch_height / 3.75f, color, 0, 3600, false, true); e2.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.0f, position.y);
			}
			if (ch == '9')
			{
				ellipse2d::ellipsef e1(position.x + ch_width / 4.0f, position.y + ch_height / 1.3f, ch_width / 3.5f, ch_height / 3.5f, color, 0, 3600, false, true); e1.draw();
				ellipse2d::ellipsef e2(position.x + ch_width / 4.0f, position.y + ch_height / 1.5f, ch_width / 3.5f, ch_height / 1.5f, color, 900, 1800, false, true); e2.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.0f, position.y);
			}
			if (ch == '0')
			{
				ellipse2d::ellipsef e1(position.x + ch_width / 5.0f, position.y + ch_height / 2.0f, ch_width / 3.0f, ch_height / 2.0f, color, 0, 3600, false, true); e1.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.1f, position.y);
			}
			if (ch == '.')
			{
				ellipse2d::ellipsef e1(1073.0f, 0.0f, ch_width / 10.0f, ch_height / 10.0f, color, 0, 3600, true, true); e1.draw();
				next_pos = new Vector2f(position.x + ch_width / 24.0f, position.y);
			}
			if (ch == '-')
			{
				vector2d::fvector v1(position.x, position.y + ch_height / 2.0f, position.x + ch_width / 1.5f, position.y + ch_height / 2.0f, color, true); v1.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.3f, position.y);
			}
			if (ch == '+')
			{
				vector2d::fvector v1(position.x, position.y + ch_height / 2.0f, position.x + ch_width / 1.5f, position.y + ch_height / 2.0f, color, true); v1.draw();
				vector2d::fvector v2(position.x + ch_width / 2.0f, position.y + ch_height, position.x + ch_width / 2.0f, position.y, color, true); v2.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.3f, position.y);
			}
			if (ch == '=')
			{
				vector2d::fvector v1(position.x, position.y + ch_height / 1.5f, position.x + ch_width / 1.5f, position.y + ch_height / 1.5f, color, true); v1.draw();
				vector2d::fvector v2(position.x, position.y + ch_height / 3.0f, position.x + ch_width / 1.5f, position.y + ch_height / 3.0f, color, true); v2.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.4f, position.y);
			}
			if (ch == '!')
			{
				draw_pixel(position.x, position.y, color);
				vector2d::fvector v1(position.x, position.y + ch_height / 4.0f, position.x, position.y + ch_height, color, true); v1.draw();
				next_pos = new Vector2f(position.x + 0.1f, position.y);
			}
			if (ch == '<')
			{
				vector2d::fvector v1(position.x, position.y + ch_height / 2.0f, position.x + ch_width / 1.5f, position.y + ch_height, color, true); v1.draw();
				vector2d::fvector v2(position.x, position.y + ch_height / 2.0f, position.x + ch_width / 1.5f, position.y, color, true); v2.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.4f, position.y);
			}
			if (ch == '>')
			{
				vector2d::fvector v1(position.x, position.y + ch_height, position.x + ch_width / 1.5f, position.y + ch_height / 2.0f, color, true); v1.draw();
				vector2d::fvector v2(position.x, position.y, position.x + ch_width / 1.5f, position.y + ch_height / 2.0f, color, true); v2.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.4f, position.y);
			}
			if (ch == ',')
			{
				ellipse2d::ellipsef e1(position.x - ch_width / 36.0f, position.y - ch_width / 30.0f, ch_width / 24.0f, ch_height / 4.0f, color, 0, 2100, false, true); e1.draw();
				next_pos = new Vector2f(position.x + ch_width / 24.0f, position.y);
			}
			if (ch == '?')
			{
				draw_pixel(position.x, position.y, color);
				vector2d::fvector v1(position.x, position.y + ch_height / 3.5f, position.x, position.y + ch_height / 2.0f, color, true); v1.draw();
				ellipse2d::ellipsef e1(position.x, position.y + ch_height / 1.33f, ch_width / 4.0f, ch_height / 4.0f, color, 0, 1800, false, true); e1.draw();
				ellipse2d::ellipsef e2(position.x, position.y + ch_height / 1.33f, ch_width / 4.0f, ch_height / 4.0f, color, 3300, 3600, false, true); e2.draw();
				next_pos = new Vector2f(position.x + ch_width / 4.0f, position.y);
			}
			if (ch == '/')
			{
				vector2d::fvector v1(position.x, position.y, position.x + ch_width / 2.0f, position.y + ch_height, color, true); v1.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.0f, position.y);
			}
			if (ch == '|')
			{
				vector2d::fvector v1(position.x, position.y, position.x, position.y + ch_height, color, true); v1.draw();
				next_pos = new Vector2f(position.x + 0.1f, position.y);
			}
			if (ch == '(')
			{
				ellipse2d::ellipsef e1(position.x + ch_width / 4.0f, position.y + ch_height / 2.0f, ch_width / 4.0f, ch_height / 2.0f, color, 1800, 3600, false, true); e1.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.0f, position.y);
			}
			if (ch == ')')
			{
				ellipse2d::ellipsef e1(position.x, position.y + ch_height / 2.0f, ch_width / 4.0f, ch_height / 2.0f, color, 0, 1800, false, true); e1.draw();
				next_pos = new Vector2f(position.x + ch_width / 2.0f, position.y);
			}
			if (ch == '_')
			{
				vector2d::fvector v1(position.x, position.y, position.x + ch_width / 1.5f, position.y, color, true); v1.draw();
				next_pos = new Vector2f(position.x + ch_width / 1.5f, position.y);
			}
			if (ch == ':')
			{
				ellipse2d::ellipsef e1(position.x, position.y + ch_height / 1.7f, ch_width / 32.0f, ch_height / 32.0f, color, 0, 3600, true, true); e1.draw();
				ellipse2d::ellipsef e2(position.x, position.y + ch_height / 7.0f, ch_width / 32.0f, ch_height / 32.0f, color, 0, 3600, true, true); e2.draw();
				next_pos = new Vector2f(position.x + ch_width / 32.0f, position.y);
			}
			if (ch == '[')
			{
				vector2d::fvector v1(position.x, position.y, position.x, position.y + ch_height, color, true); v1.draw();
				vector2d::fvector v2(position.x, position.y + ch_height, position.x + ch_width / 3.0f, position.y + ch_height, color, true); v2.draw();
				vector2d::fvector v3(position.x, position.y, position.x + ch_width / 3.0f, position.y, color, true); v3.draw();
				next_pos = new Vector2f(position.x + ch_width / 3.0f, position.y);
			}
			if (ch == ']')
			{
				vector2d::fvector v1(position.x + ch_width / 3.0f, position.y, position.x + ch_width / 3.0f, position.y + ch_height, color, true); v1.draw();
				vector2d::fvector v2(position.x, position.y + ch_height, position.x + ch_width / 3.0f, position.y + ch_height, color, true); v2.draw();
				vector2d::fvector v3(position.x, position.y, position.x + ch_width / 3.0f, position.y, color, true); v3.draw();
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
		text(const char* txt, float x_, float y_, float ch_width_, float ch_height_, float ch_space_, uint32 color_, bool visible_)
		{
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
		//think about default constructor for text
		text()
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
class Sprite
{
public:
	const char* texture_path = "";
	float x = 0.0f;
	float y = 0.0f;
	bool visible = true;
	int texture_original_width = 0;
	int texture_original_height = 0;
	float texture_width = 0.0f;
	float texture_height = 0.0f;
	TextureFilter texturefilter = neutral;
	int rgb_channels = 3;
	Sprite(Texture& texture_, float x_, float y_, float img_width_, float img_height_, bool visible_)
	{
		texture_path = texture_.path;
		texturefilter = texture_.texturefilter;
		x = x_;
		y = y_;
		texture_width = img_width_;
		texture_height = img_height_;
		if (visible_)
		{
			visible = true;
			draw_sprite();
		}
	}
	Sprite(const char* path_, float x_, float y_, float img_width_, float img_height_, bool visible_)
	{
		texture_path = path_;
		x = x_;
		y = y_;
		texture_width = img_width_;
		texture_height = img_height_;
		if (visible_)
		{
			visible = true;
			draw_sprite();
		}
	}
	Sprite()
	{

	}
	~Sprite()
	{

	}
	void draw_sprite()
	{
		unsigned char* data = stbi_load(texture_path, &texture_original_width, &texture_original_height, &rgb_channels, 0);
		// ... process data if not NULL ..
		int ipx = 0;
		float y_ = y + (texture_original_height / 2.0f) * PIXEL_SIZE_MAX * texture_height; // to achieve the center pivot of the picture
		float x_ = x - (texture_original_width / 2.0f) * PIXEL_SIZE_MAX * texture_width; // to achieve the center pivot of the picture
		if (data != nullptr && texture_original_height > 0 && texture_original_width > 0)
		{
			for (int i = 0; i < texture_original_height; i++)
			{
				for (int j = 0; j < texture_original_width; j++)
				{
					unsigned int r1 = static_cast<int>(data[ipx]);
					unsigned int g1 = static_cast<int>(data[ipx + 1]);
					unsigned int b1 = static_cast<int>(data[ipx + 2]);
					uint32 col = 0;
					if (texturefilter == neutral)
					{
						col = rgb(r1, g1, b1);
					}
					else if (texturefilter == grayscale)
					{
						col = rgb((r1 + b1 + g1) / 3, (r1 + b1 + g1) / 3, (r1 + b1 + g1) / 3);
					}
					draw_pixel(x_ + j * PIXEL_SIZE_MAX * texture_width, y_ - i * PIXEL_SIZE_MAX * texture_height, col);
					ipx += 3;
					//std::cout << index << " pixel: RGB " << static_cast<int>(data[index]) << " " << static_cast<int>(data[index + 1]) << " " << static_cast<int>(data[index + 2]) << std::endl;
				}
			}
		}
		stbi_image_free(data);
	}
};