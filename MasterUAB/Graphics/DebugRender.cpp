#include "DebugRender.h"

#include "VertexTypes.h"
#include "Effect.h"

CDebugRender::CDebugRender(ID3D11Device* _Device)
{
	{
		//AXIS
		MV_POSITION_COLOR_VERTEX l_AxisVtxs[6] =
		{
			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(1.0f, 0.0f, 0.0f, 1.0f) },
			{ Vect3f(1.0f, 0.0f, 0.0f), CColor(1.0f, 0.0f, 0.0f, 1.0f) },

			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(0.0f, 1.0f, 0.0f, 1.0f) },
			{ Vect3f(0.0f, 1.0f, 0.0f), CColor(0.0f, 1.0f, 0.0f, 1.0f) },

			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(0.0f, 0.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, 0.0f, 1.0f), CColor(0.0f, 0.0f, 1.0f, 1.0f) }
		};

		m_Axis = new CLinesListRenderableVertexs<MV_POSITION_COLOR_VERTEX>(_Device, l_AxisVtxs, 6, 3);
		m_AxisBSRadi = 1;
		m_AxisBBMin = Vect3f(0, 0, 0);
		m_AxisBBMax = Vect3f(1, 1, 1);
	}
	{
		MV_POSITION_COLOR_VERTEX l_LongAxisVtxs[12] =
		{
			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(1.0f, 0.0f, 0.0f), CColor(1.0f, 0.0f, 0.0f, 1.0f) },

			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, 1.0f, 0.0f), CColor(0.0f, 1.0f, 0.0f, 1.0f) },

			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, 0.0f, 1.0f), CColor(0.0f, 0.0f, 1.0f, 1.0f) },

			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-1.0f, 0.0f, 0.0f), CColor(0.0f, 1.0f, 1.0f, 1.0f) },

			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, -1.0f, 0.0f), CColor(1.0f, 0.0f, 1.0f, 1.0f) },

			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, 0.0f, -1.0f), CColor(1.0f, 1.0f, 0.0f, 1.0f) }
		};

		m_LongAxis = new CLinesListRenderableVertexs<MV_POSITION_COLOR_VERTEX>(_Device, l_LongAxisVtxs, 12, 6);
		m_LongAxisBSRadi = 1;
		m_LongAxisBBMin = Vect3f(-1, -1, -1);
		m_LongAxisBBMax = Vect3f(1, 1, 1);
	}
	{
		//SIMPLE TRIANGLE
		MV_POSITION_COLOR_VERTEX l_SimpleTriangleVtxs[3] =
		{
			{ Vect3f(-0.5f, -0.5f, 0.5f), CColor(1.0f, 0.0f, 0.0f, 1.0f) },
			{ Vect3f(+0.0f, +0.5f, 0.5f), CColor(0.0f, 1.0f, 0.0f, 1.0f) },
			{ Vect3f(+0.5f, -0.5f, 0.5f), CColor(0.0f, 0.0f, 1.0f, 1.0f) },
		};

		m_SimpleTriangle = new CTrianglesListRenderableVertexs<MV_POSITION_COLOR_VERTEX>(_Device, l_SimpleTriangleVtxs, 3, 1);
		m_SimpleTriangleBSRadi = 0.86603f;
		m_SimpleTriangleBBMin = Vect3f(-0.5f, -0.5f, 0.5f);
		m_SimpleTriangleBBMax = Vect3f(0.5f, 0.5f, 0.5f);
	}
	{
		//SIMPLE TRIANGLE
		MV_POSITION_COLOR_VERTEX l_SimpleTriangleVtxs[3] =
		{
			{ Vect3f(-0.5f, -0.5f, 0.5f), CColor(1.0f, 0.0f, 0.0f, 0.5f) },
			{ Vect3f(+0.0f, +0.5f, 0.5f), CColor(0.0f, 1.0f, 0.0f, 0.5f) },
			{ Vect3f(+0.5f, -0.5f, 0.5f), CColor(0.0f, 0.0f, 1.0f, 0.5f) },
		};

		m_ClassicBlendTriangle = new CTrianglesListRenderableVertexs<MV_POSITION_COLOR_VERTEX>(_Device, l_SimpleTriangleVtxs, 3, 1);
		m_ClassicBlendTriangleBSRadi = 0.86603f;
		m_ClassicBlendTriangleBBMin = Vect3f(-0.5f, -0.5f, 0.5f);
		m_ClassicBlendTriangleBBMax = Vect3f(0.5f, 0.5f, 0.5f);
	}
	{
		//SIMPLE TRIANGLE
		MV_POSITION_COLOR_VERTEX l_SimpleTriangleVtxs[3] =
		{
			{ Vect3f(-0.5f, -0.5f, 0.5f), CColor(0.5f, 0.0f, 0.0f, 0.5f) },
			{ Vect3f(+0.0f, +0.5f, 0.5f), CColor(0.0f, 0.5f, 0.0f, 0.5f) },
			{ Vect3f(+0.5f, -0.5f, 0.5f), CColor(0.0f, 0.0f, 0.5f, 0.5f) },
		};

		m_PremultBlendTriangle = new CTrianglesListRenderableVertexs<MV_POSITION_COLOR_VERTEX>(_Device, l_SimpleTriangleVtxs, 3, 1);
		m_PremultBlendTriangleBSRadi = 0.86603f;
		m_PremultBlendTriangleBBMin = Vect3f(-0.5f, -0.5f, 0.5f);
		m_PremultBlendTriangleBBMax = Vect3f(0.5f, 0.5f, 0.5f);
	}
	{
		// Simple Grid
		MV_POSITION_COLOR_VERTEX l_SimpleGridVtxs[] =
		{
			{ Vect3f(+1.0f, 0.0f, +1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },   // 0
			{ Vect3f(+1.0f, 0.0f, -1.0f), CColor(1.0f, 1.0f, 0.0f, 1.0f) },  // 1
			{ Vect3f(-1.0f, 0.0f, -1.0f), CColor(1.0f, 0.0f, 0.0f, 1.0f) }, // 2
			{ Vect3f(-1.0f, 0.0f, +1.0f), CColor(1.0f, 0.0f, 1.0f, 1.0f) },  // 3
			{ Vect3f(+1.0f, 0.0f, +0.0f), CColor(1.0f, 1.0f, 0.5f, 1.0f) },   // 4
			{ Vect3f(-1.0f, 0.0f, +0.0f), CColor(1.0f, 0.0f, 0.5f, 1.0f) },  // 5
			{ Vect3f(+0.0f, 0.0f, +1.0f), CColor(1.0f, 0.5f, 1.0f, 1.0f) },   // 6
			{ Vect3f(+0.0f, 0.0f, -1.0f), CColor(1.0f, 0.5f, 0.0f, 1.0f) },  // 7
		};

		uint16_t l_SimpleGridIdxs[] = { 0, 1, 0, 3, 2, 1, 2, 3, 4, 5, 6, 7 };

		m_SimpleGrid = new CLinesListRenderableIndexed16Vertexs<MV_POSITION_COLOR_VERTEX>(_Device, l_SimpleGridVtxs, 8, l_SimpleGridIdxs, 12);
		m_SimpleGridBSRadi = 1.414236f;
		m_SimpleGridBBMin = Vect3f(-1.f, 0.f, -1.f);
		m_SimpleGridBBMax = Vect3f(1.f, 0.f, 1.f);
	}
	{
		// Simple Cube
		MV_POSITION_COLOR_VERTEX l_SimpleCubeVtxs[] =
		{
			{ Vect3f(+0.5f, 0.0f, +0.5f), CColor(0.0f, 1.0f, 1.0f, 1.0f) },   // 0
			{ Vect3f(+0.5f, 0.0f, -0.5f), CColor(0.0f, 1.0f, 0.0f, 1.0f) },  // 1
			{ Vect3f(-0.5f, 0.0f, -0.5f), CColor(0.0f, 0.0f, 0.0f, 1.0f) }, // 2
			{ Vect3f(-0.5f, 0.0f, +0.5f), CColor(0.0f, 0.0f, 1.0f, 1.0f) },  // 3
			{ Vect3f(+0.5f, 1.0f, +0.5f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },   // 4
			{ Vect3f(+0.5f, 1.0f, -0.5f), CColor(1.0f, 1.0f, 0.0f, 1.0f) },  // 5
			{ Vect3f(-0.5f, 1.0f, -0.5f), CColor(1.0f, 0.0f, 0.0f, 1.0f) },   // 6
			{ Vect3f(-0.5f, 1.0f, +0.5f), CColor(1.0f, 0.0f, 1.0f, 1.0f) },  // 7
		};

		/*
		uint16_t l_SimpleCubeIdxs[] = {
		0, 1, 0, 3, 2, 1, 2, 3,
		4, 5, 4, 7, 6, 5, 6, 7,
		0, 4, 1, 5, 2, 6, 3, 7 };
		*/
		uint16_t l_SimpleCubeIdxs[] = {
			0, 2, 1, 3, 2, 0, // bot
			0, 4, 3, 3, 4, 7, // front
			0, 1, 4, 4, 1, 5,
			3, 7, 2, 2, 7, 6,
			1, 2, 5, 2, 6, 5,
			4, 5, 6, 6, 7, 4,
		};

		//m_SimpleCube = new CLinesListRenderableIndexed16Vertexs<MV_POSITION_COLOR_VERTEX>(_Device, l_SimpleCubeVtxs, 8, l_SimpleCubeIdxs, 24);
		m_SimpleCube = new CTriangleListRenderableIndexed16Vertexs<MV_POSITION_COLOR_VERTEX>(_Device, l_SimpleCubeVtxs, 8, l_SimpleCubeIdxs, 36);
		m_SimpleCubeBSRadi = 1.224745f;
		m_SimpleCubeBBMin = Vect3f(-.5f, 0.f, -.5f);
		m_SimpleCubeBBMax = Vect3f(.5f, 1.f, .5f);
	}
}
