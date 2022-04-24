/////////////////////////////////////////////////////////////////////
// File    : TExpression.h
// Desc    : 
// Created : Tuesday, February 20, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "SimExports.h"

#include <Util/iff.h>

class World;
class Player;

class SelectionInterface;
class CameraInterface;

class BiFF;

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------

#define TEXPRESSION_CLONE(classname) \
TExpression* Clone() const \
{ classname* clone = new classname; *clone = *this; return clone; }

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------

class SIM_API TArgument
{
public:
	TArgument() : m_type( ARG_UNKNOWN ), m_intArg(0) {}

	// argument types - order is important
	enum ArgType 
	{
		ARG_UNKNOWN = -1,
		ARG_INT,				// 0
		ARG_FLOAT,				// 1
		ARG_ENTITY,				// 2
		ARG_PLAYER,				// 3
		ARG_BPCOMPARETYPE,		// 4
		ARG_BPCOMPAREID,		// 5
		ARG_SCRIPTID,			// 6
		ARG_TRIGGERID,			// 7
		ARG_COMPARE,			// 8
		ARG_SOUND,				// 9
		ARG_TRUTH,				// 10
		ARG_MESSAGE,			// 11
		ARG_GROUPID,			// 12
		ARG_EBPNETID,			// 13
		ARG_KEYID,				// 14
		ARG_CAMERAANIMID,		// 15
		ARG_OBJANIMID,			// 16
		ARG_MOVEANIMID,			// 17
		ARG_TRIGGERVARID,		// 18
		ARG_TRIGGERVAROP,		// 19
		// ARG_RESEARCHTYPE,		// 20
		ARG_DIALOGID = 21,		// 21
		ARG_DECALID,			// 22
		// ARG_ENTITYCMD,			// 23
		// ARG_OBJSTATE,			// 24
		// ARG_STANCETYPE,			// 25
		// ARG_HUD,					// 26
		// ARG_ADDONTYPE,			// 27
		// ARG_CHAR_ABILITY,		// 28
		ARG_MOVIE = 29,			// 29
		ARG_ENUM,				// 30
		ARG_ARMY,				// 31
		ARG_STOCK				// 32
	};
	
	// comparison types
	enum CompareType {CMP_LTEQ, CMP_GTEQ, CMP_EQ, CMP_NEQ};
	enum EBPCompareType {BY_ID, BY_CONTROLLER};
	enum TriggerVarOpType {OP_PLUS, OP_MINUS};
	
	enum PlayerSet // sequencial and not 2-base because only one is chosen in each argument
	{
		PS_PLAYER1,
			PS_PLAYER2,
			PS_PLAYER3,
			PS_PLAYER4,
			PS_CURRENTPLAYER,
			PS_ALLENEMIES,
			PS_FORCE1,
			PS_FORCE2,
			PS_FORCE3,
			PS_FORCE4,
			PS_PLAYER5,
			PS_PLAYER6,
			PS_MOTHERNATURE,
			PS_ALLPLAYERS,
			PS_LAST,
	};
	
