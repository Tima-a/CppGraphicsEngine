#include <complex> //for complex numbers
#include <time.h>
const float PI = 3.1f;
const float HALF_PI = 1.5f;
const float DOUBLE_PI = 6.2f;
const float SIN_CURVE_A = 0.04f;
const float SIN_CURVE_B = 0.0f;
typedef unsigned int uint32;
const int int_maxq = 10;
const double epsilon = 2.22045e-16;
#define MAX_TEXT_SIZE 1000
#define MAX_CNST_CHAR_SIZE

static inline float fast_cos(std::complex <float> z)
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
static inline float fast_sin(std::complex <float> z)
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
inline static float clamp(float min, float val, float max)
{
	if (val < min)
	{
		return min;
	}
	else if (val > max)
	{
		return max;
	}
	return val;
}
static uint32 rgb(float r, float g, float b)
{
	r = clamp(0, r, 255); g = clamp(0, g, 255); b = clamp(0, b, 255);
	uint32 r_ = (uint32)r;
	uint32 g_ = (uint32)g;
	uint32 b_ = (uint32)b;
	uint32 rgbnumber = (r_ << 16) + (g_ << 8) + b_;
	return rgbnumber;
}
static uint32 rgba(float r, float g, float b, float a)
{
	
	r = clamp(0, r, 255); g = clamp(0, g, 255); b = clamp(0, b, 255); a = clamp(0, a, 255);
	r /= (255.0f / a);
	g /= (255.0f / a);
	b /= (255.0f / a);
	uint32 r_ = (uint32)r;
	uint32 g_ = (uint32)g;
	uint32 b_ = (uint32)b;
	uint32 rgbnumber = (r_ << 16) + (g_ << 8) + b_;
	return rgbnumber;
}
inline static bool neg_or_pos_num(float num) // remove this stupid function
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
inline static int random(int low, int high)
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
struct Vector2f_r
{
	float x;
	float y;
};
Vector2f_r rotate_point(float x, float y, float origin_x, float origin_y, float angle)
{
    Vector2f_r p1;
	angle = angle * 3.14f / 180.0f;
	float d = sin(angle) * (x - origin_x);
	float t = cos(angle) * (y - origin_y);
	x = ((cos(angle) * (x - origin_x)) - (sin(angle) * (y - origin_y))) + origin_x;
	y = (d+t) + origin_y;
	p1.x = x;
	p1.y = y;
	return p1;
}
static inline float truncA(float num, int digit) //gets the number by inputted amount of digits of a number. Example: truncA(9.81, 1) will return 9. truncA(9.81, 2) will return 9,8. truncA(9.81, 3) will return 9.81.
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
int floorA(int n) //Floors integers, not floats. Example: floorA(983)=980;
{
	int n_ = n;
	while (n_ % 10 != 0) // just substracts 1 from the number until it can be divided by ten with no remainder.
	{
		n_--;
	}
	return n_;
}
int floorAA(int n)
{
	int n_ = n;
	while (n_ % 100 != 0) // just substracts 1 from the number until it can be divided by ten with no remainder.
	{
		n_--;
	}
	return n_;

}
static inline int get_quantity_of_digits(double a, bool after_dot)
{
	int digits_num_after_dot = 0;
	int digits_num_before_dot = 0;
	double x = a;
	if ((a < 1.0 || (a>-1.0&&a<0.0)) && !after_dot)
	{
		return 1;
	}
	if (!after_dot)
	{
		for (int i = 0; i < 1000; i++)
		{
			x = a / pow(10.0, i); // divide number by 10 in for-i cycle and when number will be less than 1 return i as quantity of digits before dot.
			if (x < 1.0)
			{
				digits_num_before_dot = i;
				break;
			}
		}
	}
	else if (after_dot)
	{
		for (int i = 3; i < 2003; i++)//starts with 3 because truncA(a,2) and truncA(a,1) will be always the same and function won't work if i will start with 1 or 2
		{
			double y = truncA(a, i+3);
			double x = truncA(a, i); // when i will be more than length of digits after dot by 1 and i - 1 will be equal to length to digits after dot truncA(i) and trunc(i-1) will equal and it will show the quantity of digits after dot. Simply, truncA(3.12f, 3) and truncA(3.12f, 4) will return the same number because 3(argument) will give the value 3.12 and any other numbers higher than 3 will give the same number because there is nothing to give more that will differ from 3.12
			if (x==y)
			{
				digits_num_after_dot = i - 1;
				break;
			}
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
	return digits_num_after_dot; // return something to fix the error;
}
static inline bool float_is_infinite(float a)
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
static inline float CelsiusToFarenheit(float c)
{
	return (c * 9.0f / 5.0f) + 32.0f;
}
static inline float FarenheitToCelcius(float f)
{
	return (f - 32.0f) * 5.0f/9.0f;
}
static inline float CelsiusToKelvin(float c)
{
	return c + 273.15f;
}
static inline float KelvinToCelsius(float k)
{
	return k - 273.15f;
}
static inline float FarenheitToKelvin(float f)
{
	return ((f - 32.0f) * 5.0f / 9.0f) + 273.15f;
}
static inline float KelvinToFarenheit(float k)
{
	return ((k - 273.15f) * 9.0f / 5.0f) + 32.0f;
}
static inline float make_float_divisible(float a, float b) //makes 1.66666666666666666666667 to 1.67(so it only takes two digits after dot)
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
static inline int get_text_last_index(const char* txt_)
{
	for (int s = 0; s < MAX_TEXT_SIZE; s++)
	{
		if (txt_[s] == '\0')
		{
			return s;
			break;
		}
	}
}
static inline int get_text_last_index(char* txt_)
{
	for (int s = 0; s < MAX_TEXT_SIZE; s++)
	{
		if (txt_[s] == '\0')
		{
			return s;
			break;
		}
	}
}
static inline int search_for_ch(const char* full_ch, char ch)
{
	for (int i = 0; i < get_text_last_index(full_ch); i++)
	{
		if (full_ch[i] == ch)
		{
			return i;
			break;
		}
	}
	return -1;
}
static inline int array_pos_ch_x(int ch_ascii)
{
	int n = 0;
	if (int(ch_ascii) == 45)
	{
		n = (int)ch_ascii - 45 + 78;
	}
	if (int(ch_ascii) == 126)
	{
		n = (int)ch_ascii - 126 + 80 + 2;
	}
	if (int(ch_ascii) >= 60 && int(ch_ascii) <= 62)
	{
		n = (int)ch_ascii - 60 + 79;
	}
	if (int(ch_ascii) >= 40 && int(ch_ascii) <= 43)
	{
		n = (int)ch_ascii - 40 + 74;
	}
	if (int(ch_ascii) >= 63 && int(ch_ascii) <= 64)
	{
		n = (int)ch_ascii - 63 + 73 - 1;
	}
	if (int(ch_ascii) >= 33 && int(ch_ascii) <= 37)
	{
		n = (int)ch_ascii - 33 + 68 - 1;
	}
	if (int(ch_ascii) >= 65 && int(ch_ascii) <= 90)
	{
		n = (int)ch_ascii - 65;
	}
	if (int(ch_ascii) >= 97 && int(ch_ascii) <= 122)
	{
		n = (int)ch_ascii - 97 + 26; // 25 is from lowercase letters begin.
	}
	if (int(ch_ascii) >= 48 && int(ch_ascii) <= 57)
	{
		n = (int)ch_ascii - 48 + 53 - 1; // 52 is from the numbers begin.
	}
	if (int(ch_ascii) >= 44 && int(ch_ascii) <= 47 && int(ch_ascii) != 45)
	{
		n = (int)ch_ascii - 44 + 63; // 63 is from these signs begin.
	}
	if (int(ch_ascii) == 38)
	{
		n = (int)ch_ascii - 38 + 63; // 62 is for &
	}
	return n;
}
static inline bool check_if_float_has_digits(float num)
{
	// if decimal - true, fractional - false
	float n = fmod(num, 1.0f);
	if (n == 0.0f)
	{
		return true;
	}
	else
	{
		return false;
	}
}
int dtoi(double n)
{
	int w = n * pow(10, get_quantity_of_digits(n, true));
	return w;
}
int icnstch(int& dest, int n)
{
	dest = dest * pow(10, get_quantity_of_digits(n, false)) + n;
	return dest;
}
char* append_chars(char* dest, const char* src)
{
	char* rdest = dest;

	while (*dest)
		dest++;
	while (*dest++ = *src++)
		;
	return rdest;
}
static inline const char* tocnstch(int n)
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
	uint32 fn = fabs(n);
	static char cnst_ch[30] = "";
	uint32 ch_i = 0;
	uint32 b = get_text_last_index(cnst_ch);
	for (int j = 0; j < b; j++)
	{
		cnst_ch[j] = '\0';
	}
	if (n < 0)
	{
		append_chars(cnst_ch, "-");
	}
	for (int i = get_quantity_of_digits(fn, false); i > 0; i--)
	{
		uint32 l = pow(10, i - 1);
		uint32 m = fn / l;
		ch_i = m - floorA(m);
		append_chars(cnst_ch, int_cnst_chars[ch_i]);
		//Example: 54. First digit: int(54/10)=5-floorA(5)=5; Second Digit or Last Digit: 54-floorA(54)=4;
		//Example: 8392. First Digit: int(8392/1000)=8-floorA(8)=8; Second Digit: int(8392/100)= 83 - floorA(83)=3; Third Digit: int(8392/10)=839-floorA(839)=9; Fourth Digit: int(8392/1)=8392-8390=2;
	}
	if (fn == 0)
	{
		ch_i = 0;
		append_chars(cnst_ch, int_cnst_chars[ch_i]);
	}
	return (const char*)cnst_ch;
}
static inline const char* tocnstch(double n_)
{
	//Alghoritm works by converting double to int in this way. Exmaple: dtoi(0.901) = 901; Example: dtoi(193.29103) = 19329103
	double n = fabs(n_);
	bool done = false;
	double m = n;
	int dot_place = get_quantity_of_digits(n, false);
	if (n < 0.0)
	{
		dot_place += 1;
	}
	n = dtoi(n);
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
	uint32 fn = fabs(n);
	static char cnst_ch[30] = "";
	uint32 ch_i = 0;
	uint32 b = get_text_last_index(cnst_ch);
	for (int j = 0; j < b; j++)
	{
		cnst_ch[j] = '\0';
	}
	if (n_ < 0)
	{
		append_chars(cnst_ch, "-");

	}
	if (trunc(m) == 0)
	{
		append_chars(cnst_ch, int_cnst_chars[0]);
	}
	if (m < 1.0)
	{
		append_chars(cnst_ch, ".");
		if (trunc(m) == 0)
		{
			// add additional zeroes when n > 0.0 && n < 1.0. Because, dtoi(0.01) = 1 and dtoi(0.00000000001) is also equals 1
			int x = get_quantity_of_digits(m, true);
			int y = get_quantity_of_digits(n, false);
			int zerodiff = x-y;
			//so here i get the difference in get_quantity_of_digits(m,true) and get_quantity_of_digits(dtoi(n),true) to get difference in zeros and add them 
			for (int j = 0; j < zerodiff; j++)
			{
				append_chars(cnst_ch, int_cnst_chars[0]);
			}
		}
	}
	for (int i = get_quantity_of_digits(fn, false); i > 0; i--)
	{
		if (dot_place==(get_quantity_of_digits(fn, false) - i) && m>=1.0f)
		{
			append_chars(cnst_ch, ".");
			if (trunc(m) == 0)
			{
				// add additional zeroes when n > 0.0 && n < 1.0. Because, dtoi(0.01) = 1 and dtoi(0.00000000001) is also equals 1
				int zerodiff = get_quantity_of_digits(m, true) - get_quantity_of_digits(n, true) - 1;
				//so here i get the difference in get_quantity_of_digits(m,true) and get_quantity_of_digits(dtoi(n),true) to get difference in zeros and add them 
				for (int j = 0; j < zerodiff; j++)
				{
					append_chars(cnst_ch, int_cnst_chars[0]);
				}
			}
		}
		uint32 l = pow(10, i - 1);
		uint32 m = fn / l;
		ch_i = m - floorA(m);
		append_chars(cnst_ch, int_cnst_chars[ch_i]);
	}
	if (fn == 0)
	{
		ch_i = 0;
		append_chars(cnst_ch, int_cnst_chars[ch_i]);
	}
	return (const char*)cnst_ch;
}
static inline int combine_ints(int a, int b) //function works by multiplying the bigger number by 10 in power of the quantity of digits of lesser number and adding the lesser number to given result
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
	if (a == 0 && b == 0)
	{
		a = 0;
		d = 0;
		j = 0;
	}
	if (a == 0 && b != 0)
	{
		a += b;
		d = 0;
		j = 0;
	}
	if (a != 0 && b == 0)
	{
		j = 1;
		d = 0;
	}
	return a * pow(10, j) + d;
}
static inline int cnstchtoint(const char* n)
{
	char int_cnst_chars[10] =
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
	int int_chars[10]
	{
		0,
		1,
		2,
		3,
		4,
		5,
		6,
		7,
		8,
		9,
	};
	int m = 0;
	int text_limit = get_text_last_index(n);
	for (int f = 0; f < text_limit; f++)
	{
		for (int z = 0; z < 10; z++)
		{
			if (n[f] == int_cnst_chars[z])
			{
				m = combine_ints(m, int_chars[z]);
				break;
			}
		}
	}
	return m;
}
static inline const char* sub_char(const char* full_ch, int pos1, int pos2)
{
	char sub_ch[100] = "";
	for (int i = pos1; i < pos2; i++)
	{
		append_chars(sub_ch, new char[2]{ full_ch[i], '\0' });
	}
	return sub_ch;
}
static inline bool ch_equals(const char* ch1, const char* ch2)
{
	int k = 0;
	for (int i = 0; i < get_text_last_index(ch1); i++)
	{
		if (ch1[k] == ch2[k])
		{
			k++;
		}
	}
	if (k == get_text_last_index(ch1))
	{
		return true;
	}
	else
	{
		return false;
	}
}
static inline float convert_pixels_to_metres(int px)
{
	return float(px) / 10000.0f;
}
static inline float convert_metres_to_pixels(float metres)
{
	return float(metres) * 10000.0f;
}
static inline void swap(int& a, int& b)
{
	a = a + b;
	b = a - b;
	a = a - b;
}
static inline void swap(uint32& a, uint32& b)
{
	a = a + b;
	b = a - b;
	a = a - b;
}
static inline void swap(float& a, float& b)
{
	a = a + b;
	b = a - b;
	a = a - b;
}
template <typename type>
static inline bool in_range(type x, type range_num, type range)
{
	type x_ = x;
	type range_num_ = range_num;
	if (x_ > range_num_)
	{
		swap(x_, range_num_);
	}
	int a = range_num_ - range;
	int b = x_ + range;
	if ((int)x_ > a && (int)range_num_ < b)
	{
		return true;
	}
	else
	{
		return false;
	}
}
static inline  void Break()
{
	int number = 0;
	int number2 = 5 / number;
}
