#include "AStar.h"
#include "DebugRender.h"
#include "XML\XMLTreeNode.h"
#include "Utils.h"
#include <algorithm>
#include <map>
#include <stdlib.h>

CAStar::CAStar() : m_IndexPoint(0){}

CAStar::CAStar(std::string _filename) : m_IndexPoint(0){
	LoadMap(_filename);
}

CAStar::~CAStar() {
	DestroyMap();
}

void CAStar::LoadMap(std::string _filename)
{
	CXMLTreeNode TreeNode = CXMLTreeNode();
	TreeNode.LoadFile(_filename.c_str());

	TNode *l_node;
	std::string nombreNodoActual;
	std::string nombreNodoVecino;

	for (int i = 0; i < TreeNode.GetNumChildren(); ++i)
	{
		CXMLTreeNode l_Element = TreeNode(i);
		if (l_Element.GetName() == std::string("node"))
		{
			l_node = new TNode();
			l_node->name = l_Element.GetPszProperty("name", "", true);
			l_node->position = l_Element.GetVect3fProperty("pos", Vect3f(0, 0, 0), true);
			m_map[l_node->name] = l_node;
		}
		else if (l_Element.GetName() == std::string("father"))
		{
			nombreNodoActual = l_Element.GetPszProperty("name", "", true);

			for (int i = 0; i < l_Element.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element2 = l_Element(i);
				if (l_Element2.GetName() == std::string("child"))
				{
					nombreNodoVecino = l_Element2.GetPszProperty("name");

					m_map[nombreNodoActual]->neighbours.push_back(
						(PNodeAndDistance(m_map[nombreNodoVecino], (m_map[nombreNodoActual]->position - m_map[nombreNodoVecino]->position).Length())));
				}
			}
		}
		else if (l_Element.GetName() == std::string("path_patrol"))
		{
			TNodePatrol *l_nodeAux;
			nombreNodoActual = l_Element.GetPszProperty("name", "", true);
			std::vector<TNodePatrol*> l_Aux;
			for (int i = 0; i < l_Element.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element2 = l_Element(i);

				l_nodeAux = new TNodePatrol();
				l_nodeAux->node = m_map[l_Element2.GetPszProperty("name")];
				l_nodeAux->wait = l_Element2.GetBoolProperty("wait");
				l_nodeAux->time_to_wait = l_Element2.GetFloatProperty("time_to_wait");
				l_Aux.push_back(l_nodeAux);
			}
			m_NodePatrolPath[nombreNodoActual] = l_Aux;
		}
	}
}

void CAStar::DestroyMap() {
	for (size_t i = 0; i<m_openList.size(); i++)
	{
		CHECKED_DELETE(m_openList[i]);
	}
	m_openList.clear();

	for (TNodeMap::iterator l_iterator = m_map.begin(); l_iterator != m_map.end(); l_iterator++)
	{
		delete l_iterator->second;
	}
	m_map.clear();
}

//void CAStar::Render( LPDIRECT3DDEVICE9 device ) {
//	D3DXMATRIX translation;
//	VNodes::const_iterator it;
//	for( it = m_map.begin(); it != m_map.end(); ++it ) {
//		const TNode *node = *it;
//		// Dibujar el nodo
//		D3DXMatrixTranslation( &translation, node->position.x, node->position.y, node->position.z );
//		device->SetTransform( D3DTS_WORLD, &translation );
//		CDebugRender::DrawSphere( device, 1.0, 0x00ff00, 10 );
//		
//		D3DXMatrixTranslation( &translation, 0.0f, 0.0f, 0.0f );
//		device->SetTransform( D3DTS_WORLD, &translation );
//
//		// Dibujar las relaciones con los vecinos
//		VNodesAndDistances::const_iterator nit;
//		for( nit = node->neighbours.begin(); nit != node->neighbours.end(); ++nit ) {
//			const TNode *neighbourNode = nit->first;
//			CDebugRender::DrawLine( device, node->position, neighbourNode->position, 0xff0000 );
//		}
//	}
//	D3DXMatrixTranslation( &translation, 0.0f, 0.0f, 0.0f );
//	device->SetTransform( D3DTS_WORLD, &translation );
//}

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

void CAStar::SearchForPath(const Vect3f &pointA, const Vect3f &pointB) {
	TNode *nodeA = GetNearestNode( pointA );
	TNode *nodeB = GetNearestNode( pointB );
	VNodes nodes = SearchNodePath( nodeA, nodeB );

	m_PathPoints.clear();

	VNodes::const_iterator it;
	for( it = nodes.begin(); it != nodes.end(); ++it ) {
		TNode *currentNode = *it;
		m_PathPoints.push_back( currentNode->position );
	}
}

Vect3f CAStar::GetActualPoint()
{
	if (m_PathPoints.size() > 0)
		return m_PathPoints[m_IndexPoint];
	else
		return Vect3f(0.0f, 0.0f, 0.0f);
}

void CAStar::IncrementActualPoint()
{
	if (m_IndexPoint < m_PathPoints.size() - 1)
		m_IndexPoint += 1;
}