#include "Globals.fxh"
#include "Samplers.fxh"

static float sprite_sheet_width = m_RawDataArray[0];
static float sprite_sheet_height = m_RawDataArray[1];
static float lerp_sprite = m_RawDataArray[2];
static float ratio_y = m_RawDataArray[2];
static float du = 1.0 / sprite_sheet_width;
static float dv = 1.0 / sprite_sheet_height;

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
	float4 Color : COLOR0;
	float Size : TEXCOORD0;
	float Angle : TEXCOORD1;
	float SpriteIndex : TEXCOORD2;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
	float2 UV : TEXCOORD0;
	float2 UV2 : TEXCOORD1;
	float TextureBlendFactor : TEXCOORD2;
};

//---------------------------------------------------------------------------------
// Vertex Shader
//---------------------------------------------------------------------------------
GS_INPUT mainVS( VS_INPUT IN )
{
	GS_INPUT l_Output = (GS_INPUT)0;
	l_Output.Pos = mul( IN.Pos, m_World );
	l_Output.Pos = mul( l_Output.Pos, m_View );
	l_Output.Color = IN.Color;
	l_Output.Size = IN.UV.x;
	l_Output.Angle = IN.UV.y;
	l_Output.SpriteIndex = IN.UV2.x;
	
	return l_Output;
}

//---------------------------------------------------------------------------------
//Simple Geometry Shader
//---------------------------------------------------------------------------------
[maxvertexcount(4)]
void mainGS( point GS_INPUT input[1], inout TriangleStream<PS_INPUT> OutputStream )
{
	PS_INPUT l_Output = (PS_INPUT)0;
	
	float halfSize = input[0].Size * 0.5;
	float4 pos = input[0].Pos;
	
	l_Output.Color = input[0].Color;
	l_Output.Pos = mul( pos + halfSize * float4(1, 1, 0.0, 0.0), m_Projection );
	l_Output.UV = float2(0, 0);
	l_Output.UV2 = float2(0, 0);
	OutputStream.Append( l_Output );
	
	l_Output.Pos = mul( pos + halfSize * float4(+1, 1, 0.0, 0.0), m_Projection );
	l_Output.UV = float2(1, 0);
	l_Output.UV2 = float2(1, 0);	
	OutputStream.Append( l_Output );
	
	l_Output.Pos = mul( pos + halfSize * float4(1, +1, 0.0, 0.0), m_Projection );
	l_Output.UV = float2(0, 1);
	l_Output.UV2 = float2(0, 1);
	OutputStream.Append( l_Output );
	
	l_Output.Pos = mul( pos + halfSize * float4(+1, +1, 0.0, 0.0), m_Projection );
	l_Output.UV = float2(1, 1);
	l_Output.UV2 = float2(1, 1);
	OutputStream.Append( l_Output );
	
	OutputStream.RestartStrip();
}

//---------------------------------------------------------------------------------
//Geometry Shader
//---------------------------------------------------------------------------------
/*[maxvertexcount(4)]
void mainGS( point GS_INPUT input[1], inout TriangleStream<PS_INPUT> OutputStream )
{
	PS_INPUT l_Output = (PS_INPUT)0;
	
	float halfSize = input[0].Size * 0.5;
	float4 pos = input[0].Pos;
	float spriteIndex1 = floor(input[0].SpriteIndex);
	float spriteIndex1X = fmod(spriteIndex1, sprite_sheet_width);
	
	// "spriteIndex / sprite_sheet_width"
	float spriteIndex1Y = floor(spriteIndex1 * du);
	float spriteIndex2 = fmod(spriteIndex1 + 1.0, sprite_sheet_width * sprite_sheet_height);
	float spriteIndex2X = fmod(spriteIndex2, sprite_sheet_width);
	
	// "spriteIndex / sprite_sheet_width"
	float spriteIndex2Y = floor(spriteIndex2 * du);
	l_Output.TextureBlendFactor = input[0].SpriteIndex spriteIndex1;
	float x = 1.41421356237 * cos(input[0].Angle + 3.14159265359 * 0.25);
	float y = 1.41421356237 * sin(input[0].Angle + 3.14159265359 * 0.25) * ratio_y;
	
	l_Output.Color = input[0].Color;
	l_Output.Pos = mul( pos + halfSize * float4(x, y, 0.0, 0.0), m_Projection );
	l_Output.UV = float2(spriteIndex1X * du, spriteIndex1Y * dv);
	l_Output.UV2 = float2(spriteIndex2X * du, spriteIndex2Y * dv);
	OutputStream.Append( l_Output );
	
	l_Output.Pos = mul( pos + halfSize * float4(+y, x, 0.0, 0.0), m_Projection );
	l_Output.UV = float2(spriteIndex1X * du + du, spriteIndex1Y * dv);
	l_Output.UV2 = float2(spriteIndex2X * du + du, spriteIndex2Y * dv);
	OutputStream.Append( l_Output );
	
	l_Output.Pos = mul( pos + halfSize * float4(y, +x, 0.0, 0.0), m_Projection );
	l_Output.UV = float2(spriteIndex1X * du, spriteIndex1Y * dv + dv);
	l_Output.UV2 = float2(spriteIndex2X * du, spriteIndex2Y * dv + dv);
	OutputStream.Append( l_Output );
	
	l_Output.Pos = mul( pos + halfSize * float4(+x, +y, 0.0, 0.0), m_Projection );
	l_Output.UV = float2(spriteIndex1X * du + du, spriteIndex1Y * dv + dv);
	l_Output.UV2 = float2(spriteIndex2X * du + du, spriteIndex2Y * dv + dv);
	OutputStream.Append( l_Output );
	
	OutputStream.RestartStrip();
}*/

//---------------------------------------------------------------------------------
//Pixel Shader
//---------------------------------------------------------------------------------
float4 mainPS( PS_INPUT IN) : SV_Target
{
	float4 t1 = T0Texture.Sample( S0Sampler, IN.UV );
	float4 t2 = T0Texture.Sample( S0Sampler, IN.UV2 );
	float a = IN.TextureBlendFactor;
	
	return IN.Color * lerp(t1, t2, float4(a, a, a, a));
}