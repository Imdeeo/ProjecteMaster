#include "AStar.h"
#include "DebugRender.h"
#include "XML\XMLTreeNode.h"
#include "XML\tinyxml2.h"
#include "Utils.h"

#ifdef _DEBUG
#include "RenderManager\RenderManager.h"
#include "Effects\EffectManager.h"
#include "Math\Color.h"
#include "Engine\UABEngine.h"
#include "ContextManager\ContextManager.h"
#include "Effects\EffectTechnique.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "RenderableObjects\RenderableVertexs.h"
#include "Math\Matrix44.h"
#endif

#include <algorithm>
#include <map>
#include <stdlib.h>

CAStar::CAStar() : m_IndexPoint(0), m_IndexPathPatrolPoint(0){}

CAStar::CAStar(std::string _filename) : m_IndexPoint(0), m_IndexPathPatrolPoint(0){
	LoadMap(_filename);
}

CAStar::~CAStar() {
	DestroyMap();
}

void CAStar::LoadMap(std::string _filename)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError l_Error = doc.LoadFile(_filename.c_str());

	tinyxml2::XMLElement* l_Element;
	tinyxml2::XMLElement* l_ElementAux;

	if (l_Error == tinyxml2::XML_SUCCESS)
	{
		TNode *l_node;
		std::string nombreNodoActual;
		std::string nombreNodoVecino;
		l_Element = doc.FirstChildElement("path")->FirstChildElement();
		while (l_Element != NULL)
		{
			if (l_Element->Name() == std::string("node"))
			{
				l_node = new TNode();
				l_node->name = l_Element->GetPszProperty("name", "");
				l_node->position = l_Element->GetVect3fProperty("pos", Vect3f(0, 0, 0));
				m_map[l_node->name] = l_node;
			}
			else if (l_Element->Name() == std::string("father"))
			{
				l_ElementAux = l_Element->FirstChildElement();
				nombreNodoActual = l_ElementAux->GetPszProperty("name", "");

				while (l_ElementAux != NULL)
				{
					nombreNodoVecino = l_ElementAux->GetPszProperty("name");
					m_map[nombreNodoActual]->neighbours.push_back((PNodeAndDistance(m_map[nombreNodoVecino], (m_map[nombreNodoActual]->position - m_map[nombreNodoVecino]->position).Length())));
					l_ElementAux = l_ElementAux->NextSiblingElement();
				}
			}
			else if (l_Element->Name() == std::string("path_patrol"))
			{
				l_ElementAux = l_Element->FirstChildElement();
				TNodePatrol *l_nodeAux;
				nombreNodoActual = l_ElementAux->GetPszProperty("name", "");
				std::vector<TNodePatrol*> l_Aux;
				while (l_ElementAux != NULL)
				{
					l_nodeAux = new TNodePatrol();
					l_nodeAux->node = m_map[l_ElementAux->GetPszProperty("name")];
					l_nodeAux->wait = l_ElementAux->GetBoolProperty("wait");
					l_nodeAux->time_to_wait = l_ElementAux->GetFloatProperty("time_to_wait");
					l_Aux.push_back(l_nodeAux);
					l_ElementAux = l_ElementAux->NextSiblingElement();
				}			
				m_NodePatrolPath[nombreNodoActual] = l_Aux;
			}			
			l_Element = l_Element->NextSiblingElement();
		}

	
	}
}

void CAStar::DestroyMap() {
	for (TNodeMap::iterator l_iterator = m_map.begin(); l_iterator != m_map.end(); l_iterator++)
	{
		delete l_iterator->second;
	}
	m_map.clear();
	m_openList.clear();
}

