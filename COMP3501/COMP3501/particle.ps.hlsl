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
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 ParticlePixelShader(PixelInputType input) : SV_TARGET {
	float4 color = (float4)0;

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	color.rgba = shaderTexture.Sample(SampleType, input.tex);

	// Make it red
	color.r = color.r*1.9;
	color.b = min(color.r, 0.2);
	if (color.w > 0.1) color.w = 0.8;

	return color;
}