/////////////////////////////////////////////////////////////////////
// File    : BiFF.h
// Desc    : 
// Created : Tuesday, September 11, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "UtilExports.h"

#include "Filesystem/ByteStream.h"
#include "Iff.h"

class FileStream;

/////////////////////////////////////////////////////////////////////
// BiFF

// cross-dressing fileformat that can be saved out as text and/or binary

class UTIL_API BiFF
{
// types
public:
	enum ReadWriteMode { MODE_READ,   MODE_WRITE };

// construction
public:
	 BiFF();
	~BiFF();

// interface
public:
	bool	Open(const char* filename, bool textmode, ReadWriteMode rw_mode);
	void	Close();

	void	StartChunk(IFFChunkType Type, unsigned long ID, const char *label, unsigned long version = 0);
	void	StopChunk(void);
	
	void	Write(const std::string &value, const char *label);
	void	Write(long value, const char *label);
	void	Write(float value, const char *label);
	
	void	Write(bool value, const char *label);
	bool	Write(void *ptr, unsigned long len );

	// get internal IFF so that functions don't have to worry about BiFF
	IFF	*	GetIFF() {return m_iff;}
	const char* 
			GetFileName() const;

// fields
private:
	ByteStreamProxy	m_filestream;
	IFF* m_iff;

// copy -- do not define
private:
	BiFF( const BiFF& );
	BiFF& operator= ( const BiFF& );
};
