/*

% Normalmap shader.

date: 27012015

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


texture NormalMapTexture  <
  string ResourceName = "";//Optional default file name
  string UIName =  "NormalMapTexture Texture";
  string ResourceType = "2D";
>;

sampler2D NormalMapTextureSampler = sampler_state {
	Texture = <NormalMapTexture>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
};
	
	

float4x4 View : View;
float4x4 World : World;
float4x4 Projection : Projection;
const float PI = 3.14159265f;
const float g_Bump = 2.4;

struct TVertexVS
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
	float3 Tangent : TANGENT;
};

struct TVertexPS
{
	float4 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
	float3 Pixelpos : TEXCOORD1;
	float3 WorldTangent: TANGENT;
	float3 WorldBinormal: BINORMAL;
};

TVertexPS mainVS(TVertexVS IN)
{
	TVertexPS l_Out = (TVertexPS)0;
	
	l_Out.Pos = mul(float4(IN.Pos.xyz, 1.0), World);
	l_Out.Pixelpos = l_Out.Pos.xyz;
	l_Out.Pos = mul(l_Out.Pos, View);
	l_Out.Pos = mul(l_Out.Pos, Projection);
	l_Out.Normal = normalize(mul(IN.Normal, (float3x3)World));
	l_Out.UV = IN.UV;
	
	l_Out.WorldTangent = mul(IN.Tangent.xyz,(float3x3)World);
	l_Out.WorldBinormal = mul(cross(IN.Tangent.xyz,IN.Normal),(float3x3)World);
	
	return l_Out;
}

float4 mainPS(TVertexPS IN) : COLOR
{
	float l_DiffuseContrib;
			
	float3 Tn=normalize(IN.WorldTangent);
	float3 Bn=normalize(IN.WorldBinormal);
	float3 bump=g_Bump*(tex2D(NormalMapTextureSampler,IN.UV).rgb - float3(0.5,0.5,0.5));
	float3 Nn = IN.Normal;
	Nn = Nn + bump.x*Tn + bump.y*Bn;
	Nn = normalize(Nn);
	
	// Diffusion
	l_DiffuseContrib = dot(Nn, IN.Pixelpos);
	l_DiffuseContrib = max(0, l_DiffuseContrib);
	
	return l_DiffuseContrib*tex2D(S0Sampler,IN.UV);
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

