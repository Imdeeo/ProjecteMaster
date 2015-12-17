/*

% Spot light shader.

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
	

float4x4 View : View;
float4x4 World : World;
float4x4 Projection : Projection;
const float PI = 3.14159265f;

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
	float3 Pixelpos : TEXCOORD1;
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
	
	return l_Out;
}

float4 mainPS(TVertexPS IN) : COLOR
{
	float l_Angle = cos(PI/12);
	float l_Falloff = cos(PI/6);
	float l_DiffuseContrib;
	float l_SpotAttenuation;
	float l_DirectionContrib;
	l_DiffuseContrib = dot(IN.Normal, normalize(Lamp0Position - IN.Pixelpos));
	l_DiffuseContrib = max(0, l_DiffuseContrib);
	l_DirectionContrib = dot(normalize(Lamp0Position - IN.Pixelpos), normalize(-Lamp0Direction));
	l_DirectionContrib = max(0, l_DirectionContrib);
	l_SpotAttenuation = 1-saturate((l_DirectionContrib-l_Angle)/(l_Falloff-l_Angle));
	l_SpotAttenuation = max(0, l_SpotAttenuation);
	return l_SpotAttenuation*l_DiffuseContrib*tex2D(S0Sampler,IN.UV)*(float4(Lamp0Color, 1.0));
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