#ifdef _DEBUG
void CAStar::Render(CRenderManager *_RenderManager)
{
	if (m_RenderNodes)
	{
		CEffectManager::m_SceneParameters.m_BaseColor = CColor(0, 1, 0, 1);
		CEffectManager::m_SceneParameters.m_BaseColor.SetAlpha(1.f);

		for (TNodeMap::iterator l_iterator = m_map.begin(); l_iterator != m_map.end(); l_iterator++)
		{
			TNode *currentNode = l_iterator->second;
			_RenderManager->GetContextManager()->SetWorldMatrix(GetTransform(currentNode->position));
			CEffectTechnique* l_EffectTechnique = UABEngine.GetRenderableObjectTechniqueManager()->GetResource("debug_lights")->GetEffectTechnique();
			CEffectManager::SetSceneConstants(l_EffectTechnique);
			GetShape(_RenderManager)->RenderIndexed(_RenderManager, l_EffectTechnique, CEffectManager::GetRawData());
		}

		CEffectManager::m_SceneParameters.m_BaseColor = CColor(0, 0, 1, 1);
		CEffectManager::m_SceneParameters.m_BaseColor.SetAlpha(1.f);

		for (TNodePatrolPath::iterator l_iterator = m_NodePatrolPath.begin(); l_iterator != m_NodePatrolPath.end(); l_iterator++)
		{
			std::vector<TNodePatrol*> currentPath = l_iterator->second;
			for (size_t i = 0; i < currentPath.size(); ++i)
			{
				TNode* currentNode = currentPath[i]->node;
				_RenderManager->GetContextManager()->SetWorldMatrix(GetTransform(currentNode->position));
				CEffectTechnique* l_EffectTechnique = UABEngine.GetRenderableObjectTechniqueManager()->GetResource("debug_lights")->GetEffectTechnique();
				CEffectManager::SetSceneConstants(l_EffectTechnique);
				GetShape(_RenderManager)->RenderIndexed(_RenderManager, l_EffectTechnique, CEffectManager::GetRawData());
			}
		}

		CEffectManager::m_SceneParameters.m_BaseColor = CColor(1, 0, 0, 1);
		CEffectManager::m_SceneParameters.m_BaseColor.SetAlpha(1.f);

		for (size_t i = 0; i < m_PathPoints.size(); ++i)
		{
			_RenderManager->GetContextManager()->SetWorldMatrix(GetTransform(m_PathPoints[i]));
			CEffectTechnique* l_EffectTechnique = UABEngine.GetRenderableObjectTechniqueManager()->GetResource("debug_lights")->GetEffectTechnique();
			CEffectManager::SetSceneConstants(l_EffectTechnique);
			GetShape(_RenderManager)->RenderIndexed(_RenderManager, l_EffectTechnique, CEffectManager::GetRawData());
		}
	}
}

CRenderableVertexs* CAStar::GetShape(CRenderManager *_RenderManager)
{
	return _RenderManager->GetDebugRender()->GetSPhere10();
}

CRenderableVertexs* CAStar::GetLine(CRenderManager *_RenderManager, Vect3f _pos1, Vect3f _pos2)
{
	return _RenderManager->GetDebugRender()->GetLine(_pos1, _pos2);
}

const Mat44f & CAStar::GetTransform(Vect3f _Position)
{
	Mat44f l_ScaleMatrix;
	l_ScaleMatrix.SetIdentity();
	l_ScaleMatrix.Scale(0.25, 0.25, 0.25);

	Mat44f l_RotationMatrix;
	l_RotationMatrix.SetIdentity();
	l_RotationMatrix = Quatf(0, 0, 0, 1).rotationMatrix();

	Mat44f l_TranslationMatrix;
	l_TranslationMatrix.SetIdentity();
	l_TranslationMatrix.SetPos(_Position.x, _Position.y, _Position.z);

	Mat44f l_TransformMatrix;
	l_TransformMatrix = l_ScaleMatrix*l_RotationMatrix*l_TranslationMatrix;

	return l_TransformMatrix;
}
#endif

bool CAStar::TCompareNodes::operator()( const TNode *nodeA, const TNode *nodeB ) {
	return nodeA->f < nodeB->f;
}

CAStar::VNodes CAStar::SearchNodePath( TNode* nodeA, TNode *nodeB ) {
	// Marcamos todos los nodos como no visitados
	for (TNodeMap::iterator l_iterator = m_map.begin(); l_iterator != m_map.end(); l_iterator++) {
		TNode *node = l_iterator->second;
		node->f = 0.0f;
		node->g = 0.0f;
		node->h = 0.0f;
		node->parent = NULL;
		node->inOpenList = false;
		node->closed = false;
	}

	// Inicializamos la lista abierta con el nodo inicial
	m_openList.clear();
	AddToOpenList( nodeA );

	// Ir visitando nodos hasta que se encuentre el destino o no queden nodos abiertos
	bool pathFound = false;
	while( !pathFound && m_openList.size() > 0 ) {
		pathFound = VisitNextNode( nodeB );
	}

	// Recorrer la lista de nodos desde el destino hasta el inicio, por los punteros que apuntan a los nodos padres
	VNodes result;
	if( pathFound ) {
		TNode *currentNode = nodeB;
		while( currentNode ) {
			result.insert( result.begin(), currentNode );
			currentNode = currentNode->parent;
		}
	}
	return result;
}

void CAStar::AddToOpenList( TNode *node ) {
	node->inOpenList = true;
	m_openList.push_back( node );
}

CAStar::TNode *CAStar::PopFirstFromOpenList() {
	TNode *firstNode = *( m_openList.begin() );
	firstNode->inOpenList = false;
	m_openList.erase( m_openList.begin() );
	return firstNode;
}

