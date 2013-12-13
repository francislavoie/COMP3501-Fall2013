////////////////////////////////////////////////////////////////////////////////
// Filename: particle.vs.hlsl
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer {
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer CameraBuffer {
	float3 cameraPosition;
	float padding;
	float3 cameraUp;
	float time;
};

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType {
	float3 pos : POSITION;
	float4 col : COLOR;
	float2 timeElapsed : TEXCOORD0;
};

struct GeometryInputType {
	float4 position : POSITION;
	float3 viewDirection : NORMAL;
	float4 col : COLOR;
	float2 time : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
GeometryInputType ParticleVertexShader(VertexInputType input) {
	GeometryInputType output = (GeometryInputType) 0;
	float4 inpos = float4(input.pos.x, input.pos.y, input.pos.z, 1);
	//float4 inpos = input.pos;

	output.position = mul(inpos, worldMatrix);
	output.viewDirection = normalize(cameraPosition.xyz - inpos.xyz);
	output.col = input.col;
	output.time = input.timeElapsed;

	return output;
}