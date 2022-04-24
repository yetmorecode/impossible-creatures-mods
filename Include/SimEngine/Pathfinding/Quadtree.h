// Quadtree.h

#pragma once

#include "ImpassMap.h"

// Forward Declarations.
class PathGraphNode;
class PathGraphEdge;

// a container for PathGraphNodes
typedef std::set<PathGraphNode*> PathGraphNodeSet;

// A container for PathGraphEdges use a vector which is indexed by a postion and a run for each PathGraphNode
typedef std::vector<PathGraphEdge> PathGraphEdgeVector;


// A node in a quadtree
class QuadNode
{
public:
	QuadNode();
	QuadNode( QuadNode * parent, unsigned int dir );
	~QuadNode();

	union	// anon union
	{
		struct	// anon struct.
		{
			QuadNode *	m_ne; // 0	-----
			QuadNode *	m_se; // 1	-3 0-
			QuadNode *	m_sw; // 2	-2 1-
			QuadNode *	m_nw; // 3	-----
		};
		QuadNode * m_children[4];
	};

	QuadNode *		m_parent;

	Vec2f			m_topleft;			// position of top left of this node on the island
	Vec2f			m_bottomright;		// position of bottom right of this node on the island
	Vec2f			m_centre;			// position of centre of this node on the island

	unsigned int	m_depth;			// depth of this node from the root node.
	PathGraphNode *	m_pathgraphnode;	// PathGraphNode coreresponding to this QuadNode (if any)
	static int s_counter;
};


// enum for quadnode indices.
enum QuadrantDir
{
	qd_ne = 0,
	qd_se = 1,
	qd_sw = 2,
	qd_nw = 3,
};


bool DivideNode( PathGraphNodeSet& pathgraphnodes, TerrainCellMap * terrainmap, QuadNode * node, int x_start, int z_start, int size);

// Given the world, the root node of a quadtree and a position find the quadnode that contains the position.
// QuadNode * FindContainingQuadNode( World * world, QuadNode * rootnode, const Vector3& position, int maxdepth )
QuadNode * FindContainingQuadNode(QuadNode * rootnode, const Vec2f& position, int maxdepth );

// Connect the leave nodes visited by following the two given split directions to the given pathnode
// Return the total number of leaves thus connected.
int ConnectLeaves( QuadNode * quadnode, PathGraphNode * pathnode, QuadrantDir qdA, QuadrantDir qdB, PathGraphEdgeVector& edgevec );




typedef int NodeLocation;

class PathGraphNode
{
public:
   Vec2f			location;	// location of node (some location representation)
   PathGraphNode *	from;		// parent node (zero pointer represents starting node)
   float			cost;		// cost to get to this node
   float			total;		// total cost (cost + heuristic estimate)
   bool				onOpen;		// on Open list
   bool				onClosed;	// on Closed list
   int				edges;		// number of edges leading away from this node
   int				edgestart;	// start location of those edges in the edge array
   QuadNode	*		quadnode;	// quadnode corresponding to this PathGraphNode.
};

bool operator <( const PathGraphNode& lhs, const PathGraphNode& rhs );

////////////////////////////////////////////////////////////////
// edge class
class PathGraphEdge
{
public:
	PathGraphEdge() : destination_node(0), water_distance(0), land_distance(0) {}
	PathGraphEdge( PathGraphNode * source, PathGraphNode * dest ) : water_distance(0), land_distance(0),
											destination_node( dest ), source_node( source ) {}

	float			water_distance;		// distance over water on this edge.
	float			land_distance;		// distance over land on this edge.
	PathGraphNode * source_node;		// this node come from this node.
	PathGraphNode *	destination_node;	// this edge go to this node.
};



QuadNode *CreateQuadtree(TerrainCellMap *);
