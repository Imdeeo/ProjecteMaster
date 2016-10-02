#include "StaticMesh\StaticMesh.h"

#include "Engine\UABEngine.h"
#include "RenderManager\RenderManager.h"
#include "Effects\EffectManager.h"
#include "Materials\MaterialManager.h"

#include "RenderableObjects\RenderableVertexs.h"
#include "RenderableObjects\VertexTypes.h"
#include "RenderableObjects\TemplatedRenderableIndexedVertexs.h"
#include "RenderableObjects\RenderableObjectTechnique.h"

#include "LevelManager\LevelManager.h"

#include "Utils.h"

#include <iostream>
#include <fstream>
#include <cstring>

#include <assert.h>

#define HEADER 65109
#define FOOTER 22014

CStaticMesh::CStaticMesh(const std::string &_LevelId):CNamed(""),CLevelInfo(_LevelId),
	m_BoundingSphereRadius(0.f)
{	
}

CStaticMesh::~CStaticMesh(void)
{
	Destroy();
}

bool CStaticMesh::Load(const std::string &FileName)
{
	m_FileName = FileName;

	std::fstream l_File(m_FileName, std::ios::binary | std::ios::in);
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
 				m_Materials.push_back(UABEngine.GetLevelManager()->GetResource(m_Level)->GetMaterialManager()->GetResource(l_BufferString));
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
				else{
					UtilsLog("Vertex type not recognized");
				}
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
				else if (l_VertexType == MV_POSITION_NORMAL_TEXTURE_TEXTURE2_TANGENT_VERTEX::GetVertexType())
				{
					if (l_IndexType == 16)
						l_RV = new CUABTriangleListRenderableIndexed16Vertexs<MV_POSITION_NORMAL_TEXTURE_TEXTURE2_TANGENT_VERTEX>(l_VertexData, l_NumVertexs, l_IndexData, l_NumIndexs);
					else
						l_RV = new CUABTriangleListRenderableIndexed32Vertexs<MV_POSITION_NORMAL_TEXTURE_TEXTURE2_TANGENT_VERTEX>(l_VertexData, l_NumVertexs, l_IndexData, l_NumIndexs);
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
					CalcTangentsAndBinormals(l_VertexData,													//void *VtxsData, 
						(unsigned short*)l_IndexData,														//unsigned short *IdxsData, 
						l_NumVertexs,																		//size_t VtxCount, 
						l_NumIndexs,																		//size_t IdxCount, 
						sizeof(MV_POSITION_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX),							//size_t VertexStride, 
						offsetof(MV_POSITION_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX, Position),				//size_t GeometryStride, 
						offsetof(MV_POSITION_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX, Normal),				//size_t NormalStride, 
						offsetof(MV_POSITION_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX, Tangent),				//size_t TangentStride,
						offsetof(MV_POSITION_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX, Binormal),				//size_t BiNormalStride,
						offsetof(MV_POSITION_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX, UV));					//size_t TextureCoordsStride

					
					if (l_IndexType == 16)
						l_RV = new CUABTriangleListRenderableIndexed16Vertexs<MV_POSITION_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX>(l_VertexData, l_NumVertexs, l_IndexData, l_NumIndexs);
					else
						l_RV = new CUABTriangleListRenderableIndexed32Vertexs<MV_POSITION_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX>(l_VertexData, l_NumVertexs, l_IndexData, l_NumIndexs);
				}
				else if (l_VertexType == MV_POSITION_NORMAL_TEXTURE_TEXTURE2_BINORMAL_TANGENT_VERTEX::GetVertexType())
				{	
					CalcTangentsAndBinormals(l_VertexData,													//void *VtxsData, 
						(unsigned short*)l_IndexData,														//unsigned short *IdxsData, 
						l_NumVertexs,																		//size_t VtxCount, 
						l_NumIndexs,																		//size_t IdxCount, 
						sizeof(MV_POSITION_NORMAL_TEXTURE_TEXTURE2_BINORMAL_TANGENT_VERTEX),				//size_t VertexStride, 
						offsetof(MV_POSITION_NORMAL_TEXTURE_TEXTURE2_BINORMAL_TANGENT_VERTEX, Position),	//size_t GeometryStride, 
						offsetof(MV_POSITION_NORMAL_TEXTURE_TEXTURE2_BINORMAL_TANGENT_VERTEX, Normal),		//size_t NormalStride, 
						offsetof(MV_POSITION_NORMAL_TEXTURE_TEXTURE2_BINORMAL_TANGENT_VERTEX, Tangent),		//size_t TangentStride,
						offsetof(MV_POSITION_NORMAL_TEXTURE_TEXTURE2_BINORMAL_TANGENT_VERTEX, Binormal),	//size_t BiNormalStride,
						offsetof(MV_POSITION_NORMAL_TEXTURE_TEXTURE2_BINORMAL_TANGENT_VERTEX, UV));			//size_t TextureCoordsStride

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

			//if (m_Name == "TriggerArtifactDoor")
			//	UtilsLog("Stahp");
			
			m_BoundingBoxMin.x = ((float*)l_BoundingBox)[0];
			m_BoundingBoxMin.y = ((float*)l_BoundingBox)[1];
			m_BoundingBoxMin.z = ((float*)l_BoundingBox)[2];
			m_BoundingBoxMax.x = ((float*)l_BoundingBox)[3];
			m_BoundingBoxMax.y = ((float*)l_BoundingBox)[4];
			m_BoundingBoxMax.z = ((float*)l_BoundingBox)[5];

			free(l_BoundingBox);

			// Read Bounding Sphere
			l_NumBytes = sizeof(float)*4;
			void *l_BoundingSphere = NULL;
			l_BoundingSphere = malloc(l_NumBytes);
			l_File.read((char *) l_BoundingSphere, l_NumBytes);

			m_BoundingSphereCenter.x = ((float*)l_BoundingSphere)[0];
			m_BoundingSphereCenter.y = ((float*)l_BoundingSphere)[1];
			m_BoundingSphereCenter.z = ((float*)l_BoundingSphere)[2];
			m_BoundingSphereRadius = ((float*)l_BoundingSphere)[3];
			
			free(l_BoundingSphere);

			// Read Footer
			l_File.read((char *) &l_BufferUnsignedShort, sizeof(short));

			if(l_BufferUnsignedShort == FOOTER)
			{
				return true;
			}
			else
			{
				assert("Mesh file unreadable!");
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
	if (m_FileName != "")
	{
		Destroy();
		return Load(m_FileName);
	}
	else
	{
		return false;
	}
}

void CStaticMesh::Render(CRenderManager *RM) const
{
	for (size_t i = 0; i<m_RVs.size(); i++)
	{
		m_Materials[i]->Apply();
		CEffectTechnique* l_ET = m_Materials[i]->GetRenderableObjectTechnique()->GetEffectTechnique();
		CEffectManager::SetSceneConstants(l_ET);
		m_RVs[i]->RenderIndexed(RM,l_ET,CEffectManager::GetRawData());
	}
}

bool CStaticMesh::Destroy()
{
	m_NumVertexs = 0;
	m_NumFaces = 0;
	for (size_t i = 0; i<m_RVs.size(); i++)
	{
		CHECKED_DELETE(m_RVs[i]);
	}
	
	m_RVs.clear();

	m_Materials.clear();

	return true;
}

void CStaticMesh::CalcTangentsAndBinormals(void *VtxsData, unsigned short *IdxsData, size_t
	VtxCount, size_t IdxCount, size_t VertexStride, size_t GeometryStride, size_t
	NormalStride, size_t TangentStride, size_t BiNormalStride, size_t TextureCoordsStride)
{
	Vect3f* tan1 = new Vect3f[VtxCount * 2];
	Vect3f* tan2 = tan1 + VtxCount;
	ZeroMemory(tan1, VtxCount * sizeof(Vect3f) * 2);
	unsigned char *l_VtxAddress = (unsigned char *)VtxsData;

	for (size_t b = 0; b<IdxCount; b += 3)
	{
		unsigned short i1 = IdxsData[b];
		unsigned short i2 = IdxsData[b + 1];
		unsigned short i3 = IdxsData[b + 2];
		Vect3f *v1 = (Vect3f *)&l_VtxAddress[i1*VertexStride + GeometryStride];
		Vect3f *v2 = (Vect3f *)&l_VtxAddress[i2*VertexStride + GeometryStride];
		Vect3f *v3 = (Vect3f *)&l_VtxAddress[i3*VertexStride + GeometryStride];
		Vect2f *w1 = (Vect2f *)&l_VtxAddress[i1*VertexStride + TextureCoordsStride];
		Vect2f *w2 = (Vect2f *)&l_VtxAddress[i2*VertexStride + TextureCoordsStride];
		Vect2f *w3 = (Vect2f *)&l_VtxAddress[i3*VertexStride + TextureCoordsStride];
		float x1 = v2->x - v1->x;
		float x2 = v3->x - v1->x;
		float y1 = v2->y - v1->y;
		float y2 = v3->y - v1->y;
		float z1 = v2->z - v1->z;
		float z2 = v3->z - v1->z;
		float s1 = w2->x - w1->x;
		float s2 = w3->x - w1->x;
		float t1 = w2->y - w1->y;
		float t2 = w3->y - w1->y;
		float r = 1.0F / (s1 * t2 - s2 * t1);
		Vect3f tdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
		Vect3f sdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);
		assert(i1<VtxCount);
		assert(i2<VtxCount);
		assert(i3<VtxCount);
		tan1[i1] += sdir;
		tan1[i2] += sdir;
		tan1[i3] += sdir;
		tan2[i1] += tdir;
		tan2[i2] += tdir;
		tan2[i3] += tdir;
	}

	for (size_t b = 0; b<VtxCount; ++b)
	{
		Vect3f *l_NormalVtx = (Vect3f *)&l_VtxAddress[b*VertexStride + NormalStride];
		Vect3f *l_TangentVtx = (Vect3f *)&l_VtxAddress[b*VertexStride + TangentStride];
		Vect4f *l_TangentVtx4 = (Vect4f *)&l_VtxAddress[b*VertexStride + TangentStride];
		Vect3f *l_BiNormalVtx = (Vect3f *)&l_VtxAddress[b*VertexStride + BiNormalStride];
		const Vect3f& t = tan1[b];
		// Gram-Schmidt orthogonalize
		Vect3f l_VAl = t - (*l_NormalVtx)*((*l_NormalVtx)*t);
		l_VAl.Normalize();
		*l_TangentVtx = l_VAl;
		// Calculate handedness
		Vect3f l_Cross;
		l_Cross = (*l_NormalVtx) ^ (*l_TangentVtx);
		l_TangentVtx4->w = (l_Cross*(tan2[b]))< 0.0f ? -1.0f : 1.0f;
		*l_BiNormalVtx = (*l_NormalVtx) ^ (*l_TangentVtx);
	}
	delete[] tan1;
}

std::vector<CMaterial *> CStaticMesh::GetMaterials()const
{
	return m_Materials;
}