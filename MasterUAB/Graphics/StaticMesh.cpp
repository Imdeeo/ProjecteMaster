#include "StaticMesh.h"
#include "RenderManager.h"
#include "RenderableVertexs.h"
#include "Material.h"
#include "VertexTypes.h"
#include "UABEngine.h"
#include "EffectManager.h"
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
		long l_BufferLong;
		float l_BufferFloat;
		std::string l_BufferString;

		short l_MatLength;
		unsigned short l_VertexType;
		unsigned short l_IndexType;
		long l_NumMaterials;
		int l_NumBytes;

		l_File.seekg(0);

		l_File.read((char *) &l_BufferUnsignedShort, sizeof(short));
		std::cout << "Header: l_BufferUnsignedShort = " << std::hex << l_BufferShort << std::endl;

		if(l_BufferUnsignedShort == HEADER)
		{

			l_File.read((char *) &l_BufferLong, sizeof(long));
			std::cout << "Number of Materials: l_BufferLong = " << std::dec << l_BufferLong << std::endl;
			l_NumMaterials = l_BufferLong;

			for(int i=0; i<l_NumMaterials; i++)
			{
				l_File.read((char *) &l_BufferShort, sizeof(short));
				std::cout << "Material length: l_BufferShort = " << std::dec << l_BufferShort << std::endl;
				l_MatLength = l_BufferShort+1;

				for(int i=0; i<l_MatLength; i++)
				{
					l_File.read(&l_BufferChar, sizeof(l_BufferChar));
					l_BufferString.append(&l_BufferChar, sizeof(l_BufferChar));
				}
				std::cout << "Material name: l_BufferString = " << l_BufferString << std::endl;
				m_Materials.push_back(UABEngine.GetMaterialManager()->GetResource(l_BufferString));
			}
			for(int i=0; i<l_NumMaterials; i++)
			{
				void *l_VtxsData=NULL;
				void *l_IdxData=NULL;

				l_File.read((char *) &l_BufferUnsignedShort, sizeof(unsigned short));
				l_VertexType = l_BufferUnsignedShort;
				std::cout << "Vertex Type: l_BufferUnsignedShort = " << std::dec << l_VertexType << std::endl;

				l_File.read((char *) &l_BufferLong, sizeof(long));
				m_NumVertexs = l_BufferLong;
				std::cout << "Number of vertexs: l_BufferLong = " << std::dec << m_NumVertexs << std::endl;

				if(l_VertexType==MV_POSITION_NORMAL_TEXTURE_VERTEX::GetVertexType())
					l_NumBytes=sizeof(MV_POSITION_NORMAL_TEXTURE_VERTEX)*m_NumVertexs;
				else if(l_VertexType==MV_POSITION_COLOR_VERTEX::GetVertexType())
					l_NumBytes=sizeof(MV_POSITION_COLOR_VERTEX)*m_NumVertexs;
				else if(l_VertexType==MV_POSITION_TEXTURE_VERTEX::GetVertexType())
					l_NumBytes=sizeof(MV_POSITION_TEXTURE_VERTEX)*m_NumVertexs;
				else if(l_VertexType==MV_POSITION_COLOR_TEXTURE_VERTEX::GetVertexType())
					l_NumBytes=sizeof(MV_POSITION_COLOR_TEXTURE_VERTEX)*m_NumVertexs;

				l_VtxsData=malloc(l_NumBytes);
				l_File.read((char *) &l_VtxsData, l_NumBytes);

				/*for(int i=0; i<m_NumVertexs; i++)
				{
				std::cout << "Vertex number " << i << std::endl;
				int l_VertexRead = 0;
				//while(l_VertexRead < l_VertexSize ){

				for(int j=0; j<8; j++){
				//std::cout << "Reading pos " << l_VertexRead << std::endl;
				//std::cout << "Current Seekg: " << l_CurrentSeekG << std::endl;
				l_File.read((char *) &l_BufferFloat, sizeof(float));
				std::cout << "Vertex Property: l_BufferFloat = " << std::dec << l_BufferFloat << std::endl;
				//l_VertexRead += sizeof(float);
				}
				}*/

				l_File.read((char *) &l_BufferUnsignedShort, sizeof(unsigned short));
				l_IndexType = l_BufferUnsignedShort;
				std::cout << "Index Type: l_BufferUnsignedShort = " << std::dec << l_IndexType << std::endl;

				/*l_File.read((char *) &l_BufferLong, sizeof(long));
				m_NumIndexs = l_BufferLong;
				std::cout << "Number of indexs: l_BufferLong = " << std::dec << m_NumIndexs << std::endl;*/

				if(l_IndexType==16)
				{
					unsigned short l_NumIndexsFile;
					l_File.read((char *) &l_NumIndexsFile, sizeof(unsigned short));
					l_NumBytes=sizeof(unsigned short)*l_NumIndexsFile;
					m_NumIndexs=(unsigned int)l_NumIndexsFile;
				}
				else if(l_IndexType==32)
				{
					unsigned int l_NumIndexsFile;
					l_File.read((char *) &l_NumIndexsFile, sizeof(unsigned int));
					l_NumBytes=sizeof(unsigned int)*l_NumIndexsFile;
					m_NumIndexs=l_NumIndexsFile;
				}

				l_IdxData=malloc(l_NumBytes);
				l_File.read((char *) &l_IdxData, l_NumBytes);

				/*for(int i=0; i<m_NumIndexs; i++)
				{
				std::cout << "Index number " << i << std::endl;
				l_File.read((char *) &l_BufferShort, sizeof(short));
				std::cout << "Index Property: l_BufferShort = " << std::dec << l_BufferShort << std::endl;
				}*/

				CRenderableVertexs *l_RV=NULL;

				if(l_VertexType==MV_POSITION_NORMAL_TEXTURE_VERTEX::GetVertexType())
				{
					if(l_IndexType==16)
						l_RV=new CUABTriangleListRenderableIndexed16Vertexs<MV_POSITION_NORMAL_TEXTURE_VERTEX>(l_VtxsData, m_NumVertexs, l_IdxData, m_NumIndexs);
					else
						l_RV=new CUABTriangleListRenderableIndexed32Vertexs<MV_POSITION_NORMAL_TEXTURE_VERTEX>(l_VtxsData, m_NumVertexs, l_IdxData, m_NumIndexs);
				}
				else if(l_VertexType==MV_POSITION_NORMAL_TEXTURE_VERTEX::GetVertexType())
				{
					if(l_IndexType==16)
						l_RV=new CUABTriangleListRenderableIndexed16Vertexs<MV_POSITION_NORMAL_TEXTURE_VERTEX>(l_VtxsData, m_NumVertexs, l_IdxData, m_NumIndexs);
					else
						l_RV=new CUABTriangleListRenderableIndexed32Vertexs<MV_POSITION_NORMAL_TEXTURE_VERTEX>(l_VtxsData, m_NumVertexs, l_IdxData, m_NumIndexs);
				}

				free(l_VtxsData);
				free(l_IdxData);

				m_RVs.push_back(l_RV);
			}

			l_File.read((char *) &l_BufferUnsignedShort, sizeof(short));
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

}

bool CStaticMesh::Destroy()
{
	m_NumVertexs = 0;
	m_NumFaces = 0;
	CHECKED_RELEASE(m_RVs);
	m_RVs.clear();
	CHECKED_RELEASE(m_Materials);
	m_Materials.clear();
}