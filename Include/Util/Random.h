/////////////////////////////////////////////////////////////////////
// File  : Random.h
// Author: 
// Date  : 2000-12-8
//      (c) relic entertainment inc.2000
// 
// *
// 

#pragma once

#include "UtilExports.h"

/////////////////////////////////////////////////////////////////////
// Random

class UTIL_API Random
{
// types
public:
	enum { DefaultLoopReps = 4 };

public:
	Random( unsigned long _loopReps = DefaultLoopReps );

// interface
public:
	// while locked, no Get*() are allowed
	inline bool				IsLocked() const { return m_locked; }
	void					Lock  ();
	void					Unlock();

	// set seed value (also resets index to 0)
	void					SetSeed(unsigned long _seed);
	unsigned long			GetSeed () const;

	unsigned long			GetIndex()	const;
	void					SetIndex(unsigned long _index);

	// random number [0.0 .. 1.0]
	inline double			GetUnitInclusive(void)		{ dbAssert( !IsLocked() ); return ((unsigned long)hashRandom()) / (double)0xffffffff; } 

	// random number (0.0 .. 1.0)
	inline double			GetUnitExclusive(void)		{ dbAssert( !IsLocked() ); return ((unsigned long)hashRandom()) * (1/(0x10000*(double)0x10000)) + (1/(0x20000*(double)0x10000)); } 

	// n-bit random number (n <= 32)
	inline unsigned long	GetBits(long n)				{ dbAssert( !IsLocked() ); return hashRandom() & ((1<<n)-1); }

	// random number [0 .. n)  (ie, < n)
	inline unsigned long	GetMax(unsigned long n)		{ dbAssert( !IsLocked() ); return n ? ((unsigned long)hashRandom()) % n : 0; }

	// random number [a .. b]  (ie, >=a && <=b)
	inline unsigned long	GetRange(unsigned long a, unsigned long b) 
														{ dbAssert( !IsLocked() ); return a + GetMax(b-a+1); }

// fields
private:
	unsigned long	m_seed;
	unsigned long	m_index;
	unsigned long	m_loopReps;
	
	bool			m_locked;

// implementation
private:
	long hashRandom();
};
