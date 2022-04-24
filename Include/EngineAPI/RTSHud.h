/////////////////////////////////////////////////////////////////////
// File  : RTSHud.h
// Author: Shelby
// Date  : 2001-1-11
//      (c) relic entertainment inc.2001
// 
// *
// 


#pragma once

#include <Localizer/Localizer.h>

#include <Assist/Callback.h>

// forward declaration
class MiniMap;
class ControllerBlueprint;

class Colour;
class Player;

namespace Plat
{
	struct InputEvent;
};

//------------------------------------------------------------------------------------
// This interface is to be used by the mod so it can build and control the HUD
// that is used in the game (it sits above userinterface which is owned and run by
// the game)
//
// The MOD needs to control the behaviour of the HUD and get access to certain 
// details inside the game to determine what to display (what is selected, 
// where is my cursor, what is it over, plus other game events...)
//
// well enough blabbing
//
//------------------------------------------------------------------------------------



class RTSHud
{
// types
public:
	typedef Callback::Obj1nRet<const std::string&>				ButtonCallback;
	typedef Callback::Obj2nRet<const std::string&,bool>			ToolTipCallback;
	typedef Callback::Obj2nRet<const std::string&,int>			ListCallback;
	typedef Callback::Obj1wRet<bool,const Plat::InputEvent&>	InputCallback;

	typedef Callback::Obj0nRet OnMovieFinishedCB;

	enum UnitAttribute
	{
		UATTR_Rank,
		UATTR_Health,
		UATTR_Armor,

		// NOTE: speed is a context sensitive enum, due to the fact that animals have different types of speed
		// LabelAttribute will assert if you pass UATTR_Speed to it, you must use the special LabelSpeed function
		UATTR_Speed,		

		UATTR_Sight,
		UATTR_Size,
		UATTR_Melee,

		UATTR_RangeAttack0,
		UATTR_RangeAttack1,

		
		UATTR_COUNT
	};

	struct AttributeBonus
	{
		AttributeBonus() : op( BO_None ), bonus( 0.0f ), descBonus( 0.0f ) {}

		enum BonusOperator
		{
			BO_None,
			BO_Add,			// attribute -> attribute + bonus
			BO_Multiply		// attribute -> attribute * bonus
		};

		BonusOperator	op;
		float			bonus;		// actual bonus to be applied to the attribute

		float			descBonus;	// bonus amount that's used for UI deccription; this may
									// differ from the actual bonus
	};


// interface
public:
	// modal dlg
	virtual bool		PopupShow(const char* screen, const InputCallback&) = 0;
	virtual void		PopupHide(const char* screen) = 0;

public:

	// NOTE: use zero-length screen to specify the main screen

	// add new elements to the screen
	virtual MiniMap*	CreateMiniMap  (const char* screen, const char* hudID) = 0;

	// change hud elements
	virtual void		SetTextureName (const char* screen, const char* hudID, const char* textureName) = 0;

	virtual void		SetPulse       (const char* screen, const char* hudID, bool begin) = 0;

	// get a callback everytime the mouse goes over a new hud
	virtual void		SetChildTooltipCB
									   (const char* uniqueID, const ToolTipCallback& ) = 0;


	virtual void		SetText        (const char* screen, const char* hudID, const wchar_t* text, bool bvertalignbottom = false ) = 0;



	virtual void		SetLabelTextColour
	                                   (const char* screen, const char* hudID, const Colour& col) = 0;

	virtual void		SetLabelColour (const char* screen, const char* hudID, const Colour& col) = 0;

	virtual void		SetLabelTextPlayerColour 
									   (const char* screen, const char* hudID, const Player* player) = 0;

	virtual void		SetLabelPlayerColour 
									   (const char* screen, const char* hudID, const Player* player) = 0;



	virtual void		SetButtonCB			(const char* screen, const char* hudID, const ButtonCallback&) = 0;
	virtual void		SetButtonRightCB    (const char* screen, const char* hudID, const ButtonCallback&) = 0;

	virtual void		SetCheckBoxState	(const char* screen, const char* hudID, bool bChecked ) = 0;
	virtual bool		GetCheckBoxState	(const char* screen, const char* hudID ) = 0;

	virtual void		SetSliderInfo		( const char* screen, const char* hudID, long range, long position ) = 0;
	virtual long		GetSliderPosition	( const char* screen, const char* hudID ) = 0;

	virtual void		SetBarColour   (const char* screen, const char* hudID, const Colour& colour) = 0;
	virtual void		SetBarLength   (const char* screen, const char* hudID, float val) = 0;

	virtual void		Show           (const char* screen, const char* hudID, bool show) = 0;
	virtual void		Enable           (const char* screen, const char* hudID, bool enable) = 0;
	virtual bool		IsEnabled      (const char* screen, const char* hudID) const = 0;

	virtual const char* GetMouseOver   () const = 0;

	virtual void		PreloadTexture (const char* textureName ) = 0;

	virtual void		ClearScrollBox( const char *screen, const char *textScrollBoxID ) = 0;
	virtual void		AddItemToScrollBox( const char *screen, const char *textScrollBoxID, const char * itemTemplateID, const wchar_t* text ) = 0;
	
	virtual void		ClearCustomList( const char* screen, const char* listID ) = 0;
	virtual int			AddCustomItemToCustomList( const char* screen, const char* listID, const char* itemID ) = 0;
	virtual bool		GetCustomListItemName( const char* screen, const char* listID, int ndx, char* name, size_t namelen ) = 0;
	virtual int			GetCustomListSelectedItem( const char* screen, const char* listID ) = 0;
	virtual void		SetCustomListSelectedItem( const char* screen, const char* listID, int ndx ) = 0;
	
	virtual void		SetOnItemActivatedCB( const char* screen, const char* listID, const ListCallback& ) = 0;
	virtual void		SetOnItemChangeCB( const char* screen, const char* listID, const ListCallback& ) = 0;

	virtual int			GetListPos( const char* screen, const char* listID ) = 0;
	virtual void		SetListPos( const char* screen, const char* listID, int pos ) = 0;
	
	virtual void		AddGlobalInputHook( const char* name, const InputCallback& ) = 0;
	virtual void		DelGlobalInputHook( const char* name ) = 0;

	virtual bool		StartMovie( const char* movieFileName, 	const OnMovieFinishedCB& cb ) = 0;

	virtual void		ApplyTooltipFile( const char* screen, const char* tooltipFile ) = 0;

	virtual void		RefreshTooltip( const char* screen ) = 0;

public:
	// unit stats 

	// sets the appropriate rating icon
	virtual void		IconAttribute  (const char* screen, const char* hudID, UnitAttribute ua, const ControllerBlueprint* cbp ) = 0;

	// sets the appropriate text number
	virtual void		LabelAttribute  (
										const char* screen, 
										const char* hudID, 
										UnitAttribute ua, 
										const ControllerBlueprint* cbp, 
										const AttributeBonus* pBonus 
										) = 0;

	// sets the speed text number
	virtual void		LabelSpeed	(
									const char* screen,
									const char* landHudID,
									const char* waterHudID,
									const char* airHudID,
									const char* pureWaterHudID,
									const ControllerBlueprint* cbp,
									const AttributeBonus* pBonus
									) = 0;

	virtual void		LabelRangeAttack
									(
									const char* screen,
									const char* title,
									const char* desc,
									const char* rangelabel,
									int	rangeNdx,
									const ControllerBlueprint* cbp
									) = 0;

	

};
