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

		m_AxisRenderableVertexs = new CLinesListRenderableVertexs<MV_POSITION_COLOR_VERTEX>(_Device, l_AxisVtxs, 6, 3);
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

		m_LongAxisRenderableVertexs = new CLinesListRenderableVertexs<MV_POSITION_COLOR_VERTEX>(_Device, l_LongAxisVtxs, 12, 6);
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
	}
}