	// accessors - sets
	inline void		SetInt			(long intArg) { m_intArg = intArg; m_type = ARG_INT; }
	inline void		SetFloat		(float floatArg) { m_floatArg = floatArg; m_type = ARG_FLOAT; }
	inline void		SetEntity		(long intArg) { m_intArg = intArg; m_type = ARG_ENTITY; }
	inline void		SetPlayer		(long intArg) { m_intArg = intArg; m_type = ARG_PLAYER; }
	inline void		SetBPCompareType(EBPCompareType bpCompareType) { m_bpCompareType = bpCompareType; m_type = ARG_BPCOMPARETYPE; }
	inline void		SetBPCompareID	(long intArg) { m_intArg = intArg; m_type = ARG_BPCOMPAREID; }
	inline void		SetScriptID		(long intArg) { m_intArg = intArg; m_type = ARG_SCRIPTID; }
	inline void		SetTriggerID	(long intArg) { m_intArg = intArg; m_type = ARG_TRIGGERID; }
	inline void		SetCompare		(CompareType compareArg) { m_compareArg = compareArg; m_type = ARG_COMPARE; }
	inline void		SetSound		(long intArg) { m_intArg = intArg; m_type = ARG_SOUND; }
	inline void		SetTruth		(long intArg) { m_intArg = intArg; m_type = ARG_TRUTH; }
	inline void		SetMsg			(long intArg) { m_intArg = intArg; m_type = ARG_MESSAGE; }
	inline void		SetGroupID		(long intArg) { m_intArg = intArg; m_type = ARG_GROUPID; }
	inline void		SetEBPNetID		(long intArg) { m_intArg = intArg; m_type = ARG_EBPNETID; }
	inline void		SetKeyID		(long intArg) { m_intArg = intArg; m_type = ARG_KEYID; }
	inline void		SetCameraAnimID	(long intArg) { m_intArg = intArg; m_type = ARG_CAMERAANIMID; }
	inline void		SetObjAnimID	(long intArg) { m_intArg = intArg; m_type = ARG_OBJANIMID; }
	inline void		SetMoveAnimID	(long intArg) { m_intArg = intArg; m_type = ARG_MOVEANIMID; }
	inline void		SetTriggerVarID	(long intArg) { m_intArg = intArg; m_type = ARG_TRIGGERVARID; }
	inline void		SetTriggerVarOp	(TriggerVarOpType triggerVarOpType) { m_triggerVarOpType = triggerVarOpType; m_type = ARG_TRIGGERVAROP; }
	inline void		SetDialogID		(long intArg) { m_intArg = intArg; m_type = ARG_DIALOGID; }
	inline void		SetDecalID		(long intArg) { m_intArg = intArg; m_type = ARG_DECALID; }
	inline void		SetMovie		(long intArg) { m_intArg = intArg; m_type = ARG_MOVIE; }
	inline void		SetEnum			(long intArg) { m_intArg = intArg; m_type = ARG_ENUM; }
	inline void		SetArmy			(long intArg) { m_intArg = intArg; m_type = ARG_ARMY; }
	inline void		SetStock		(long intArg) { m_intArg = intArg; m_type = ARG_STOCK; }

	inline void		InitAs			(ArgType type) { m_type = type; m_intArg = 0; }
	inline void		SetAsInt		(long intArg) { m_intArg = intArg; }

	// accessors - gets
	inline long		GetInt			() const { dbAssert( m_type == ARG_INT); return m_intArg; }
	inline float	GetFloat		() const { dbAssert( m_type == ARG_FLOAT); return m_floatArg; }
	inline long		GetEntity		() const { dbAssert( m_type == ARG_ENTITY); return m_intArg; }
	inline long		GetPlayer		() const { dbAssert( m_type == ARG_PLAYER); return m_intArg; }
	inline EBPCompareType
					GetBPCompareType() const { dbAssert( m_type == ARG_BPCOMPARETYPE); return m_bpCompareType; }
	inline long		GetBPCompareID	() const { dbAssert( m_type == ARG_BPCOMPAREID); return m_intArg; }
	inline long		GetScriptID		() const { dbAssert( m_type == ARG_SCRIPTID); return m_intArg; }
	inline long		GetTriggerID	() const { dbAssert( m_type == ARG_TRIGGERID); return m_intArg; }
	inline CompareType
					GetCompare		() const { dbAssert( m_type == ARG_COMPARE); return m_compareArg; }
	inline long		GetSound		() const { dbAssert( m_type == ARG_SOUND); return m_intArg; }
	inline long		GetTruth		() const { dbAssert( m_type == ARG_TRUTH); return m_intArg; }
	inline long		GetMsg			() const { dbAssert( m_type == ARG_MESSAGE); return m_intArg; }
	inline long		GetGroupID		() const { dbAssert( m_type == ARG_GROUPID); return m_intArg; }
	inline long		GetEBPNetID		() const { dbAssert( m_type == ARG_EBPNETID); return m_intArg; }
	inline long		GetKeyID		() const { dbAssert( m_type == ARG_KEYID); return m_intArg; }
	inline long		GetCameraAnimID	() const { dbAssert( m_type == ARG_CAMERAANIMID); return m_intArg; }
	inline long		GetObjAnimID	() const { dbAssert( m_type == ARG_OBJANIMID); return m_intArg; }
	inline long		GetMoveAnimID	() const { dbAssert( m_type == ARG_MOVEANIMID); return m_intArg; }
	inline long		GetTriggerVarID	() const { dbAssert( m_type == ARG_TRIGGERVARID); return m_intArg; }
	inline TriggerVarOpType
					GetTriggerVarOp	() const { dbAssert( m_type == ARG_TRIGGERVAROP); return m_triggerVarOpType; }
	inline long		GetDialogID		() const { dbAssert( m_type == ARG_DIALOGID); return m_intArg; }
	inline long		GetDecalID		() const { dbAssert( m_type == ARG_DECALID); return m_intArg; }
	inline long		GetMovie		() const { dbAssert( m_type == ARG_MOVIE); return m_intArg; }
	
