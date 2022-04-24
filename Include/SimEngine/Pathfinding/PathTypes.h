#pragma once

/////////////////////////////////////////////////////////////////////
//	This file will define some basic types used by the Pathfinding.
//
//		If these change, likely some pathfinding save/load code will 
//		have to increase version.
//
//		Please don't put any #includes in this file.
//

// PathSectorMap types
typedef unsigned short			PathSubCell;

typedef std::set<PathSubCell>	PathSubCellSet;

const PathSubCell PATHSUBCELL_Invalid		= 0;
const PathSubCell PATHSUBCELL_Impassible	= 1; 
const PathSubCell PATHSUBCELL_First			= 2;
const PathSubCell PATHSUBCELL_Max			= 65535;
const PathSubCell PATHSUBCELL_New			= 65535;

const int		  MAX_ENTITY_SIZE			= 7;
const int		  MAX_ENTITY_DIAMETER		= 3;		// 1, 3, 5

typedef unsigned char	PathMajorCell;

enum
{
	PATHMAJORCELL_Impassible	= 0,
	PATHMAJORCELL_Tiny,
	PATHMAJORCELL_First,		
	PATHMAJORCELL_Invalid = 0xFF
};

enum PathIgnoreFlags
{
	PIF_None	= 0,
	PIF_Group	= 1 << 0,
	PIF_Moving	= 1 << 1
};

// ImpassMap types
typedef unsigned char TerrainCell;
typedef TerrainCell TCMask;


typedef std::vector<Vec2f>		 WorldPath;
typedef std::vector<Vec2i>		 GridPath;
typedef std::vector<PathSubCell> SectorPath;


// A* pathfinding results
class AstarPathfindingResult
{
// Types:
public:
	enum PathType
	{
		NoPath,
		GotPathToGoal,
		GotPathImperfect
	};

// Data
 public:
	 
	PathType	pathType;
	size_t		numSearched;
	bool		bFullSearch;
};



class AstarSectorPathResult
{
// Types
public:
	enum PathType
	{
		NoPath,
		PathToGoal,
		PathThruBuilding,
		PathThruBuildingToGoal,
		PathBlocked,
	};

// Data
public:
	
	PathType	pathType;
	size_t		numSearched;
	bool		bFullSearch;
};


typedef unsigned long PathLockHandle;
enum
{
	PathLockHandle_Invalid = 0,
};


class GroundAttributes
{
public:
	
	GroundAttributes() {;}
	GroundAttributes( int preciseSize, TCMask passibleTypes, int groupLevel, int groupID ) :
		m_preciseSize	( preciseSize	), 
		m_passibleTypes	( passibleTypes ), 
		m_groundLevel	( groupLevel	), 
		m_groupID		( groupID		) {;}
	  
	int			m_preciseSize;
	int			m_groundLevel;
	int			m_groupID;
	TCMask		m_passibleTypes;
};

