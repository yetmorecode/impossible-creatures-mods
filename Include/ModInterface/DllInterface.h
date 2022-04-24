/////////////////////////////////////////////////////////////////////
// File    : DllInterface.h
// Desc    : 
// Created : November 17, 2000
// Author  : Drew
// 
// (c) 2001 Relic Entertainment Inc.
//

// * exported interface for MOD dlls

#pragma once

// 
#include <Platform/InputTypes.h>
#include <Platform/Typedef.h>
#include <SurfVol/Rec2.h>

// forward declarations
	// NOTE: these classes are all exported by the GAME for the MOD
class SimEngineInterface;
class CommandInterface;
class SelectionInterface;
class CameraInterface;
class SoundInterface;
class FXInterface;
class GhostInterface;
class DecalInterface;
class TerrainOverlayInterface;
class UIInterface;
class MessageInterface;
class RTSHud;
class SPPersistenceInterface;

	// NOTE: these classes are exported by the MOD
class GameAI;
class EntityFilter;
class ModSimVis;
class ModUIEvent;
class NISletInterface;

	// NOTE: these classes are exported by the SIMENGINE
class Entity;
class EntityGroup;
class Player;
class World;
class ControllerBlueprint;

	//
class Vec3f;
class LuaConfig;
class IFF;

class ImpassEditArray;

///////////////////////////////////////////////////////////////////// 
// exported functions

class DLLInterface;
class DLLSetupInterface;
class DLLScoreInterface;
class DLLGameInterface;
class DLLCpuInterface;
class DLLGuiInterface;
class DLLSimInterface;

///////////////////////////////////////////////////////////////////// 
// Dll Interface version #'s  These should get incremented whenever
// a change to the interface is made, if it's a small change increment the minor
// version #, if it's a big change increment the major version #
//

const unsigned short MajorVersion = 4;
const unsigned short MinorVersion = 0;

#define MODMAKE_VERSION( major, minor )	((((unsigned long)major)<<16) | ((unsigned long)(minor)))

#define MODGET_MAJORVERSION( version )	(((unsigned long)version)>>16)
#define MODGET_MINORVERSION( version )	(((unsigned long)version) & 0x0000ffff)

extern "C"
{
	typedef DLLInterface* (__cdecl *modGetDLLInterface)( void );
	typedef unsigned long (__cdecl *modGetDLLVersion  )( void );
};

///////////////////////////////////////////////////////////////////// 
// DLLInterface

class DLLInterface
{
// types
public:
	enum ZsProgress
	{
		ZSP_Failure,
		ZSP_Transacting,
		ZSP_Done
	};

// interface
public:
	virtual const wchar_t*		GetName		() = 0;
	
	// query for scenario compatibility, this needs to be accessible at all 
	// times, hence it's not in a particular interface
	virtual bool				IsScenarioCompatible( const char* modname ) const = 0;

	// initialize mod resources. not loading a game yet. fe stuff mostly
	virtual bool				Initialize	( const char* version ) = 0;
	virtual void				Shutdown	() = 0;

	// to override the default game settings
		// NOTE: this interface can only exists before the DLLGameInterface is instantiated
	virtual DLLSetupInterface*	SetupCreate	() = 0;
	virtual void				SetupDestroy( DLLSetupInterface* ) = 0;

	// just before loading a game
		// NOTE 1: must have been initialized previously
		// NOTE 2: only 1 game can exists at a time
	virtual DLLGameInterface*	GameCreate	( SimEngineInterface* ) = 0;

	// when the game is over
	virtual void				GameDestroy	( DLLGameInterface* ) = 0;

	// after a game, to fill the scoreboard
	virtual DLLScoreInterface*	ScoreCreate () = 0;
	virtual void				ScoreDestroy( DLLScoreInterface* ) = 0;

	// zone stats
		// call this one first
	virtual ZsProgress			ZsPublish() = 0;
		// then call this one in a loop, until success or failure
	virtual ZsProgress			ZsUpdate () = 0;
		// or call this to abort
	virtual ZsProgress			ZsAbort  () = 0;
};

///////////////////////////////////////////////////////////////////// 
// DLLSetupInterface

class DLLSetupInterface
{
// types
public:
	enum GameType 
	{ 
		GT_SP,	// single-player experience, with NIS, MISlet
		GT_MP,	// skirmish or multiplayer
	};

