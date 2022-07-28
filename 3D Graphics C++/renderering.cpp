#define white 0xffffff
#define green 0x00ff00
#define red 0xff0000
#define blue 0x0000ff
#define yellow 0xffff00
#define black 0
#define PIXEL_SIZE_DEF 0.01f
#define PIXEL_SIZE_TRIANGLE 0.1f
#define VECTOR_HQ 0.01f
#define VECTOR_MQ 0.05f
#define VECTOR_LQ 0.1f
#define PIXEL_SIZE_STLN 0.1f
#define pi 3.14f
#define d_pi 6.28f
#define CHANNEL_NUM 3

static float render_scale = 0.01f;
static float gravity = -9.81f;
static float Gravitational_constant = 0.000000000016f;
//Set window screen color
inline static void update_screen(unsigned int color) // processing screen visibility. This function is done to place update_screen function to the bottom of the code.
{
	screen.update_screen = true;
	screen.scr_refresh_color = color;
}
inline static void refresh_screen(unsigned int color)
{
	if (screen.update_screen)
	{
		unsigned int* pixel = (unsigned int*)render.memory;
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
inline static void draw_pixel(float x, float y, unsigned int color)
{
	float x_pixel_size = PIXEL_SIZE_DEF;
	float y_pixel_size = PIXEL_SIZE_DEF;
	x *= render.height * render_scale;
	y *= render.height * render_scale;
	x_pixel_size *= render.height * render_scale;
	y_pixel_size *= render.height * render_scale;

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

	unsigned int* pixel = (unsigned int*)render.memory + x0 + y0 * render.width;
	*pixel++ = color;
}
class VERTEX
{
public:
	float x;
	float y;
};
namespace rectangle
{
	class RECT
	{
	public:
		float x;
		float y;
		float a;
		float b;
		unsigned int color;
		//Draw rectangle
		RECT(float x_, float y_, float a_, float b_, unsigned int color_, bool visible = true)
		{
			x = x_;
			y = y_;
			a = a_;
			b = b_;
			color = color_;
			if (visible == true)
			{
				draw_rect(x_, y_, a_, b_, color_);
			}
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
		void draw()
		{
			draw_rect(x, y, a, b, color);
		}
	private:
		void draw_rect(float x, float y, float half_size_x, float half_size_y, unsigned int color)
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
				unsigned int* pixel = (unsigned int*)render.memory + x0 + y * render.width;
				for (int x = x0; x < x1; x++)
				{
					*pixel++ = color;
				}
			}
		}
	};
}
namespace vector2d
{
	class VECTOR
	{
	private:
		void check(float x1, float x2, float y1, float y2, float slope_x_modf, float slope_y_modf, float& slope_x_incr, float& slope_y_incr, bool directions[4])
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
		void draw_vector(float x1, float y1, float x2, float y2, float vectorQuality, unsigned int color)
		{
			bool x_y_outweight = false, xy_swap = false; // if x2>y2, false, y2>x2, true; if x2&&y2 < x1&&y1 swap them
			bool straight_line_drawing = false;
			bool draw_one_pixel = false;
			int straight_line_type = -1;
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
				if (highlight_vertexes)
				{
					for (int i = 0; i < 5; i++)
					{
						draw_pixel(x1, y1 + i, highlightColor);
						draw_pixel(x2, y2 + i, highlightColor);
					}
					draw_pixel(0.0f, 0.0f, highlightColor);
				}
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
					max_pixels = adx * 100.0f;
				}
				else if (ady > adx)
				{
					max_pixels = ady * 100.0f;
				}
				else if (adx == ady)
				{
					max_pixels = ady * 100.0f; // or max_pixels = ((y2 - y1) * 100) - 1; there is no matter
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
								if (check_number_type(slope_x) == false)// floor(fabs(slope_y)) will detect the last loop and that's why there is ceil in for-i loop statement.
								{ //if statement checks the last loop of for-i loop because only last pixel's size must be changed in x_cycle or y_cycle
									if (x == floor(fabs(slope_x)))
									{
										slope_x_incr = (fabs(slope_x) - floor(fabs(slope_x))) * vectorQuality * direction_modifier_x;
									}
									else if (x != floor(fabs(slope_x)))// if the loop is not the last then it doesn't change pixels' size.
									{
										slope_x_incr = vectorQuality * direction_modifier_x;
									}
								}
								check(x1, x2, y1, y2, slope_x_modf, slope_y_modf, slope_x_incr, slope_y_incr, directions);
								slope_x_modf += slope_x_incr;
								draw_pixel(x1 + slope_x_modf, y1 + slope_y_modf, color);
							}
						}
						else if (x_y_outweight == true)
						{
							for (int x = 0; x < 2; x++)
							{
								float slope_x_incr = vectorQuality * direction_modifier_x, slope_y_incr = vectorQuality * direction_modifier_y;
								draw_pixel(x1 + slope_x_modf, y1 + slope_y_modf, color);
								check(x1, x2, y1, y2, slope_x_modf, slope_y_modf, slope_x_incr, slope_y_incr, directions);
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
								if (check_number_type(slope_y) == false)
								{
									if (y == floor(fabs(slope_y))) // floor(fabs(slope_y)) will detect the last loop and that's why there is ceil in for-i loop statement.
									{ //if statement checks the last loop of for-i loop because only last pixel's size must be changed in x_cycle or y_cycle
										slope_y_incr = (fabs(slope_y) - floor(fabs(slope_y))) * vectorQuality * direction_modifier_y;
									}
									else if (y != floor(fabs(slope_y))) // if the loop is not the last then it doesn't change pixels' size.
									{
										slope_y_incr = vectorQuality * direction_modifier_y;
									}
								}
								check(x1, x2, y1, y2, slope_x_modf, slope_y_modf, slope_x_incr, slope_y_incr, directions);
								slope_y_modf += slope_y_incr;
								draw_pixel(x1 + slope_x_modf, y1 + slope_y_modf, color);
							}
						}
						else if (x_y_outweight == false)
						{
							for (int x = 0; x < 2; x++) //+2 because of < sign in for, and because when the first pixel generates at y1+slope_y_modf it will not be counted in for
							{
								float slope_x_incr = vectorQuality * direction_modifier_y, slope_y_incr = vectorQuality * direction_modifier_y;
								draw_pixel(x1 + slope_x_modf, y1 + slope_y_modf, color);
								check(x1, x2, y1, y2, slope_x_modf, slope_y_modf, slope_x_incr, slope_y_incr, directions);
								slope_y_modf += slope_y_incr;
							}
						}
					}
					float slope_x_incr = vectorQuality * direction_modifier_y, slope_y_incr = vectorQuality * direction_modifier_y;
					check(x1, x2, y1, y2, slope_x_modf, slope_y_modf, slope_x_incr, slope_y_incr, directions);
				}
			}
			else if (straight_line_drawing == true)
			{
				int max_pixels = 0;
				if (straight_line_type == 0 || straight_line_type == 1)
				{
					max_pixels = ady * 10.0f;
				}
				else if (straight_line_type == 2 || straight_line_type == 3)
				{
					max_pixels = adx * 10.0f;
				}
				for (int x = 0; x < max_pixels; x++)
				{
					if (straight_line_type == 0) //down
					{
						float slope_y_incr = PIXEL_SIZE_STLN; float slope_x_incr = PIXEL_SIZE_STLN;
						draw_pixel(x1, y1 + slope_y_modf, color);
						slope_y_modf += slope_y_incr;
					}
					else if (straight_line_type == 1) //up
					{
						float slope_y_incr = PIXEL_SIZE_STLN; float slope_x_incr = PIXEL_SIZE_STLN;
						draw_pixel(x1, y1 + slope_y_modf, color);
						slope_y_modf -= slope_y_incr;
					}
					else if (straight_line_type == 2) //right
					{
						float slope_y_incr = PIXEL_SIZE_STLN; float slope_x_incr = PIXEL_SIZE_STLN;
						draw_pixel(x1 + slope_x_modf, y1, color);
						slope_x_modf += slope_x_incr;
					}
					else if (straight_line_type == 3) //left
					{
						float slope_y_incr = PIXEL_SIZE_STLN; float slope_x_incr = PIXEL_SIZE_STLN;
						draw_pixel(x1 + slope_x_modf, y1, color);
						slope_x_modf -= slope_x_incr;
					}
					px_quantity = x;
				}
			}
		}
	public:
		int px_quantity;
		char name[20];
		float x1;
		float y1;
		float x2;
		float y2;
		bool visible;
		float vectorQuality;
		bool highlight_vertexes;
		unsigned int highlightColor;
		unsigned int color;
		int thickness_l = 1; //thickness left
		int thickness_r = 1; //thickness right
		//Draw 2d vector.
		VECTOR(float x1_, float y1_, float x2_, float y2_, unsigned int color_, bool visible_ = true)
		{
			x1 = x1_;
			y1 = y1_;
			x2 = x2_;
			y2 = y2_;
			color = color_;
			vectorQuality = PIXEL_SIZE_DEF;
			highlight_vertexes = false;
			highlightColor = white;
			if (visible_ == true)
			{
				draw();
				visible = true;
			}
		}
		VECTOR()
		{

		}
		VECTOR(VERTEX& vx1, VERTEX& vx2, unsigned int color_, bool visible_ = true)
		{
			x1 = vx1.x;
			y1 = vx1.y;;
			x2 = vx2.x;
			y2 = vx2.y;
			color = color_;
			vectorQuality = PIXEL_SIZE_DEF;
			if (visible_ == true)
			{
				draw();
				visible = true;
			}
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
		void add_vector(float x2_, float y2_)
		{
			x2 = x2_;
			y2 = y2_;
			draw();
		}
		//Draw function
		void draw()
		{
			if (visible)
			{
				for (int i = 0; i < thickness_l + thickness_r + 1; i++) // +1 because thickness_l and thickness_r are by default zero and by default normal vector must be build and therefore to start for-loop with 1 loop.
				{
					//this if-statements are to merge two expected for-loops(one for-loop is for left thickness and the second for-loop is for right thickness) to one for-loop
					if (i < thickness_l) //this is to draw left thickness
					{
						draw_vector(x1 - (0.1f * i), y1 + (0.1f * i), x2 - (0.1f * i), y2 + (0.1f * i), vectorQuality, color);
					}
					if (i > thickness_l && i < thickness_r + thickness_r)
					{
						draw_vector(x1 + (0.1f * (i - thickness_l)), y1 - (0.1f * (i - thickness_l)), x2 + (0.1f * (i - thickness_l)), y2 - (0.1f * (i - thickness_l)), vectorQuality, color);
					}
				}
			}
		}
	};
	class ANGLED_VECTOR
	{
	private:
		bool save_pixels_x_;
		bool save_pixels_y_;
		/*void angle_vector_array_fill(float xpixelmatrix[1000], float ypixelmatrix[1000], int vpixelindex, bool save_pixels_x, bool save_pixels_y)
		{
			if (save_pixels_x == true && save_pixels_y == true)
			{
				for (int i = 0; i < vpixelindex; i++)
				{
					pixelsposx[i] = xpixelmatrix[i];
					pixelsposy[i] = ypixelmatrix[i];
				}
			}
			else if (save_pixels_x == true)
			{
				for (int i = 0; i < vpixelindex; i++)
				{
					pixelsposx[i] = xpixelmatrix[i];
				}
			}
			else if (save_pixels_y == true)
			{
				for (int i = 0; i < vpixelindex; i++)
				{
					pixelsposy[i] = ypixelmatrix[i];
				}
			}
		}*/
		void angle_draw_vector(bool save_pixels_x = false, bool save_pixels_y = false) // no negative angle or length
		{
			int a = 0;
			int b = 0;
			bool xy_length = false; // if x true, y false
			bool straight_angles = false;
			float x = 0.0f;
			float y = 0.0f;
			if (angle == 360.0f)
			{
				angle = 0.0f;
				straight_angles = true;
			}
			if (angle == 0.0f)
			{
				vector2d::VECTOR v1(x1, y1, 0.0f, y1 + length, color); v1.draw();
				if (save_pixels_x == true || save_pixels_y == true)
				{
					//angle_vector_array_fill(v1.pixelsposx, v1.pixelsposy, v1.pixelindex, save_pixels_x, save_pixels_y);
				}
				px_quantity = v1.px_quantity;
				straight_angles = true;
			}
			if (angle == 45.0f)
			{
				vector2d::VECTOR v1(x1, y1, x1 + length, y1 + length, color); v1.draw();
				straight_angles = true;
				if (save_pixels_x == true || save_pixels_y == true)
				{
					//angle_vector_array_fill(v1.pixelsposx, v1.pixelsposy, v1.pixelindex, save_pixels_x, save_pixels_y);
				}
				px_quantity = v1.px_quantity;
			}
			if (angle == 90.0f)
			{
				vector2d::VECTOR v1(x1, y1, x1 + length, 0.0f, color); v1.draw();
				straight_angles = true;
				if (save_pixels_x == true || save_pixels_y == true)
				{
					//angle_vector_array_fill(v1.pixelsposx, v1.pixelsposy, v1.pixelindex, save_pixels_x, save_pixels_y);
				}
				px_quantity = v1.px_quantity;
			}
			if (angle == 135.0f)
			{
				vector2d::VECTOR v1(x1, y1, x1 + length, y1 - length, color); v1.draw();
				straight_angles = true;
				if (save_pixels_x == true || save_pixels_y == true)
				{
					//angle_vector_array_fill(v1.pixelsposx, v1.pixelsposy, v1.pixelindex, save_pixels_x, save_pixels_y);
				}
				px_quantity = v1.px_quantity;
			}
			if (angle == 180.0f)
			{
				vector2d::VECTOR v1(x1, y1, 0.0f, y1 - length, color); v1.draw();
				straight_angles = true;
				if (save_pixels_x == true || save_pixels_y == true)
				{
					//angle_vector_array_fill(v1.pixelsposx, v1.pixelsposy, v1.pixelindex, save_pixels_x, save_pixels_y);
				}
				px_quantity = v1.px_quantity;
			}
			if (angle == 225.0f)
			{
				vector2d::VECTOR v1(x1, y1, x1 - length, y1 - length, color); v1.draw();
				straight_angles = true;
				if (save_pixels_x == true || save_pixels_y == true)
				{
					//angle_vector_array_fill(v1.pixelsposx, v1.pixelsposy, v1.pixelindex, save_pixels_x, save_pixels_y);
				}
				px_quantity = v1.px_quantity;
			}
			if (angle == 270.0f)
			{
				vector2d::VECTOR v1(x1, y1, x1 - length, 0.0f, color); v1.draw();
				straight_angles = true;
				if (save_pixels_x == true || save_pixels_y == true)
				{
					//angle_vector_array_fill(v1.pixelsposx, v1.pixelsposy, v1.pixelindex, save_pixels_x, save_pixels_y);
				}
				px_quantity = v1.px_quantity;
			}
			if (angle == 315.0f)
			{
				vector2d::VECTOR v1(x1, y1, x1 - length, y1 + length, color); v1.draw();
				straight_angles = true;
				if (save_pixels_x == true || save_pixels_y == true)
				{
					//angle_vector_array_fill(v1.pixelsposx, v1.pixelsposy, v1.pixelindex, save_pixels_x, save_pixels_y);
				}
				px_quantity = v1.px_quantity;
			}
			if (straight_angles == false)
			{
				if (angle > 0.0f && angle < 45.0f)
				{
					y = length + y1;
					xy_length = false;
				}
				if (angle > 45.0f && angle < 90.0f)
				{
					x = length + x1;
					xy_length = true;
				}
				if (angle > 90.0f && angle < 135.0f)
				{
					x = length + x1;
					xy_length = true;
				}
				if (angle > 135.0f && angle < 180.0f)
				{
					y = -1.0f * (length)+y1;
					xy_length = false;
				}
				if (angle > 180.0f && angle < 225.0f)
				{
					y = -1.0f * (length)+y1;
					xy_length = false;
				}
				if (angle > 225.0f && angle < 270.0f)
				{
					x = -1.0f * (length)+x1;
					xy_length = true;
				}
				if (angle > 270.0f && angle < 315.0f)
				{
					x = -1.0f * (length)+x1;
					xy_length = true;
				}
				if (angle > 315.0f && angle < 360.0f)
				{
					y = length + y1;;
					xy_length = false;
				}
				if (xy_length == false)
				{
					x = tan(pi * angle / 180) * y;
				}
				if (xy_length == true)
				{
					y = x / (tan(pi * angle / 180));
				}
				vector2d::VECTOR v1(x1, y1, x, y, color); v1.draw();
				if (save_pixels_x == true || save_pixels_y == true)
				{
					//angle_vector_array_fill(v1.pixelsposx, v1.pixelsposy, v1.pixelindex, save_pixels_x, save_pixels_y);
				}
				px_quantity = v1.px_quantity;
			}
		}
	public:
		float pixelsposx[1000];
		float pixelsposy[1000];
		int px_quantity;
		float angle;
		float length;
		float x1, y1, x2, y2 = 0.0f;
		float vectorQuality;
		unsigned int color;
		//Angled vector draw function
		ANGLED_VECTOR(float angle_, float length_, float x1_, float y1_, unsigned int color_, bool visible = true, bool save_pixels_x = false, bool save_pixels_y = false)
		{
			x1 = x1_;
			y1 = y1_;
			angle = angle_;
			length = length_;
			color = color_;
			save_pixels_x_ = save_pixels_x;
			save_pixels_y_ = save_pixels_y;
			if (visible == true)
			{
				angle_draw_vector(save_pixels_x, save_pixels_y);
			}
		}
		//calculate X Center of a vector
		float centerx()
		{
			return (x2 - x1) / 2.0f;
		}
		//Calculate Y Center of a vector
		float centery()
		{
			return (y2 - y1) / 2.0f;
		}
		//Calculate magnitude of a vector
		float magnitude()
		{
			float adx = fabs(x2) - fabs(x1);
			float ady = fabs(y2) - fabs(y1);
			float mag = sqrt(adx * adx + ady * ady);
			return mag;
		}
		void draw()
		{
			angle_draw_vector(save_pixels_x_, save_pixels_y_);
		}
	};
	class A_VECTOR
	{
	private:
		void check(float x1, float x2, float y1, float y2, float slope_x_modf, float slope_y_modf, float& slope_x_incr, float& slope_y_incr, bool directions[4])
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

		void draw_vector(float x1, float y1, float x2, float y2, float vectorQuality, unsigned int color, bool save_pixels_matrix, bool save_pixels_matrix_x_cycle, bool save_pixels_matrix_y_cycle, float pixel_size)
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
				draw_pixel(x2, y2, color);
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
			//	draw_pixel(x1, y1 + i, PIXEL_SIZE, PIXEL_SIZE, rgb(255, 255, 0));
			//	draw_pixel(x2, y2 + i, PIXEL_SIZE, PIXEL_SIZE, rgb(255, 255, 0));
			//}
			//draw_pixel(0.0f, 0.0f, PIXEL_SIZE, PIXEL_SIZE, rgb(255, 255, 0));
			float slope_x = 1.0f, slope_y = 1.0f;
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
					max_pixels = adx * 100.0f;
				}
				else if (ady > adx)
				{
					max_pixels = ady * 100.0f;
				}
				else if (adx == ady)
				{
					max_pixels = adx * 100.0f; // or max_pixels = ((y2 - y1) * 100) - 1; there is no matter
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
								if (check_number_type(slope_x) == false)// floor(fabs(slope_y)) will detect the last loop and that's why there is ceil in for-i loop statement.
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
									px_quantity = i;
									break_loop = true;
									break;
								}
								slope_x_modf += slope_x_incr;
								draw_pixel(x1 + slope_x_modf, y1 + slope_y_modf, color);
								if (save_pixels_matrix == true)
								{
									matrix_pixels[g].x = x1 + slope_x_modf;
									matrix_pixels[g].y = y1 + slope_y_modf;
									g++;
								}
								if (save_pixels_matrix_x_cycle == true)
								{
									matrix_pixels_x_cycle[p].x = x1 + slope_x_modf;
									matrix_pixels_x_cycle[p].y = y1 + slope_y_modf;
									p++;
								}
							}
						}
						else if (x_y_outweight == true)
						{
							for (int x = 0; x < 2; x++)
							{
								float slope_x_incr = vectorQuality * direction_modifier_x, slope_y_incr = vectorQuality * direction_modifier_x;
								draw_pixel(x1 + slope_x_modf, y1 + slope_y_modf, color);
								if (save_pixels_matrix == true)
								{
									matrix_pixels[g].x = x1 + slope_x_modf;
									matrix_pixels[g].y = y1 + slope_y_modf;
									g++;
								}
								if (save_pixels_matrix_x_cycle == true)
								{
									matrix_pixels_x_cycle[p].x = x1 + slope_x_modf;
									matrix_pixels_x_cycle[p].y = y1 + slope_y_modf;
									p++;
								}
								check(x1, x2, y1, y2, slope_x_modf, slope_y_modf, slope_x_incr, slope_y_incr, directions);
								if (slope_x_incr == 0.0f && slope_y_incr == 0.0f)
								{
									px_quantity = i;
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
								if (check_number_type(slope_y) == false)
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
								draw_pixel(x1 + slope_x_modf, y1 + slope_y_modf, color);
								if (save_pixels_matrix == true)
								{
									matrix_pixels[g].x = x1 + slope_x_modf;
									matrix_pixels[g].y = y1 + slope_y_modf;
									g++;
								}
								if (save_pixels_matrix_y_cycle == true)
								{
									matrix_pixels_y_cycle[q].x = x1 + slope_x_modf;
									matrix_pixels_y_cycle[q].y = y1 + slope_y_modf;
									q++;
								}
							}
						}
						else if (x_y_outweight == false)
						{
							for (int x = 0; x < 2; x++) //+2 because of < sign in for, and because when the first pixel generates at y1+slope_y_modf it will not be counted in for
							{
								float slope_x_incr = vectorQuality * direction_modifier_y, slope_y_incr = vectorQuality * direction_modifier_y;
								draw_pixel(x1 + slope_x_modf, y1 + slope_y_modf, color);
								if (save_pixels_matrix == true)
								{
									matrix_pixels[g].x = x1 + slope_x_modf;
									matrix_pixels[g].y = y1 + slope_y_modf;
									g++;
								}
								if (save_pixels_matrix_y_cycle == true)
								{
									matrix_pixels_y_cycle[q].x = x1 + slope_x_modf;
									matrix_pixels_y_cycle[q].y = y1 + slope_y_modf;
									q++;
								}

								check(x1, x2, y1, y2, slope_x_modf, slope_y_modf, slope_x_incr, slope_y_incr, directions);
								if (slope_x_incr == 0.0f && slope_y_incr == 0.0f)
								{
									px_quantity = i;
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
						px_quantity = i;
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
					max_pixels = ady * 10.0f;
				}
				else if (straight_line_type == 2 || straight_line_type == 3)
				{
					max_pixels = adx * 10.0f;
				}
				for (int x = 0; x < max_pixels; x++)
				{
					if (straight_line_type == 0) //down
					{
						float slope_y_incr = PIXEL_SIZE_STLN;
						draw_pixel(x1, y1 + slope_y_modf, color);
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
						draw_pixel(x1, y1 + slope_y_modf, color);
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
						draw_pixel(x1 + slope_x_modf, y1, color);
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
						draw_pixel(x1 + slope_x_modf, y1, color);
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
		int px_quantity;
		int px_quantity_x_cycle;
		int px_quantity_y_cycle;
		VERTEX matrix_pixels[MAX_STORAGE_SIZE];
		VERTEX matrix_pixels_x_cycle[MAX_STORAGE_SIZE];
		VERTEX matrix_pixels_y_cycle[MAX_STORAGE_SIZE];
		char name[20];
		float x1;
		float y1;
		float x2;
		float y2;
		float vectorQuality;
		unsigned int color;
		bool visible;
		bool save_pixels_position;
		bool save_pixels_position_x_cycle;
		bool save_pixels_position_y_cycle;
		int px_quantity_x_cycle_each;
		int px_quantity_y_cycle_each;
		int thickness_l; //thickness left
		int thickness_r; //thickness right
		//Draw 2d vector.
		A_VECTOR(float x1_, float y1_, float x2_, float y2_, unsigned int color_, bool visible_ = true, int thickness_l_ = 1, int thickness_r_ = 1, bool save_pixels_position_ = false, bool save_pixels_position_x_cycle_ = false, bool save_pixels_position_y_cycle_ = false, bool save_all_pixels_from_x_y_cycles_ = false)
		{
			x1 = x1_;
			y1 = y1_;
			x2 = x2_;
			y2 = y2_;
			color = color_;
			save_pixels_position = save_pixels_position_;
			save_pixels_position_x_cycle = save_pixels_position_x_cycle_;
			save_pixels_position_y_cycle = save_pixels_position_y_cycle_;
			thickness_l = thickness_l_;
			thickness_r = thickness_r_;
			vectorQuality = PIXEL_SIZE_DEF;
			visible = visible_;
			if (visible_ == true)
			{
				draw();
			}
		}
		A_VECTOR(VERTEX& vx1, VERTEX& vx2, unsigned int color_, bool visible_ = true, int thickness_l_ = 1, int thickness_r_ = 1, bool save_pixels_position_ = false, bool save_pixels_position_x_cycle_ = false, bool save_pixels_position_y_cycle_ = false, bool save_all_pixels_from_x_y_cycles_ = false)
		{
			x1 = vx1.x;
			y1 = vx1.y;
			x2 = vx2.x;
			y2 = vx2.y;
			color = color_;
			save_pixels_position = save_pixels_position_;
			save_pixels_position_x_cycle = save_pixels_position_x_cycle_;
			save_pixels_position_y_cycle = save_pixels_position_y_cycle_;
			thickness_l = thickness_l_;
			thickness_r = thickness_r_;
			vectorQuality = PIXEL_SIZE_DEF;
			visible = visible_;
			if (visible_ == true)
			{
				draw();
			}
		}
		float magnitude()
		{
			return px_quantity;
		}
		//Calculate Vector's angle
		float angle()
		{
			float xbase = false;
			float x1 = x1;
			float y1 = y1;
			float x2_ = x2;
			float y2_ = y2;
			float ybase = false;
			bool xy_swap = false;
			float theta = atan((y2 - y1) / (x2 - x1)) * 180.0f / pi;
			fabs(x1) > fabs(x2_) && fabs(y1) > fabs(y2_) ? xy_swap = true : xy_swap = false;
			if (xy_swap == true)
			{
				swap(x2_, x1);
				swap(y2_, y1);
			}
			bool directions[4]
			{
				false, // up_right
				false, // up_left
				false, // down_right
				false // down_left
			};
			float dx = x2_ - x1;
			float dy = y2_ - y1;
			dx > 0.0f && dy > 0.0f ? directions[0] = true : directions[0] = false;
			dx < 0.0f && dy > 0.0f ? directions[1] = true : directions[1] = false;
			dx > 0.0f && dy < 0.0f ? directions[2] = true : directions[2] = false;
			dx < 0.0f && dy < 0.0f ? directions[3] = true : directions[3] = false;
			if (dx != dy)
			{
				if (directions[2] == true)
				{
					theta = 90.0f + fabs(theta);
				}
				if (directions[3] == true)
				{
					theta = 270.0f - fabs(theta);
				}
				if (directions[1] == true)
				{
					theta = 360.0f - fabs(theta);
				}
			}
			if (dx == dy)
			{
				if (directions[0] == true)
				{
					theta = 45.0f;
				}
				if (directions[2] == true)
				{
					theta = 135.0f;
				}
				if (directions[3] == true)
				{
					theta = 225.0f;
				}
				if (directions[1] == true)
				{
					theta = 315.0f;
				}
			}
			//
			return theta;
		}
		void create_projection(bool vh, bool ur, static int color) // vh - false - x, true - y; vh - true, if ur - false - left, ur - true - right; vh - false, if ur - false - bootom, if ur - true - top 
		{
			if (vh == false)
			{
				if (ur == false)
				{
					vector2d::VECTOR vprojectionx(x1, y1, x2, y1, color);
				}
				if (ur == true)
				{
					vector2d::VECTOR vprojectionx(x1, y2, x2, y2, color);
				}
			}
			if (vh == true)
			{
				if (ur == false)
				{
					vector2d::VECTOR vprojectiony(x1, y1, x1, y2, color);
				}
				if (ur == true)
				{
					vector2d::VECTOR vprojectiony(x2, y1, x2, y2, color);
				}
			}
		}
		//Draw function
		void draw()
		{
			for (int i = 0; i < thickness_l + thickness_r + 1; i++) // +1 because thickness_l and thickness_r are by default zero and by default normal vector must be build and therefore to start for-loop with 1 loop.
			{
				//this if-statements are to merge two expected for-loops(one for-loop is for left thickness and the second for-loop is for right thickness) to one for-loop
				if (i < thickness_l) //this is to draw left thickness
				{
					draw_vector(x1 - (0.1f * i), y1 + (0.1f * i), x2 - (0.1f * i), y2 + (0.1f * i), vectorQuality, color, save_pixels_position, save_pixels_position_x_cycle, save_pixels_position_y_cycle, vectorQuality);
				}
				if (i > thickness_l && i < thickness_r + thickness_r)
				{
					draw_vector(x1 + (0.1f * (i - thickness_l)), y1 - (0.1f * (i - thickness_l)), x2 + (0.1f * (i - thickness_l)), y2 - (0.1f * (i - thickness_l)), vectorQuality, color, save_pixels_position, save_pixels_position_x_cycle, save_pixels_position_y_cycle, vectorQuality);
				}
			}
		}
	};
}
inline static void draw_coordinate_grid(unsigned int color)
{
	vector2d::VECTOR v1(-150.0f, 0.0f, 150.0f, 0.0f, color, true);
	vector2d::VECTOR v2(0.0f, -50.0f, 0.0f, 50.0f, color, true);
}
namespace cube
{
	class CUBE
	{
	private:
		void draw_cube(float x, float y, float a, int r, int g, int b) // size of each side of a cube
		{
			vector2d::VECTOR v1(x, y, x - (a / 2.0f), y - (a / 2.0f), rgb(r - 10, g - 10, b - 10)); v1.draw(); px_cube += v1.magnitude();
			vector2d::VECTOR v2(x - (a / 2.0f), y - (a / 2.0f), x + (a / 2.0f), y - (a / 2.0f), rgb(r, g, b)); v2.draw(); px_cube += v2.magnitude();
			vector2d::VECTOR v3(x - (a / 2.0f), y - (a / 2.0f), x - (a / 2.0f), y + (a / 2.0f), rgb(r, g, b)); v3.draw(); px_cube += v3.magnitude();
			vector2d::VECTOR v4(x - (a / 2.0f), y + (a / 2.0f), x + (a / 2.0f), y + (a / 2.0f), rgb(r, g, b)); v4.draw(); px_cube += v4.magnitude();
			vector2d::VECTOR v5(x + (a / 2.0f), y - (a / 2.0f), x + (a / 2.0f), y + (a / 2.0f), rgb(r, g, b)); v5.draw(); px_cube += v5.magnitude();
			vector2d::VECTOR v6(x, y, x, y + a, rgb(r - 10, g - 10, b - 10)); v6.draw(); px_cube += v6.magnitude();
			vector2d::VECTOR v7(x, y + a, x + a, y + a, rgb(r, g, b)); v7.draw(); px_cube += v7.magnitude();
			vector2d::VECTOR v8(x, y + a, x - (a / 2.0f), y + (a / 2.0f), rgb(r, g, b)); v8.draw(); px_cube += v8.magnitude();
			vector2d::VECTOR v9(x, y, x + a, y, rgb(r - 10, g - 10, b - 10)); v9.draw(); px_cube += v9.magnitude();
			vector2d::VECTOR v10(x + a, y + a, x + (a / 2.0f), y + (a / 2.0f), rgb(r, g, b)); v10.draw(); px_cube += v10.magnitude();
			vector2d::VECTOR v11(x + (a / 2.0f), y - (a / 2.0f), x + a, y, rgb(r, g, b)); v11.draw(); px_cube += v11.magnitude();
			vector2d::VECTOR v12(x + a, y + a, x + a, y, rgb(r, g, b)); v12.draw(); px_cube += v12.magnitude();
			//fix bug with drawing last line
		}
	public:
		float x;
		float y;
		float a;
		float r;
		float g;
		float b;
		int px_cube;
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
	class TRIANGLE
	{
	private:
		void draw_triangle(float a, float b, float c, float h_apex, float x, float y, unsigned int color, bool filled)
		{
			vector2d::A_VECTOR v1(x - c / 2.0f, y, x + c / 2.0f, y, color, true, 1, 1, true, true, true); //c-line
			vector2d::A_VECTOR v2(x - c / 2.0f, y, (x - c / 2.0f) + a, y + h_apex, color, false, 1, 1, true, true, true); //a-line
			vector2d::A_VECTOR v3(x + c / 2.0f, y, v2.x2, v2.y2, color, false, 1, 1, true, true, true); //b-line
			v2.vectorQuality = VECTOR_LQ;
			v3.vectorQuality = VECTOR_LQ;
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
							vector2d::VECTOR v4(mxp_v1, v1y, mxp_v1, v2.matrix_pixels_x_cycle[j].y, color, true);
							//y1 is never changing because c is straight horizontal vector and there is no need to get it every time if it does not alter. Only y2 is a changing value of every a-line pixel's y.
						}
						for (int z = v2.px_quantity_x_cycle; z < v2.px_quantity_x_cycle + v3.px_quantity_x_cycle; z++)
						{
							int r = v3.px_quantity_x_cycle - (z - v2.px_quantity_x_cycle) - 1;
							float mxp_v1 = v3.matrix_pixels_x_cycle[r].x; //mxp_v1 is the pixel's x of c- and a-line and they must be equal to build straight upward vector. Thus, here program gets one variable to not get it twice in x1 and x2
							vector2d::VECTOR v4(mxp_v1, v1y, mxp_v1, v3.matrix_pixels_x_cycle[r].y, color, true);
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
					//			vector2d::VECTOR v4(mxp_v1, y, mxp_v1, v2.matrix_pixels_x_cycle[z].y, color, true);
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
					//			vector2d::VECTOR v4(mxp_v1, v2.matrix_pixels_x_cycle[z].y - bug_fx, mxp_v1, v2.matrix_pixels_x_cycle[z].y, color, true);

					//			//y1 is never changing because c is straight horizontal vector and there is no need to get it every time if it does not alter. Only y2 is a changing value of every a-line pixel's y.
					//		}
					//		for (int z = index_v3; z < index_v3 + v3.px_quantity_x_cycle - ac_modfA; z++) // v3.px_quantity_x_cycle-2
					//		{
					//			int r = z - index_v3;
					//			float mxp_v1 = v3.matrix_pixels_x_cycle[r].x; //mxp_v1 is the pixel's x of c- and a-line and they must be equal to build straight upward vector. Thus, here program gets one variable to not get it twice in x1 and x2
					//			vector2d::VECTOR v4(mxp_v1, v3.matrix_pixels_x_cycle[r].y, mxp_v1, v2.matrix_pixels_x_cycle[z].y, color, true);

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
					//			vector2d::VECTOR v4(mxp_v1, v3.matrix_pixels_x_cycle[r].y, mxp_v1, v2.matrix_pixels_x_cycle[w].y, color, true);

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
					//			vector2d::VECTOR v4(mxp_v1, y, mxp_v1, v3.matrix_pixels_x_cycle[z].y, color, true);
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
					//			vector2d::VECTOR v4(mxp_v1, v3.matrix_pixels_x_cycle[z].y - bug_fx, mxp_v1, v3.matrix_pixels_x_cycle[z].y, color, true);

					//			//y1 is never changing because c is straight horizontal vector and there is no need to get it every time if it does not alter. Only y2 is a changing value of every a-line pixel's y.
					//		}
					//		for (int z = index_v3; z < index_v3 + v2.px_quantity_x_cycle - bc_modfA-3; z++) // v3.px_quantity_x_cycle-2
					//		{
					//			int r = z - index_v3;
					//			float mxp_v1 = v2.matrix_pixels_x_cycle[r].x; //mxp_v1 is the pixel's x of c- and a-line and they must be equal to build straight upward vector. Thus, here program gets one variable to not get it twice in x1 and x2
					//			vector2d::VECTOR v4(mxp_v1, v2.matrix_pixels_x_cycle[r].y, mxp_v1, v3.matrix_pixels_x_cycle[z].y, color, true);

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
					//			vector2d::VECTOR v4(mxp_v1, v2.matrix_pixels_x_cycle[r].y, mxp_v1, v3.matrix_pixels_x_cycle[w].y, color, true);

					//			//y1 is never changing because c is straight horizontal vector and there is no need to get it every time if it does not alter. Only y2 is a changing value of every a-line pixel's y.
					//		}
					//	}
					//}
				}
			}
		}
	public:
		float a;
		float b;
		float c;
		float h_apex;
		float x;
		float y;
		char name[20];
		bool visible;
		unsigned int color;
		bool filled;
		//Draw triangle funciton. Exception: a + b > c && a + c > b && b + c > a and a,b,c,h > 0
		TRIANGLE(float a_, float b_, float c_, float h_apex_, float x_, float y_, unsigned int color_, bool filled_, bool visible = true)
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
		TRIANGLE()
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
		void draw()
		{
			if (visible)
			{
				draw_triangle(a, b, c, h_apex, x, y, color, filled);
			}
		}
	};
}
namespace ellipse2d
{
	class ELLIPSE
	{
	private:
		void draw_ellipse(float x, float y, float ra, float rb, unsigned int color, int px_quantity_, bool filled)
		{
			float angle = 0.0f;
			float j = rb; //temp_j is pre-cycle y value to check if y to place vectors is changed; j is by default is the highest point of the circle
			for (int i = 0; i < px_quantity_; i++)
			{
				int b = 0;
				float temp_j = 0.0f;
				float fx2 = 0.0f;
				float x2 = 0.0f;
				float y2 = 0.0f;
				if (i <= 1800)
				{
					float circle_trigonometric_var = angle * pi / 180.0f;
					x2 = ra * fast_sin(circle_trigonometric_var);
					y2 = rb * fast_cos(circle_trigonometric_var);
					px_right_half_matrix[i].x = x2;
					px_right_half_matrix[i].y = y2;
				}
				if (i > 1800)
				{
					x2 = x - px_right_half_matrix[i - 1801].x;
					y2 = y - px_right_half_matrix[i - 1801].y;
				}
				if (filled == true)
				{
					fx2 = fast_trunc_ellipse_function(fabs(x2)); //rounds to tenths(because of coordinate unit and pixel proportion which is 0.1f) here to place certain number of pixels for every line to fill circle
					temp_j = y2; //temp_j is temperate y value to check y current cycle status
				}
				if (filled == false)
				{
					draw_pixel(x2 + x, y2 + y, color);
				}
				if (filled == true)
				{
					if (fast_trunc_ellipse_function(temp_j) != j)
					{
						j = fast_trunc_ellipse_function(temp_j);
						// bug with lack of pixels in center
						if (i >= 900 && i <= 1000)
						{
							vector2d::VECTOR v1(fx2 + x, y, x - fx2, y, color, true);
							draw_pixel(v1.x2, v1.y1, color);
							draw_pixel(v1.x2, v1.y1 + 0.3f, color);
							draw_pixel(v1.x2, v1.y1 + 0.2f, color);
							draw_pixel(v1.x2, v1.y1 + 0.1f, color);
							draw_pixel(v1.x2, v1.y1 - 0.1f, color);
							draw_pixel(v1.x2, v1.y1 - 0.2f, color);
							draw_pixel(v1.x2, v1.y1 - 0.3f, color);
						}
						if (i <= 1800)
						{
							vector2d::VECTOR v1(fx2 + x, y2, x - fx2, y2, color, true);
						}
						//matrix_pixels_pos[i].x = x2 + x;
						//matrix_pixels_pos[i].y = y2 + y;
					}
					else
					{
						draw_pixel(x2 + x, y2 + y, color);
						//matrix_pixels_pos[i].x = x2 + x;
						//matrix_pixels_pos[i].y = y2 + y;
					}
				}
				angle += 0.1f;
			}
		}
	public:
		VERTEX px_right_half_matrix[1800];
		float x;
		float y;
		float radiusa;
		float radiusb;
		bool visible;
		char name[20];
		//VERTEX matrix_pixels_pos[1];
		bool filled;
		bool get_ellipse_additional_methods;
		unsigned int color;
		int px_circum_ = 0;
		int px_circumference;
		int px_area = 0;
		bool ignore_warnings = true;
		ELLIPSE(float x_, float y_, float radiusa_, float radiusb_, unsigned int color_, int px_circumference_, bool filled_, bool visible = true)
		{
			x = x_;
			y = y_;
			radiusa = radiusa_;
			radiusb = radiusb_;
			filled = filled_;
			//if (ignore_warnings == false)
			//{
			//		//std::call_once(onceFlag, [] { MessageBox(hWnd, "Radius of the ellipse cannot be negative.", NULL, 0x00000031L); });
			//}
			color = color_;
			px_circumference = px_circumference_;
			if (visible == true)
			{
				draw();
			}
		}
		ELLIPSE()
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
		void draw()
		{
			if (visible)
			{
				draw_ellipse(x, y, radiusa, radiusb, color, px_circumference, filled);
			}
		}
	};
}
namespace text2d
{
	class TEXT
	{
	public:

	};
}