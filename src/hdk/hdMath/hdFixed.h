/*
 Copyright (c) 2006 Henry Strickland & Ryan Seto
 2007-2008 Tobias Weyand (modifications and extensions)
 
 Permission is hereby granted, free of charge, to any person obtaining a
 copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation
 the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
 
 (* http://www.opensource.org/licenses/mit-license.php *)
 */


/*
 
 Modified by David Petrie specifically to be used with the hd animation
 library.
 
 Renamed to hdFixed to avoid clash with mac types.
 
 */

#ifndef _HD_FIXED_H_
#define _HD_FIXED_H_

#include <stdio.h>

/*
#ifdef TARGET_IS_NDS

#include "nds.h"

#endif
 */

#define FIXED_BP        16
#define FIXED_MAX       ((1<<(32-FIXED_BP-1))-1)
#define FIXED_MIN       (-(1<<(32-FIXED_BP-1)))
#define FIXED_EPSILON   (hdFixed(0.00007f))

#define G_1_DIV_PI		20861

class hdFixed {
	
private:
	
	int	g; // the guts
	
	const static int BP= FIXED_BP;  // how many low bits are right of Binary Point
	const static int BP2= BP*2;  // how many low bits are right of Binary Point
	const static int BPhalf= BP/2;  // how many low bits are right of Binary Point
	
	double STEP();  // smallest step we can represent
	
	// for private construction via guts
	enum hdFixedRaw { RAW };
	hdFixed(hdFixedRaw, int guts);
	
#ifdef BOOST_CONFIG_HPP
	friend class boost::serialization::access;
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		float f = (* this);
		ar & f;
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		float f;
		ar & f;
		(* this) = hdFixed(f);
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()
#endif
	
public:
	
	hdFixed();
	hdFixed(const hdFixed &a);
	hdFixed(float a);
	hdFixed(double a);
	hdFixed(int a);
	hdFixed(long a);
	
	hdFixed& operator =(const hdFixed a);
	hdFixed& operator =(float a);
	hdFixed& operator =(double a);
	hdFixed& operator =(int a);
	hdFixed& operator =(long a);
	
	operator float();
	operator double();
	operator int();
	operator long();
	operator unsigned short();
	
	int raw() const;
	//operator Fixed();
	
	operator float() const;
	operator int() const;
	
	hdFixed operator +() const;
	hdFixed operator -() const;
	
	hdFixed operator +(const hdFixed a) const;
	hdFixed operator -(const hdFixed a) const;
#if 1
	// more acurate, using long long
	hdFixed operator *(const hdFixed a) const;
#else
	// faster, but with only half as many bits right of binary point
	hdFixed operator *(const hdFixed a) const;
#endif
	hdFixed operator /(const hdFixed a) const;
	
	hdFixed operator *(unsigned short a) const;
	hdFixed operator *(int a) const;
	
	hdFixed operator +(float a) const;
	hdFixed operator -(float a) const;
	hdFixed operator *(float a) const;
	hdFixed operator /(float a) const;
	
	hdFixed operator +(double a) const;
	hdFixed operator -(double a) const;
	hdFixed operator *(double a) const;
	hdFixed operator /(double a) const;
	
	hdFixed operator >>(int a) const;
	hdFixed operator <<(int a) const;
	
	hdFixed& operator +=(hdFixed a);
	hdFixed& operator -=(hdFixed a);
	hdFixed& operator *=(hdFixed a);
	hdFixed& operator /=(hdFixed a);
	
	hdFixed& operator +=(int a);
	hdFixed& operator -=(int a);
	hdFixed& operator *=(int a);
	hdFixed& operator /=(int a);
	
	hdFixed& operator +=(long a);
	hdFixed& operator -=(long a);
	hdFixed& operator *=(long a);
	hdFixed& operator /=(long a);
	
	hdFixed& operator +=(float a);
	hdFixed& operator -=(float a);
	hdFixed& operator *=(float a);
	hdFixed& operator /=(float a);
	
	hdFixed& operator +=(double a);
	hdFixed& operator -=(double a);
	hdFixed& operator *=(double a);
	hdFixed& operator /=(double a);
	
	bool operator ==(const hdFixed a) const;
	bool operator !=(const hdFixed a) const;
	bool operator <=(const hdFixed a) const;
	bool operator >=(const hdFixed a) const;
	bool operator  <(const hdFixed a) const;
	bool operator  >(const hdFixed a) const;
	
	bool operator ==(float a) const;
	bool operator !=(float a) const;
	bool operator <=(float a) const;
	bool operator >=(float a) const;
	bool operator  <(float a) const;
	bool operator  >(float a) const;
	
