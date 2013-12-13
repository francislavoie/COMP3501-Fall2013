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
	float4 pos : POSITION;
	float3 vel : NORMAL;
};

struct GeometryInputType {
	float4 position : POSITION;
	float3 viewDirection : NORMAL;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
GeometryInputType ParticleVertexShader(VertexInputType input) {
	GeometryInputType output = (GeometryInputType) 0;
	//float4 inpos = float4(input.pos.x, input.pos.y, input.pos.z, 1);
	float4 inpos = input.pos;

	//float t = input.timeElapsed;

	// Calculate the position of the vertex
	//inpos.xyz += input.vel * t;
	//inpos.y += (-0.98) * t * t;

	output.position = mul(inpos, worldMatrix);
	output.viewDirection = normalize(cameraPosition.xyz - inpos.xyz);

	return output;
}