	enum { MAXOPTIONS = 10 };

// interface
public:
	//
	virtual void		SetGameType( GameType gt ) = 0;
	virtual void		SetNetworkGame( bool bNetwork ) = 0;

	//
	virtual void		SetRandomSeed( unsigned long seed ) = 0;
	
	// stats
		// tells the stats system the scenario in play
	virtual void		SetStatsScenario ( const char* scenfile, const wchar_t* scenname ) = 0;
	
	virtual void		SetStatsGameID   ( const GUID& id ) = 0;

	// returns the option ID for the game mode
	virtual unsigned long
						GetGameModeOptionID( ) const = 0;

	// this must be set before any other player function
	virtual void		PlayerSetCount   ( size_t n ) = 0;
	virtual size_t		PlayerGetCount   () const = 0;

	virtual void		PlayerSetMax     ( size_t n ) = 0;

	virtual void		PlayerSetToAI    ( size_t playerIndex ) = 0;
	virtual void		PlayerSetToHuman ( size_t playerIndex ) = 0;
	virtual void		PlayerSetName    ( size_t playerIndex, const wchar_t* name ) = 0;
	virtual void		PlayerSetPassport( size_t playerIndex, const wchar_t* passport ) = 0;
	virtual void		PlayerSetRace    ( size_t playerIndex, size_t race ) = 0;
	virtual void		PlayerSetTeam    ( size_t playerIndex, size_t team ) = 0;

	// game options
	virtual void		OptionsInit		 ( GameType gt, bool bNetwork ) = 0;
	virtual size_t		OptionsCount     () const = 0;

	virtual unsigned long
						OptionID		 ( size_t option ) const = 0;
	virtual void		OptionName       ( wchar_t* out, size_t outlen, unsigned long optionID ) const = 0;
	virtual void		OptionTooltip    ( wchar_t* out, size_t outlen, unsigned long optionID ) const = 0;
	virtual size_t		OptionChoices	 ( unsigned long optionID ) const = 0;
	virtual size_t		OptionChoiceDef  ( unsigned long optionID ) const = 0;
	virtual bool		OptionChoiceValid( unsigned long optionID, size_t choice ) const = 0;
	virtual void		OptionChoiceName ( wchar_t* out, size_t outlen, unsigned long optionID, size_t choice ) const = 0;
	virtual bool		OptionChoiceAISupport
										 ( unsigned long optionID, size_t choice ) const = 0;
	virtual void		OptionSet        ( unsigned long optionID, size_t choice ) = 0;

	// race options
	virtual size_t		RaceGetCount	 () const = 0;		//	Returns 0 if Armies should be used
	virtual const wchar_t*	
						RaceGetName		 ( size_t index ) const = 0;
	virtual const wchar_t*	
						RaceGetDesc		 ( size_t index ) const = 0;
	virtual const char*	RaceGetImageFilename
										 ( size_t index ) const = 0;

	// team options
	virtual bool		TeamGetEnabled	 () const = 0;
};

///////////////////////////////////////////////////////////////////// 
// DLLGameInterface

class DLLGameInterface
{
public:
	virtual DLLCpuInterface*	GetCpuInterface() = 0;

	virtual DLLGuiInterface*	GetGuiInterface() = 0;

	virtual DLLSimInterface*	GetSimInterface() = 0;
};

/////////////////////////////////////////////////////////////////////
// DLLCpuInterface

class DLLCpuInterface
{
public:
	virtual GameAI*	CreateGameAI( CommandInterface* command ) = 0;

	virtual void	InitLuaAI( LuaConfig* ) = 0;
	virtual void	ShutLuaAI( LuaConfig* ) = 0;
};

///////////////////////////////////////////////////////////////////// 
// DLLGuiInterface

class DLLGuiInterface
{
// interface
public:
	virtual void				InitLuaGui( LuaConfig* ) = 0;
	virtual void				ShutLuaGui( LuaConfig* ) = 0;

	virtual void				OnEntityCreate( const Entity* ) = 0;

	virtual void				ChangePlayerArmy( unsigned long PlayerID, const std::vector< long >& soldiers ) = 0;
	
	// can be NULL
	virtual ModSimVis*			GetModSimVis() = 0;

	// can be NULL
	virtual ModUIEvent*			GetModUIEvent() = 0;

