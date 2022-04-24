/////////////////////////////////////////////////////////////////////
// m_file  : LogFile.h
// Author: 
// Date  : 2000-12-5
//      (c) relic entertainment inc.2000
// 
// * this class is for writing to a text file
// 

#pragma once

#include "UtilExports.h"

#include "Filesystem/ByteStream.h"

#include <Assist/FixedString.h>

///////////////////////////////////////////////////////////////////// 
// LogFile

class UTIL_API LogFile
{
// construction
public:
	 LogFile();

	// the destructor will close the file
	~LogFile();

// interface
public:
	// write the formatted string out to the text file
		// NOTE: the line has a total limit of 2048 characters
	void			printf( const char* format, ... );

	// write the string out to the text file
		// NOTE: no limit of characters
	void			puts  ( const char* str );

	// Create the specified log m_file.  It creates a unique m_fileName based on the date and time and player name
	// Returns true on success, false on failure.
	bool			Create( const char* filedesc, const char* extension = ".txt" );

	// Creates a log file with the explicit name fullname, no mangling is done
	bool			CreateWithName( const char* fullname );

	// closes the file
	void			Close();

	// 
	bool			IsOpened() const;

public:
	static void MakeFilename( char* s );
	static void MakeFilename( char* fname, const char* name, const char* ext );

// files
private:
	ByteStreamProxy
				m_file;

	char		m_chbuf[ 2048 ];

// implementation
private:
	void WriteString( const char* s );
};
