#include "Globals.fxh"
#include "Samplers.fxh"

static float sprite_sheet_width = m_RawDataArray[0];
static float sprite_sheet_height = m_RawDataArray[1];
static float du = 1.0 / sprite_sheet_width;
static float dv = 1.0 / sprite_sheet_height;

//---------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 Pos : POSITION;
	float4 Color : COLOR0;
	float2 UV : TEXCOORD0;
};

struct GS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
	float Size : TEXCOORD0;
	float SpriteIndex : TEXCOORD2;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
	float2 UV : TEXCOORD0;
};

//---------------------------------------------------------------------------------
// Vertex Shader
//---------------------------------------------------------------------------------
GS_INPUT mainVS( VS_INPUT IN )
{
	GS_INPUT l_Output = (GS_INPUT)0;
	l_Output.Pos = IN.Pos;
	l_Output.Color = IN.Color;
	l_Output.SpriteIndex = IN.UV.x;
	l_Output.Size = IN.UV.y;
	return l_Output;
}

[maxvertexcount(4)]
void mainGS( point GS_INPUT input[1], inout TriangleStream<PS_INPUT> OutputStream )
{
	PS_INPUT l_Output = (PS_INPUT)0;
	
	float halfSize = 3;
	float4 pos = input[0].Pos;
	
	float spriteIndex = floor(input[0].SpriteIndex);
	float spriteIndexX = fmod(spriteIndex, sprite_sheet_width);
	float spriteIndexY = floor(spriteIndex * du);
	
	l_Output.Color = input[0].Color;
	
	float size = input[0].Size/10;
	float4 auxPos = input[0].Pos;
	
	l_Output.Pos =  float4(auxPos.x - size, auxPos.y - size, 0.01,1);
	l_Output.UV = float2(spriteIndexX * du, spriteIndexY * dv);
	OutputStream.Append( l_Output );
	
	l_Output.Pos = float4(auxPos.x + size, auxPos.y - size, 0.01,1);
	l_Output.UV = float2(spriteIndexX * du + du, spriteIndexY * dv);
	OutputStream.Append( l_Output );
	
	l_Output.Pos = float4(auxPos.x - size, auxPos.y + size, 0.01,1);
	l_Output.UV = float2(spriteIndexX * du, spriteIndexY * dv + dv);
	OutputStream.Append( l_Output );
	
	l_Output.Pos = float4(auxPos.x + size, auxPos.y + size, 0.01,1);
	l_Output.UV = float2(spriteIndexX * du + du, spriteIndexY * dv + dv);
	OutputStream.Append( l_Output );
	
	OutputStream.RestartStrip();
}

//---------------------------------------------------------------------------------
//Pixel Shader
//---------------------------------------------------------------------------------
float4 mainPS( PS_INPUT IN) : SV_Target
{
	//if (IN.Pos.w > 1 )
	//	return float4(1,0,0,1);
	//return IN.Color;
	
	return IN.Color * T0Texture.Sample( S0Sampler, IN.UV );
}