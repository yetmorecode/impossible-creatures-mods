/////////////////////////////////////////////////////////////////////
// File    : Target.h
// Desc    : 
// Created : Thurdsay, December 13, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//


#pragma once

#include "SimExports.h"

#include "EntityGroup.h"
#include "Math/Vec3.h"


class Entity;
class IFF;
class EntityFactory;


class SIM_API Target
{

// Data.
	
public:
	enum TargetType { eTargetPos, eTargetEntity };

private:
	TargetType		m_Type;

	// Possible targets. It will be one (and only one) of these.
	Vec3f			m_Pos;
	EntityGroup		m_Eg;

// Functions.

public:
	Target();
	explicit Target( Entity *pEntity );									// initializes the Target to hold an Entity
	explicit Target( const Vec3f& pos );								// initializes the Target to hold a point
	explicit Target( const Target& );									// initializes the Target to be a copy of another

	TargetType				GetType() const;
	void					SetType(TargetType type);

	const Vec3f&			GetPos () const;
	
	// this function is only valid for type eTargetPos
	void					SetPos ( const Vec3f& pos );

	// this function is only valid for type eTargetEntity
	void					SetEntity( Entity *pEntity );
	
	inline const Entity*	GetEntity( ) const;
	inline Entity*			GetEntity( );

	Target& 				operator = ( const Target& rhs );

	bool					Valid() const;

	void					SaveEmbedded( IFF& iff ) const;
	void					LoadEmbedded( IFF& iff, EntityFactory* pEFac );

private:	

	void					GeneralInit();				// all constructors must call this
};


///////////////////////////////////////////////////////////////////// 
// Inlines

inline const Entity* Target::GetEntity( ) const
{
	dbAssert( m_Type == eTargetEntity );
	
	return m_Eg.front();
}

inline Entity* Target::GetEntity( )
{
	dbAssert( m_Type == eTargetEntity );
	
	return m_Eg.front();
}