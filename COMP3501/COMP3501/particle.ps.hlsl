////////////////////////////////////////////////////////////////////////////////
// Filename: particle.ps.hlsl
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType {
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 viewDirection : NORMAL;
	float4 col : COLOR;
	float2 time : TEXCOORD1;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 ParticlePixelShader(PixelInputType input) : SV_TARGET {
	float4 color = (float4)0;

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	color.rgba = shaderTexture.Sample(SampleType, input.tex);

	//float4 inputcol = float4(input.col.r, input.col.g, input.col.b, 1.0);

	//color *= inputcol;

	//color *= float4(input.time.x, input.time.x, input.time.x, 1.0);

	color *= float4(0.9, 0.5, 0.0, 1.0);

	//if (input.time > 0.1) color.w = 0;

	//color.w *= lerp(1.0, 0.0, input.time / 2.0);

	// Make it red
	//color.r = color.r*1.9;
	//color.b = min(color.r, 0.2);
	//if (color.w > 0.1) color.w = 0.8;

	return color;
}