	bool operator ==(double a) const;
	bool operator !=(double a) const;
	bool operator <=(double a) const;
	bool operator >=(double a) const;
	bool operator  <(double a) const;
	bool operator  >(double a) const;
	
	bool operator  >(int a) const;
	bool operator  <(int a) const;
	bool operator  >=(int a) const;
	bool operator  <=(int a) const;
	
	hdFixed abs();
	hdFixed sqrt();
#ifdef TARGET_IS_NDS
	hdFixed cosf();
	hdFixed sinf();
	hdFixed tanf();
#endif
	
};

//
// Implementation
//

inline double hdFixed::STEP() { return 1.0 / (1<<BP); }  // smallest step we can represent

// for private construction via guts
inline hdFixed::hdFixed(hdFixedRaw, int guts) : g(guts) {}

inline hdFixed::hdFixed() : g(0) {}
inline hdFixed::hdFixed(const hdFixed &a) : g( a.g ) {}
inline hdFixed::hdFixed(float a) : g( int(a * (float)(1<<BP)) ) {}
inline hdFixed::hdFixed(double a) : g( int(a * (double)(1<<BP) ) ) {}
inline hdFixed::hdFixed(int a) : g( a << BP ) {}
inline hdFixed::hdFixed(long a) : g( a << BP ) {}

inline hdFixed& hdFixed::operator =(const hdFixed a) { g= a.g; return *this; }
inline hdFixed& hdFixed::operator =(float a) { g= hdFixed(a).g; return *this; }
inline hdFixed& hdFixed::operator =(double a) { g= hdFixed(a).g; return *this; }
inline hdFixed& hdFixed::operator =(int a) { g= hdFixed(a).g; return *this; }
inline hdFixed& hdFixed::operator =(long a) { g= hdFixed(a).g; return *this; }

inline hdFixed::operator float() { return g * (float)STEP(); }
inline hdFixed::operator double() { return g * (double)STEP(); }

// HACK HACK HACK
//
// Test of modification of an int cast return the
// raw guts so we can use this as glFixed.
//
// Old code commented out.
//
//inline hdFixed::operator int() { return g>>BP; }
inline hdFixed::operator int() { return g; }
inline hdFixed::operator int() const { return g; }

inline hdFixed::operator long() { return g>>BP; }
//#pragma warning(disable: 4244) //HARDWIRE added pragma to prevent VS2005 compilation error
inline hdFixed::operator unsigned short() { return g>>BP; }
inline hdFixed::operator float() const { return g / (float)(1<<BP); }

inline hdFixed hdFixed::operator +() const { return hdFixed(RAW,g); }
inline hdFixed hdFixed::operator -() const { return hdFixed(RAW,-g); }

inline hdFixed hdFixed::operator +(const hdFixed a) const { return hdFixed(RAW, g + a.g); }
inline hdFixed hdFixed::operator -(const hdFixed a) const { return hdFixed(RAW, g - a.g); }

#if 1
// more acurate, using long long
inline hdFixed hdFixed::operator *(const hdFixed a) const { return hdFixed(RAW,  (int)( ((long long)g * (long long)a.g ) >> BP)); }

#elif 0

// check for overflow and figure out where.  Must specify -rdynamic in linker
#include <execinfo.h>
#include <signal.h>
#include <exception>

inline hdFixed hdFixed::operator *(const hdFixed a) const {
	long long x =  ((long long)g * (long long)a.g );
	if(x > 0x7fffffffffffLL || x < -0x7fffffffffffLL) {
		printf("overflow");
		void *array[2];
		int nSize = backtrace(array, 2);
		char **symbols = backtrace_symbols(array, nSize);
		for(int i=0; i<nSize; i++) {
			printf(" %s", symbols[i]);
		}
		printf("\n");
	}
	return hdFixed(RAW, (int)(x>>BP)); 
}

#else
// faster, but with only half as many bits right of binary point
inline hdFixed hdFixed::operator *(const hdFixed a) const { return hdFixed(RAW, (g>>BPhalf) * (a.g>>BPhalf) ); }
#endif


#ifdef TARGET_IS_NDS
// Division using the DS's maths coprocessor
inline hdFixed hdFixed::operator /(const hdFixed a) const
{
	//printf("%d %d\n", (long long)g << BP, a.g);
	return hdFixed(RAW, int( div64((long long)g << BP, a.g) ) );
}
#else
inline hdFixed hdFixed::operator /(const hdFixed a) const
{
	return hdFixed(RAW, int( (((long long)g << BP2) / (long long)(a.g)) >> BP) );
	//return hdFixed(RAW, int( (((long long)g << BP) / (long long)(a.g)) ) );
}
#endif

