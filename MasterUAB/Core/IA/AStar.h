#ifndef INC_A_STAR_H_
#define INC_A_STAR_H_

#include "Math\Matrix44.h"

#include <vector>
#include <string>
#include <map>

typedef std::vector< Vect3f > VPoints3;

class CXMLTreeNode;

class CAStar {

public:	
	CAStar();
	CAStar(std::string _filename);
	~CAStar();
	void LoadMap(std::string _filename);
	void DestroyMap();
	void SearchForPath(const Vect3f &pointA, const Vect3f &pointB);
	Vect3f GetActualPoint();
	void IncrementActualPoint();
	//void Render( LPDIRECT3DDEVICE9 device );
private:
	int m_IndexPoint;
	VPoints3 m_PathPoints;
	
	struct TNode;	
	typedef std::pair< TNode*, float > PNodeAndDistance;
	typedef std::vector< PNodeAndDistance > VNodesAndDistances;

	struct TNode {
		std::string name;
		Vect3f position;
		VNodesAndDistances neighbours;
		TNode *parent;
		float g;
		float h;
		float f;
		bool inOpenList;
		bool closed;
	};
	
	typedef std::map<std::string, TNode*> TNodeMap;
	TNodeMap m_map;

	struct TCompareNodes {
		bool operator ()( const TNode *nodeA, const TNode *nodeB );
	};

	typedef std::vector< TNode* > VNodes;
	VNodes m_openList;

	void AddToOpenList( TNode *node );
	TNode *PopFirstFromOpenList();	

	VNodes SearchNodePath( TNode* nodeA, TNode *nodeB );
	bool VisitNextNode( TNode *destinationNode );

	TNode *GetNearestNode( const Vect3f &point );
};

#endif // INC_A_STAR_H_
