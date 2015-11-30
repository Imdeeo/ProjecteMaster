#ifndef VERTEX_TYPES_H
#define VERTEX_TYPES_H

#include <string>
#include "Math\Vector2.h"
#include "Math\Vector3.h"
#include "Math\Vector4.h"
#include "Math\Color.h"

#include "RenderManager.h"

#include <d3d11.h>
#include <d3dCommon.h>


#define MV_VERTEX_TYPE_POSITION				0x01
#define MV_VERTEX_TYPE_COLOR				0x02
#define MV_VERTEX_TYPE_NORMAL				0x04
#define MV_VERTEX_TYPE_BLEND_INDICES		0x08
#define MV_VERTEX_TYPE_BLEND_WEIGHT			0x10
#define MV_VERTEX_TYPE_TEXTURE1				0x20
#define MV_VERTEX_TYPE_TEXTURE2				0x40
#define MV_VERTEX_TYPE_POSITION4			0x80



#define CREATE_INPUT_LAYOUT(LayoutVariable, IdLayoutVariable, OffsetBytesVariable, NumBytes) \
	LayoutVariable[IdLayoutVariable].AlignedByteOffset=OffsetBytesVariable; \
	OffsetBytesVariable+=NumBytes; \
	++IdLayoutVariable;

//Position
#define IFDEF_CREATE_MV_0_POSITION_VERTEX
#define IFDEF_CREATE_MV_1_POSITION_VERTEX Vect3f Position;
#define IFDEF_CREATE_GET_VERTEX_TYPE_0_POSITION 0
#define IFDEF_CREATE_GET_VERTEX_TYPE_1_POSITION MV_VERTEX_TYPE_POSITION
#define IFDEF_CREATE_GET_VERTEX_TYPE_0_POSITION_CREATE_LAYOUT
#define IFDEF_CREATE_GET_VERTEX_TYPE_1_POSITION_CREATE_LAYOUT { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
#define IFDEF_CREATE_GET_VERTEX_TYPE_0_POSITION_CREATE_LAYOUT_DETAIL
#define IFDEF_CREATE_GET_VERTEX_TYPE_1_POSITION_CREATE_LAYOUT_DETAIL(LayoutVariable, IdLayoutVariable, OffsetBytesVariable) \
	CREATE_INPUT_LAYOUT(LayoutVariable, IdLayoutVariable, OffsetBytesVariable, 12);

//Position 4
#define IFDEF_CREATE_MV_0_POSITION4_VERTEX
#define IFDEF_CREATE_MV_1_POSITION4_VERTEX Vect4f Position;
#define IFDEF_CREATE_GET_VERTEX_TYPE_0_POSITION4 0
#define IFDEF_CREATE_GET_VERTEX_TYPE_1_POSITION4 MV_VERTEX_TYPE_POSITION4
#define IFDEF_CREATE_GET_VERTEX_TYPE_0_POSITION4_CREATE_LAYOUT
#define IFDEF_CREATE_GET_VERTEX_TYPE_1_POSITION4_CREATE_LAYOUT { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
#define IFDEF_CREATE_GET_VERTEX_TYPE_0_POSITION4_CREATE_LAYOUT_DETAIL
#define IFDEF_CREATE_GET_VERTEX_TYPE_1_POSITION4_CREATE_LAYOUT_DETAIL(LayoutVariable, IdLayoutVariable, OffsetBytesVariable) \
	CREATE_INPUT_LAYOUT(LayoutVariable, IdLayoutVariable, OffsetBytesVariable, 16);

//Normal
#define IFDEF_CREATE_MV_0_NORMAL_VERTEX
#define IFDEF_CREATE_MV_1_NORMAL_VERTEX Vect3f Normal;
#define IFDEF_CREATE_GET_VERTEX_TYPE_0_NORMAL 0
#define IFDEF_CREATE_GET_VERTEX_TYPE_1_NORMAL MV_VERTEX_TYPE_NORMAL
#define IFDEF_CREATE_GET_VERTEX_TYPE_0_NORMAL_CREATE_LAYOUT
#define IFDEF_CREATE_GET_VERTEX_TYPE_1_NORMAL_CREATE_LAYOUT { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
#define IFDEF_CREATE_GET_VERTEX_TYPE_0_NORMAL_CREATE_LAYOUT_DETAIL
#define IFDEF_CREATE_GET_VERTEX_TYPE_1_NORMAL_CREATE_LAYOUT_DETAIL(LayoutVariable, IdLayoutVariable, OffsetBytesVariable) \
	CREATE_INPUT_LAYOUT(LayoutVariable, IdLayoutVariable, OffsetBytesVariable, 12);

//Weight and indices
#define IFDEF_CREATE_MV_0_WEIGHT_INDICES_VERTEX
#define IFDEF_CREATE_MV_1_WEIGHT_INDICES_VERTEX \
	Vect4f		Weights; \
	Vect4f 		Indices;
