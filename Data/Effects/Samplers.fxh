#ifndef SAMPLERS_FXH
#define SAMPLERS_FXH

//Samplers.fxh
Texture2D T0Texture: register( t0 );
Texture2D T1Texture: register( t1 );
Texture2D T2Texture: register( t2 );
Texture2D T3Texture: register( t3 );
Texture2D T4Texture: register( t4 );
Texture2D T5Texture: register( t5 );
Texture2D T6Texture: register( t6 );
Texture3D CubeTexture : register( t7 );

SamplerState S0Sampler: register( s0 );
SamplerState S1Sampler: register( s1 );
SamplerState S2Sampler: register( s2 );
SamplerState S3Sampler: register( s3 );
SamplerState S4Sampler: register( s4 );
SamplerState S5Sampler: register( s5 );
SamplerState S6Sampler: register( s6 );
SamplerState CubeSampler : register( s7 );

#endif //SAMPLERS_FXH
