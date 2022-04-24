/////////////////////////////////////////////////////////////////////
// File    : UIInterface.h
// Desc    : 
// Created : Tuesday, February 20, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//
#pragma once

#include <Assist/Callback.h>

// forward declarations
class Entity;
class ControllerBlueprint;
class Vec3f;
class Matrix43f;
class ArmyBuilderInterface;

/////////////////////////////////////////////////////////////////////
// UIInterface Interface

class UIInterface
{
// types
public:
	enum UIMode { UIM_Normal, UIM_Fullscreen, UIM_Cinematic };

	enum ChatAllowedResult
	{
		CHATALLOW_Ok,
		CHATALLOW_NotMP,
		CHATALLOW_NoLocal,
		CHATALLOW_COPPA
	};
	
	enum ChatMode
	{
		CHAT_All,
		CHAT_Allies
	};

	enum ModalModes
	{
		MM_None,
		MM_Cursor,
		MM_LockCursor,
		MM_BuildStructure,
	};

	typedef Callback::Obj0nRet					ModalCBAbort;
	typedef Callback::Obj2nRet<Vec3f,int>		ModalCBClick;
	typedef Callback::Obj3nRet<Vec3f,Vec3f,int>	ModalCBTwoClick;

	typedef Callback::Obj4nRet<unsigned long, Vec3f&, bool, Entity*>				ModalCBCursorUpdate;
	typedef Callback::Obj4nRet<Matrix43f&, bool&, const ControllerBlueprint*, bool>	ModalCBPlaceEntityUpdate;
	typedef Callback::Obj6nRet<const Vec3f&, const Vec3f&, std::vector<Matrix43f>&, std::vector<bool>&, int&, const ControllerBlueprint*>	ModalCBPlaceFenceUpdate;

// interface
public:
	virtual void		ModalUIStart 
							( 
							unsigned long modalMode, 
							unsigned long userData,
							const ModalCBAbort& cbAbort,
							const ModalCBClick& cbClick,
							const ModalCBCursorUpdate& cbUpdate
							) = 0;
	virtual void		ModalUIStart 
							( 
							const ControllerBlueprint* ebp, 
							const ModalCBAbort& cbAbort,
							const ModalCBClick& cbClick,
							const ModalCBPlaceEntityUpdate& cbUpdate
							) = 0;		// modal version used for building buildings

	virtual void		ModalUIStart 
							( 
							const ControllerBlueprint* ebp, 
							const ModalCBAbort& cbAbort,
							const ModalCBTwoClick& cbTwoClick,
							const ModalCBPlaceFenceUpdate& cbUpdate
							) = 0;		// modal version used for building fences

	virtual void		ModalUIStop() = 0;

	virtual ModalModes	GetModalMode() = 0;
						
	virtual UIMode		GetMode() const = 0;
	virtual void		SetMode( const UIMode& mode ) = 0;
							
	virtual bool		GetEnabled() const = 0;
	virtual void		SetEnabled( bool bEnabled ) = 0;
						
	virtual bool		ArmyBuilderAllowed() const = 0;
	virtual ChatAllowedResult
						ChatAllowed() const = 0;
	virtual bool		SpeechRecallAllowed() const = 0;
	virtual bool		DiploAllowed() const = 0;
	virtual bool		FastSpeedAllowed() const = 0;
						
	virtual void		ShowChat( ChatMode mode ) = 0;
	virtual void		HideChat() = 0;

	virtual void		ShowArmy   ( ArmyBuilderInterface* ) = 0;
	virtual void		ShowPauseMenu() = 0;
	virtual void		ShowSpeechRecall() = 0;

	// NOTE: requests to pause the game will be ignored during multiplayer games
	virtual void		SimulationPause   ( bool bPause ) = 0;
	virtual bool		SimulationPaused  () const = 0;

	// NOTE: requests to change sim rate in mulitplayer Games will be ignored
	virtual void		SetSimulationRate ( float rate ) = 0;
	virtual float		GetSimulationRate ( ) = 0;

	virtual	bool		NISletPlaySpeech(	const std::string&	patchName, 
											int					textID, 
											int					charID, 
											float				placeHolderTime, 
											bool				continuebutton = false,
											bool				queueSpeech = false ) = 0;

	virtual int			GetTeamColourIndex( unsigned long playerID ) = 0;

	// MiniMap UI interface
	virtual void		SetMMRotateCamera( bool bRotate ) = 0;
	virtual bool		GetMMRotateCamera( ) = 0;

	virtual void		SetMMPanCamera( bool bPan ) = 0;
	virtual bool		GetMMPanCamera( ) = 0;

	virtual void		SetMMZoom( bool bZoom ) = 0;
	virtual bool		GetMMZoom( ) = 0;

};
