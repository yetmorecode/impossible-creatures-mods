/////////////////////////////////////////////////////////////////////
// File    : ModDataPack.h
// Desc    : 
// Created : Thursday, May 09, 2002
// Author  : 
// 
// (c) 2002 Relic Entertainment Inc.
//

#pragma once

// information about a data only Mod.  Data only Mod's sit on-top of a code mod
class ModDataPack
{
// types
public:
	enum { IDMAX   = 64 };
	enum { NAMEMAX = 32 };

	typedef unsigned char Signature[16];

// construction
public:
	ModDataPack( const char* id, const wchar_t* name, const char* file, const Signature& crc );
	ModDataPack();

	ModDataPack( const ModDataPack& );
	ModDataPack& operator= ( const ModDataPack& );

// interface
public:
	static const ModDataPack Invalid;

public:
	const wchar_t*		GetName() const;
	const char*			GetID  () const;
	const char*			GetFile() const;
	const Signature& 	GetSignature() const;

	bool operator== ( const ModDataPack& rhs ) const;
	bool operator!= ( const ModDataPack& rhs ) const;
	bool operator== ( const char* id ) const;

// fields
private:
	char			m_id  [    IDMAX];
	wchar_t			m_name[  NAMEMAX];
	char			m_file[_MAX_PATH];
	Signature		m_crc;

};
