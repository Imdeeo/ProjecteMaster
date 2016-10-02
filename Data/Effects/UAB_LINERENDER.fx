#include "Globals.fxh"
#include "Samplers.fxh"

static float m_size = m_RawDataArray[0];


//---------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 Pos : POSITION;
	float4 Color : COLOR0;
	float2 UV : TEXCOORD0;
	float2 UV2 : TEXCOORD1;
};

struct GS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Pos2 : TEXCOORD0;
	float4 Color : COLOR0;
	float2 UV2 : TEXCOORD2;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD0;
	float4 Color : COLOR0;
};

//---------------------------------------------------------------------------------
// Vertex Shader
//---------------------------------------------------------------------------------
GS_INPUT mainVS( VS_INPUT IN )
{
	GS_INPUT l_Output = (GS_INPUT)0;
	l_Output.Pos = float4(IN.Pos.xyz,1);
	l_Output.Pos2 = float4(IN.Pos.w, IN.UV,1);
	l_Output.Color = IN.Color;
	l_Output.UV2 = IN.UV2;
	return l_Output;
}

//---------------------------------------------------------------------------------
//Geometry Shader
//---------------------------------------------------------------------------------
[maxvertexcount(4)]
void mainGS( point GS_INPUT input[1], inout TriangleStream<PS_INPUT> OutputStream )
{
	PS_INPUT l_Output = (PS_INPUT)0;
	float size1 = input[0].UV2.x;
	float halfSize1 = size1 * 0.5;
	float4 halfSizeVect1 = halfSize1 * m_CameraUpVector;
	
	float size2 = input[0].UV2.y;
	float halfSize2 = size2 * 0.5;
	float4 halfSizeVect2 = halfSize2 * m_CameraUpVector;
	//float halfSize = m_size * 0.5;
	//float4 pos = input[0].Pos;
	
	l_Output.Color = input[0].Color;
	
	l_Output.Pos = input[0].Pos;
	l_Output.Pos = l_Output.Pos+halfSizeVect1;
	l_Output.Pos = mul( l_Output.Pos, m_World );
	l_Output.Pos = mul( l_Output.Pos, m_View );	
	l_Output.Pos = mul( l_Output.Pos, m_Projection );
	l_Output.UV = float2(0,1);
	OutputStream.Append( l_Output );
	
	l_Output.Pos = input[0].Pos;
	l_Output.Pos = l_Output.Pos-halfSizeVect1;
	l_Output.Pos = mul( l_Output.Pos, m_World );	
	l_Output.Pos = mul( l_Output.Pos, m_View );	
	l_Output.Pos = mul( l_Output.Pos, m_Projection );
	l_Output.UV = float2(0,0);
	OutputStream.Append( l_Output );
	
	l_Output.Pos = input[0].Pos2;
	l_Output.Pos = l_Output.Pos+halfSizeVect2;
	l_Output.Pos = mul(l_Output.Pos, m_World );	
	l_Output.Pos = mul( l_Output.Pos, m_View );	
	l_Output.Pos = mul( l_Output.Pos, m_Projection );
	l_Output.UV = float2(1,1);
	OutputStream.Append( l_Output );	

	l_Output.Pos = input[0].Pos2;
	l_Output.Pos = l_Output.Pos-halfSizeVect2;
	l_Output.Pos = mul(l_Output.Pos, m_World );	
	l_Output.Pos = mul( l_Output.Pos, m_View );	
	l_Output.Pos = mul( l_Output.Pos, m_Projection );
	l_Output.UV = float2(1,0);
	OutputStream.Append( l_Output );
	OutputStream.RestartStrip();	
}

//---------------------------------------------------------------------------------
//Pixel Shader
//---------------------------------------------------------------------------------
float4 mainPS( PS_INPUT IN) : SV_Target
{
	float4 t1 = T0Texture.Sample( S0Sampler, IN.UV );
	return t1*IN.Color;
}