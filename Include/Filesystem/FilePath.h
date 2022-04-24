/////////////////////////////////////////////////////////////////////
// File  : FilePath.h
// Author: Drew Dunlop
// Date  : 2000-12-6
//      (c) relic entertainment inc.2000
// 
// * This class allows the user to set multiple aliases to folder
// * so multiple folders will be searched, in order, for a specific file
// 
// * The system uses aliased filename. Alias can be folder, or archive files
// * The filenames are in the form "alias:relative path/filename.ext"

#pragma once

#include "FilesystemExports.h"

#include "ByteStream.h"

#include <Memory/MemoryAlloc.h>

/////////////////////////////////////////////////////////////////////
// FilePath

class FILESYSTEM_API FilePath
{
// types
public:
	class FileSet;
	class FileListObj;
	
	class Source;
	class Listener;

	enum FILE_Type
	{
		FILE_Invalid,
		FILE_File,
		FILE_Directory,
	};

	enum
	{
		MAXPATH = _MAX_PATH,
	};

// interface
public:

	///////////////////////////////////////////////////////////////////// 
	// Listener, your class will be called whenever a file is opened

	static bool				AddListener( const char* id, Listener* listener );
	static bool				RemoveListener( const char* id );

	///////////////////////////////////////////////////////////////////// 
	// Aliases
	
	// adds an alias search path
		// priority = bigger are searched first
		// id is used only for removal
		// takes ownership of source
	static bool				AddAlias
								( 
								const char* alias, 
								const char* id,
								const long priority, 
								Source*	source
								);

	// remove a previously added alias search path
		// returns false if the specified id is not found
	static bool				RemoveAlias( const char* alias, const char* id );

	///////////////////////////////////////////////////////////////////// 
	// Files
	
	// find and open byte stream to specified file
	static ByteStreamProxy	OpenStream( const char* streamName, StreamMode mode);

	// find
	static FileSet			FindFiles( const char* filespec );

	// This function will return the type of file found
	static FILE_Type		GetFileType( const char* filename );

	// this function will delete the file from the storage device
	static bool				RemoveFile( const char* filename );

	///////////////////////////////////////////////////////////////////// 
	// Filenames & Aliases
	
	// convert an alias based filename to an os filename
		// returns false if the specified path is not found
		// NOTE: this function handles wildcards filenames
	static bool				ConvertFromAlias( char* fname, StreamMode mode);

	// convert a true path to an alias
		// return false if no alias map to that folder
	static bool				ConvertToAlias( char* fname );

	// determine if the given filname contains an alias
		// return true if there is an alias
	static bool				ContainsAlias( const char* fname );

	// this function will split the path into its components
		// NOTE: all out parms must be at least MAXPATH long
	static bool				SplitPath
								( 
								const char* fullpath, 
								char*		outalias,
								char*		outpath,
								char*		outfname,
								char*		outextension
								);

	static bool				MakePath
								( 
								char*		fullpath, 
								const char*	alias,
								const char*	path,
								const char*	fname,
								const char*	extension
								);

// construction -- do not define
private:
	 FilePath();
	 FilePath( const FilePath& );
	~FilePath();
};

///////////////////////////////////////////////////////////////////// 
// FilePath::Source

class FILESYSTEM_API FilePath::Source
{
// construction
public:
	virtual ~Source();

// interface
public:
	// convert an alias based filename to an os filename
		// NOTE: obviously, this function only make sense for HD sources
	virtual bool		ConvertFromAlias( char* fname, const char* inname, const StreamMode mode ) = 0;
	virtual bool		ConvertToAlias  ( char* fname, const char* inname ) = 0;

	// find and open byte stream to specified file
	virtual ByteStream*	FileOpen( const char* filename, StreamMode mode ) = 0;

	// this function will delete the file from the storage device
	virtual bool		FileDel ( const char* filename ) = 0;

	// This function will return the type of file found
	virtual FilePath::FILE_Type		
						FileType( const char* filename ) = 0;

	// findfirst/findnext
		// NOTE: do NOT clear out parm
	virtual void		FileList( const char* filespec, FilePath::FileListObj* out ) = 0;
};

///////////////////////////////////////////////////////////////////// 
// FilePath::FileListObj

class FILESYSTEM_API FilePath::FileListObj
{
public:
	virtual void AddFile( const char* filename ) = 0;
};

///////////////////////////////////////////////////////////////////// 
// FilePath::FileSet

class FILESYSTEM_API FilePath::FileSet : private FilePath::FileListObj
{
	friend class FilePath;

// only FilePath can create a FileSet object
private:
	FileSet( const char* path );

// construction
public:
	FileSet( const FileSet& );
	FileSet& operator= ( const FileSet& );
	
// interface
public:
	size_t 				GetCount () const;
	// returns the filename alone
	const char*			GetFileAt( size_t index ) const;
	// returns the fully qualified path, e.g. 'data:art\ebps\stock\cheetah.lua'
	const char*			GetFullPathAt( size_t index ) const;

// inherited -- FileListObj	
private: 
	virtual void AddFile( const char* filename );
	
// fields
private:
	struct FileEntry
	{
		fstring<FilePath::MAXPATH>	file;
	};

	typedef std::vector< FileEntry, mempool_alloc< FileEntry > >
		FileVector;

	struct FoundFiles
	{
		FoundFiles();

		FileVector					files;
		fstring<FilePath::MAXPATH>	path;
	};

	class Data;
	Data* m_pimpl;
};

///////////////////////////////////////////////////////////////////// 
// FilePath::Listener
class FilePath::Listener
{
public:

	virtual void		OnFileOpen( const char* filename, const char* alias, const char* sourceID ) = 0;

};