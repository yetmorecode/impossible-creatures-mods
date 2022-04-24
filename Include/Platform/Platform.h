/////////////////////////////////////////////////////////////////////
// File    : Platform.h
// Desc    : 
// Created : Monday, January 1, 1999
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

// * platform abstraction namespaces

#pragma once

#include "Typedef.h"
#include "InputTypes.h"

#include <Math/Vec2.h>
#include <Math/Vec4.h>
#include <SurfVol/Rec2.h>

///////////////////////////////////////////////////////////////////// 
// Plat : the Platform namespace

namespace Plat
{

	//
	//	Application
	//
	namespace App
	{
		PLATFORM_API int PlatWinMain
		( 
		void* hInstance,			// handle to current instance
		void* hPrevInstance,		// handle to previous instance
		char *lpCmdLine,			// pointer to command line
		int nCmdShow,				// show state of window
		int (*RunApplication)( void )
		);

		PLATFORM_API bool EulaAgreement( const char* regKeyName);

		PLATFORM_API void EnableExceptionHandling();

		typedef void (*OnCrashCallback)();
		PLATFORM_API void RegisterOnCrashCallback( OnCrashCallback, float priority=0.f );
		PLATFORM_API void DeregisterOnCrashCallback( OnCrashCallback );
	};

	//
	//	Exec
	//
	namespace Exec
	{
		PLATFORM_API void Break();	//	Traps to the debugger
		PLATFORM_API void Exit();	//	Exits the application in an unexpected manner
		PLATFORM_API void Sleep( unsigned long ms );
	};

	//
	//	Window
	//
	namespace Window
	{
		//	Create a new window
		PLATFORM_API bool Setup( unsigned long hwndParent = 0, const GUID& guidApp = GUIDNil() );
		//	Attach to an existing window handle
		PLATFORM_API bool Attach( unsigned long OSWndHandle );
		//	Shutdown the window
		PLATFORM_API bool Shutdown();

		PLATFORM_API void ProcessMessages();
		PLATFORM_API bool ShutdownRequested();

		PLATFORM_API void SetWindowSize( long x, long y, long width, long height );	//	Forces windowed behaviour
		PLATFORM_API void SetFullscreenSize( long width, long height );				//	Forces fullscreen behaviour
		PLATFORM_API void SetVisible( bool bVisible );
		PLATFORM_API void SetTitle( const char* title );
		PLATFORM_API void SetPaintProc(void (*pfunc)());
		PLATFORM_API void SetInputProc(int (*ifunc)());

		PLATFORM_API bool IsActive();
		PLATFORM_API long GetWidth();
		PLATFORM_API long GetHeight();
		PLATFORM_API float GetAspectRatio();		//	height/width
		PLATFORM_API bool GetWindowed();
		PLATFORM_API bool GetWindowPos( long &Xpos, long &Ypos, long &width, long &height );
		PLATFORM_API unsigned long GetOSWindowHandle();

		PLATFORM_API void Render( void );

		PLATFORM_API bool MessageBox( const wchar_t* pMessage, bool okCancel=false/*default:ok only*/ );
	};

	//
	//	Time
	//
	namespace Time
	{
		PLATFORM_API bool		Setup();
		PLATFORM_API bool		Shutdown();
		PLATFORM_API double		GetSecondsDouble();
		PLATFORM_API float		GetSeconds();
		PLATFORM_API longlong	GetCycle();
		PLATFORM_API longlong	GetCycleFreq();

		//
		struct SystemTime
		{ 
			short	year;		// i.e. 2001
			short	month;		// 1-12
			short	day;		// 1-31
			short	hour;		// 0-23
			short	minute;		// 0-59
			short	second;		// 0-59
		};

		PLATFORM_API void		TimeStart( SystemTime& t );
		PLATFORM_API void		TimeNow  ( SystemTime& t );
	};

	//
	//	Input
	//
	namespace Input
	{
		PLATFORM_API bool Setup( bool bToolInput );
		PLATFORM_API bool Shutdown();
		
