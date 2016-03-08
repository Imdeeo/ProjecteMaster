#ifndef INC_A_STAR_H_
#define INC_A_STAR_H_

#include <d3dx9.h>
#include <vector>

typedef std::vector< D3DXVECTOR3 > VPoints3;

class CXMLTreeNode;

class CAStar {

public:	
	CAStar();
	CAStar(CXMLTreeNode &TreeNode);
	~CAStar();		
private:
	struct TNode;
	typedef std::pair< TNode*, float > PNodeAndDistance;
	typedef std::vector< PNodeAndDistance > VNodesAndDistances;

	struct TNode {
		D3DXVECTOR3 position;
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

	void LoadMap();
	void DestroyMap();

	VNodes SearchPath( TNode* nodeA, TNode *nodeB );
	bool VisitNextNode( TNode *destinationNode );

	TNode *GetNearestNode( const D3DXVECTOR3 &point );

public:
	void Render( LPDIRECT3DDEVICE9 device );
	VPoints3 SearchPath( const D3DXVECTOR3 &pointA, const D3DXVECTOR3 &pointB );

};

#endif // INC_A_STAR_H_