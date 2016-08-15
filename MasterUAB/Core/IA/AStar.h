#ifndef INC_A_STAR_H_
#define INC_A_STAR_H_

#include "Math\Matrix44.h"

#include <vector>
#include <string>
#include <map>

typedef std::vector< Vect3f > VPoints3;

class CXMLTreeNode;
class CRenderManager;
class CRenderableVertexs;

class CAStar {

public:	
	CAStar();
	CAStar(std::string _filename);
	~CAStar();
	void LoadMap(std::string _filename);
	void DestroyMap();
	int SearchForPath(const Vect3f &pointA, const Vect3f &pointB);

#ifdef _DEBUG
	void Render(CRenderManager *_RenderManager);
	CRenderableVertexs* CAStar::GetShape(CRenderManager *_RenderManager);
	CRenderableVertexs* CAStar::GetLine(CRenderManager *_RenderManager, Vect3f _pos1, Vect3f _pos2);
	const Mat44f &GetTransform(Vect3f _Position);
#endif

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

	struct TNodePatrol {
		TNode *node;
		bool wait;
		float time_to_wait;
	};

	Vect3f GetActualPoint();
	bool IncrementActualPoint();
	TNodePatrol* GetActualPatrolPoint(std::string _patrolName);
	void IncrementActualPatrolPoint(std::string _patrolName);
private:
	int m_IndexPoint;
	VPoints3 m_PathPoints;	
	
	typedef std::map<std::string, TNode*> TNodeMap;
	TNodeMap m_map;	

	int m_IndexPathPatrolPoint;
	typedef std::map<std::string, std::vector<TNodePatrol*>> TNodePatrolPath;
	TNodePatrolPath m_NodePatrolPath;

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
