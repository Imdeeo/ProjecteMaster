#include "StaticMesh.h"
#include "RenderManager.h"
#include "UABEngine.h"
#include "RenderableVertexs.h"
#include "VertexTypes.h"
#include "TemplatedRenderableIndexedVertexs.h"
#include "Utils.h"

#include <iostream>
#include <fstream>
#include <cstring>

#define HEADER 65109
#define FOOTER 22014

CStaticMesh::CStaticMesh(void):CNamed(""),
	m_BoundingSphereRadius(0.f)
{
	m_BoundingBoxMax = new Vect3f();
	m_BoundingBoxMin = new Vect3f();
	m_BoundingSphereCenter = new Vect3f();
}

CStaticMesh::~CStaticMesh(void)
{
	delete m_BoundingBoxMax;
	delete m_BoundingBoxMin;
	delete m_BoundingSphereCenter;
}

bool CStaticMesh::Load(const std::string &FileName)
{
	m_Name = FileName;

	std::fstream l_File(m_Name, std::ios::binary | std::ios::in);
	if (!l_File.is_open())
	{
		return false;
	}
	else
	{
		char l_BufferChar;
		short l_BufferShort;
		unsigned short l_BufferUnsignedShort;
		unsigned int l_BufferUnsignedInt;
		long l_BufferLong;
		float l_BufferFloat;
		std::string l_BufferString;

		short l_MatLength;
		unsigned short l_VertexType;
		unsigned short l_IndexType;
		long l_NumMaterials;

		l_File.seekg(0);

		// Read Header
		l_File.read((char *) &l_BufferUnsignedShort, sizeof(short));

		if(l_BufferUnsignedShort == HEADER)
		{
			// Read Number of Materials
			l_File.read((char *) &l_BufferLong, sizeof(long));
			l_NumMaterials = l_BufferLong;

			for(int i=0; i<l_NumMaterials; i++)
			{
				// Read Material Name Length
				l_BufferString = "";
				l_File.read((char *) &l_BufferShort, sizeof(short));
				l_MatLength = l_BufferShort;

				for(int i=0; i<l_MatLength; i++)
				{
					l_File.read(&l_BufferChar, sizeof(l_BufferChar));
					l_BufferString.append(&l_BufferChar, sizeof(l_BufferChar));
				}
				l_File.read(&l_BufferChar, sizeof(l_BufferChar));
 				m_Materials.push_back(UABEngine.GetMaterialManager()->GetResource(l_BufferString));
			}

			for(int i=0; i<l_NumMaterials; i++)
			{
				// Read Vertex Type
				l_File.read((char *) &l_BufferUnsignedShort, sizeof(unsigned short));
				l_VertexType = l_BufferUnsignedShort;

				// Read Number of Vertexs
				l_File.read((char *) &l_BufferLong, sizeof(long));
				int l_NumVertexs = l_BufferLong;

				int l_NumBytes = 0;

				if(l_VertexType==MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX::GetVertexType())
					l_NumBytes = sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX)*l_NumVertexs;
				else if(l_VertexType==MV_POSITION_NORMAL_TEXTURE_VERTEX::GetVertexType())
					l_NumBytes = sizeof(MV_POSITION_NORMAL_TEXTURE_VERTEX)*l_NumVertexs;
				else if(l_VertexType==MV_POSITION4_COLOR_TEXTURE_VERTEX::GetVertexType())
					l_NumBytes = sizeof(MV_POSITION4_COLOR_TEXTURE_VERTEX)*l_NumVertexs;
				else if(l_VertexType==MV_POSITION_COLOR_VERTEX::GetVertexType())
					l_NumBytes = sizeof(MV_POSITION_COLOR_VERTEX)*l_NumVertexs;
				else if(l_VertexType==MV_POSITION_TEXTURE_VERTEX::GetVertexType())
					l_NumBytes = sizeof(MV_POSITION_TEXTURE_VERTEX)*l_NumVertexs;
				else if(l_VertexType==MV_POSITION_COLOR_TEXTURE_VERTEX::GetVertexType())
					l_NumBytes = sizeof(MV_POSITION_COLOR_TEXTURE_VERTEX)*l_NumVertexs;

				void *l_VertexData = NULL;
				l_VertexData = malloc(l_NumBytes);

				// Read Vertex Data
				l_File.read((char *) l_VertexData, l_NumBytes);

				// Read Index Type
				l_File.read((char *) &l_BufferUnsignedShort, sizeof(unsigned short));
				l_IndexType = l_BufferUnsignedShort;

				long l_NumIndexs = 0;
				// Read Number of Indexs
				l_File.read((char *) &l_BufferLong, sizeof(unsigned long));
				l_NumIndexs = l_BufferLong;
				if(l_IndexType==16)
				{
					l_NumBytes=sizeof(unsigned short)*l_NumIndexs;
				}
				else if(l_IndexType==32)
				{
					l_NumBytes=sizeof(unsigned int)*l_NumIndexs;
				}

				void *l_IndexData = NULL;
				l_IndexData = malloc(l_IndexType*l_NumIndexs);

				// Read Index Data
				l_File.read((char *) l_IndexData, l_NumBytes);

				CRenderableVertexs *l_RV = NULL;

				if(l_VertexType==MV_POSITION_NORMAL_TEXTURE_VERTEX::GetVertexType())
				{
					if(l_IndexType==16)
						l_RV=new CUABTriangleListRenderableIndexed16Vertexs<MV_POSITION_NORMAL_TEXTURE_VERTEX>(l_VertexData, l_NumVertexs, l_IndexData, l_NumIndexs);
					else
						l_RV=new CUABTriangleListRenderableIndexed32Vertexs<MV_POSITION_NORMAL_TEXTURE_VERTEX>(l_VertexData, l_NumVertexs, l_IndexData, l_NumIndexs);
				}
				else if(l_VertexType==MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX::GetVertexType())
				{
					if(l_IndexType==16)
						l_RV=new CUABTriangleListRenderableIndexed16Vertexs<MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX>(l_VertexData, l_NumVertexs, l_IndexData, l_NumIndexs);
					else
						l_RV=new CUABTriangleListRenderableIndexed32Vertexs<MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX>(l_VertexData, l_NumVertexs, l_IndexData, l_NumIndexs);
				}
				else if(l_VertexType==MV_POSITION4_COLOR_TEXTURE_VERTEX::GetVertexType())
				{
					if(l_IndexType==16)
						l_RV=new CUABTriangleListRenderableIndexed16Vertexs<MV_POSITION4_COLOR_TEXTURE_VERTEX>(l_VertexData, l_NumVertexs, l_IndexData, l_NumIndexs);
					else
						l_RV=new CUABTriangleListRenderableIndexed32Vertexs<MV_POSITION4_COLOR_TEXTURE_VERTEX>(l_VertexData, l_NumVertexs, l_IndexData, l_NumIndexs);
				}
				else if(l_VertexType==MV_POSITION_COLOR_VERTEX::GetVertexType())
				{
					if(l_IndexType==16)
						l_RV=new CUABTriangleListRenderableIndexed16Vertexs<MV_POSITION_COLOR_VERTEX>(l_VertexData, l_NumVertexs, l_IndexData, l_NumIndexs);
					else
						l_RV=new CUABTriangleListRenderableIndexed32Vertexs<MV_POSITION_COLOR_VERTEX>(l_VertexData, l_NumVertexs, l_IndexData, l_NumIndexs);
				}
				else if(l_VertexType==MV_POSITION_TEXTURE_VERTEX::GetVertexType())
				{
					if(l_IndexType==16)
						l_RV=new CUABTriangleListRenderableIndexed16Vertexs<MV_POSITION_TEXTURE_VERTEX>(l_VertexData, l_NumVertexs, l_IndexData, l_NumIndexs);
					else
						l_RV=new CUABTriangleListRenderableIndexed32Vertexs<MV_POSITION_TEXTURE_VERTEX>(l_VertexData, l_NumVertexs, l_IndexData, l_NumIndexs);
				}

				else if(l_VertexType==MV_POSITION_COLOR_TEXTURE_VERTEX::GetVertexType())
				{
					if(l_IndexType==16)
						l_RV=new CUABTriangleListRenderableIndexed16Vertexs<MV_POSITION_COLOR_TEXTURE_VERTEX>(l_VertexData, l_NumVertexs, l_IndexData, l_NumIndexs);
					else
						l_RV=new CUABTriangleListRenderableIndexed32Vertexs<MV_POSITION_COLOR_TEXTURE_VERTEX>(l_VertexData, l_NumVertexs, l_IndexData, l_NumIndexs);
				}

				free(l_VertexData);
				free(l_IndexData);

				m_RVs.push_back(l_RV);
			}

			// Read Footer
			l_File.read((char *) &l_BufferUnsignedShort, sizeof(short));

			if(l_BufferUnsignedShort == FOOTER)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
}

bool CStaticMesh::Reload()
{
	if(m_Name!="")
	{
		Destroy();
		return Load(m_Name);
	}
	else
	{
		return false;
	}
}

void CStaticMesh::Render(CRenderManager *RM) const
{
	for(int i = 0 ;i<m_RVs.size();i++)
	{
		m_Materials[i]->Apply();
		CEffectTechnique* l_ET = m_Materials[i]->GetEffectTechnique();
		m_RVs[i]->RenderIndexed(RM,l_ET,&(UABEngine.GetEffectManager()->m_Parameters));
	}
}

bool CStaticMesh::Destroy()
{
	m_NumVertexs = 0;
	m_NumFaces = 0;
	for(int i = 0 ;i<m_RVs.size();i++)
	{
		CHECKED_DELETE(m_RVs[i]);
	}
	
	m_RVs.clear();
	for(int i = 0 ;i<m_Materials.size();i++)
	{
		CHECKED_DELETE(m_Materials[i]);
	}
	m_Materials.clear();

	return true;
}