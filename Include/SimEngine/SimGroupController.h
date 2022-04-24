/////////////////////////////////////////////////////////////////////
// File    : SimGroupController.h
// Desc    : 
// Created : Thursday, April 25, 2002
// Author  : dswinerd
// 
// (c) 2002 Relic Entertainment Inc.
//

#pragma once

#include "GroupController.h"

#include <SimEngine/Target.h>
#include <SimEngine/SimHighPath.h>

///////////////////////////////////////////////////////////////////// 
// Forward Declarations

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class: SimGroupController

class SIM_API SimGroupController : public GroupController
{
// construction
public:
	SimGroupController();
	virtual ~SimGroupController();


// interface
public:

	inline float	GetMaxSpeed() const;
	inline TCMask	GetPassibleTypes() const;


	void			SetMaxSpeed( float maxSpeed );
	void			SetPassibleTypes( TCMask passibleTypes );
	
	/*	// not really doing group movement, so removing this to eliminate the unused overhead
	virtual void	SetLeader( const Entity* pLeader );
	*/

protected:

	void			RequestMove( const Vec3f& destination, float AP );

// inherited - GroupController
protected:

	virtual bool	Update();
	virtual void	Execute();

	virtual void	PreExecute( );
	virtual void	PostExecute( );

	virtual void	DoExecuteEntity( Entity *pEntity );

	virtual void	Save( IFF& iff ) const;
	virtual void	Load( IFF& iff );

// 
private:
	static unsigned long		HandleGCSM( IFF&, ChunkNode*, void*, void* );

private:

/*	// not really doing group movement, so removing this to eliminate the unused overhead
	EntityGroup		m_leader;
	SimHighPath		m_highPath;

	Target			m_target;
	float			m_AP;
*/

	float			m_maxSpeed;
	TCMask			m_passibleTypes;
};


/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the max speed for the group
//
inline float SimGroupController::GetMaxSpeed() const
{
	return m_maxSpeed;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the passible mask for the group
//
inline TCMask SimGroupController::GetPassibleTypes() const
{
	return m_passibleTypes;
}
