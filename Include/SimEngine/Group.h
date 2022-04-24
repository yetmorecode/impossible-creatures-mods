// Group.h

#pragma once

#include "SimExports.h"
#include "Entity.h"

class SIM_API Group
{
	// Group is the crappiest name ever, but EntityGroup is currently being used. Perhaps I will rename
	//	The EntityGroup to something else, and change this to EntityGroup (Since that is what this really is).

	// This Class is responsible for keeping track of a group and managing various things such as:
	//	- speed of the group.
	//	- Centroid.
	//	- Commander?

// Data.
public:
	std::vector<Entity *> m_Entities;	// The entities within the group.
							//	Note: This doesn't use EntityGroup because it needs a list of Entities,
							//	Before the Entity is deleted, it should be removed from this group.
							//	Using the RemoveEntity function.


//	Entity *Commander;		// Specify the commander.
//	Vec3f Centroid;			// Relative to Commander?
		

private:





// Functions.
public:
	Group(Entity *e);
	~Group();

	void AddEntity(Entity *e);
	bool RemoveEntity(Entity *e);	// Returns 0 if list is empty, returns 1 otherwise.

private:
	Group();

};
