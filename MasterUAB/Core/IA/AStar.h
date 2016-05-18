#ifndef INC_A_STAR_H_
#define INC_A_STAR_H_

#include "Math\Matrix44.h"

#include <vector>

typedef std::vector< Vect3f > VPoints3;

class CXMLTreeNode;

class CAStar {

public:	
	CAStar(CXMLTreeNode &TreeNode);
	~CAStar();
	void LoadMap();
	void DestroyMap();
private:
	struct TNode;
	typedef std::pair< TNode*, float > PNodeAndDistance;
	typedef std::vector< PNodeAndDistance > VNodesAndDistances;

	struct TNode {
		Vect3f position;
		VNodesAndDistances neighbours;
		TNode *parent;
		float g;
		float h;
		float f;
		bool inOpenList;
		bool closed;
	};
	typedef std::vector< TNode* > VNodes;
	VNodes m_map;

	struct TCompareNodes {
		bool operator ()( const TNode *nodeA, const TNode *nodeB );
	};
	VNodes m_openList;

	void AddToOpenList( TNode *node );
	TNode *PopFirstFromOpenList();	

	VNodes SearchPath( TNode* nodeA, TNode *nodeB );
	bool VisitNextNode( TNode *destinationNode );

	TNode *GetNearestNode( const Vect3f &point );

public:
	//void Render( LPDIRECT3DDEVICE9 device );
	VPoints3 SearchPath( const Vect3f &pointA, const Vect3f &pointB );

};

#endif // INC_A_STAR_H_
