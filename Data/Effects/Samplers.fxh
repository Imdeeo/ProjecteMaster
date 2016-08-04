//Samplers.fxh
#ifndef SAMPLERS_FXH
#define SAMPLERS_FXH

Texture2D T0Texture: register( t0 );	// UV
Texture2D T1Texture: register( t1 );	// UV2
Texture2D T2Texture: register( t2 );	// Bump
Texture2D T3Texture: register( t3 );	// 
Texture2D T4Texture: register( t4 );	// 
Texture2D T5Texture: register( t5 );	// Glossiness map
Texture2D T6Texture: register( t6 );	// Shadowmap
Texture2D T7Texture: register( t7 );	// Shadowmask
TextureCube T8Texture: register( t8 );	// Reflection
Texture3D T9Texture: register( t9 );	// Skybox

SamplerState S0Sampler: register( s0 );
SamplerState S1Sampler: register( s1 );
SamplerState S2Sampler: register( s2 );
SamplerState S3Sampler: register( s3 );
SamplerState S4Sampler: register( s4 );
SamplerState S5Sampler: register( s5 );
SamplerState S6Sampler: register( s6 );
SamplerState S7Sampler: register( s7 );
SamplerState S8Sampler: register( s8 );
SamplerState S9Sampler: register( s9 );

#endif //SAMPLERS_FXH
