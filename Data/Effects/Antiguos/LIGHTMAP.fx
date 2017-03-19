/*

% Lightmap Shader.

date: 17122015

*/


texture S0  <
	  string ResourceName = "";//Optional default file name
	  string UIName =  "S0 Texture";
	  string ResourceType = "2D";
>;

sampler2D S0Sampler = sampler_state {
	  Texture = <S0>;
	  MinFilter = Linear;
	  MagFilter = Linear;
	  MipFilter = Linear;
	  AddressU = Wrap;
	  AddressV = Wrap;
};
	

texture L0  <
	  string ResourceName = "";//Optional default file name
	  string UIName =  "L0 Texture";
	  string ResourceType = "2D";
>;

sampler2D L0Sampler = sampler_state {
	  Texture = <L0>;
	  MinFilter = Linear;
	  MagFilter = Linear;
	  MipFilter = Linear;
	  AddressU = Wrap;
	  AddressV = Wrap;
};
	

float4x4 View : View;
float4x4 World : World;
float4x4 Projection : Projection;

struct TVertexVS
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
};

struct TVertexPS
{
	float4 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
	float2 Lightmap : TEXCOORD1;
};

TVertexPS mainVS(TVertexVS IN)
{
	TVertexPS l_Out = (TVertexPS)0;
	
	l_Out.Pos = mul(float4(IN.Pos.xyz, 1.0), World);
	l_Out.Pos = mul(l_Out.Pos, View);
	l_Out.Pos = mul(l_Out.Pos, Projection);	
	l_Out.UV = IN.UV;
	l_Out.Lightmap = IN.UV;
	
	return l_Out;
}

float4 mainPS(TVertexPS IN) : COLOR
{
	return tex2D(L0Sampler, IN.Lightmap)*tex2D(S0Sampler, IN.UV);
}

technique technique0
{
	pass p0
	{
		CullMode = None;
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}