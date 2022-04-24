/////////////////////////////////////////////////////////////////////
// File    : StatGraph.h
// Desc    : 
// Created : Saturday, March 31, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "UtilExports.h"

#include "Colour.h" 

// * to display real-time stats on the screen

// * USAGE
// * void function()
// * {
// *	block.Start();
// *		DoSomething();
// *	block.Stop();
// *	
// *	StatGraph::ChannelSubmit( "something", block.Count() );
// * }

///////////////////////////////////////////////////////////////////// 
// StatGraph

class UTIL_API StatGraph
{
// types
public:
	class Sink;

	enum { MAX_VALUES = 50 };
	
	struct Channel
	{
		std::string	name;

		bool		visible;
		Colour		colour;

		std::vector< float > 
					values;
		float		valuesAvg;
		float		valuesMax;
		float		valuesSum;

		// this is the current index for the values
			// NOTE: the values array is a circular queue
		size_t		index;

		// this is an internal var used to recalculate the max value
		int			refresh;
	};
	
	class UTIL_API Handle
	{
	public:
		void* reserved;

	public:
		Handle();

	public:
		operator const void* () const;
	};

	// lua
	static const char* LuaLibName();

// construction
public:
	// you don't need to register a sink to actually use the class
		// takes ownership of pointer
	static void	Register  ( Sink* );
	static void	Unregister( Sink* );

// interface
public:
	// the channel is turned off by default
	static Handle	ChannelCreate ( const char* channel );
	static void		ChannelDestroy( const char* channel );
	static void		ChannelDestroy( Handle& channel );
	
	// set maximum for this channel, the channel must have been created first
		// use 0 for auto-expand
	static void		ChannelSetMax ( const char* channel, float max );

	// send data to the channel, the channel must have been created first
	static void		ChannelSubmit ( const Handle& channel, float value );
	static void		ChannelSubmit ( const char* channel, float value );

	static bool		ChannelEnabled( const char* channel );
	static bool		ChannelEnabled( const Handle& channel );
	static void		ChannelEnable ( const char* channel, bool bEnable );

	// list all currently created channels
	static void		ListChannels( std::list< std::string >& out );

	// turns display on/off
	static void		SetVisible( bool bOn );
	static bool		GetVisible();

	// turns pausing on/off
	static void		SetPause( bool );
	static bool		GetPause();

	// call this once per frame
	static void		Update();

// construction -- do not define
private:
	StatGraph();
	StatGraph( const StatGraph& );
	StatGraph& operator= ( const StatGraph& );
};

///////////////////////////////////////////////////////////////////// 
// StatGraph::Sink

class UTIL_API StatGraph::Sink
{
// construction
public:
	virtual ~Sink();

// interface
public:
	virtual void Update
		( 
		const StatGraph::Channel* const* first,
		const StatGraph::Channel* const* last,
		const bool visible
		) = 0;
};

///////////////////////////////////////////////////////////////////// 
// StatgraphFile

class StatgraphFile : private StatGraph::Sink
{
// construction
private:
	StatgraphFile();

public:
	UTIL_API static StatGraph::Sink* Create();

	virtual ~StatgraphFile();

// inherited
public:
	virtual void Update
		( 
		const StatGraph::Channel* const* first,
		const StatGraph::Channel* const* last,
		const bool visible
		);

// fields
private:
	class Data;	
	Data* m_pimpl;

// copy -- do not define
private:
	StatgraphFile( const StatgraphFile& );
	StatgraphFile& operator= ( const StatgraphFile& );
};
