//globals.fxh
#define MAXBONES 29

cbuffer ConstantBuffer : register( b0 )
{
	float4x4 m_World;
	float4x4 m_View;
	float4x4 m_Projection;
	float4 m_BaseColor;
	float4x4 m_Bones[MAXBONES];
	float4 m_CameraRightVector;
	float4 m_CameraUpVector;
	float4 m_BillboardPosition;
	float4 m_BillboardSize;
	float4 m_DebugRenderScale;
}