		// These functions will keep a count, if the count is greater than 0 then IME will be disabled.
		PLATFORM_API void DisableIME( );
		PLATFORM_API void EnableIME( );

		PLATFORM_API bool EnableTextCapture( const wchar_t* str, size_t caret, size_t maxChars, const Rect2f& textRect, const char* font, bool bEnableIME = true, long IMEToggleState = 1 );
			//	Enables text capture mode and sets up initial text and caret position

		PLATFORM_API bool DisableTextCapture( long& IMEToggleState );
			//	Disables text capture mode. WARNING: text and caret position are lost after being disabled

		PLATFORM_API bool UpdateTextInfo( const wchar_t* str, size_t caret, size_t maxChars, const Rect2f& textRect, const char* font );
			// updates the currently composed string and caret pos

		PLATFORM_API bool GetTextInfo( wchar_t* str, size_t strlen, size_t& caret );
			// retrieves the currently composed string and updated caret position

		PLATFORM_API void Poll();
			//	Polls any devices and updates queues

		PLATFORM_API void Enable( bool );
			//	Toggles the capture of events

		PLATFORM_API bool PopEvent( InputEvent& );
			//	event is valid only if 'true' returned

		PLATFORM_API const Vec2f& GetCursorPos();
			//	queries for cursor position based on current position in queue

		PLATFORM_API void SetCursorPos(const Vec2f &pos);
			//	Sets the cursor position based to the position specified

		PLATFORM_API bool IsKeyPressed( InputKey k );
			//	queries for keystate based on current position in queue

		PLATFORM_API bool GetDragStart( InputKey k, Vec2f& );
			//	Will return the start of the dragging if that button is being dragged

		PLATFORM_API void Flush();
			//	Process all key changes
			//	return

		PLATFORM_API void FreezeCursor();
		PLATFORM_API void UnfreezeCursor();
			//	(un)Locks cursor to current position

		PLATFORM_API void EnableCursorClip( bool clip );
		PLATFORM_API void ClipCursor( float minx, float miny, float maxx, float maxy );
		PLATFORM_API void UnClipCursor( );

		PLATFORM_API void LockCursor();
		PLATFORM_API void UnLockCursor();
			// (un)Locks cursor to the screen extents

		PLATFORM_API void HideCursor();
		PLATFORM_API void ShowCursor();
			//	Allows cursor to be (in)visible

		struct CursorImage	//	32x32 image
		{
			int	hotSpotX, hotSpotY;
			unsigned long andMask[32];
			unsigned long xorMask[32];
		};
		PLATFORM_API int CreateCursor( const CursorImage& );	//	returns handle for SetCursor(), 0 on failure
		PLATFORM_API void SetCursor( int handle );
			//	Handles OS cursor management

		PLATFORM_API void SetMouseSensitivity( float );
		PLATFORM_API float GetMouseSensitivity();
		PLATFORM_API unsigned short Key2Char( InputKey key );

		PLATFORM_API const char *GetKeyName( InputKey key );
		PLATFORM_API InputKey GetKeyFromName( const char *key );

		// gets the localized name of the key, returns true if successful
		PLATFORM_API bool GetKeyLocalizedName( InputKey key, wchar_t* name, size_t namelen );
		PLATFORM_API bool GetComboKeyLocalizedName( const ComboKey& key, wchar_t* name, size_t namelen );

		PLATFORM_API bool CheckInputQueueForKeyPress(InputKey Key);
		PLATFORM_API bool CheckInputQueueForKeyRelease(InputKey Key);

		// keyboard setting accessors
		PLATFORM_API float GetKeyRepeatDelay( void );
		PLATFORM_API float GetKeyRepeatRate( void );
	//	float GetCursorBlinkRate( void );

		// returns a combokey structure from a string of this format = "Control+Shift+A"
		PLATFORM_API ComboKey GetComboKeyFromName( const char* combokey );
		PLATFORM_API void GetComboKeyName( const ComboKey& key, std::string &str );

		// check to see if this combo key is pressed
		PLATFORM_API bool IsComboKeyPressed( const ComboKey& combokey, const InputEvent* ie );
		
	};

