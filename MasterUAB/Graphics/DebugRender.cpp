#include "DebugRender.h"

#include "RenderableObjects\VertexTypes.h"
#include "RenderableObjects\TemplatedRenderableVertexs.h"
#include "RenderableObjects\TemplatedRenderableIndexedVertexs.h"
#include "Effects\Effect.h"
#include "Engine\UABEngine.h"

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

		m_Axis = new CUABLinesListRenderableVertexs<MV_POSITION_COLOR_VERTEX>(l_AxisVtxs, 6, 3);
		m_AxisBSRadi = 1;
		m_AxisBBMin = Vect3f(0, 0, 0);
		m_AxisBBMax = Vect3f(1, 1, 1);
	}

	//LongAxis
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

	m_LongAxis = new CUABLinesListRenderableVertexs<MV_POSITION_COLOR_VERTEX>(l_LongAxisVtxs, 12, 6);
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

		m_SimpleTriangle = new CUABTrianglesListRenderableVertexs<MV_POSITION_COLOR_VERTEX>(l_SimpleTriangleVtxs, 3, 1);
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

		m_ClassicBlendTriangle = new CUABTrianglesListRenderableVertexs<MV_POSITION_COLOR_VERTEX>(l_SimpleTriangleVtxs, 3, 1);
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

		m_PremultBlendTriangle = new CUABTrianglesListRenderableVertexs<MV_POSITION_COLOR_VERTEX>(l_SimpleTriangleVtxs, 3, 1);
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

		m_SimpleGrid = new CUABLineListRenderableIndexed16Vertexs<MV_POSITION_COLOR_VERTEX>(l_SimpleGridVtxs, 8, l_SimpleGridIdxs, 12);
		m_SimpleGridBSRadi = 1.414236f;
		m_SimpleGridBBMin = Vect3f(-1.f, 0.f, -1.f);
		m_SimpleGridBBMax = Vect3f(1.f, 0.f, 1.f);
	}
	{
		// Simple Grid
		MV_POSITION_COLOR_VERTEX l_BigGridVtxs[] =
		{
			{ Vect3f(5.0f, 0.0f, 5.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//0
			{ Vect3f(5.0f, 0.0f, 4.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//1
			{ Vect3f(5.0f, 0.0f, 3.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//2
			{ Vect3f(5.0f, 0.0f, 2.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//3
			{ Vect3f(5.0f, 0.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//4
			{ Vect3f(5.0f, 0.0f, 0.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//5
			{ Vect3f(5.0f, 0.0f, -1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//6
			{ Vect3f(5.0f, 0.0f, -2.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//7
			{ Vect3f(5.0f, 0.0f, -3.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//8
			{ Vect3f(5.0f, 0.0f, -4.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//9
			{ Vect3f(5.0f, 0.0f, -5.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//10

			{ Vect3f(-5.0f, 0.0f, 5.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//11
			{ Vect3f(-5.0f, 0.0f, 4.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//12
			{ Vect3f(-5.0f, 0.0f, 3.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//13
			{ Vect3f(-5.0f, 0.0f, 2.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//14
			{ Vect3f(-5.0f, 0.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//15
			{ Vect3f(-5.0f, 0.0f, 0.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//16
			{ Vect3f(-5.0f, 0.0f, -1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//17
			{ Vect3f(-5.0f, 0.0f, -2.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//18
			{ Vect3f(-5.0f, 0.0f, -3.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//19
			{ Vect3f(-5.0f, 0.0f, -4.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//20
			{ Vect3f(-5.0f, 0.0f, -5.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//21

			{ Vect3f(4.0f, 0.0f, 5.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//22
			{ Vect3f(3.0f, 0.0f, 5.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//23
			{ Vect3f(2.0f, 0.0f, 5.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//24
			{ Vect3f(1.0f, 0.0f, 5.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//25
			{ Vect3f(0.0f, 0.0f, 5.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//26
			{ Vect3f(-1.0f, 0.0f, 5.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//27
			{ Vect3f(-2.0f, 0.0f, 5.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//28
			{ Vect3f(-3.0f, 0.0f, 5.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//29
			{ Vect3f(-4.0f, 0.0f, 5.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//30

			{ Vect3f(4.0f, 0.0f, -5.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//31
			{ Vect3f(3.0f, 0.0f, -5.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//32
			{ Vect3f(2.0f, 0.0f, -5.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//33
			{ Vect3f(1.0f, 0.0f, -5.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//34
			{ Vect3f(0.0f, 0.0f, -5.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//35
			{ Vect3f(-1.0f, 0.0f, -5.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//36
			{ Vect3f(-2.0f, 0.0f, -5.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//37
			{ Vect3f(-3.0f, 0.0f, -5.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//38
			{ Vect3f(-4.0f, 0.0f, -5.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },	//39
		};

		uint16_t l_BigGridIdxs[] = { 0, 11, 1, 12, 2, 13, 3, 14, 4, 15, 5, 16, 6, 17, 7, 18, 8, 19, 9, 20, 10, 21,
			0 , 10, 22, 31, 23, 32, 24, 33, 25, 34, 26, 35, 27, 36, 28, 37, 29, 38, 30, 39, 11, 21 };

		m_BigGrid = new CUABLineListRenderableIndexed16Vertexs<MV_POSITION_COLOR_VERTEX>(l_BigGridVtxs, 40, l_BigGridIdxs, 44);
	}
	{
		// Quad 2D

		MV_POSITION4_COLOR_TEXTURE_VERTEX l_ScreenVertexsQuad[4] =
		{
			{ Vect4f(-1.0f, 1.0f, 0.5f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f),
			Vect2f(0.0f, 0.0f) },
			{ Vect4f(-1.0f, -1.0f, 0.5f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f),
			Vect2f(0.0f, 1.0f) },
			{ Vect4f(1.0f, 1.0f, 0.5f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f),
			Vect2f(1.0f, 0.0f) },
			{ Vect4f(1.0f, -1.0f, 0.5f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f),
			Vect2f(1.0f, 1.0f) }
		};
		m_DrawQuadRV = new CUABTrianglesStripRenderableVertexs<MV_POSITION4_COLOR_TEXTURE_VERTEX>(l_ScreenVertexsQuad, 4, 2);
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
		m_SimpleCube = new CUABTriangleListRenderableIndexed16Vertexs<MV_POSITION_COLOR_VERTEX>( l_SimpleCubeVtxs, 8, l_SimpleCubeIdxs,36);
		m_SimpleCubeBSRadi = 1.224745f;
		m_SimpleCubeBBMin = Vect3f(-0.5f, 0.f, -0.5f);
		m_SimpleCubeBBMax = Vect3f(.5f, 1.f, .5f);
	}
	
	{

		MV_POSITION_COLOR_VERTEX l_Sphere_10SegVtxs[42] =
		{
			{ Vect3f(0.0f, 0.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, 0.587785f, 0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.345491f, 0.475528f, 0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.559017f, 0.181636f, 0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.559017f, -0.181636f, 0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.345492f, -0.475528f, 0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, -0.587785f, 0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.345492f, -0.475528f, 0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.559017f, -0.181635f, 0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.559017f, 0.181636f, 0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.345491f, 0.475529f, 0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, 0.951057f, 0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.559017f, 0.769421f, 0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.904508f, 0.293893f, 0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.904509f, -0.293893f, 0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.559017f, -0.769421f, 0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, -0.951057f, 0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.559017f, -0.769421f, 0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.904509f, -0.293892f, 0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.904508f, 0.293893f, 0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.559016f, 0.769421f, 0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, 0.951056f, -0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.559017f, 0.769421f, -0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.904508f, 0.293893f, -0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.904508f, -0.293893f, -0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.559017f, -0.769421f, -0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, -0.951056f, -0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.559017f, -0.769421f, -0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.904509f, -0.293892f, -0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.904508f, 0.293893f, -0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.559016f, 0.769421f, -0.309017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, 0.587785f, -0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.345491f, 0.475528f, -0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.559017f, 0.181636f, -0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.559017f, -0.181636f, -0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.345492f, -0.475528f, -0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, -0.587785f, -0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.345492f, -0.475528f, -0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.559017f, -0.181635f, -0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.559017f, 0.181636f, -0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.345491f, 0.475529f, -0.809017f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, 0.0f, -1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) }
		};

		uint16_t l_Sphere_10SegIdxs[] = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 5, 6, 0, 6, 7, 0, 7, 8, 0, 8, 9, 0, 9, 10, 0, 10, 1, 1, 11, 12, 1, 12, 2, 2, 12, 13, 2, 13, 3, 3, 13, 14, 3, 14, 4, 4, 14, 15, 4, 15, 5, 5, 15, 16, 5, 16, 6, 6, 16, 17, 6, 17, 7, 7, 17, 18, 7, 18, 8, 8, 18, 19, 8, 19, 9, 9, 19, 20, 9, 20, 10, 10, 20, 11, 10, 11, 1, 11, 21, 22, 11, 22, 12, 12, 22, 23, 12, 23, 13, 13, 23, 24, 13, 24, 14, 14, 24, 25, 14, 25, 15, 15, 25, 26, 15, 26, 16, 16, 26, 27, 16, 27, 17, 17, 27, 28, 17, 28, 18, 18, 28, 29, 18, 29, 19, 19, 29, 30, 19, 30, 20, 20, 30, 21, 20, 21, 11, 21, 31, 32, 21, 32, 22, 22, 32, 33, 22, 33, 23, 23, 33, 34, 23, 34, 24, 24, 34, 35, 24, 35, 25, 25, 35, 36, 25, 36, 26, 26, 36, 37, 26, 37, 27, 27, 37, 38, 27, 38, 28, 28, 38, 39, 28, 39, 29, 29, 39, 40, 29, 40, 30, 30, 40, 31, 30, 31, 21, 41, 32, 31, 41, 33, 32, 41, 34, 33, 41, 35, 34, 41, 36, 35, 41, 37, 36, 41, 38, 37, 41, 39, 38, 41, 40, 39, 41, 31, 40 };

		m_Sphere_10Seg = new CUABTriangleListRenderableIndexed16Vertexs<MV_POSITION_COLOR_VERTEX>(l_Sphere_10SegVtxs, 42, l_Sphere_10SegIdxs, 240);
		m_Sphere_10SegBSRadi = 1.0f;
		m_Sphere_10SegBSCenter = Vect3f(0.0f, 1.19209e-007f, 0.0f);
		m_Sphere_10SegBBMin = Vect3f(1e+030f, 1e+030f, 1e+030f);
		m_Sphere_10SegBBMax = Vect3f(-1e+030f, -1e+030f, -1e+030f);


	}

	{

		MV_POSITION_COLOR_VERTEX l_ConeVtxs[122] =
		{
			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.965926f, 0.0f, 0.258819f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-1.0f, 0.0f, 0.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.866025f, 0.0f, 0.5f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.707107f, 0.0f, 0.707107f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.5f, 0.0f, 0.866025f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.258819f, 0.0f, 0.965926f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, 0.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.258819f, 0.0f, 0.965926f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.5f, 0.0f, 0.866026f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.707107f, 0.0f, 0.707107f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.866025f, 0.0f, 0.5f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.965926f, 0.0f, 0.258819f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(1.0f, 0.0f, 3.89414e-007f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.965926f, 0.0f, -0.258819f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.866026f, 0.0f, -0.5f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.707107f, 0.0f, -0.707106f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.5f, 0.0f, -0.866025f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.258819f, 0.0f, -0.965926f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(4.64912e-007f, 0.0f, -1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.258819f, 0.0f, -0.965926f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.499999f, 0.0f, -0.866026f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.707106f, 0.0f, -0.707107f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.866025f, 0.0f, -0.500001f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.965926f, 0.0f, -0.25882f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.772741f, 0.2f, 0.207055f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.8f, 0.2f, 0.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.69282f, 0.2f, 0.4f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.565685f, 0.2f, 0.565685f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.4f, 0.2f, 0.69282f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.207055f, 0.2f, 0.772741f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, 0.2f, 0.8f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.207055f, 0.2f, 0.772741f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.4f, 0.2f, 0.69282f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.565685f, 0.2f, 0.565686f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.69282f, 0.2f, 0.4f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.772741f, 0.2f, 0.207055f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.8f, 0.2f, 3.11531e-007f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.772741f, 0.2f, -0.207055f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.69282f, 0.2f, -0.4f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.565686f, 0.2f, -0.565685f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.4f, 0.2f, -0.69282f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.207056f, 0.2f, -0.772741f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(3.7193e-007f, 0.2f, -0.8f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.207055f, 0.2f, -0.772741f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.4f, 0.2f, -0.692821f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.565685f, 0.2f, -0.565686f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.69282f, 0.2f, -0.4f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.772741f, 0.2f, -0.207056f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.579556f, 0.4f, 0.155291f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.6f, 0.4f, 0.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.519615f, 0.4f, 0.3f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.424264f, 0.4f, 0.424264f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.3f, 0.4f, 0.519615f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.155291f, 0.4f, 0.579556f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, 0.4f, 0.6f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.155291f, 0.4f, 0.579556f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.3f, 0.4f, 0.519615f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.424264f, 0.4f, 0.424264f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.519615f, 0.4f, 0.3f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.579555f, 0.4f, 0.155292f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.6f, 0.4f, 2.33649e-007f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.579556f, 0.4f, -0.155291f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.519615f, 0.4f, -0.3f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.424264f, 0.4f, -0.424264f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.3f, 0.4f, -0.519615f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.155292f, 0.4f, -0.579555f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(2.78947e-007f, 0.4f, -0.6f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.155291f, 0.4f, -0.579556f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.3f, 0.4f, -0.519615f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.424264f, 0.4f, -0.424264f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.519615f, 0.4f, -0.3f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.579555f, 0.4f, -0.155292f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.38637f, 0.6f, 0.103528f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.4f, 0.6f, 0.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.34641f, 0.6f, 0.2f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.282843f, 0.6f, 0.282843f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.2f, 0.6f, 0.34641f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.103528f, 0.6f, 0.38637f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, 0.6f, 0.4f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.103528f, 0.6f, 0.38637f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.2f, 0.6f, 0.34641f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.282843f, 0.6f, 0.282843f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.34641f, 0.6f, 0.2f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.38637f, 0.6f, 0.103528f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.4f, 0.6f, 1.55766e-007f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.38637f, 0.6f, -0.103527f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.34641f, 0.6f, -0.2f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.282843f, 0.6f, -0.282843f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.2f, 0.6f, -0.34641f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.103528f, 0.6f, -0.38637f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(1.85965e-007f, 0.6f, -0.4f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.103527f, 0.6f, -0.38637f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.2f, 0.6f, -0.34641f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.282843f, 0.6f, -0.282843f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.34641f, 0.6f, -0.2f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.38637f, 0.6f, -0.103528f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.193185f, 0.8f, 0.0517638f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.2f, 0.8f, 0.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.173205f, 0.8f, 0.1f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.141421f, 0.8f, 0.141421f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.1f, 0.8f, 0.173205f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.0517638f, 0.8f, 0.193185f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, 0.8f, 0.2f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0517638f, 0.8f, 0.193185f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.1f, 0.8f, 0.173205f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.141421f, 0.8f, 0.141421f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.173205f, 0.8f, 0.1f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.193185f, 0.8f, 0.0517639f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.2f, 0.8f, 0.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.193185f, 0.8f, -0.0517637f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.173205f, 0.8f, -0.0999999f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.141421f, 0.8f, -0.141421f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.1f, 0.8f, -0.173205f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0517639f, 0.8f, -0.193185f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, 0.8f, -0.2f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.0517637f, 0.8f, -0.193185f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.0999999f, 0.8f, -0.173205f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.141421f, 0.8f, -0.141421f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.173205f, 0.8f, -0.1f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-0.193185f, 0.8f, -0.0517639f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, 1.0f, 0.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) }
		};

		uint16_t l_ConeIdxs[] = { 0, 1, 2, 0, 3, 1, 0, 4, 3, 0, 5, 4, 0, 6, 5, 0, 7, 6, 0, 8, 7, 0, 9, 8, 0, 10, 9, 0, 11, 10, 0, 12, 11, 0, 13, 12, 0, 14, 13, 0, 15, 14, 0, 16, 15, 0, 17, 16, 0, 18, 17, 0, 19, 18, 0, 20, 19, 0, 21, 20, 0, 22, 21, 0, 23, 22, 0, 24, 23, 0, 2, 24, 2, 25, 26, 2, 1, 25, 1, 27, 25, 1, 3, 27, 3, 28, 27, 3, 4, 28, 4, 29, 28, 4, 5, 29, 5, 30, 29, 5, 6, 30, 6, 31, 30, 6, 7, 31, 7, 32, 31, 7, 8, 32, 8, 33, 32, 8, 9, 33, 9, 34, 33, 9, 10, 34, 10, 35, 34, 10, 11, 35, 11, 36, 35, 11, 12, 36, 12, 37, 36, 12, 13, 37, 13, 38, 37, 13, 14, 38, 14, 39, 38, 14, 15, 39, 15, 40, 39, 15, 16, 40, 16, 41, 40, 16, 17, 41, 17, 42, 41, 17, 18, 42, 18, 43, 42, 18, 19, 43, 19, 44, 43, 19, 20, 44, 20, 45, 44, 20, 21, 45, 21, 46, 45, 21, 22, 46, 22, 47, 46, 22, 23, 47, 23, 48, 47, 23, 24, 48, 24, 26, 48, 24, 2, 26, 26, 49, 50, 26, 25, 49, 25, 51, 49, 25, 27, 51, 27, 52, 51, 27, 28, 52, 28, 53, 52, 28, 29, 53, 29, 54, 53, 29, 30, 54, 30, 55, 54, 30, 31, 55, 31, 56, 55, 31, 32, 56, 32, 57, 56, 32, 33, 57, 33, 58, 57, 33, 34, 58, 34, 59, 58, 34, 35, 59, 35, 60, 59, 35, 36, 60, 36, 61, 60, 36, 37, 61, 37, 62, 61, 37, 38, 62, 38, 63, 62, 38, 39, 63, 39, 64, 63, 39, 40, 64, 40, 65, 64, 40, 41, 65, 41, 66, 65, 41, 42, 66, 42, 67, 66, 42, 43, 67, 43, 68, 67, 43, 44, 68, 44, 69, 68, 44, 45, 69, 45, 70, 69, 45, 46, 70, 46, 71, 70, 46, 47, 71, 47, 72, 71, 47, 48, 72, 48, 50, 72, 48, 26, 50, 50, 73, 74, 50, 49, 73, 49, 75, 73, 49, 51, 75, 51, 76, 75, 51, 52, 76, 52, 77, 76, 52, 53, 77, 53, 78, 77, 53, 54, 78, 54, 79, 78, 54, 55, 79, 55, 80, 79, 55, 56, 80, 56, 81, 80, 56, 57, 81, 57, 82, 81, 57, 58, 82, 58, 83, 82, 58, 59, 83, 59, 84, 83, 59, 60, 84, 60, 85, 84, 60, 61, 85, 61, 86, 85, 61, 62, 86, 62, 87, 86, 62, 63, 87, 63, 88, 87, 63, 64, 88, 64, 89, 88, 64, 65, 89, 65, 90, 89, 65, 66, 90, 66, 91, 90, 66, 67, 91, 67, 92, 91, 67, 68, 92, 68, 93, 92, 68, 69, 93, 69, 94, 93, 69, 70, 94, 70, 95, 94, 70, 71, 95, 71, 96, 95, 71, 72, 96, 72, 74, 96, 72, 50, 74, 74, 97, 98, 74, 73, 97, 73, 99, 97, 73, 75, 99, 75, 100, 99, 75, 76, 100, 76, 101, 100, 76, 77, 101, 77, 102, 101, 77, 78, 102, 78, 103, 102, 78, 79, 103, 79, 104, 103, 79, 80, 104, 80, 105, 104, 80, 81, 105, 81, 106, 105, 81, 82, 106, 82, 107, 106, 82, 83, 107, 83, 108, 107, 83, 84, 108, 84, 109, 108, 84, 85, 109, 85, 110, 109, 85, 86, 110, 86, 111, 110, 86, 87, 111, 87, 112, 111, 87, 88, 112, 88, 113, 112, 88, 89, 113, 89, 114, 113, 89, 90, 114, 90, 115, 114, 90, 91, 115, 91, 116, 115, 91, 92, 116, 92, 117, 116, 92, 93, 117, 93, 118, 117, 93, 94, 118, 94, 119, 118, 94, 95, 119, 95, 120, 119, 95, 96, 120, 96, 98, 120, 96, 74, 98, 98, 121, 121, 98, 97, 121, 97, 121, 121, 97, 99, 121, 99, 121, 121, 99, 100, 121, 100, 121, 121, 100, 101, 121, 101, 121, 121, 101, 102, 121, 102, 121, 121, 102, 103, 121, 103, 121, 121, 103, 104, 121, 104, 121, 121, 104, 105, 121, 105, 121, 121, 105, 106, 121, 106, 121, 121, 106, 107, 121, 107, 121, 121, 107, 108, 121, 108, 121, 121, 108, 109, 121, 109, 121, 121, 109, 110, 121, 110, 121, 121, 110, 111, 121, 111, 121, 121, 111, 112, 121, 112, 121, 121, 112, 113, 121, 113, 121, 121, 113, 114, 121, 114, 121, 121, 114, 115, 121, 115, 121, 121, 115, 116, 121, 116, 121, 121, 116, 117, 121, 117, 121, 121, 117, 118, 121, 118, 121, 121, 118, 119, 121, 119, 121, 121, 119, 120, 121, 120, 121, 121, 120, 98, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121 };

		m_Cone = new CUABTriangleListRenderableIndexed16Vertexs<MV_POSITION_COLOR_VERTEX>(l_ConeVtxs, 122, l_ConeIdxs, 864);
		m_ConeBSRadi = 1.0f;
		m_ConeBSCenter = Vect3f(-1.19209e-007f, 3.27826e-007f, 0.0f);
		m_ConeBBMin = Vect3f(1e+030f, 1e+030f, 1e+030f);
		m_ConeBBMax = Vect3f(-1e+030f, -1e+030f, -1e+030f);


	}

	m_EffectTechnique = UABEngine.GetEffectManager()->GetResource("debug_render_technique");

}

CRenderableVertexs * CDebugRender::GetLine(Vect3f inici, Vect3f final) const
{
	MV_POSITION_COLOR_VERTEX l_Line[2] =
	{
		{ inici, CColor(1.f, 1.f, 1.f, 1.f) },
		{ final, CColor(1.f, 1.f, 1.f, 1.f) }
	};
	return new CUABLinesListRenderableVertexs<MV_POSITION_COLOR_VERTEX>(l_Line,2,1);
}