bool CAStar::VisitNextNode( TNode *destinationNode ) {
	// Mirar si quedan nodos en la lista de nodos abiertos
	if( m_openList.size() > 0 ) {
		// Coge el primer nodo y sacarlo de la lista abierta
		TNode *currentNode = PopFirstFromOpenList();

		// Marcarlo como cerrado
		currentNode->closed = true;

		// Mirar todos sus vecinos
		VNodesAndDistances::iterator it;
		for( it = currentNode->neighbours.begin(); it != currentNode->neighbours.end(); ++it ) {
			// Solo para los vecinos que no estén cerrados
			TNode *currentNeighbour = it->first;
			float distanceToNode = it->second;
			if( !currentNeighbour->closed ) {
				// Calcular su g llegando a este nodo desde el currentNode
				float g = currentNode->g + distanceToNode;

				// Comprobar si ya estaba en la lista de abiertos
				if( !currentNeighbour->inOpenList ) {
					// Si no estaba en la lista

					// Hacer que su padre sea el actual
					currentNeighbour->parent = currentNode;

					// Si el vecino que comprobamos es el destino, ya tenemos el camino
					if( currentNeighbour == destinationNode ) {
						return true;
					} else {
						// Asignar los valores de g, h y f
						currentNeighbour->g = g;

						currentNeighbour->h = (currentNeighbour->position - currentNode->position).Length();
						currentNeighbour->f = currentNeighbour->g + currentNeighbour->h;

						// Meterlo en la lista de abiertos
						AddToOpenList( currentNeighbour );
					}
				} else {
					// Si ya estaba en la lista

					// Comprobamos que el camino nuevo, sea mejor que el camino ya conocido
					if( g < currentNeighbour->g ) {
						// Si el nuevo camino es mejor, nos quedamos con ese

						// Hacer que su padre sea el actual
						currentNeighbour->parent = currentNode;

						// Asignar la nueva g y recalcular f (h no cambia)
						currentNeighbour->g = g;
						currentNeighbour->f = currentNeighbour->g + currentNeighbour->h;
					}
				}
			}
		}

		// Ordenar la lista abierta después de las posibles inserciones y cambios de valor de f
		std::sort( m_openList.begin(), m_openList.end(), TCompareNodes() );
	}
	return false;
}

CAStar::TNode *CAStar::GetNearestNode( const Vect3f &point ) {
	TNode *bestNode = NULL;
	float nearestSquaredDistance = FLT_MAX;

	for (TNodeMap::iterator l_iterator = m_map.begin(); l_iterator != m_map.end(); l_iterator++) {
		TNode *currentNode = l_iterator->second;
		float currentSquaredDistance = (currentNode->position - point).SquaredLength();
		if( currentSquaredDistance < nearestSquaredDistance ) {
			nearestSquaredDistance = currentSquaredDistance;
			bestNode = currentNode;
		}
	}
	return bestNode;
}

int CAStar::SearchForPath(const Vect3f &pointA, const Vect3f &pointB) {
	TNode *nodeA = GetNearestNode( pointA );
	TNode *nodeB = GetNearestNode( pointB );
	VNodes nodes = SearchNodePath( nodeA, nodeB );
	
	m_IndexPoint = 0;
	m_PathPoints.clear();

	VNodes::const_iterator it;
	for( it = nodes.begin(); it != nodes.end(); ++it ) {
		TNode *currentNode = *it;
		m_PathPoints.push_back( currentNode->position );
	}

	return m_PathPoints.size();
}

Vect3f CAStar::GetActualPoint()
{
	if (m_PathPoints.size() > 0 && m_IndexPoint < (int)m_PathPoints.size())
		return m_PathPoints[m_IndexPoint];
	else
		return Vect3f(0.0f, 0.0f, 0.0f);
}

bool CAStar::IncrementActualPoint()
{
	if (m_IndexPoint < (int)m_PathPoints.size() - 1)
	{
		m_IndexPoint += 1;
		return true;
	}
	else
		return false;
}

CAStar::TNodePatrol* CAStar::GetActualPatrolPoint(std::string _patrolName)
{
	if (m_NodePatrolPath.size() > 0)
		return m_NodePatrolPath[_patrolName][m_IndexPathPatrolPoint];
	else
		return NULL;
}

void CAStar::IncrementActualPatrolPoint(std::string _patrolName)
{
	if (m_IndexPathPatrolPoint < (int)m_NodePatrolPath[_patrolName].size() - 1)
		m_IndexPathPatrolPoint += 1;
	else
		m_IndexPathPatrolPoint = 0;
}