	virtual EntityFilter*		GetEntityFilter() = 0;

	virtual NISletInterface*	GetNISletInterface() = 0;

	virtual void				DoCommand( const EntityGroup& ) = 0;
	
	virtual void				DoCommand( const Vec3f*, unsigned long numVecs ) = 0;

	virtual bool				ProcessInput( const Plat::InputEvent &ie ) = 0;

	virtual const char*			GetCursor( const Entity* mouseOverEntity ) = 0;

	virtual void				CreateHUD
									( 
									const Player* localplayer, 
									RTSHud* hud, 
									CommandInterface* command,
									UIInterface* ui, 
									MessageInterface* message,
									SelectionInterface* sel, 
									CameraInterface* cam, 
									SoundInterface* sound,
									FXInterface* fx
									) = 0;

	virtual void				ShutdownHUD() = 0;

	virtual void				UpdateHUD( float elapsedSeconds) = 0;

	virtual void				UIPause( bool bPause ) = 0;

	virtual void				Save( IFF& ) = 0;
	virtual void				Load( IFF& ) = 0;

	virtual void				ShowModOptions( void ) = 0;
};

///////////////////////////////////////////////////////////////////// 
// DLLSimInterface

class DLLSimInterface
{
// types
public:
	enum NetworkKillType
	{
		NKT_Disconnected,
		NKT_Aborted,
		NKT_KickedOut,
		NKT_OutOfSync,
	};

	enum GameType 
	{ 
		GT_SP,	// single-player experience, with NIS, MISlet
		GT_MP,	// skirmish or multiplayer
	};


// interface
public:
	virtual bool			InitLuaSim				( LuaConfig* ) = 0;
	virtual void			ShutLuaSim				( LuaConfig* ) = 0;
							
	virtual void			InitTriggers			() = 0;
							
	virtual void			SetDecalInterface		( DecalInterface* decal ) = 0;
							
	virtual void			SetTerrainOverlayInterface( TerrainOverlayInterface* overlay ) = 0;

	virtual void			SetGhostInterface		( GhostInterface* ghost ) = 0;

	virtual void			SaveWorldStaticData		( IFF&, const ImpassEditArray* ) = 0;
	virtual void			LoadWorldStaticData		( IFF& ) = 0;

	virtual void			LoadSPPersistentData	( IFF&, SPPersistenceInterface* ) = 0;
	virtual void			SaveSPPersistentData	( IFF&, SPPersistenceInterface* ) = 0;

	// sim object creation
	virtual World*			CreateNewWorld			( bool bMissionEd ) = 0;
	virtual Player*			CreateNewPlayer			() = 0;
	virtual Entity*			CreateNewEntity			( void* buffer, unsigned long EID, const ControllerBlueprint* pCBP ) = 0;
								
	virtual void			OnTerrainModify			( const Rect2f&, const ImpassEditArray* impassEdit ) = 0;
	virtual bool			IsCellImpassible		( int x, int y ) = 0;

	// returns the number of players that the map supports, in MP number of discrete players, in SP 1 human player plus number of AI oponents
	virtual size_t			GetPlayerCount			( GameType ) = 0;

	// calculate a crc value for the mod read-only data
	// does NOT clear array
	virtual void			GetDataToken( std::vector< std::pair< unsigned long, const char* > >& crcArray ) const = 0;
							
	// sim query function
	virtual bool			IsPlayerAlly			( unsigned long idPlayer1, unsigned long idPlayer2 ) const = 0;
	virtual bool			IsPlayerEnemy			( unsigned long idPlayer1, unsigned long idPlayer2 ) const = 0;

		// map human & ai players to sim players
	virtual unsigned long	MapPlayerToSimulation	( size_t playerIndex ) const = 0;

		// check if scenario was a success for the specified player
			// NOTE: although this is a duplicate of the stats interface, it needs to
			// reside here 'cuz this function is called while the scenario is still loaded
	virtual bool			IsScenarioSuccess       ( unsigned long idPlayer ) const = 0;

	// stats stuff
		// tells the stats system that the game was aborted
	virtual void			StatsGameAbort          () = 0;

		// save the zonestats to disk, for uploading later
	virtual void			StatsZSSave             () = 0;

	// network player stuff
	virtual void			NetworkKillPlayer		( unsigned long idplayer, NetworkKillType type ) = 0;
};