inline hdFixed hdFixed::operator *(unsigned short a) const { return operator*(hdFixed(a)); }
inline hdFixed hdFixed::operator *(int a) const { return operator*(hdFixed(a)); }

inline hdFixed hdFixed::operator +(float a) const { return hdFixed(RAW, g + hdFixed(a).g); }
inline hdFixed hdFixed::operator -(float a) const { return hdFixed(RAW, g - hdFixed(a).g); }
inline hdFixed hdFixed::operator *(float a) const { return hdFixed(RAW, (g>>BPhalf) * (hdFixed(a).g>>BPhalf) ); }
//inline hdFixed hdFixed::operator /(float a) const { return hdFixed(RAW, int( (((long long)g << BP2) / (long long)(hdFixed(a).g)) >> BP) ); }
inline hdFixed hdFixed::operator /(float a) const { return operator/(hdFixed(a)); }

inline hdFixed hdFixed::operator +(double a) const { return hdFixed(RAW, g + hdFixed(a).g); }
inline hdFixed hdFixed::operator -(double a) const { return hdFixed(RAW, g - hdFixed(a).g); }
inline hdFixed hdFixed::operator *(double a) const { return hdFixed(RAW, (g>>BPhalf) * (hdFixed(a).g>>BPhalf) ); }
//inline hdFixed hdFixed::operator /(double a) const { return hdFixed(RAW, int( (((long long)g << BP2) / (long long)(hdFixed(a).g)) >> BP) ); }
inline hdFixed hdFixed::operator /(double a) const { return operator/(hdFixed(a)); }

inline hdFixed hdFixed::operator >>(int a) const { return hdFixed(RAW, g >> a); }
inline hdFixed hdFixed::operator <<(int a) const { return hdFixed(RAW, g << a); }

inline hdFixed& hdFixed::operator +=(hdFixed a) { return *this = *this + a; }
inline hdFixed& hdFixed::operator -=(hdFixed a) { return *this = *this - a; }
inline hdFixed& hdFixed::operator *=(hdFixed a) { return *this = *this * a; }
//inline hdFixed& hdFixed::operator /=(hdFixed a) { return *this = *this / a; }
inline hdFixed& hdFixed::operator /=(hdFixed a) { return *this = operator/(a); }

inline hdFixed& hdFixed::operator +=(int a) { return *this = *this + (hdFixed)a; }
inline hdFixed& hdFixed::operator -=(int a) { return *this = *this - (hdFixed)a; }
inline hdFixed& hdFixed::operator *=(int a) { return *this = *this * (hdFixed)a; }
//inline hdFixed& hdFixed::operator /=(int a) { return *this = *this / (hdFixed)a; }
inline hdFixed& hdFixed::operator /=(int a) { return *this = operator/((hdFixed)a); }

inline hdFixed& hdFixed::operator +=(long a) { return *this = *this + (hdFixed)a; }
inline hdFixed& hdFixed::operator -=(long a) { return *this = *this - (hdFixed)a; }
inline hdFixed& hdFixed::operator *=(long a) { return *this = *this * (hdFixed)a; }
//inline hdFixed& hdFixed::operator /=(long a) { return *this = *this / (hdFixed)a; }
inline hdFixed& hdFixed::operator /=(long a) { return *this = operator/((hdFixed)a); }

inline hdFixed& hdFixed::operator +=(float a) { return *this = *this + a; }
inline hdFixed& hdFixed::operator -=(float a) { return *this = *this - a; }
inline hdFixed& hdFixed::operator *=(float a) { return *this = *this * a; }
//inline hdFixed& hdFixed::operator /=(float a) { return *this = *this / a; }
inline hdFixed& hdFixed::operator /=(float a) { return *this = operator/(a); }

inline hdFixed& hdFixed::operator +=(double a) { return *this = *this + a; }
inline hdFixed& hdFixed::operator -=(double a) { return *this = *this - a; }
inline hdFixed& hdFixed::operator *=(double a) { return *this = *this * a; }
//inline hdFixed& hdFixed::operator /=(double a) { return *this = *this / a; }
inline hdFixed& hdFixed::operator /=(double a) { return *this = operator/(a); }

inline hdFixed operator +(int a, const hdFixed b) { return hdFixed(a)+b; }
inline hdFixed operator -(int a, const hdFixed b) { return hdFixed(a)-b; }
inline hdFixed operator *(int a, const hdFixed b) { return hdFixed(a)*b; }
inline hdFixed operator /(int a, const hdFixed b) { return hdFixed(a)/b; };