#define IFDEF_CREATE_GET_VERTEX_TYPE_0_WEIGHT_INDICES 0
#define IFDEF_CREATE_GET_VERTEX_TYPE_1_WEIGHT_INDICES MV_VERTEX_TYPE_BLEND_WEIGHT | MV_VERTEX_TYPE_BLEND_INDICES
#define IFDEF_CREATE_GET_VERTEX_TYPE_0_WEIGHT_INDICES_CREATE_LAYOUT
#define IFDEF_CREATE_GET_VERTEX_TYPE_1_WEIGHT_INDICES_CREATE_LAYOUT \
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }, \
		{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
#define IFDEF_CREATE_GET_VERTEX_TYPE_0_WEIGHT_INDICES_CREATE_LAYOUT_DETAIL
#define IFDEF_CREATE_GET_VERTEX_TYPE_1_WEIGHT_INDICES_CREATE_LAYOUT_DETAIL(LayoutVariable, IdLayoutVariable, OffsetBytesVariable) \
	CREATE_INPUT_LAYOUT(LayoutVariable, IdLayoutVariable, OffsetBytesVariable, 16); \
	CREATE_INPUT_LAYOUT(LayoutVariable, IdLayoutVariable, OffsetBytesVariable, 16);

//Color
#define IFDEF_CREATE_MV_0_COLOR_VERTEX
#define IFDEF_CREATE_MV_1_COLOR_VERTEX CColor Color;
#define IFDEF_CREATE_GET_VERTEX_TYPE_0_COLOR 0
#define IFDEF_CREATE_GET_VERTEX_TYPE_1_COLOR MV_VERTEX_TYPE_COLOR
#define IFDEF_CREATE_GET_VERTEX_TYPE_0_COLOR_CREATE_LAYOUT
#define IFDEF_CREATE_GET_VERTEX_TYPE_1_COLOR_CREATE_LAYOUT { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
#define IFDEF_CREATE_GET_VERTEX_TYPE_0_COLOR_CREATE_LAYOUT_DETAIL
#define IFDEF_CREATE_GET_VERTEX_TYPE_1_COLOR_CREATE_LAYOUT_DETAIL(LayoutVariable, IdLayoutVariable, OffsetBytesVariable) \
	CREATE_INPUT_LAYOUT(LayoutVariable, IdLayoutVariable, OffsetBytesVariable, 16);

//UV
#define IFDEF_CREATE_MV_0_UV_VERTEX

#define IFDEF_CREATE_MV_1_UV_VERTEX Vect2f		UV;
#define IFDEF_CREATE_GET_VERTEX_TYPE_0_UV 0
#define IFDEF_CREATE_GET_VERTEX_TYPE_1_UV MV_VERTEX_TYPE_TEXTURE1
#define IFDEF_CREATE_GET_VERTEX_TYPE_0_UV_CREATE_LAYOUT
#define IFDEF_CREATE_GET_VERTEX_TYPE_1_UV_CREATE_LAYOUT { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
#define IFDEF_CREATE_GET_VERTEX_TYPE_0_UV_CREATE_LAYOUT_DETAIL
#define IFDEF_CREATE_GET_VERTEX_TYPE_1_UV_CREATE_LAYOUT_DETAIL(LayoutVariable, IdLayoutVariable, OffsetBytesVariable) \
	CREATE_INPUT_LAYOUT(LayoutVariable, IdLayoutVariable, OffsetBytesVariable, 8);

//UV2
#define IFDEF_CREATE_MV_0_UV2_VERTEX
#define IFDEF_CREATE_MV_1_UV2_VERTEX Vect2f		UV2;
#define IFDEF_CREATE_GET_VERTEX_TYPE_0_UV2 0
#define IFDEF_CREATE_GET_VERTEX_TYPE_1_UV2 MV_VERTEX_TYPE_TEXTURE2
#define IFDEF_CREATE_GET_VERTEX_TYPE_0_UV2_CREATE_LAYOUT
#define IFDEF_CREATE_GET_VERTEX_TYPE_1_UV2_CREATE_LAYOUT { "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
#define IFDEF_CREATE_GET_VERTEX_TYPE_0_UV2_CREATE_LAYOUT_DETAIL
#define IFDEF_CREATE_GET_VERTEX_TYPE_1_UV2_CREATE_LAYOUT_DETAIL(LayoutVariable, IdLayoutVariable, OffsetBytesVariable) \
	CREATE_INPUT_LAYOUT(LayoutVariable, IdLayoutVariable, OffsetBytesVariable, 8);

#define CREATE_MVD3D11_VERTEX(StructName, HasPosition, HasPosition4, HasNormal, HasWeightIndices, HasColor, HasUV, HasUV2) \
struct StructName \
{ \
	IFDEF_CREATE_MV_##HasPosition##_POSITION_VERTEX; \
	IFDEF_CREATE_MV_##HasPosition4##_POSITION4_VERTEX; \
	IFDEF_CREATE_MV_##HasWeightIndices##_WEIGHT_INDICES_VERTEX; \
	IFDEF_CREATE_MV_##HasNormal##_NORMAL_VERTEX; \
	IFDEF_CREATE_MV_##HasColor##_COLOR_VERTEX; \
	IFDEF_CREATE_MV_##HasUV##_UV_VERTEX; \
	IFDEF_CREATE_MV_##HasUV2##_UV2_VERTEX; \
\
	static bool CreateInputLayout(CRenderManager *RenderManager, ID3DBlob *VSBlob,ID3D11InputLayout **VertexLayout)\
	{\
		D3D11_INPUT_ELEMENT_DESC l_Layout[] =\
		{\
			IFDEF_CREATE_GET_VERTEX_TYPE_##HasPosition##_POSITION_CREATE_LAYOUT\
			IFDEF_CREATE_GET_VERTEX_TYPE_##HasPosition4##_POSITION4_CREATE_LAYOUT\
			IFDEF_CREATE_GET_VERTEX_TYPE_##HasWeightIndices##_WEIGHT_INDICES_CREATE_LAYOUT\
			IFDEF_CREATE_GET_VERTEX_TYPE_##HasNormal##_NORMAL_CREATE_LAYOUT\
			IFDEF_CREATE_GET_VERTEX_TYPE_##HasColor##_COLOR_CREATE_LAYOUT\
			IFDEF_CREATE_GET_VERTEX_TYPE_##HasUV##_UV_CREATE_LAYOUT\
			IFDEF_CREATE_GET_VERTEX_TYPE_##HasUV2##_UV2_CREATE_LAYOUT\
		};\
		UINT l_NumElements=ARRAYSIZE(l_Layout);\
		HRESULT l_HR=RenderManager->GetDevice()->CreateInputLayout(l_Layout,\
		l_NumElements, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), VertexLayout);\
		return !FAILED(l_HR);\
	}\
\
	static unsigned int GetVertexType() \
		{ \
		return IFDEF_CREATE_GET_VERTEX_TYPE_##HasPosition##_POSITION |	IFDEF_CREATE_GET_VERTEX_TYPE_##HasPosition4##_POSITION4 | IFDEF_CREATE_GET_VERTEX_TYPE_##HasNormal##_NORMAL | IFDEF_CREATE_GET_VERTEX_TYPE_##HasWeightIndices##_WEIGHT_INDICES | IFDEF_CREATE_GET_VERTEX_TYPE_##HasColor##_COLOR | IFDEF_CREATE_GET_VERTEX_TYPE_##HasUV##_UV | IFDEF_CREATE_GET_VERTEX_TYPE_##HasUV2##_UV2; \
		} \
};
/*static bool CreateInputLayout(CRenderManager *RenderManager, ID3DBlob *VSBlob,ID3D11InputLayout **VertexLayout)\
	{\
		D3D11_INPUT_ELEMENT_DESC l_Layout[] =\
		{\
			IFDEF_CREATE_GET_VERTEX_TYPE_##HasPosition##_POSITION_CREATE_LAYOUT\
			IFDEF_CREATE_GET_VERTEX_TYPE_##HasPosition4##_POSITION4_CREATE_LAYOUT\
			IFDEF_CREATE_GET_VERTEX_TYPE_##HasWeightIndices##_WEIGHT_INDICES_CREATE_LAYOUT\
			IFDEF_CREATE_GET_VERTEX_TYPE_##HasNormal##_NORMAL_CREATE_LAYOUT\
			IFDEF_CREATE_GET_VERTEX_TYPE_##HasColor##_COLOR_CREATE_LAYOUT\
			IFDEF_CREATE_GET_VERTEX_TYPE_##HasUV##_UV_CREATE_LAYOUT\
			IFDEF_CREATE_GET_VERTEX_TYPE_##HasUV2##_UV2_CREATE_LAYOUT\
		};\
		UINT l_NumElements=ARRAYSIZE(l_Layout);\
		HRESULT l_HR=RenderManager->GetDevice()->CreateInputLayout(l_Layout,\
		l_NumElements, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), VertexLayout);\
		return !FAILED(l_HR);\
	}\*/

CREATE_MVD3D11_VERTEX(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX, 1, 0, 1, 1, 0, 1, 0);
CREATE_MVD3D11_VERTEX(MV_POSITION_NORMAL_TEXTURE_VERTEX, 1, 0, 1, 0, 0, 1, 0);
CREATE_MVD3D11_VERTEX(MV_POSITION4_COLOR_TEXTURE_VERTEX, 0, 1, 0, 0, 1, 1, 0);
CREATE_MVD3D11_VERTEX(MV_POSITION_COLOR_VERTEX, 1, 0, 0, 0, 1, 0, 0);
CREATE_MVD3D11_VERTEX(MV_POSITION_TEXTURE_VERTEX, 1, 0, 0, 0, 0, 1, 0);
CREATE_MVD3D11_VERTEX(MV_POSITION_COLOR_TEXTURE_VERTEX, 1, 0, 0, 0, 1, 1, 0);


#endif //VERTEX_TYPES_H