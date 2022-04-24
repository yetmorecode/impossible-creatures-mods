/////////////////////////////////////////////////////////////////////
//	File	: PerfBlock.h
//	Desc.	: Performance timing mechanism
//		(c)2000 relic entertainment inc.
//

#pragma once

#include "UtilExports.h"

#include "StatGraph.h"

#include "MeanValue.h"

#include <Platform/Platform.h>

/////////////////////////////////////////////////////////////////////
//	PerfBlock

class UTIL_API PerfBlock
{
// construction
public:
	 PerfBlock( const char* = "" );
	~PerfBlock();

// interface
public:
	void		Start();
	void		Stop ();

	void		Pause ();
	void		Resume();

	void		SetTicks( longlong );
	void		SetTime ( float );

	longlong	GetAverageTicks() const;
	longlong	GetLastTicks   () const;

	float		GetAverageTime () const;
	float		GetLastTime    () const;

	void		SetResolution  ( float unitsPerSecond );
	float		GetResolution  () const;

// fields
private:
	bool		isActive       () const;

	MeanValue<longlong,8>	m_cDuration;
	longlong				m_ticksStart;
	longlong				m_ticksLast;
	float					m_unitsPerSecond;
	bool					m_bActive;

	StatGraph::Handle		m_handle;

// copy -- do not define
private:
	PerfBlock( const PerfBlock& );
	PerfBlock& operator = ( const PerfBlock& );
};
