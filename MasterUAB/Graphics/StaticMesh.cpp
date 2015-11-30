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

CStaticMesh::CStaticMesh(void):CNamed("")
{
}

CStaticMesh::~CStaticMesh(void)
{
}

bool CStaticMesh::Load(const std::string &FileName)
{
	m_Name = FileName;

	std::fstream l_file(m_Name, std::ios::binary | std::ios::in);
	if (!l_file.is_open())
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

		l_file.seekg(0);

		l_file.read((char *) &l_BufferUnsignedShort, sizeof(short));
		std::cout << "Header: l_BufferUnsignedShort = " << std::hex << l_BufferShort << std::endl;

		if(l_BufferUnsignedShort == HEADER)
		{

			l_file.read((char *) &l_BufferLong, sizeof(long));
			std::cout << "Number of Materials: l_BufferLong = " << std::dec << l_BufferLong << std::endl;
			l_NumMaterials = l_BufferLong;

			for(int i=0; i<l_NumMaterials; i++)
			{
				l_file.read((char *) &l_BufferShort, sizeof(short));
				std::cout << "Material length: l_BufferShort = " << std::dec << l_BufferShort << std::endl;
				l_MatLength = l_BufferShort+1;

				for(int i=0; i<l_MatLength; i++)
				{
					l_file.read(&l_BufferChar, sizeof(l_BufferChar));
					l_BufferString.append(&l_BufferChar, sizeof(l_BufferChar));
				}
				std::cout << "Material name: l_BufferString = " << l_BufferString << std::endl;
				m_Materials.push_back(UABEngine.GetMaterialManager()->GetResource(l_BufferString));
			}
			for(int i=0; i<l_NumMaterials; i++)
			{
				l_file.read((char *) &l_BufferUnsignedShort, sizeof(unsigned short));
				l_VertexType = l_BufferUnsignedShort;
				std::cout << "Vertex Size: l_BufferUnsignedShort = " << std::dec << l_VertexType << std::endl;

				l_file.read((char *) &l_BufferLong, sizeof(long));
				int l_NumVertexs = l_BufferLong;
				std::cout << "Number of vertexs: l_BufferLong = " << std::dec << m_NumVertexs << std::endl;

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

				void* l_Data = NULL;
				l_Data = malloc(l_NumBytes);

				l_file.read((char *) l_Data, l_NumBytes);

				l_file.read((char *) &l_BufferUnsignedShort, sizeof(unsigned short));
				l_IndexType = l_BufferUnsignedShort;
				std::cout << "Index Size: l_BufferUnsignedShort = " << std::dec << l_IndexType << std::endl;

				l_file.read((char *) &l_BufferLong, sizeof(long));
				int l_NumIndexs = l_BufferLong;
				std::cout << "Number of indexs: l_BufferLong = " << std::dec << l_NumIndexs << std::endl;

				void* l_IndexData = NULL;
				l_IndexData = malloc(l_IndexType*l_NumIndexs);

				l_file.read((char *) l_IndexData, l_NumBytes);

				CRenderableVertexs* l_RV;

				if(l_VertexType==MV_POSITION_NORMAL_TEXTURE_VERTEX::GetVertexType())
				{
					if(l_IndexType==16)
						l_RV=new CUABTriangleListRenderableIndexed16Vertexs<MV_POSITION_NORMAL_TEXTURE_VERTEX>(l_Data, l_NumVertexs, l_IndexData, l_NumIndexs);
					else
						l_RV=new CUABTriangleListRenderableIndexed32Vertexs<MV_POSITION_NORMAL_TEXTURE_VERTEX>(l_Data, l_NumVertexs, l_IndexData, l_NumIndexs);
				}
				else if(l_VertexType==MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX::GetVertexType())
				{
					if(l_IndexType==16)
						l_RV=new CUABTriangleListRenderableIndexed16Vertexs<MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX>(l_Data, l_NumVertexs, l_IndexData, l_NumIndexs);
					else
						l_RV=new CUABTriangleListRenderableIndexed32Vertexs<MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX>(l_Data, l_NumVertexs, l_IndexData, l_NumIndexs);
				}
				else if(l_VertexType==MV_POSITION4_COLOR_TEXTURE_VERTEX::GetVertexType())
				{
					if(l_IndexType==16)
						l_RV=new CUABTriangleListRenderableIndexed16Vertexs<MV_POSITION4_COLOR_TEXTURE_VERTEX>(l_Data, l_NumVertexs, l_IndexData, l_NumIndexs);
					else
						l_RV=new CUABTriangleListRenderableIndexed32Vertexs<MV_POSITION4_COLOR_TEXTURE_VERTEX>(l_Data, l_NumVertexs, l_IndexData, l_NumIndexs);
				}
				else if(l_VertexType==MV_POSITION_COLOR_VERTEX::GetVertexType())
				{
					if(l_IndexType==16)
						l_RV=new CUABTriangleListRenderableIndexed16Vertexs<MV_POSITION_COLOR_VERTEX>(l_Data, l_NumVertexs, l_IndexData, l_NumIndexs);
					else
						l_RV=new CUABTriangleListRenderableIndexed32Vertexs<MV_POSITION_COLOR_VERTEX>(l_Data, l_NumVertexs, l_IndexData, l_NumIndexs);
				}
				else if(l_VertexType==MV_POSITION_TEXTURE_VERTEX::GetVertexType())
				{
					if(l_IndexType==16)
						l_RV=new CUABTriangleListRenderableIndexed16Vertexs<MV_POSITION_TEXTURE_VERTEX>(l_Data, l_NumVertexs, l_IndexData, l_NumIndexs);
					else
						l_RV=new CUABTriangleListRenderableIndexed32Vertexs<MV_POSITION_TEXTURE_VERTEX>(l_Data, l_NumVertexs, l_IndexData, l_NumIndexs);
				}

				else if(l_VertexType==MV_POSITION_COLOR_TEXTURE_VERTEX::GetVertexType())
				{
					if(l_IndexType==16)
						l_RV=new CUABTriangleListRenderableIndexed16Vertexs<MV_POSITION_COLOR_TEXTURE_VERTEX>(l_Data, l_NumVertexs, l_IndexData, l_NumIndexs);
					else
						l_RV=new CUABTriangleListRenderableIndexed32Vertexs<MV_POSITION_COLOR_TEXTURE_VERTEX>(l_Data, l_NumVertexs, l_IndexData, l_NumIndexs);
				}

				m_RVs.push_back(l_RV);
			}
			l_file.read((char *) &l_BufferUnsignedShort, sizeof(short));
			std::cout << "Footer: l_BufferUnsignedShort = " << std::hex << l_BufferShort << std::endl;

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
		Load(m_Name);
		return true;
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
		CEffectTechnique* l_ET = m_Materials[i]->GetEffectTechnique();
		m_RVs[i]->RenderIndexed(RM,l_ET,0);
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