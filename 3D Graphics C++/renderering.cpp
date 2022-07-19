
static float render_scale = 0.01f;
static float gravity = -9.81f;
static float Gravitational_constant = 0.000000000016f;
//Set window screen color
static void clear_screen(unsigned int color) // accessing struct to an argument, to contrinue
{
	unsigned int* pixel = (unsigned int*)render.memory;
	for (int y = 0; y < render.height; y++)
	{
		for (int x = 0; x < render.width; x++)
		{
			*pixel++ = color;
		}
	}
}
//Draw pixel on x, y positions
static void draw_pixel(float x, float y, float pixelsizex, float pixelsizey, unsigned int color)
{
	float x_pixel_size = pixelsizex;
	float y_pixel_size = pixelsizey;
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
		RECT(float x_, float y_, float a_, float b_, unsigned int color_, bool initial_draw = true)
		{
			x = x_;
			y = y_;
			a = a_;
			b = b_;
			color = color_;
			if (initial_draw == true)
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
		void check(float x1, float x2, float y1, float y2, float x_modifier, float y_modifier, float& x_increasement, float& y_increasement, bool directions[4])
		{
			float differencex = 0.0f;
			float differencey = 0.0f;
			if (x2 > x1)
			{
				if (directions[0] == true || directions[2] == true)
				{
					differencex = x2 - (x1 + x_modifier);
				}
				if (directions[1] == true || directions[3] == true)
				{
					differencex = x2 - (x1 - x_modifier);
				}
			}
			if (x1 > x2)
			{
				if (directions[0] == true || directions[2] == true)
				{
					differencex = x1 - (x2 + x_modifier);
				}
				if (directions[1] == true || directions[3] == true)
				{
					differencex = x1 - (x2 - x_modifier);
				}
			}
			if (y2 > y1)
			{
				if (directions[0] == true || directions[1] == true)
				{
					differencey = y2 - (y1 + y_modifier);
				}
				if (directions[2] == true || directions[3] == true)
				{
					differencey = y2 - (y1 - y_modifier);
				}
			}
			if (y1 > y2)
			{
				if (directions[0] == true || directions[1] == true)
				{
					differencey = y1 - (y2 + y_modifier);
				}
				if (directions[2] == true || directions[3] == true)
				{
					differencey = y1 - (y2 - y_modifier);
				}
			}
			if (differencex < 0.0f)
			{
				x_increasement = 0.0f;
			}
			if (differencey < 0.0f)
			{
				y_increasement = 0.0f;
			}
		}
		void draw_vector(float x1, float y1, float x2, float y2, float pixelsize, unsigned int color)
		{
			bool x_y_outweight = false, xy_swap = false; // if x2>y2, false, y2>x2, true; if x2&&y2 < x1&&y1 swap them
			bool straight_line_drawing = false;
			bool draw_just_pixel = false;
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
				draw_pixel(x2, y2, PIXEL_SIZE, PIXEL_SIZE, color);
				draw_just_pixel = true;
			}
			int i_px = 0;
			float x_modifier = 0.0f, y_modifier = 0.0f;
			float adx = fabs(x2 - x1);
			float ady = fabs(y2 - y1);
			if (straight_line_drawing == false && draw_just_pixel == false)
			{
				fabs(x1) > fabs(x2) && fabs(y1) > fabs(y2) ? xy_swap = true : xy_swap = false;
				if (xy_swap == true)
				{
					swap(x2, x1);
					swap(y2, y1);
					//swapping function
				}
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
				//for (int i = 0; i < 5; i++)
				//{
				//	draw_pixel(x1, y1 + i, PIXEL_SIZE, PIXEL_SIZE, rgb(255, 255, 0));
				//	draw_pixel(x2, y2 + i, PIXEL_SIZE, PIXEL_SIZE, rgb(255, 255, 0));
				//}
				//draw_pixel(0.0f, 0.0f, PIXEL_SIZE, PIXEL_SIZE, rgb(255, 255, 0));
				float x_addition = 1.0f, y_addition = 1.0f;
				int start_x = 0, start_y = 0;
				fabs(x2) > fabs(y2) ? x_addition = make_float_divisible(fabs(adx), fabs(ady)) * 2.0f : y_addition = make_float_divisible(fabs(ady), fabs(adx)) * 2.0f;
				fabs(x2) > fabs(y2) ? x_y_outweight = false : x_y_outweight = true;
				fabs(x2) > fabs(y2) ? start_x = 1 : start_y = 1;
				float cl_x_add = ceil(x_addition);
				float cl_y_add = ceil(y_addition);
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
								float x_increasement = PIXEL_SIZE_CU * direction_modifier_x, y_increasement = PIXEL_SIZE_CU * direction_modifier_y;
								if (check_number_type(x_addition) == false)// floor(fabs(y_addition)) will detect the last loop and that's why there is ceil in for-i loop statement.
								{ //if statement checks the last loop of for-i loop because only last pixel's size must be changed in x_cycle or y_cycle
									if (x == floor(fabs(x_addition)))
									{
										x_increasement = (fabs(x_addition) - floor(fabs(x_addition))) * PIXEL_SIZE_CU * direction_modifier_x;
									}
									else if (x != floor(fabs(x_addition)))// if the loop is not the last then it doesn't change pixels' size.
									{
										x_increasement = PIXEL_SIZE_CU * direction_modifier_x;
									}
								}
								check(x1, x2, y1, y2, x_modifier, y_modifier, x_increasement, y_increasement, directions);
								if (x_increasement == 0.0f && y_increasement == 0.0f)
								{
									px_quantity = i;
									break_loop = true;
									break;
								}
								x_modifier += x_increasement;
								if (x == floor(fabs(x_addition)))
								{
									draw_pixel(x1 + x_modifier, y1 + y_modifier, PIXEL_SIZE, PIXEL_SIZE, color);
								}
							}
						}
						else if (x_y_outweight == true)
						{
							for (int x = 0; x < 2; x++)
							{
								float x_increasement = PIXEL_SIZE_CU * direction_modifier_x, y_increasement = PIXEL_SIZE_CU * direction_modifier_y;
								draw_pixel(x1 + x_modifier, y1 + y_modifier, PIXEL_SIZE, PIXEL_SIZE, color);
								check(x1, x2, y1, y2, x_modifier, y_modifier, x_increasement, y_increasement, directions);
								if (x_increasement == 0.0f && y_increasement == 0.0f)
								{
									px_quantity = i;
									break_loop = true;
									break;
								}
								x_modifier += x_increasement;
							}
						}
					}
					if (y_cycle == true)
					{
						if (x_y_outweight == true)
						{
							for (int y = 0; y < cl_y_add; y++)
							{
								float x_increasement = PIXEL_SIZE_CU * direction_modifier_y, y_increasement = PIXEL_SIZE_CU * direction_modifier_y;
								if (check_number_type(y_addition) == false)
								{
									if (y == floor(fabs(y_addition))) // floor(fabs(y_addition)) will detect the last loop and that's why there is ceil in for-i loop statement.
									{ //if statement checks the last loop of for-i loop because only last pixel's size must be changed in x_cycle or y_cycle
										y_increasement = (fabs(y_addition) - floor(fabs(y_addition))) * PIXEL_SIZE_CU * direction_modifier_y;
									}
									else if (y != floor(fabs(y_addition))) // if the loop is not the last then it doesn't change pixels' size.
									{
										y_increasement = PIXEL_SIZE_CU * direction_modifier_y;
									}
								}
								check(x1, x2, y1, y2, x_modifier, y_modifier, x_increasement, y_increasement, directions);
								if (x_increasement == 0.0f && y_increasement == 0.0f)
								{
									px_quantity = i;
									break_loop = true;
									break;
								}
								y_modifier += y_increasement;
								if (y == floor(fabs(x_addition)))
								{
									draw_pixel(x1 + x_modifier, y1 + y_modifier, PIXEL_SIZE, PIXEL_SIZE, color);
								}
							}
						}
						else if (x_y_outweight == false)
						{
							for (int x = 0; x < 2; x++) //+2 because of < sign in for, and because when the first pixel generates at y1+y_modifier it will not be counted in for
							{
								float x_increasement = PIXEL_SIZE_CU * direction_modifier_y, y_increasement = PIXEL_SIZE_CU * direction_modifier_y;
								draw_pixel(x1 + x_modifier, y1 + y_modifier, PIXEL_SIZE, PIXEL_SIZE, color);
								check(x1, x2, y1, y2, x_modifier, y_modifier, x_increasement, y_increasement, directions);
								if (x_increasement == 0.0f && y_increasement == 0.0f)
								{
									px_quantity = i;
									break_loop = true;
									break;
								}
								y_modifier += y_increasement;
							}
						}
					}
					float x_increasement = PIXEL_SIZE_CU * direction_modifier_y, y_increasement = PIXEL_SIZE_CU * direction_modifier_y;
					check(x1, x2, y1, y2, x_modifier, y_modifier, x_increasement, y_increasement, directions);
					if (x_increasement == 0.0f && y_increasement == 0.0f)
					{
						px_quantity = i;
						break_loop = true;
						break;
					}
					i_px = i;
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
						float y_increasement = PIXEL_SIZE_STLN; float x_increasement = PIXEL_SIZE_STLN;
						draw_pixel(x1, y1 + y_modifier, PIXEL_SIZE, PIXEL_SIZE, color);
						y_modifier += y_increasement;
					}
					else if (straight_line_type == 1) //up
					{
						float y_increasement = PIXEL_SIZE_STLN; float x_increasement = PIXEL_SIZE_STLN;
						draw_pixel(x1, y1 + y_modifier, PIXEL_SIZE, PIXEL_SIZE, color);
						y_modifier -= y_increasement;
					}
					else if (straight_line_type == 2) //right
					{
						float y_increasement = PIXEL_SIZE_STLN; float x_increasement = PIXEL_SIZE_STLN;
						draw_pixel(x1 + x_modifier, y1, PIXEL_SIZE, PIXEL_SIZE, color);
						x_modifier += x_increasement;
					}
					else if (straight_line_type == 3) //left
					{
						float y_increasement = PIXEL_SIZE_STLN; float x_increasement = PIXEL_SIZE_STLN;
						draw_pixel(x1 + x_modifier, y1, PIXEL_SIZE, PIXEL_SIZE, color);
						x_modifier -= x_increasement;
					}
					i_px = x;
				}
			}
			px_quantity = i_px;
		}
	public:
		int px_quantity;
		float startx;
		float starty;
		float x2;
		float y2;
		float pixelsize;
		unsigned int color;
		int thickness_l; //thickness left
		int thickness_r; //thickness right
		//Draw 2d vector.
		VECTOR(float startx_, float starty_, float x2_, float y2_, unsigned int color_, bool initial_draw = true, int thickness_l_ = 1, int thickness_r_ = 1)
		{
			startx = startx_;
			starty = starty_;
			x2 = x2_;
			y2 = y2_;
			color = color_;
			thickness_l = thickness_l_;
			thickness_r = thickness_r_;
			if (initial_draw == true)
			{
				draw();
			}
		}
		VECTOR(VERTEX& vx1, VERTEX& vx2, unsigned int color_, bool initial_draw = true)
		{
			startx = vx1.x;
			starty = vx1.y;
			x2 = vx2.x;
			y2 = vx2.y;
			color = color_;
			if (initial_draw == true)
			{
				draw();
			}
		}
		void create_projection(bool vh, bool ur, static int color) // vh - false - x, true - y; vh - true, if ur - false - left, ur - true - right; vh - false, if ur - false - bootom, if ur - true - top 
		{
			if (vh == false)
			{
				if (ur == false)
				{
					vector2d::VECTOR vprojectionx(startx, starty, x2, starty, color);
				}
				if (ur == true)
				{
					vector2d::VECTOR vprojectionx(startx, y2, x2, y2, color);
				}
			}
			if (vh == true)
			{
				if (ur == false)
				{
					vector2d::VECTOR vprojectiony(startx, starty, startx, y2, color);
				}
				if (ur == true)
				{
					vector2d::VECTOR vprojectiony(x2, starty, x2, y2, color);
				}
			}
		}
		//Calculate X Center of a vector
		float centerx()
		{
			return (x2 - startx) / 2.0f;
		}
		//Calculate Y Center of a vector
		float centery()
		{
			return (y2 - starty) / 2.0f;
		}
		//Calculate Vector's magnitude in pixels
		float magnitude()
		{
			return px_quantity + 2; // 2 is start and end pixels of the vector
		}
		//Calculate Vector's angle
		float angle()
		{
			float xbase = false;
			float x1 = startx;
			float y1 = starty;
			float x2_ = x2;
			float y2_ = y2;
			float ybase = false;
			bool xy_swap = false;
			float theta = atan((y2 - starty) / (x2 - startx)) * 180.0f / pi;
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
		//Draw function
		void draw()
		{
			for (int i = 0; i < thickness_l + thickness_r + 1; i++) // +1 because thickness_l and thickness_r are by default zero and by default normal vector must be build and therefore to start for-loop with 1 loop.
			{
				//this if-statements are to merge two expected for-loops(one for-loop is for left thickness and the second for-loop is for right thickness) to one for-loop
				if (i < thickness_l) //this is to draw left thickness
				{
					draw_vector(startx - (0.1f * i), starty + (0.1f * i), x2 - (0.1f * i), y2 + (0.1f * i), pixelsize, color);
				}
				if (i > thickness_l && i < thickness_r + thickness_r)
				{
					draw_vector(startx + (0.1f * (i - thickness_l)), starty - (0.1f * (i - thickness_l)), x2 + (0.1f * (i - thickness_l)), y2 - (0.1f * (i - thickness_l)), pixelsize, color);
				}
			}
		}
	};
	class ANGLE_VECTOR
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
		float pixelsize;
		unsigned int color;
		//Angled vector draw function
		ANGLE_VECTOR(float angle_, float length_, float x1_, float y1_, unsigned int color_, bool initial_draw = true, bool save_pixels_x = false, bool save_pixels_y = false)
		{
			x1 = x1_;
			y1 = y1_;
			angle = angle_;
			length = length_;
			color = color_;
			save_pixels_x_ = save_pixels_x;
			save_pixels_y_ = save_pixels_y;
			if (initial_draw == true)
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
	class VECTOR_SAVE_PIXELS_POSITIONS
	{
	private:
		void check(float x1, float x2, float y1, float y2, float x_modifier, float y_modifier, float& x_increasement, float& y_increasement, bool directions[4])
		{
			float differencex;
			float differencey;
			if (x2 > x1)
			{
				if (directions[0] == true || directions[2] == true)
				{
					differencex = x2 - (x1 + x_modifier);
				}
				if (directions[1] == true || directions[3] == true)
				{
					differencex = x2 - (x1 - x_modifier);
				}
			}
			if (x1 > x2)
			{
				if (directions[0] == true || directions[2] == true)
				{
					differencex = x1 - (x2 + x_modifier);
				}
				if (directions[1] == true || directions[3] == true)
				{
					differencex = x1 - (x2 - x_modifier);
				}
			}
			if (y2 > y1)
			{
				if (directions[0] == true || directions[1] == true)
				{
					differencey = y2 - (y1 + y_modifier);
				}
				if (directions[2] == true || directions[3] == true)
				{
					differencey = y2 - (y1 - y_modifier);
				}
			}
			if (y1 > y2)
			{
				if (directions[0] == true || directions[1] == true)
				{
					differencey = y1 - (y2 + y_modifier);
				}
				if (directions[2] == true || directions[3] == true)
				{
					differencey = y1 - (y2 - y_modifier);
				}
			}
			if (differencex < 0.0f)
			{
				x_increasement = 0.0f;
			}
			if (differencey < 0.0f)
			{
				y_increasement = 0.0f;
			}
		}

		void draw_vector_(float x1, float y1, float x2, float y2, float pixelsize, unsigned int color, bool save_pixels_matrix, bool save_pixels_matrix_x_cycle, bool save_pixels_matrix_y_cycle)
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
				draw_pixel(x2, y2, PIXEL_SIZE, PIXEL_SIZE, color);
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
			float adx = fabs(x2 - x1);
			float ady = fabs(y2 - y1);
			float x_modifier = 0.0f, y_modifier = 0.0f;
			for (int i = 0; i < 5; i++)
			{
				draw_pixel(x1, y1 + i, PIXEL_SIZE, PIXEL_SIZE, rgb(255, 255, 0));
				draw_pixel(x2, y2 + i, PIXEL_SIZE, PIXEL_SIZE, rgb(255, 255, 0));
			}
			draw_pixel(0.0f, 0.0f, PIXEL_SIZE, PIXEL_SIZE, rgb(255, 255, 0));
			int px_i = 0;
			float x_addition = 1.0f, y_addition = 1.0f;
			if (straight_line_drawing == false && draw_just_pixel == false)
			{
				int max_pixels = 0;
				fabs(x1) > fabs(x2) && fabs(y1) > fabs(y2) ? xy_swap = true : xy_swap = false;
				if (xy_swap == true)
				{
					swap(x2, x1);
					swap(y2, y1);
					//swapping function
				}
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
				int start_x = 0, start_y = 0;
				fabs(x2) > fabs(y2) ? x_addition = make_float_divisible(fabs(adx), fabs(ady)) * 2.0f : y_addition = make_float_divisible(fabs(ady), fabs(adx)) * 2.0f;
				fabs(x2) > fabs(y2) ? x_y_outweight = false : x_y_outweight = true;
				fabs(x2) > fabs(y2) ? start_x = 1 : start_y = 1;
				float cl_x_add = ceil(x_addition);
				float cl_y_add = ceil(y_addition);
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
								float x_increasement = PIXEL_SIZE_CU * direction_modifier_x, y_increasement = PIXEL_SIZE_CU * direction_modifier_x;
								if (check_number_type(x_addition) == false)// floor(fabs(y_addition)) will detect the last loop and that's why there is ceil in for-i loop statement.
								{ //if statement checks the last loop of for-i loop because only last pixel's size must be changed in x_cycle or y_cycle
									if (x == floor(fabs(x_addition)))
									{
										x_increasement = (fabs(x_addition) - floor(fabs(x_addition))) * PIXEL_SIZE_CU * direction_modifier_x;
									}
									else if (x != floor(fabs(x_addition)))// if the loop is not the last then it doesn't change pixels' size.
									{
										x_increasement = PIXEL_SIZE_CU * direction_modifier_x;
									}
								}
								check(x1, x2, y1, y2, x_modifier, y_modifier, x_increasement, y_increasement, directions);
								if (x_increasement == 0.0f && y_increasement == 0.0f)
								{
									px_quantity = i;
									break_loop = true;
									break;
								}
								x_modifier += x_increasement;
								if (x == floor(fabs(x_addition)))
								{
									draw_pixel(x1 + x_modifier, y1 + y_modifier, PIXEL_SIZE, PIXEL_SIZE, color);
									if (save_pixels_matrix == true)
									{
										matrix_pixels[g].x = x1 + x_modifier;
										matrix_pixels[g].y = y1 + y_modifier;
									}
									if (save_pixels_matrix_x_cycle == true)
									{
										matrix_pixels_x_cycle[p].x = x1 + x_modifier;
										matrix_pixels_x_cycle[p].y = y1 + y_modifier;
									}
									g += 1;
									p += 1;
								}
							}
						}
						else if (x_y_outweight == true)
						{
							for (int x = 0; x < 2; x++)
							{
								float x_increasement = PIXEL_SIZE_CU * direction_modifier_x, y_increasement = PIXEL_SIZE_CU * direction_modifier_x;
								draw_pixel(x1 + x_modifier, y1 + y_modifier, PIXEL_SIZE, PIXEL_SIZE, color);
								if (save_pixels_matrix == true)
								{
									matrix_pixels[g].x = x1 + x_modifier;
									matrix_pixels[g].y = y1 + y_modifier;
								}
								if (save_pixels_matrix_x_cycle == true)
								{
									matrix_pixels_x_cycle[p].x = x1 + x_modifier;
									matrix_pixels_x_cycle[p].y = y1 + y_modifier;
								}
								g += 1;
								p += 1;
								check(x1, x2, y1, y2, x_modifier, y_modifier, x_increasement, y_increasement, directions);
								if (x_increasement == 0.0f && y_increasement == 0.0f)
								{
									px_quantity = i;
									break_loop = true;
									break;
								}
								x_modifier += x_increasement;
							}
						}
					}
					if (y_cycle == true)
					{
						if (x_y_outweight == true)
						{
							for (int y = 0; y < cl_y_add; y++)
							{
								float x_increasement = PIXEL_SIZE_CU * direction_modifier_y, y_increasement = PIXEL_SIZE_CU * direction_modifier_y;
								if (check_number_type(y_addition) == false)
								{
									if (y == floor(fabs(y_addition))) // floor(fabs(y_addition)) will detect the last loop and that's why there is ceil in for-i loop statement.
									{ //if statement checks the last loop of for-i loop because only last pixel's size must be changed in x_cycle or y_cycle
										y_increasement = (fabs(y_addition) - floor(fabs(y_addition))) * PIXEL_SIZE_CU * direction_modifier_y;
									}
									else if (y != floor(fabs(y_addition))) // if the loop is not the last then it doesn't change pixels' size.
									{
										y_increasement = PIXEL_SIZE_CU * direction_modifier_y;
									}
								}
								check(x1, x2, y1, y2, x_modifier, y_modifier, x_increasement, y_increasement, directions);
								if (x_increasement == 0.0f && y_increasement == 0.0f)
								{
									px_quantity = i;
									break_loop = true;
									break;
								}
								if (x1 == 15.0f)
								{
									int h = 0;
								}
								y_modifier += y_increasement;
								if (y == floor(fabs(cl_y_add))-1)
								{
									draw_pixel(x1 + x_modifier, y1 + y_modifier, PIXEL_SIZE, PIXEL_SIZE, color);
									if (save_pixels_matrix == true)
									{
										matrix_pixels[g].x = x1 + x_modifier;
										matrix_pixels[g].y = y1 + y_modifier;
									}
									if (save_pixels_matrix_y_cycle == true)
									{
										matrix_pixels_y_cycle[q].x = x1 + x_modifier;
										matrix_pixels_y_cycle[q].y = y1 + y_modifier;
									}
									g += 1;
									q += 1;
								}
							}
						}
						else if (x_y_outweight == false)
						{
							for (int x = 0; x < 2; x++) //+2 because of < sign in for, and because when the first pixel generates at y1+y_modifier it will not be counted in for
							{
								float x_increasement = PIXEL_SIZE_CU * direction_modifier_y, y_increasement = PIXEL_SIZE_CU * direction_modifier_y;
								draw_pixel(x1 + x_modifier, y1 + y_modifier, PIXEL_SIZE, PIXEL_SIZE, color);
								if (save_pixels_matrix == true)
								{
									matrix_pixels[g].x = x1 + x_modifier;
									matrix_pixels[g].y = y1 + y_modifier;
								}
								if (save_pixels_matrix_y_cycle == true)
								{
									matrix_pixels_y_cycle[q].x = x1 + x_modifier;
									matrix_pixels_y_cycle[q].y = y1 + y_modifier;
								}
								g += 1;
								q += 1;
								check(x1, x2, y1, y2, x_modifier, y_modifier, x_increasement, y_increasement, directions);
								if (x_increasement == 0.0f && y_increasement == 0.0f)
								{
									px_quantity = i;
									break_loop = true;
									break;
								}
								y_modifier += y_increasement;
							}
						}
					}
					float x_increasement = PIXEL_SIZE_CU * direction_modifier_y, y_increasement = PIXEL_SIZE_CU * direction_modifier_y;
					check(x1, x2, y1, y2, x_modifier, y_modifier, x_increasement, y_increasement, directions);
					if (x_increasement == 0.0f && y_increasement == 0.0f)
					{
						px_quantity = i;
						break_loop = true;
						break;
					}
					px_i = i;
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
						float y_increasement = PIXEL_SIZE_STLN;
						draw_pixel(x1, y1 + y_modifier, PIXEL_SIZE, PIXEL_SIZE, color);
						if (save_pixels_matrix == true)
						{
							matrix_pixels[g].x = x1;
							matrix_pixels[g].y = y1 + y_modifier;
						}
						if (save_pixels_matrix_y_cycle == true)
						{
							matrix_pixels_y_cycle[q].x = x1;
							matrix_pixels_y_cycle[q].y = y1 + y_modifier;
						}
						q += 1;
						g += 1;
						y_modifier += y_increasement;
						if (x >= max_pixels)
						{
							px_quantity = x;
							y_increasement = 0.0f;
							y_modifier = 0.0f;
							break;
						}
					}
					else if (straight_line_type == 1) //up
					{
						float y_increasement = PIXEL_SIZE_STLN;
						draw_pixel(x1, y1 + y_modifier, PIXEL_SIZE, PIXEL_SIZE, color);
						if (save_pixels_matrix == true)
						{
							matrix_pixels[g].x = x1;
							matrix_pixels[g].y = y1 + y_modifier;
						}
						if (save_pixels_matrix_y_cycle == true)
						{
							matrix_pixels_y_cycle[q].x = x1;
							matrix_pixels_y_cycle[q].y = y1 + y_modifier;
						}
						q += 1;
						g += 1;
						y_modifier -= y_increasement;
						if (x >= max_pixels)
						{
							px_quantity = x;
							y_increasement = 0.0f;
							y_modifier = 0.0f;
							break;
						}
					}
					else if (straight_line_type == 2) //right
					{
						float x_increasement = PIXEL_SIZE_STLN;
						draw_pixel(x1 + x_modifier, y1, PIXEL_SIZE, PIXEL_SIZE, color);
						if (save_pixels_matrix == true)
						{
							matrix_pixels[g].x = x1 + x_modifier;
							matrix_pixels[g].y = y1;
						}
						if (save_pixels_matrix_x_cycle == true)
						{
							matrix_pixels_x_cycle[p].x = x1 + x_modifier;
							matrix_pixels_x_cycle[p].y = y1;
						}
						p += 1;
						g += 1;
						x_modifier += x_increasement;
						if (x >= max_pixels)
						{
							px_quantity = x;
							x_increasement = 0.0f;
							x_modifier = 0.0f;
							break;
						}
					}
					else if (straight_line_type == 3) //left
					{
						float x_increasement = PIXEL_SIZE_STLN;
						draw_pixel(x1 + x_modifier, y1, PIXEL_SIZE, PIXEL_SIZE, color);
						if (save_pixels_matrix == true)
						{
							matrix_pixels[g].x = x1 + x_modifier;
							matrix_pixels[g].y = y1;
						}
						if (save_pixels_matrix_x_cycle == true)
						{
							matrix_pixels_x_cycle[p].x = x1 + x_modifier;
							matrix_pixels_x_cycle[p].y = y1;
						}
						p += 1;
						g += 1;
						x_modifier -= x_increasement;
						if (x >= max_pixels)
						{
							px_quantity = x;
							x_increasement = 0.0f;
							x_modifier = 0.0f;
							break;
						}
					}
					px_i = x;
				}
			}
			px_quantity = px_i;
		}
	public:
		int px_quantity;
		VERTEX matrix_pixels[MAX_STORAGE_SIZE];
		VERTEX matrix_pixels_x_cycle[MAX_STORAGE_SIZE];
		VERTEX matrix_pixels_y_cycle[MAX_STORAGE_SIZE];
		float startx;
		float starty;
		float x2;
		float y2;
		float pixelsize;
		unsigned int color;
		bool save_pixels_position;
		bool save_pixels_position_x_cycle;
		bool save_pixels_position_y_cycle;
		int thickness_l; //thickness left
		int thickness_r; //thickness right
		//Draw 2d vector.
		VECTOR_SAVE_PIXELS_POSITIONS(float startx_, float starty_, float x2_, float y2_, unsigned int color_, bool initial_draw = true, int thickness_l_ = 1, int thickness_r_ = 1, bool save_pixels_position_ = false, bool save_pixels_position_x_cycle_ = false, bool save_pixels_position_y_cycle_ = false)
		{
			startx = startx_;
			starty = starty_;
			x2 = x2_;
			y2 = y2_;
			color = color_;
			save_pixels_position = save_pixels_position_;
			save_pixels_position_x_cycle = save_pixels_position_x_cycle_;
			save_pixels_position_y_cycle = save_pixels_position_y_cycle_;
			thickness_l = thickness_l_;
			thickness_r = thickness_r_;
			if (initial_draw == true)
			{
				draw_();
			}
		}
		VECTOR_SAVE_PIXELS_POSITIONS(VERTEX& vx1, VERTEX& vx2, unsigned int color_, bool initial_draw = true)
		{
			startx = vx1.x;
			starty = vx1.y;
			x2 = vx2.x;
			y2 = vx2.y;
			color = color_;
			if (initial_draw == true)
			{
				draw_();
			}
		}
		//Draw function
		void draw_()
		{
			for (int i = 0; i < thickness_l + thickness_r + 1; i++) // +1 because thickness_l and thickness_r are by default zero and by default normal vector must be build and therefore to start for-loop with 1 loop.
			{
				//this if-statements are to merge two expected for-loops(one for-loop is for left thickness and the second for-loop is for right thickness) to one for-loop
				if (i < thickness_l) //this is to draw left thickness
				{
					draw_vector_(startx - (0.1f * i), starty + (0.1f * i), x2 - (0.1f * i), y2 + (0.1f * i), pixelsize, color, save_pixels_position, save_pixels_position_x_cycle, save_pixels_position_y_cycle);
				}
				if (i > thickness_l && i < thickness_r + thickness_r)
				{
					draw_vector_(startx + (0.1f * (i - thickness_l)), starty - (0.1f * (i - thickness_l)), x2 + (0.1f * (i - thickness_l)), y2 - (0.1f * (i - thickness_l)), pixelsize, color, save_pixels_position, save_pixels_position_x_cycle, save_pixels_position_y_cycle);
				}
			}
		}
	};
}
static void draw_coordinate_grid(unsigned int color)
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
		CUBE(float x_, float y_, float a_, float r_, float g_, float b_, bool initial_draw = true)
		{
			x = x_;
			y = y_;
			a = a_;
			r = r_;
			g = g_;
			b = b_;
			if (initial_draw == true)
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
	public:
		float a;
		float b;
		float c;
		float h_apex;
		float x;
		float y;
		unsigned int color;
		bool filled;
		//Draw triangle funciton. Exception: a + b > c && a + c > b && b + c > a and a,b,c,h > 0
		TRIANGLE(float a_, float b_, float c_, float h_apex_, float x_, float y_, unsigned int color_, bool filled_, bool initial_draw = true)
		{
			a = a_;
			b = b_;
			c = c_;
			h_apex = h_apex_;
			x = x_;
			y = y_;
			color = color_;
			filled = filled_;
			if (initial_draw == true)
			{
				draw_triangle(a_, b_, c_, h_apex_, x_, y_, color_, filled_);
			}
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
		void draw_triangle(float a, float b, float c, float h_apex, float x, float y, unsigned int color, bool filled)
		{
			vector2d::VECTOR_SAVE_PIXELS_POSITIONS v1(x - c / 2.0f, y, x + c / 2.0f, y, color, true, 1, 1, true, true, true); //c-line
			vector2d::VECTOR_SAVE_PIXELS_POSITIONS v2(x - c / 2.0f, y, (x - c / 2.0f) + a, y + h_apex, color, true, 1, 1, true, true, true); //a-line
			vector2d::VECTOR_SAVE_PIXELS_POSITIONS v3(x + c/2.0f, y, v2.x2, v2.y2, color, true, 1, 1, true, true, true); //b-line
			//Drawing triangle works simply by drawing the base line(c-line), then drawing a- or b-line depending on which is bigger and connecting them to each other.
			//Filling triangle algorithm works simply by connecting all c-line pixels x, y with a-line only y_cycle pixels x, y.
			float v1y = v1.matrix_pixels[0].y; 
			if (filled == true)
			{
				if (a > b)
				{
					for (int j = 0; j < a * 10.0f; j++)
					{
						float mxp_v1 = v1.matrix_pixels[j].x; //mxp_v1 is the pixel's x of c- and a-line and they must be equal to build straight upward vector. Thus, here program gets one variable to not get it twice in x1 and x2
						vector2d::VECTOR v4(mxp_v1, v1y, mxp_v1, v2.matrix_pixels_y_cycle[j].y, color, true);
						//y1 is never changing because c is straight horizontal vector and there is no need to get it every time if it does not alter. Only y2 is a changing value of every a-line pixel's y.
					}
					for (int z = (a * 10.0f)-1; z < (a * 10.0f) + v3.px_quantity/2; z++)
					{
						int r = int(v3.px_quantity/2 - (z - ((a * 10.0f) - 1)));
						float mxp_v1 = v1.matrix_pixels[z].x; //mxp_v1 is the pixel's x of c- and a-line and they must be equal to build straight upward vector. Thus, here program gets one variable to not get it twice in x1 and x2
						vector2d::VECTOR v4(mxp_v1, v1y, mxp_v1, v3.matrix_pixels_y_cycle[r].y, color, true);
						//y1 is never changing because c is straight horizontal vector and there is no need to get it every time if it does not alter. Only y2 is a changing value of every a-line pixel's y.
					}
				}
			}
		}
	};
}
namespace ellipse2d
{
	class ELLIPSE
	{
	private:
		void draw_ellipse(float x, float y, float ra, float rb, unsigned int color, int px_quantity_, bool filled, bool get_ellipse_additional_methods, bool invisible = true, bool save_pixels_x = false, bool save_pixels_y = false)
		{
			float angle = 0.0f;
			bool middle_line_drawn = false;
			float j = rb; //temp_j is pre-cycle y value to check if y to place vectors is changed; j is by default is the highest point of the circle from circle is starting to be drawn
			for (int i = 0; i < px_quantity_; i++)
			{
				int b = 0;
				float temp_j = 0.0f;
				float fx2 = 0.0f;
				float circle_trigonometric_var = angle * pi / 180.0f;
				float x2 = ra * fast_sin(circle_trigonometric_var);
				float y2 = rb * fast_cos(circle_trigonometric_var);
				if (filled == true)
				{
					fx2 = fast_trunc_ellipse_function(fabs(x2)); //rounds to tenths(because of coordinate unit and pixel proportion which is 0.1f) here to place certain number of pixels for every line to fill circl
					temp_j = y2; //temp_j is temperate y value to check y current cycle status
				}
				if (filled == false)
				{
					draw_pixel(x2 + x, y2 + y, PIXEL_SIZE, PIXEL_SIZE, color);
				}
				if (get_ellipse_additional_methods == true)
				{
					px_circum_ += 1;
				}
				//int q = 0;
				//if (y2 >= 0.0f)
				//{
				//	q = 2; //because rounding positive number or 0 to tenths in truncA(a, must be 2)
				//}
				//else
				//{
				//	q = 3; //because rounding positive number or 0 to tenths in truncA(a, must be 3) because of sign -
				//}
				if (filled == true || get_ellipse_additional_methods == true)
				{
					if (fast_trunc_ellipse_function(temp_j) != j)
					{
						j = fast_trunc_ellipse_function(temp_j);
						if (i >= 0 && i <= 3600)
						{
							//b is value found to remove some obnoxious bugs
							if (((i >= 870 && i <= 900) || (i >= 910 && i <= 930))) // some bugs with wrong additional pixels in center
							{
								b = 4;
							}
							else
							{
								b = 0;
							}
							if ((x2 >= x - ra && x2 <= x + ra))
							{
								middle_line_drawn = true;
							}
							//if (((i >= 2700 && i <= 2710) || (i >= 870 && i <= 910))) //also fixing some strange bugs
							//{
							//	//y2 += 0.1f;
							//}
							int k = 0;
							if (i < 1800)
							{
								k = 1;
							}
							if (i >= 1800)
							{
								k = 2;
							}
							// k = 2 when drawing left hand side because right hand side was drawn whe k was 1 and then it must be 2 to draw next hand side (as left)
							//fx2 * 10 because we draw pixels and we must convert coordinate unit to pixels(because of their 0.1 proportion) 
							for (int z = 0; z < b + int(fx2 * 10.0f) * k; z++) //fx2 because fx2 is the distance from center of circle to every pixel on same y and fx2 is needed to draw specific number of pixels in every y(as lines) of circle.
							{
								if (k == 1)
								{
									if (x2 + x - z * 0.1f >= x) // if z is higher than x is to build lines only on the right hand side, fixing bugs when i <= 1800 and some pixel are placed on the left of the ellipse's center
									{
										px_area += 1;
										if (filled == true)
										{
											if (invisible == false)
											{
												draw_pixel(x2 + x - z * 0.1f, y2 + y, PIXEL_SIZE, PIXEL_SIZE, color); //to place pixels on left half of ellipse
											}
											//matrix_pixels_pos[i].x = x2 + x - z * 0.1f;
											//matrix_pixels_pos[i].y = y2 + y;
										}
									}
								}	// z * 0.1f and for cycle are representation of fast hand-made vector function 
								if (k == 2)
								{
									px_area += 1;
									if (filled == true)
									{
										if (invisible == false)
										{
											draw_pixel(x2 + x + z * 0.1f, y2 + y, PIXEL_SIZE, PIXEL_SIZE, color); //to place pixels on right half of ellipse
										}
										//matrix_pixels_pos[i].x = x2 + x + z * 0.1f;
										//matrix_pixels_pos[i].y = y2 + y;
									}
								}
								// z * 0.1f and for cycle are representation of fast hand-made vector function
							}
							matrix_pixels_pos[i].x = x2 + x;
							matrix_pixels_pos[i].y = y2 + y;
						}
					}
					else
					{
						if (invisible == false)
						{
							draw_pixel(x2 + x, y2 + y, PIXEL_SIZE, PIXEL_SIZE, color);
						}
						matrix_pixels_pos[i].x = x2 + x;
						matrix_pixels_pos[i].y = y2 + y;
					}
				}
				angle += 0.1f;
			}
		}
	public:
		float x;
		float y;
		float radiusa;
		float radiusb;
		VERTEX matrix_pixels_pos[3600];
		bool filled;
		bool get_ellipse_additional_methods;
		unsigned int color;
		int px_circum_ = 0;
		int px_circumference;
		int px_area = 0;
		bool ignore_warnings = false;
		bool invisible;
		ELLIPSE(float x_, float y_, float radiusa_, float radiusb_, unsigned int color_, int px_circumference_, bool filled_, bool ignore_warnings_, bool get_ellipse_additional_methods_, bool invisible_ = false, bool initial_draw = true)
		{
			x = x_;
			y = y_;
			radiusa = radiusa_;
			radiusb = radiusb_;
			filled = filled_;
			ignore_warnings = ignore_warnings_;
			invisible = invisible_;
			//if (ignore_warnings == false)
			//{
			//		//std::call_once(onceFlag, [] { MessageBox(hWnd, "Radius of the ellipse cannot be negative.", NULL, 0x00000031L); });
			//}
			color = color_;
			px_circumference = px_circumference_;
			get_ellipse_additional_methods = get_ellipse_additional_methods_;
			if (initial_draw == true)
			{
				draw_ellipse(x_, y_, radiusa_, radiusb_, color_, px_circumference_, filled_, get_ellipse_additional_methods_, invisible_);
			}
		}
		//Calculate circle's size
		int circumference()
		{
			return px_circum_;
		}
		int area()
		{
			return px_area;
		}
		void draw()
		{
			draw_ellipse(x, y, radiusa, radiusb, color, px_circumference, filled, get_ellipse_additional_methods, invisible);
		}
	};
}
//For text
//int main()
//{
//	char a[6] = "Hello";
//	cout << a << endl;
//	return 0;
//}
