#ifndef GRAPH_MATH_H
#define GRAPH_MATH_H

#include <math.h>

namespace D6KGraphics
{
#ifndef M_PI   // pi
#define M_PI       3.14159265358979323846
#endif

#ifndef M_PI_2 // pi/2
#define M_PI_2     1.57079632679489661923
#endif

#ifndef M_LN2  // ln(2)
#define M_LN2      0.693147180559945309417
#endif

	inline int dst_sqr(int value)
	{
		return value * value;
	};

	inline double dst_sqr(double value)
	{
		return value * value;
	};

	inline int dst_sign(int value)
	{
		if (value == 0)
		{
			return 0;
		}

		return value > 0 ? 1 : -1;
	}

	inline double dst_sign(double value)
	{
		if (value == 0.0)
		{
			return 0.0;
		}

		return value > 0.0 ? 1.0 : -1.0;
	}

	inline int dst_round(double val)
	{
		return int((val - int(val - 0.5)) >= 0.5 ? val + 0.5 : val - 0.5);
	}

	inline double dst_frac(double value)
	{
		return value - (int)value;
	}

	inline double dst_clamp(double value, double low, double high)
	{
		return value < low ? low : (value > high ? high : value);
	}

	inline int dst_clamp(int value, int low, int high)
	{
		return value < low ? low : (value > high ? high : value);
	}

	inline double dst_clamp_to_byte(double value)
	{
		return dst_clamp(value, 0.0, 255.0);
	}

	inline double dst_distance(double dx, double dy)
	{
		return sqrt(dx * dx + dy * dy);
	}


}

 


#endif // GRAPH_MATH_H
