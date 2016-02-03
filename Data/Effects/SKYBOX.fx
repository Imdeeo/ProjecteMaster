textureCUBE <float4> T0;

samplerCUBE T0Sampler = sampler_state
{
	Texture = <T0>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};
	
	

float4x4 View : View;
float4x4 World : World;
float4x4 Projection : Projection;
float4x4 InverseView : ViewInverse;
const float PI = 3.14159265f;
const float g_EnvironmentFactor = 1.0;

struct TVertexVS
{
	float3 Pos : POSITION;
};

struct TVertexPS
{
	float4 Pos : POSITION;
	float3 Normal : NORMAL;
	float3 UV : TEXCOORD0;
	float3 Pixelpos : TEXCOORD1;
};

TVertexPS mainVS(TVertexVS IN)
{
	TVertexPS l_Output=(TVertexPS)0;
	l_Output.UV=IN.Pos-float3(0.0, 0.5, 0.0);
	l_Output.Pos=float4(m_InverseView[3].xyz+l_Output.UV*m_CameraProjectionInfo.y,1.0);
	l_Output.Pos=mul(l_Output.Pos, m_View);
	l_Output.Pos=mul(l_Output.Pos, m_Projection);
	l_Output.Pos=l_Output.Pos.xyzw;
	return l_Output;
}

float4 mainPS(TVertexPS IN) : COLOR
{	
	return T0CubeTexture.Sample(S0Sampler, IN.UV);
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