	//
	//	File
	//
	namespace File
	{
		//
		typedef unsigned long Handle;

		//
		enum FileAttributes
		{
			F_ATTRIBUTE_DIRECTORY = 1 << 0,
			F_ATTRIBUTE_FILE      = 1 << 1,
			F_ATTRIBUTE_READ	  = 1 << 8,
			F_ATTRIBUTE_WRITE     = 1 << 9,
		};

		enum FileOpenMode
		{
			FO_READ,
			FO_READWRITE,
			FO_WRITE
		};

		//
		PLATFORM_API bool Setup();
		PLATFORM_API void Override( const char* wkDir );
		PLATFORM_API void Shutdown();

		PLATFORM_API const char*	GetPathExe();
        PLATFORM_API const char*	GetPathCWD();
		PLATFORM_API const char*	GetPathDrivers();

		// replace all invalid characters in the supplied string
		PLATFORM_API std::string	ConvertToFilename( const char* fname );

		// 
		PLATFORM_API unsigned long	GetFileAttr( const char* fname );

		// File I/O
			// return 0 on error
		PLATFORM_API Handle			FileOpen  ( const char* fname, FileOpenMode mode );
		PLATFORM_API void			FileClose ( Handle h );

		PLATFORM_API unsigned long	FileGetPos( Handle h );
		PLATFORM_API bool			FileSetPos( Handle h, unsigned long pos );
		PLATFORM_API unsigned long	FileGetLen( Handle h );

		PLATFORM_API unsigned long	FileWrite ( Handle h, const void* data, unsigned long len );
		PLATFORM_API unsigned long	FileRead  ( Handle h, void* data, unsigned long len );
		PLATFORM_API void			FileFlush ( Handle h );
	};

	//
	//	Command Line
	//
	namespace CmdLine
	{
		PLATFORM_API bool Setup( const char* sCmdLine = NULL);
		PLATFORM_API bool Setup( int argc, char* argv[] );
		PLATFORM_API bool Shutdown();
		
		PLATFORM_API bool Exist(const char *key);
		PLATFORM_API bool Get(const char *key, std::string &buf);
		PLATFORM_API bool Get(const char *key, char* out, size_t len);
		PLATFORM_API bool Get(const char *key, int &val);
		PLATFORM_API bool Get(const char *key, float &val);
		PLATFORM_API bool Get(const char *key, bool &val);
	};

	//
	//	Console
	//
	namespace Console
	{
		PLATFORM_API bool		Setup   ();
		PLATFORM_API bool		Shutdown(  long exitcode );

		PLATFORM_API bool		CreateDebugWindow( long Xpos, long Ypos, long width, long height );
		PLATFORM_API void		CloseDebugWindow ();
		PLATFORM_API bool		GetDebugWindowPos( long &Xpos, long &Ypos, long &width, long &height );

		// NOTE: these must be THREAD-SAFE, and must NOT allocate memory
		PLATFORM_API void		Write( const char* str );
		PLATFORM_API void		Write( const char* strArray[], size_t strCount, unsigned long id = 0 );
	};

	//
	//	CriticalSection
	//
	class CriticalSection
	{
	public:
		PLATFORM_API CriticalSection();
		PLATFORM_API ~CriticalSection();

		PLATFORM_API void Enter();
		PLATFORM_API void Leave();

	private:
		void* m_pData;

	private:
		CriticalSection( const CriticalSection& );
		CriticalSection& operator= ( const CriticalSection& );
	};

	//
	//	CriticalLock
	//
	class CriticalLock
	{
	private:
		CriticalSection& m_refCS;
		bool m_in;
	
	public:
		 CriticalLock( CriticalSection& cs )
			: m_refCS( cs )
		{
			m_refCS.Enter();
			m_in = true;
		}

		~CriticalLock()
		{
			Unlock();
		}

	public:
		void Unlock()
		{
			if( m_in )
			{
				m_refCS.Leave();
				m_in = false;
			}
		}

	private:
		CriticalLock( const CriticalLock& );
		CriticalLock& operator= ( const CriticalLock& );
	};
}

//	eof