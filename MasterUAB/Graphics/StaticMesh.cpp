#include "StaticMesh.h"
#include "RenderManager.h"
#include <iostream>
#include <fstream>
#include <cstring>

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
		long l_BufferLong;
		float l_BufferFloat;
		std::string l_BufferString;

		short l_MatLength;
		unsigned short l_VertexSize;
		unsigned short l_IndexSize;
		long l_NumMaterials;

		l_file.seekg(0);

		l_file.read((char *) &l_BufferShort, sizeof(short));
		std::cout << "Header: l_BufferShort = " << std::hex << l_BufferShort << std::endl;

		if(l_BufferShort == -427)
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
			}

			l_file.read((char *) &l_BufferUnsignedShort, sizeof(unsigned short));
			l_VertexSize = l_BufferUnsignedShort;
			std::cout << "Vertex Size: l_BufferUnsignedShort = " << std::dec << l_VertexSize << std::endl;

			l_file.read((char *) &l_BufferLong, sizeof(long));
			m_NumVertexs = l_BufferLong;
			std::cout << "Number of vertexs: l_BufferLong = " << std::dec << m_NumVertexs << std::endl;

			for(int i=0; i<m_NumVertexs; i++)
			{
				std::cout << "Vertex number " << i << std::endl;
				int l_VertexRead = 0;
				//while(l_VertexRead < l_VertexSize ){
				for(int j=0; j<8; j++){
					//std::cout << "Reading pos " << l_VertexRead << std::endl;
					//std::cout << "Current Seekg: " << l_CurrentSeekG << std::endl;
					l_file.read((char *) &l_BufferFloat, sizeof(float));
					std::cout << "Vertex Property: l_BufferFloat = " << std::dec << l_BufferFloat << std::endl;
					//l_VertexRead += sizeof(float);
				}
			}

			l_file.read((char *) &l_BufferUnsignedShort, sizeof(unsigned short));
			l_IndexSize = l_BufferUnsignedShort;
			std::cout << "Index Size: l_BufferUnsignedShort = " << std::dec << l_VertexSize << std::endl;

			l_file.read((char *) &l_BufferLong, sizeof(long));
			m_NumIndexs = l_BufferLong;
			std::cout << "Number of indexs: l_BufferLong = " << std::dec << l_NumVertexs << std::endl;

			for(int i=0; i<m_NumIndexs; i++)
			{
				std::cout << "Index number " << i << std::endl;
				l_file.read((char *) &l_BufferShort, sizeof(short));
				std::cout << "Index Property: l_BufferShort = " << std::dec << l_BufferShort << std::endl;
			}

			l_file.read((char *) &l_BufferShort, sizeof(short));
			std::cout << "Footer: l_BufferShort = " << std::hex << l_BufferShort << std::endl;

			if(l_BufferShort == 22014)
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