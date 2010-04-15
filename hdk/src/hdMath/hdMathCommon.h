/*
 *  hdMathCommon.h
 *  hdk
 *
 *  Created by David Petrie on 10/04/10.
 *  Copyright 2010 Hackdirt Ltd. All rights reserved.
 *
 */

#ifndef HD_MATH_COMMON_H
#define HD_MATH_COMMON_H


#include <math.h>
#include <cstdlib>

#include <stdio.h>
#include <iostream>

//typedef struct hdVec2;
//typedef struct hdVec3;

//#include "hdMathSerializable.h"




#define	hdSqrt(x)	sqrtf(x)

const float hd_2_pi = 6.2831853071796f;
const float hd_pi = 3.14159265359f;
const float hd_half_pi = 1.570796326795f;

struct hdColor4
{
	hdColor4() {}
	
	hdColor4 (float rf, float gf, float bf, float af) : 
	r((uint8)(rf * 255)), 
	g((uint8)(gf * 255)), 
	b((uint8)(bf * 255)), 
	a((uint8)(af * 255)) {}
	
	hdColor4 (uint8 r, uint8 g, uint8 b, uint8 a) : r(r), g(g), b(b), a(a) {}
	
	hdColor4 (int r, int g, int b, int a) : r((uint8)r), g((uint8)g), b((uint8)b), a((uint8)a) {}
	
	void Setf(float rf, float gf, float bf, float af)
	{
		r = (uint8)(rf * 255);
		g = (uint8)(gf * 255); 
		b = (uint8)(bf * 255); 
		a = (uint8)(af * 255);
	}
	
	void Set(uint8 r_, uint8 g_, uint8 b_, uint8 a_)
	{
		r = r_;
		g = g_;
		b = b_;
		a = a_;
	}
	
	uint8 r, g, b, a;
};


struct hdVec2
{
	hdVec2 () {}
	
	hdVec2 (hdFloat x, hdFloat y) : x(x), y(y) {}
	
	void SetZero() { x = 0.0f; y = 0.0f; }
	
	/// Set this vector to some specified coordinates.
	void Set(hdFloat x_, hdFloat y_) { x = x_; y = y_; }
	
	float Length() const
	{
#ifdef TARGET_FLOAT32_IS_FIXED
		float est = hdAbs(x) + hdAbs(y);
		if(est == 0.0f) {
			return 0.0;
		} else if(est < 0.1) {
			return (1.0/256.0) * hdVec2(x<<8, y<<8).Length();
		} else if(est < 180.0f) {
			return hdSqrt(x * x + y * y);
		} else {
			return 256.0 * (hdVec2(x>>8, y>>8).Length());
		}
#else
		return hdSqrt(x * x + y * y);
#endif 
	}
	
	/// Negate this vector.
	hdVec2 operator -() const { return hdVec2(-x, -y); }
	
	/// Add a vector to this vector.
	void operator += (const hdVec2& v)
	{
		x += v.x; y += v.y;
	}
	
	/// Subtract a vector from this vector.
	void operator -= (const hdVec2& v)
	{
		x -= v.x; y -= v.y;
	}
	
	/// Multiply this vector by a scalar.
	void operator *= (hdFloat a)
	{
		x *= a; y *= a;
	}
	
	// dot
	hdFloat operator * (const hdVec2& a) const
	{
		return x * a.x + y * a.y;
	}
	
	hdFloat x, y;
};



struct hdVec3
{
	hdVec3 () {}
	
	hdVec3 (hdFloat x, hdFloat y, hdFloat z) : x(x), y(y), z(z) {}
	
	void SetZero() { x = 0.0f; y = 0.0f; z = 0.0f; }
	
	/// Set this vector to some specified coordinates.
	void Set(hdFloat x_, hdFloat y_, hdFloat z_) { x = x_; y = y_; z = z_; }
	
	/// Negate this vector.
	hdVec3 operator -() const { hdVec3 v; v.Set(-x, -y, -z); return v; }
	
	/// Add a vector to this vector.
	void operator += (const hdVec3& v)
	{
		x += v.x; y += v.y; z += v.z;
	}
	
	/// Subtract a vector from this vector.
	void operator -= (const hdVec3& v)
	{
		x -= v.x; y -= v.y; z -= v.z;
	}
	
	/// Multiply this vector by a scalar.
	void operator *= (hdFloat a)
	{
		x *= a; y *= a; z *= a;
	}
	
	// dot product
	hdFloat operator * (const hdVec3& a) const
	{
		return x*a.x + y*a.y + z*a.z;
	}
	
	hdFloat x, y, z;
};


extern const hdVec3 kZeroVector3;


extern const hdVec2 kZeroVector2;



/// Add two vectors component-wise.
inline hdVec3 operator + (const hdVec3& a, const hdVec3& b)
{
	hdVec3 v; v.Set(a.x + b.x, a.y + b.y, a.z + b.z);
	return v;
}

/// Subtract two vectors component-wise.
inline hdVec3 operator - (const hdVec3& a, const hdVec3& b)
{
	hdVec3 v; v.Set(a.x - b.x, a.y - b.y, a.z - b.z);
	return v;
}

inline hdVec3 operator * (hdFloat s, const hdVec3& a)
{
	hdVec3 v; v.Set(s * a.x, s * a.y, s * a.z);
	return v;
}

inline bool operator == (const hdVec3& a, const hdVec3& b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z;
}




/// Add two vectors component-wise.
inline hdVec2 operator + (const hdVec2& a, const hdVec2& b)
{
	hdVec2 v; v.Set(a.x + b.x, a.y + b.y);
	return v;
}


