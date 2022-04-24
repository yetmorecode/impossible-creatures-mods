/////////////////////////////////////////////////////////////////////
// File  : FastMath.h
// Author: Jrehn
// Date  : 2000-11-15
//      (c) relic entertainment inc.2000
// 
// *
// 

#pragma once

inline float InvSqrt(float v)
{
	dbAssert( v > 0.f );

	// Fast invert sqrt technique similar to Jim Blinn's.
	//	I think it is doing some sort of newton iteration to reduce the error at the end.

   float x2 = v * (float)0.5F;
   float y = v;
   long i = *(long *) &y;
   i = 0x5f3759df - (i>>1);
   y = *(float *)&i;

   y = y * (1.5f - (x2 * y * y));
   y = y * (1.5f - (x2 * y * y));

   return y;
}

inline bool IsPowerOfTwo( unsigned long n )
{
	return ( (n & (n - 1)) == 0 );
}

inline long HighestOnBitIndex( unsigned long x )
{
	dbAssert( IsPowerOfTwo( x ) );

	long result = -1;

	if (!x)
		return result;

	if (x&0xffff0000)
	{
		result+=16;
		x>>=16;
	}

	if (x&0xff00)
	{
		result+=8;
		x>>=8;
	}

	if (x&0xf0)
	{
		result+=4;
		x>>=4;
	}	

	if (x&0xc)
	{
		result+=2;
		x>>=2;
	}	

	if (x&4)
	{
		result+=1;
		x>>=1;
	}	

	return result;
}

//	fast ftol
//	- only works with positive floats
//	- rounds to nearest integer (so subtract 0.5f for clamping)
//	- source : Jan Svarovsky <Jan@MUCKYFOOT.COM> post on DirectX mail list
//		http://discuss.microsoft.com/SCRIPTS/WA-MSD.EXE?A2=ind9906D&L=DIRECTXDEV&P=R1343
inline int fast_ftol( float f )
{
	dbAssert( f >= 0.f );

	f += (float)(1L << (23));				// Force lower 23 bits to be the integer part
	return (*((int *)(&f))) & 0x007fffff;   // Strip off top 8-bits of crap (exponent)
}

///////////////////////////////////////////////////////////////////// 
// This Macro converts a float to a long, only works if the flt
// is positive. It subtracts 0.500001f so that you clamp.
// NOTE: if you use this there is a slight inaccuracy in that
// if your number is slightly less than 1 you can end up with
// an integer value of 1

#define _FTOL_POS( flt, iout )				\
	{										\
		const float __half = 0.50001f;		\
		__asm fld	flt						\
		__asm fadd	__half					\
		__asm fistp iout					\
		__asm dec	iout					\
	}