///////////////////////////////////////////////////////////////////// 
// DLLScoreInterface

class DLLScoreInterface
{
// types
public:
	enum { NUMENTRIES = 4 };
	enum { NUMTABS    = 7 };
	enum { NUMCOLUMNS = 6 };

	enum ValueType
	{
		VT_Number,
		VT_String,
		VT_ControllerBlueprint,
		VT_Time,

		VT_COUNT
	};

	enum DecoratorFunc
	{
		DF_None,
		DF_MaxEqual,
		DF_MinEqual,

		DF_COUNT
	};

	enum TabType
	{
		TT_ByPlayer,
		TT_ByUnit,
		TT_Custom,

		TT_COUNT
	};

	enum PlayerState
	{
		PS_DISCONNECTED,
		PS_ABORTED,
		PS_OUTOFSYNC,
		PS_KILLED,
		PS_KICKED,
		PS_WON,
		PS_WONTEAM
	};

// interface
public:
	// duration of simulation, in seconds
	virtual int			TotalDuration() const = 0;

	// win, lose, and special status
	virtual PlayerState	PlayerFinalState( unsigned long idplayer ) const = 0;

	virtual const wchar_t* PlayerName	( unsigned long idplayer ) const = 0;

	// advanced tab
		// stat function list

	virtual size_t		UnitsTypeCount	  ( unsigned long idplayer ) const = 0; 

	virtual long		UnitsTypeAt		  ( unsigned long idplayer, size_t index ) const = 0;

	virtual int			UnitTotal		  ( unsigned long idplayer, long ebpid ) const = 0;

	virtual size_t		StatListCount	  ( ) const = 0;

	virtual void		StatListAt		  ( size_t idx, wchar_t* out, size_t outlen ) const = 0;
	virtual void		StatListTooltipAt ( size_t idx, wchar_t* out, size_t outlen ) const = 0;

	virtual TabType		StatListTabTypeAt ( size_t idx ) const = 0;

	virtual ValueType	StatListTypeAt	  ( size_t idx ) const = 0;

	virtual DecoratorFunc StatListDecoratorFuncAt( size_t idx ) const = 0;
	
	// value for that column on that tab
	virtual void		StatListValue
							( 
							size_t			idx, 
							unsigned long	idplayer, 
							int&			out 
							) const = 0;

	virtual void		StatListValue
							( 
							size_t			idx,
							unsigned long	idplayer,
							long			ebpnetid,
							int&			out
							) const = 0;

	// scores tab
		// qty of scores tab to display
	virtual size_t		ScoresTabsQty  () const = 0;

		// get the type of this tab - how should the rows be displayed
	virtual TabType		ScoresTabType  ( size_t tabIndex ) const = 0;

		// name of that scores tab
	virtual void		ScoresTabName  ( size_t tabIndex, wchar_t* out, size_t outlen ) const = 0;

	virtual DecoratorFunc ScoresTabColumnDecoratorFunc( size_t tabIndex, size_t columnIndex ) const = 0;

		// indicate whether that column should be displayed on that tab
	virtual bool		ScoresTabColumn( size_t tabIndex, size_t columnIndex ) const = 0;

		// name of that column on that tab
	virtual void		ScoresTabColumnName ( size_t tabIndex, size_t columnIndex, wchar_t* out, size_t outlen ) const = 0;

		// tooltip string of that column on that tab
	virtual void		ScoresTabColumnTooltip( size_t tabIndex, size_t columnIndex, wchar_t* out, size_t outlen ) const = 0;

		// type of value for that column on that tab
	virtual ValueType	ScoresTabColumnType ( size_t tabIndex, size_t columnIndex ) const = 0;

		// value for that column on that tab
	virtual void		ScoresTabColumnValue
							( 
							size_t			tabIndex, 
							size_t			columnIndex, 
							unsigned long	idplayer, 
							int&			out 
							) const = 0;

	virtual void		ScoresTabColumnValue
							( 
							size_t			tabIndex, 
							size_t			columnIndex, 
							unsigned long	idplayer,
							long			ebpnetid,
							int&			out
							) const = 0;

	virtual void		ScoresTabColumnValue
							( 
							size_t			tabIndex, 
							size_t			columnIndex, 
							unsigned long	idplayer, 
							wchar_t*		out, 
							size_t			outlen 
							) const = 0;
};