inline hdFixed operator +(float a, const hdFixed b) { return hdFixed(a)+b; }
inline hdFixed operator -(float a, const hdFixed b) { return hdFixed(a)-b; }
inline hdFixed operator *(float a, const hdFixed b) { return hdFixed(a)*b; }
inline hdFixed operator /(float a, const hdFixed b) { return hdFixed(a)/b; }

inline bool hdFixed::operator ==(const hdFixed a) const { return g == a.g; }
inline bool hdFixed::operator !=(const hdFixed a) const { return g != a.g; }
inline bool hdFixed::operator <=(const hdFixed a) const { return g <= a.g; }
inline bool hdFixed::operator >=(const hdFixed a) const { return g >= a.g; }
inline bool hdFixed::operator  <(const hdFixed a) const { return g  < a.g; }
inline bool hdFixed::operator  >(const hdFixed a) const { return g  > a.g; }

inline bool hdFixed::operator ==(float a) const { return g == hdFixed(a).g; }
inline bool hdFixed::operator !=(float a) const { return g != hdFixed(a).g; }
inline bool hdFixed::operator <=(float a) const { return g <= hdFixed(a).g; }
inline bool hdFixed::operator >=(float a) const { return g >= hdFixed(a).g; }
inline bool hdFixed::operator  <(float a) const { return g  < hdFixed(a).g; }
inline bool hdFixed::operator  >(float a) const { return g  > hdFixed(a).g; }

inline bool hdFixed::operator ==(double a) const { return g == hdFixed(a).g; }
inline bool hdFixed::operator !=(double a) const { return g != hdFixed(a).g; }
inline bool hdFixed::operator <=(double a) const { return g <= hdFixed(a).g; }
inline bool hdFixed::operator >=(double a) const { return g >= hdFixed(a).g; }
inline bool hdFixed::operator  <(double a) const { return g  < hdFixed(a).g; }
inline bool hdFixed::operator  >(double a) const { return g  > hdFixed(a).g; }

inline bool hdFixed::operator  >(int a) const { return g > hdFixed(a).g; }
inline bool hdFixed::operator  <(int a) const { return g < hdFixed(a).g; }
inline bool hdFixed::operator  >=(int a) const{ return g >= hdFixed(a).g; };
inline bool hdFixed::operator  <=(int a) const{ return g <= hdFixed(a).g; };

inline bool operator ==(float a, const hdFixed b) { return hdFixed(a) == b; }
inline bool operator !=(float a, const hdFixed b) { return hdFixed(a) != b; }
inline bool operator <=(float a, const hdFixed b) { return hdFixed(a) <= b; }
inline bool operator >=(float a, const hdFixed b) { return hdFixed(a) >= b; }
inline bool operator  <(float a, const hdFixed b) { return hdFixed(a)  < b; }
inline bool operator  >(float a, const hdFixed b) { return hdFixed(a)  > b; }

inline hdFixed operator +(double a, const hdFixed b) { return hdFixed(a)+b; }
inline hdFixed operator -(double a, const hdFixed b) { return hdFixed(a)-b; }
inline hdFixed operator *(double a, const hdFixed b) { return hdFixed(a)*b; }
inline hdFixed operator /(double a, const hdFixed b) { return hdFixed(a)/b; }

inline bool operator ==(double a, const hdFixed b) { return hdFixed(a) == b; }
inline bool operator !=(double a, const hdFixed b) { return hdFixed(a) != b; }
inline bool operator <=(double a, const hdFixed b) { return hdFixed(a) <= b; }
inline bool operator >=(double a, const hdFixed b) { return hdFixed(a) >= b; }
inline bool operator  <(double a, const hdFixed b) { return hdFixed(a)  < b; }
inline bool operator  >(double a, const hdFixed b) { return hdFixed(a)  > b; }

inline bool operator ==(int a, const hdFixed b) { return hdFixed(a) == b; }
inline bool operator !=(int a, const hdFixed b) { return hdFixed(a) != b; }
inline bool operator <=(int a, const hdFixed b) { return hdFixed(a) <= b; }
inline bool operator >=(int a, const hdFixed b) { return hdFixed(a) >= b; }
inline bool operator  <(int a, const hdFixed b) { return hdFixed(a)  < b; }
inline bool operator  >(int a, const hdFixed b) { return hdFixed(a)  > b; }

inline int& operator +=(int& a, const hdFixed b) { a = (hdFixed)a + b; return a; }
inline int& operator -=(int& a, const hdFixed b) { a = (hdFixed)a - b; return a; }
inline int& operator *=(int& a, const hdFixed b) { a = (hdFixed)a * b; return a; }
inline int& operator /=(int& a, const hdFixed b) { a = (hdFixed)a / b; return a; }