	inline long		GetEnum			() const 
	{ 
		dbAssert(	(m_type == ARG_ENUM) ||

					// backward compatibility
					(m_type == 20) ||	// ARG_RESEARCHTYPE
					(m_type == 23) ||	// ARG_ENTITYCMD
					(m_type == 24) ||	// ARG_OBJSTATE
					(m_type == 25) ||	// ARG_STANCETYPE
					(m_type == 26) ||	// ARG_HUD
					(m_type == 27) ||	// ARG_ADDONTYPE
					(m_type == 28)		// ARG_CHAR_ABILITY
				); 
		return m_intArg; 
	}

	inline long		GetArmy			() const { dbAssert( m_type == ARG_ARMY); return m_intArg; }
	inline long		GetStock		() const { dbAssert( m_type == ARG_STOCK); return m_intArg; }

	inline long		AsInt			() const { return m_intArg; }

	// get argument type
	inline ArgType	GetType			() const { return m_type; }

	static bool GetPlayerIndex(TArgument::PlayerSet playerSet, int& playerIndex)
	{
		if (playerSet >= TArgument::PS_PLAYER1 && playerSet <= TArgument::PS_PLAYER4)
		{
			playerIndex = int(playerSet);
			return true;
		}
		else
		if (playerSet >= TArgument::PS_PLAYER5 && playerSet <= TArgument::PS_PLAYER6)
		{
			playerIndex = (playerSet - TArgument::PS_PLAYER5) + 1 + TArgument::PS_PLAYER4;
			return true;
		}

		return false;
	}

public:
	// load/save argument
	void					Load( IFF& iff );
	void					Save( IFF& iff );

private:
	// combination of all the types
	union
	{
		long					m_intArg;
		float					m_floatArg;
		EBPCompareType			m_bpCompareType;
		CompareType				m_compareArg;
		TriggerVarOpType		m_triggerVarOpType;
	};

	
	// argument type
	ArgType						m_type;	
};

//-------------------------------------------------------------------------------
// TExpression
//-------------------------------------------------------------------------------

class SIM_API TExpression
{
// types
public:
	enum 
	{
		CLASS_CONDITION,
		CLASS_ACTION,
	};

	struct EvaluateParms
	{
		Player*				player;
		World*				world;

		SelectionInterface*	selection;
		CameraInterface*	camera;
	};
	
// construction
public:
	TExpression()
		: m_loadedArgs(0), m_bValid(true)
		{
		}

	virtual ~TExpression()
		{
		}

// interface
public:
	// default expression
	virtual bool			Evaluate( EvaluateParms& ep ) = 0;
	virtual TExpression*	Clone() const = 0;

	// optional expression information
	virtual bool			CanReportProgress() { return false; }
	virtual bool			IsRunning() { return false; }
	virtual void			RecoverSavedState() {}

	// flags used for determining if a condition/action is deterministic or not
	// Note:
	//
	// A condition is deterministic only if it uses only simulation state.
	// An action is deterministic only if it is transmitted across the network.
	//
	// 1. trigger groups are considered simulation states
	// 2. trigger states are considered simulation states
	// 3. trigger flags are considered simulation states
	// 4. trigger variables are considered simulation states
	virtual bool			Deterministic() = 0;
	virtual bool			NetworkSafeAction() { return false; }

	// load/save expression states
	void					LoadState( IFF& iff, EvaluateParms& ep );
	void					SaveState( BiFF& biff );

	// expression validity
	bool					IsValid() { return m_bValid; }
	
public:
	// set methods
	void					SetNumArgs(int size) {m_args.resize(size);}
	// arg coming in should have proper type
	void					SetArg(int index, const TArgument &arg) {m_args[index] = arg;}
	void					SetType(const std::string& name) {m_type = name;}
	void					SetClass(int classtype) {m_class = classtype;}
	
	// get methods
	int						GetNumArgs() const {return m_args.size();}
	TArgument				&GetArg(int index) {return m_args[index];}
	const TArgument			&GetArg(int index) const {return m_args[index];}
	const std::string&		GetType() const {return m_type;}
	int						GetClass() const {return m_class;}
	
	// for loading - kinda ugly cuz of IFF - should add error checking
	TArgument				&GetNextArg() {return m_args[m_loadedArgs++];}

protected:
	virtual void			LoadStateCust( IFF& , EvaluateParms& ) {}
	virtual void			SaveStateCust( BiFF& ) {}

	void					SetValid( bool valid );
	
private:
	// list of arguments
	std::string					m_type;
	int							m_class;
	std::vector< TArgument >	m_args;
	int							m_loadedArgs;

	// expression validity
	bool						m_bValid;
};

