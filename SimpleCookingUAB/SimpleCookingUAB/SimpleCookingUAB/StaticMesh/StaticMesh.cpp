#include "StaticMesh\StaticMesh.h"

#include "RenderableVertexs.h"
#include "VertexTypes.h"
#include "TemplatedRenderableIndexedVertexs.h"

#include "Base\Utils.h"

#include <iostream>
#include <fstream>
#include <cstring>

#include <assert.h>

#define HEADER 65109
#define FOOTER 22014

CStaticMesh::CStaticMesh(void) :CNamed(""), m_PhysxMeshesDirectory("")
{	
}

CStaticMesh::~CStaticMesh(void)
{
	Destroy();
}

bool CStaticMesh::Load(const std::string &FileName)
{
	std::fstream l_File(FileName, std::ios::binary | std::ios::in);
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
			}

			for(int i=0; i<l_NumMaterials; i++)
			{
				int l_NumBytes = 0;
				// Read Vertex Type
				l_File.read((char *) &l_BufferUnsignedShort, sizeof(unsigned short));
				l_VertexType = l_BufferUnsignedShort;

				// Read Number of Vertexs
				l_File.read((char *) &l_BufferLong, sizeof(long));
				int l_NumVertexs = l_BufferLong;

				if(l_VertexType==MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX::GetVertexType())
					l_NumBytes = sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX)*l_NumVertexs;
				else if(l_VertexType==MV_POSITION_NORMAL_TEXTURE_VERTEX::GetVertexType())
					l_NumBytes = sizeof(MV_POSITION_NORMAL_TEXTURE_VERTEX)*l_NumVertexs;
				else if (l_VertexType == MV_POSITION_NORMAL_TEXTURE_TEXTURE2_VERTEX::GetVertexType())
					l_NumBytes = sizeof(MV_POSITION_NORMAL_TEXTURE_TEXTURE2_VERTEX)*l_NumVertexs;
				else if(l_VertexType==MV_POSITION4_COLOR_TEXTURE_VERTEX::GetVertexType())
					l_NumBytes = sizeof(MV_POSITION4_COLOR_TEXTURE_VERTEX)*l_NumVertexs;
				else if(l_VertexType==MV_POSITION_COLOR_VERTEX::GetVertexType())
					l_NumBytes = sizeof(MV_POSITION_COLOR_VERTEX)*l_NumVertexs;
				else if(l_VertexType==MV_POSITION_TEXTURE_VERTEX::GetVertexType())
					l_NumBytes = sizeof(MV_POSITION_TEXTURE_VERTEX)*l_NumVertexs;
				else if(l_VertexType==MV_POSITION_COLOR_TEXTURE_VERTEX::GetVertexType())
					l_NumBytes = sizeof(MV_POSITION_COLOR_TEXTURE_VERTEX)*l_NumVertexs;
				else if(l_VertexType==MV_POSITION_NORMAL_TEXTURE_TANGENT_VERTEX::GetVertexType())
					l_NumBytes = sizeof(MV_POSITION_NORMAL_TEXTURE_TANGENT_VERTEX)*l_NumVertexs;
				else if (l_VertexType == MV_POSITION_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX::GetVertexType())
					l_NumBytes = sizeof(MV_POSITION_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX)*l_NumVertexs;
				else if (l_VertexType == MV_POSITION_NORMAL_TEXTURE_TEXTURE2_BINORMAL_TANGENT_VERTEX::GetVertexType())
					l_NumBytes = sizeof(MV_POSITION_NORMAL_TEXTURE_TEXTURE2_BINORMAL_TANGENT_VERTEX)*l_NumVertexs;
				
				// Read Vertex Data
				void *l_VertexData = NULL;
				l_VertexData = malloc(l_NumBytes);
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
				else if(l_VertexType==MV_POSITION_NORMAL_TEXTURE_TEXTURE2_VERTEX::GetVertexType())
				{
					if(l_IndexType==16)
						l_RV=new CUABTriangleListRenderableIndexed16Vertexs<MV_POSITION_NORMAL_TEXTURE_TEXTURE2_VERTEX>(l_VertexData, l_NumVertexs, l_IndexData, l_NumIndexs);
					else
						l_RV=new CUABTriangleListRenderableIndexed32Vertexs<MV_POSITION_NORMAL_TEXTURE_TEXTURE2_VERTEX>(l_VertexData, l_NumVertexs, l_IndexData, l_NumIndexs);
				}
				else if(l_VertexType==MV_POSITION_NORMAL_TEXTURE_TANGENT_VERTEX::GetVertexType())
				{
					if(l_IndexType==16)
						l_RV=new CUABTriangleListRenderableIndexed16Vertexs<MV_POSITION_NORMAL_TEXTURE_TANGENT_VERTEX>(l_VertexData, l_NumVertexs, l_IndexData, l_NumIndexs);
					else
						l_RV=new CUABTriangleListRenderableIndexed32Vertexs<MV_POSITION_NORMAL_TEXTURE_TANGENT_VERTEX>(l_VertexData, l_NumVertexs, l_IndexData, l_NumIndexs);
				}
				else if (l_VertexType == MV_POSITION_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX::GetVertexType())
				{
/*
					void CStaticMesh::CalcTangentsAndBinormals(
						void *VtxsData, 
						unsigned short *IdxsData, 
						size_t VtxCount, 
						size_t IdxCount, 
						size_t VertexStride, 
						size_t GeometryStride, 
						size_t NormalStride, 
						size_t TangentStride,
						size_t BiNormalStride,
						size_t TextureCoordsStride)*/

					/*CalcTangentsAndBinormals(l_VertexData, (unsigned short*)l_IndexData, l_NumVertexs, l_NumIndexs,
						sizeof(MV_POSITION_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX), 0, sizeof(float) * 3, sizeof(float) * 8, sizeof(float) * 12, sizeof(float) * 6);
*/			
					if (l_IndexType == 16)
						l_RV = new CUABTriangleListRenderableIndexed16Vertexs<MV_POSITION_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX>(l_VertexData, l_NumVertexs, l_IndexData, l_NumIndexs);
					else
						l_RV = new CUABTriangleListRenderableIndexed32Vertexs<MV_POSITION_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX>(l_VertexData, l_NumVertexs, l_IndexData, l_NumIndexs);
				}
				else if (l_VertexType == MV_POSITION_NORMAL_TEXTURE_TEXTURE2_BINORMAL_TANGENT_VERTEX::GetVertexType())
				{
					//CalcTangentsAndBinormals(l_VertexData, (unsigned short*)l_IndexData, l_NumVertexs, l_NumIndexs,
					//	sizeof(MV_POSITION_NORMAL_TEXTURE_TEXTURE2_BINORMAL_TANGENT_VERTEX), 0, sizeof(float) * 3, sizeof(float) * 8, sizeof(float) * 12, sizeof(float) * 6);
					if (l_IndexType == 16)
						l_RV = new CUABTriangleListRenderableIndexed16Vertexs<MV_POSITION_NORMAL_TEXTURE_TEXTURE2_BINORMAL_TANGENT_VERTEX>(l_VertexData, l_NumVertexs, l_IndexData, l_NumIndexs);
					else
						l_RV = new CUABTriangleListRenderableIndexed32Vertexs<MV_POSITION_NORMAL_TEXTURE_TEXTURE2_BINORMAL_TANGENT_VERTEX>(l_VertexData, l_NumVertexs, l_IndexData, l_NumIndexs);
				}
				
				
				free(l_VertexData);
				free(l_IndexData);

				m_RVs.push_back(l_RV);
			}

			int l_NumBytes = 0;

			// Read Bounding Box
			l_NumBytes = sizeof(float)*6;
			void *l_BoundingBox = NULL;
			l_BoundingBox = malloc(l_NumBytes);
			l_File.read((char *) l_BoundingBox, l_NumBytes);

			free(l_BoundingBox);

			// Read Bounding Sphere
			l_NumBytes = sizeof(float)*4;
			void *l_BoundingSphere = NULL;
			l_BoundingSphere = malloc(l_NumBytes);
			l_File.read((char *) l_BoundingSphere, l_NumBytes);
			
			free(l_BoundingSphere);

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

bool CStaticMesh::Destroy()
{
	return true;
}