/// Subtract two vectors component-wise.
inline hdVec2 operator - (const hdVec2& a, const hdVec2& b)
{
	hdVec2 v; v.Set(a.x - b.x, a.y - b.y);
	return v;
}


inline hdVec2 operator * (hdFloat s, const hdVec2& a)
{
	hdVec2 v; v.Set(s * a.x, s * a.y);
	return v;
}


inline bool operator == (const hdVec2& a, const hdVec2& b)
{
	return a.x == b.x && a.y == b.y;
}


inline hdVec3 hdCross(const hdVec3& a, const hdVec3& b)
{
	return hdVec3((a.y * b.z) - (a.z * b.y),
				  (a.z * b.x) - (a.x * b.z),
				  (a.x * b.y) - (a.y * b.x));
}

inline hdVec3 hdNormalize3(const hdVec3& vec)
{
	float	f;
	double temp;
	
	temp = (double)(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	temp = 1.0 / sqrt(temp);
	f = (float)temp;
	
	return hdVec3(vec.x * f, vec.y * f, vec.z * f);
}


inline hdVec2 hdNormalize(const hdVec2& vec)
{
	float length = vec.Length();
	//if (length < FLT_EPSILON)
	//{
	//	return vec;
	//}
	float invLength = 1.0f / length;
	float x = vec.x * invLength;
	float y = vec.y * invLength;
	
	return hdVec2(x,y);
}


#if 0 //(TARGET_IPHONE_SIMULATOR == 1) || (TARGET_OS_IPHONE == 1)
#define HD_SIN_TABLE_SIZE 4096
#define HD_SIN_TABLE_BIT_MASK 4095

static float step = hd_2_pi / (float)HD_SIN_TABLE_SIZE;
static float invstep = 1.0f / step;
static float sinvals[HD_SIN_TABLE_SIZE];
static bool isSinTableSetup = false;

inline void hdSetupsintable()
{
	unsigned i;
	float f = 0.0f;
	for (i = 0; i <= HD_SIN_TABLE_SIZE; i++)
    {
		sinvals[i] = sinf(f);
		f += step;
    }
	isSinTableSetup = true;
}
#endif


inline float hdSinf(const float rads)
{
#if 0 //(TARGET_IPHONE_SIMULATOR == 1) || (TARGET_OS_IPHONE == 1)
#ifdef _ARM_ARCH_7
	return sinf(rads);
#else
	if (!isSinTableSetup) hdSetupsintable();
	return sinvals[((int)(rads * invstep)) & HD_SIN_TABLE_BIT_MASK];
#endif
#else
	return sinf(rads);
#endif
}


inline float hdCosf(const float rads)
{
#if 0 //(TARGET_IPHONE_SIMULATOR == 1) || (TARGET_OS_IPHONE == 1)
#ifdef _ARM_ARCH_7
	return cosf(rads);
#else 
	if (!isSinTableSetup) hdSetupsintable();
	return sinvals[((int)((rads + hd_half_pi) * invstep)) & HD_SIN_TABLE_BIT_MASK];
#endif
#else
	return cosf(rads);
#endif
}







template <typename T>
inline T hdMin(T a, T b)
{
	return a < b ? a : b;
}

inline hdVec2 hdMin(const hdVec2& a, const hdVec2& b)
{
	hdVec2 c;
	c.x = hdMin(a.x, b.x);
	c.y = hdMin(a.y, b.y);
	return c;
}

inline hdVec3 hdMin(const hdVec3& a, const hdVec3& b)
{
	hdVec3 c;
	c.x = hdMin(a.x, b.x);
	c.y = hdMin(a.y, b.y);
	c.z = hdMin(a.z, b.z);
	return c;
}

template <typename T>
inline T hdMax(T a, T b)
{
	return a > b ? a : b;
}

inline hdVec2 hdMax(const hdVec2& a, const hdVec2& b)
{
	hdVec2 c;
	c.x = hdMax(a.x, b.x);
	c.y = hdMax(a.y, b.y);
	return c;
}

inline hdVec3 hdMax(const hdVec3& a, const hdVec3& b)
{
	hdVec3 c;
	c.x = hdMax(a.x, b.x);
	c.y = hdMax(a.y, b.y);
	c.z = hdMax(a.z, b.z);
	return c;
}


template <typename T>
inline T hdClamp(T a, T min, T max)
{
	return hdMin(max, hdMax(min, a));
}


inline hdVec2 hdVec3toVec2(const hdVec3& a)
{
	return hdVec2(a.x, a.y);
}

inline hdVec3 hdVec2toVec3(const hdVec2& a)
{
	return hdVec3(a.x, a.y, 0);
}

inline hdFloat hdAbs(hdFloat a)
{
	return a > 0.0f ? a : -a;
}


/*
 * from rgba demo crew - l4m3r copy and paste.
 */
static unsigned int mirand = 1; 
inline float sf_rand( void ) 
{ 
    unsigned int a; 
    mirand *= 16807; 
    a = (mirand&0x007fffff) | 0x40000000; 
    return( *((float*)&a) - 3.0f ); 
} 


inline float hRand()
{
	return fabs(sf_rand());
}


#define	RAND_LIMIT	32767
inline hdFloat hdRandom(hdFloat lo, hdFloat hi)
{
	hdFloat r = fabs(sf_rand()); //(hdFloat)(rand() & (RAND_LIMIT));
	//r /= RAND_LIMIT;
	r = (hi - lo) * r + lo;
	return r;
}

/*
 #define	RAND_LIMIT	32767
 inline hdFloat hdRandom(hdFloat lo, hdFloat hi)
 {
 hdFloat r = (hdFloat)(rand() & (RAND_LIMIT));
 r /= RAND_LIMIT;
 r = (hi - lo) * r + lo;
 return r;
 }*/



#endif