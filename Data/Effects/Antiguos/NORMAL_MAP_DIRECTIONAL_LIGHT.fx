/*

% Normalmap+Spot light shader.

date: 17122015

*/


float3 Lamp0Position : POSITION <
	  string Object = "Spot Light 0";
	  string UIName =  "Lamp 0 Position";
	  string Space = "World";
> = {-0.5f,2.0f,1.25f};

float3 Lamp0Direction : DIRECTION <
	  string Object = "Spot Light 0";
	  string UIName =  "Lamp 0 Direction";
	  string Space = "World";
> = {-0.5f,2.0f,1.25f};	

float3 Lamp0Color : COLOR <
	  string Object = "Spot Light 0";
	  string UIName =  "Lamp 0 Color";
	  string UIWidget = "Color";
> = {1.0f,1.0f,1.0f};


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
	// Light properties
	float l_Angle = PI / 3; // The total angle just like in 3ds Max, not deviation from the centre.
	float l_Falloff = l_Angle + PI / 12;
	float l_StartAttDist = 2;
	float l_EndAttDist = 5;
	
	// Factors in the final multiplication.
	float l_DiffuseContrib;
	float l_DistanceAttenuation;
	float l_SpotAttenuation;
	
	// Intermediate values
	float3 l_RayDirection = normalize(IN.Pixelpos - Lamp0Position);
	float l_Distance = distance(IN.Pixelpos, Lamp0Position);
	float l_DirectionContrib = dot(l_RayDirection, normalize(Lamp0Direction));
		
	float3 Tn=normalize(IN.WorldTangent);
	float3 Bn=normalize(IN.WorldBinormal);
	//La variable g_Bump es una constante que nos dará la profundidad, podemos utilizar un valor de 2.4
	float3 bump=g_Bump*(tex2D(NormalMapTextureSampler,IN.UV).rgb - float3(0.5,0.5,0.5));
	float3 Nn = IN.Normal;
	Nn = Nn + bump.x*Tn + bump.y*Bn;
	Nn = normalize(Nn);
	
	// Diffusion
	l_DiffuseContrib = dot(Nn, -l_RayDirection);
	l_DiffuseContrib = max(0, l_DiffuseContrib);
	
	// Distance attenuation (linear)
	l_DistanceAttenuation = 1 - saturate((l_Distance - l_StartAttDist) / (l_EndAttDist - l_StartAttDist));

	// Angle attenuation (linear)
	l_SpotAttenuation = 1 - saturate((acos(l_DirectionContrib) - l_Angle/2) / (l_Falloff/2 - l_Angle/2));

	
	return l_DiffuseContrib*l_DistanceAttenuation*l_SpotAttenuation*tex2D(S0Sampler,IN.UV)*(float4(Lamp0Color, 1.0));
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
