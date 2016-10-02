#ifndef INC_A_STAR_H_
#define INC_A_STAR_H_

#include "Math\Matrix44.h"

#include <vector>
#include <string>
#include <map>

typedef std::vector< Vect3f > VPoints3;

class CRenderManager;
class CRenderableVertexs;

class CAStar {

public:	
	CAStar();
	CAStar(std::string _filename);
	~CAStar();
	void LoadMap(std::string _filename);
	void DestroyMap();
	int SearchForPath(const Vect3f &pointA, const Vect3f &pointB, const std::string _key);

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

	Vect3f GetPoint(std::string _key, int _index);
	TNodePatrol* GetPatrolPoint(std::string _patrolName, int _index);
	int GetTotalPatrolNodes(std::string _patrolName);
	void GetRenderNodes(bool _RenderNodes){ m_RenderNodes = _RenderNodes; }
	bool SwitchRenderNodes(){ m_RenderNodes = !m_RenderNodes; return m_RenderNodes; }
private:
	bool m_RenderNodes = false;
	std::map<std::string, VPoints3> m_PathPoints;
	
	typedef std::map<std::string, TNode*> TNodeMap;
	TNodeMap m_map;	

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
