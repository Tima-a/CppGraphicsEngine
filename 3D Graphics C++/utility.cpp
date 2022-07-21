#include <complex> //for complex numbers
#include <time.h>
const float PI = 3.1f;
const float HALF_PI = 1.5f;
const float DOUBLE_PI = 6.2f;
const float SIN_CURVE_A = 0.04f;
const float SIN_CURVE_B = 0.0f;

float fast_cos(std::complex <float> z)
{
	float cs;
	std::complex <float> value;
	float x;
	float y;

	x = std::real(z);
	y = std::imag(z);

	cs = cos(x);

	value = std::complex <float>(cs * cosh(y), -sin(x) * sinh(y));

	return std::real(value);
}
float fast_sin(std::complex <float> z)
{
	float sn;
	std::complex <float> value;
	float x;
	float y;

	x = std::real(z);
	y = std::imag(z);

	sn = sin(x);

	value = std::complex <float>(sn * cosh(y), cos(x) * sinh(y));

	return std::real(value);
}
static float clamp(float min, float val, float max)
{
	if (val < min)
	{
		return min;
	}
	if (val > max)
	{
		return max;
	}
	return val;
}
static unsigned int rgb(unsigned int r, unsigned int g, unsigned int b)
{
	unsigned int rgbnumber = (r << 16) + (g << 8) + b;
	return rgbnumber;
}
static bool neg_or_pos_num(float num)
{
	//neg - false, pos - true
	if (num >= 0.0f)
	{
		return true;
	}
	else
	{
		return false;
	}
}
static int random(int low, int high)
{
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	time_t now = time(0);
	tm ltm;
	localtime_s(&ltm, &now);
	int seconds = ltm.tm_sec;
	int diff = high - low;
	int proportion_sec = 60.0f / diff;
	int num = 1;
	int numsec = -1;
	for (int i = 0; i < diff; i++)
	{
		int proportionsec_num = proportion_sec * num;
		int proportionsec_nummin = proportion_sec * (num - 1);
		if (seconds <= proportionsec_num && seconds >= proportionsec_nummin)
		{
			numsec = num - 1;
			break;
		}
		else
		{
			num++;
		}
	}
	int randomvalue = low + numsec;
	return randomvalue;
}
static inline long long int GetTicks()
{
	LARGE_INTEGER ticks;
	if (!QueryPerformanceCounter(&ticks))
	{
		return -1;
	}
	return ticks.QuadPart;
}
float fast_trunc_ellipse_function(float num)
{
	return int(num * 10.0f) / 10.0f;
}
float truncA(float num, int digit)
{
	float numa = num;
	float numb = 1.0f;
	int numra = 0;
	float numrfa = 0.0f;
	for (int i = 0; i < digit - 1; i++)
	{
		numb *= 10.0f;
	}
	numa *= numb;
	if (num >= 0.0f)
	{
		numa += 0.5f;
	}
	if (num < 0.0f)
	{
		numa -= 0.5f;
	}
	numra = int(numa); numrfa = float(numra) / numb;
	return numrfa;
}
static int get_quantity_of_digits(float a, bool after_dot)
{
	int digits_num_after_dot = 0;
	int digits_num_before_dot = 0;
	float x = 0.0f;
	for (int i = 0; i < 1000; i++)
	{
		x = a / pow(10.0f, i); // divide number by 10 in for-i cycle and when number will be less than 1 return i as quantity of digits before dot.
		if (x < 1.0f)
		{
			digits_num_before_dot = i;
			break;
		}
	}
	for (int i = 3; i < 2003; i++)//starts with 3 because truncA(a,2) and truncA(a,1) will be always the same and function won't work if i will start with 1 or 2
	{
		float y = truncA(a, i);
		float x = truncA(a, i - 1); // when i will be more than length of digits after dot by 1 and i - 1 will be equal to length to digits after dot truncA(i) and trunc(i-1) will equal and it will show the quantity of digits after dot. Simply, truncA(3.12f, 3) and truncA(3.12f, 4) will return the same number because 3(argument) will give the value 3.12 and any other numbers higher than 3 will give the same number because there is nothing to give more that will differ from 3.12
		if (x == y)
		{
			digits_num_after_dot = i - 2;
			break;
		}
	}
	if (after_dot == false)
	{
		return digits_num_before_dot;
	}
	if (after_dot == true)
	{
		return digits_num_after_dot;
	}
}
static bool float_is_infinite(float a)
{
	if (get_quantity_of_digits(a, true) >= 39)
	{
		return true;
	}
	else
	{
		return false;
	}
}
static float make_float_divisible(float a, float b) //makes 1.66666666666666666666667 to 1.67(so it only takes two digits after dot)
{ // function works by adding from 0.1f to 0.9f until result would give not infinite fraction (Ex.: 5/3=1.6666666666667; 5.01/3=1.67)
	if (float_is_infinite(a / b))
	{
		for (int i = 0; i < 9; i++)
		{
			if (get_quantity_of_digits((a + i * 0.01f) / b, true) < 39) //39 is the min quantity of digits in infinite fraction
			{
				return (a + i * 0.01f) / b;
				break;
			}
		}
	}
	else
	{
		return a / b;
	}
}

static bool check_number_type(float num)
{
	// if decimal - true, fractional - false
	if (num == round(num))
	{
		return true; // true
	}
	else
	{
		return false; // false
	}
}
int combine_ints(int a, int b) //function works by multiplying the bigger number by 10 in power of the quantity of digits of lesser number and adding the lesser number to given result
{ //Ex:combine_ints(932,124) = 932 * 10^3 + 124 = 932000+124=932124
	int j = get_quantity_of_digits(b, false);
	int d = b;
	if (a < 0)
	{
		d = -1 * b;
	}
	if (a > 0)
	{
		d = b;
	}
	return a * pow(10, j) + d;
}
static float convert_pixels_to_metres(int px)
{
	return float(px) / 10000.0f;
}
static float convert_metres_to_pixels(float metres)
{
	return float(metres) * 10000.0f;
}
static void swap(int& a, int& b)
{
	a = a + b;
	b = a - b;
	a = a - b;
}
static void swap(float& a, float& b)
{
	a = a + b;
	b = a - b;
	a = a - b;
}
static void Break()
{
	int number = 0;
	int number2 = 5 / number;
}