inline long& operator +=(long& a, const hdFixed b) { a = (hdFixed)a + b; return a; }
inline long& operator -=(long& a, const hdFixed b) { a = (hdFixed)a - b; return a; }
inline long& operator *=(long& a, const hdFixed b) { a = (hdFixed)a * b; return a; }
inline long& operator /=(long& a, const hdFixed b) { a = (hdFixed)a / b; return a; }

inline float& operator +=(float& a, const hdFixed b) { a = a + b; return a; }
inline float& operator -=(float& a, const hdFixed b) { a = a - b; return a; }
inline float& operator *=(float& a, const hdFixed b) { a = a * b; return a; }
inline float& operator /=(float& a, const hdFixed b) { a = a / b; return a; }

inline double& operator +=(double& a, const hdFixed b) { a = a + b; return a; }
inline double& operator -=(double& a, const hdFixed b) { a = a - b; return a; }
inline double& operator *=(double& a, const hdFixed b) { a = a * b; return a; }
inline double& operator /=(double& a, const hdFixed b) { a = a / b; return a; }

inline hdFixed hdFixed::abs() { return (g>0) ? hdFixed(RAW, g) : hdFixed(RAW, -g); }
inline hdFixed abs(hdFixed f) { return f.abs(); }

//inline hdFixed atan2(hdFixed a, hdFixed b) { return atan2f((float) a, (float) b); }
inline hdFixed atan2(hdFixed y, hdFixed x)
{
	hdFixed abs_y = y.abs() + FIXED_EPSILON;	// avoid 0/0
	hdFixed r, angle;
	
	if(x >= 0.0f) {
		r = (x - abs_y) / (x + abs_y);
		angle = 3.1415926/4.0;
	} else {
		r = (x + abs_y) / (abs_y - x);
		angle = 3.0*3.1415926/4.0;
	}
	angle += hdFixed(0.1963) * (r * r * r) - hdFixed(0.9817) * r;
	return (y < 0) ? -angle : angle;
}

#if TARGET_IS_NDS

static inline long nds_sqrt64(long long a)
{
	SQRT_CR = SQRT_64;
	while(SQRT_CR & SQRT_BUSY);
	SQRT_PARAM64 = a;
	while(SQRT_CR & SQRT_BUSY);
	
	return SQRT_RESULT32;
}

static inline int32 div6464(int64 num, int64 den)
{
	DIV_CR = DIV_64_64;
	while(DIV_CR & DIV_BUSY);
	DIV_NUMERATOR64 = num;
	DIV_DENOMINATOR64 = den;
	while(DIV_CR & DIV_BUSY);
	
	return (DIV_RESULT32);
}

inline hdFixed hdFixed::sqrt()
{
	return hdFixed(RAW, nds_sqrt64(((long long)(g))<<BP));
}
#else
inline hdFixed hdFixed::sqrt()
{
	long long m, root = 0, left = (long long)g<<FIXED_BP;
	for ( m = (long long)1<<( (sizeof(long long)<<3) - 2); m; m >>= 2 )
	{
		if ( ( left & -m ) > root ) 
			left -= ( root += m ), root += m;
		root >>= 1;
	}
	return hdFixed(RAW, root);
}
#endif

inline hdFixed sqrt(hdFixed a) { return a.sqrt(); }
inline hdFixed sqrtf(hdFixed a) { return a.sqrt(); }

#endif

#ifdef TARGET_IS_NDS
// Use the libnds lookup tables for trigonometry functions
inline hdFixed hdFixed::cosf() {
	int idx = (((long long)g*(long long)G_1_DIV_PI)>>24)%512;
	if(idx < 0)
		idx += 512;
	return hdFixed(RAW, COS_bin[idx] << 4);
}
inline hdFixed cosf(hdFixed x) { return x.cosf(); }
inline hdFixed hdFixed::sinf() {
	int idx = (((long long)g*(long long)G_1_DIV_PI)>>24)%512;
	if(idx < 0)
		idx += 512;
	return hdFixed(RAW, SIN_bin[idx] << 4);
}
inline hdFixed sinf(hdFixed x) { return x.sinf(); }
inline hdFixed hdFixed::tanf() {
	int idx = (((long long)g*(long long)G_1_DIV_PI)>>24)%512;
	if(idx < 0)
		idx += 512;
	return hdFixed(RAW, TAN_bin[idx] << 4);
}
inline hdFixed tanf(hdFixed x) { return x.tanf(); }

inline int hdFixed::raw() const
{
	return g;
}

#endif



