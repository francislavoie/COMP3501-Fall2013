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
	float3 cameraUp;
	float time;
	float padding;
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
	//float4 inpos = float4(input.position.x, input.position.y, input.position.z, 1);
	float4 inpos = input.pos;

	float t = time -4 * floor(time / 4);

	// Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
	output.viewDirection = cameraPosition.xyz - output.position.xyz;
	output.viewDirection = normalize(output.viewDirection);

	float3 side = cross(cameraUp, output.viewDirection);

	// Calculate the position of the vertex
	inpos.x += input.vel.x * t;
	inpos.y += input.vel.y * t + (-0.9) * t * t;
	inpos.z += input.vel.z * t;
	output.position = mul(inpos, worldMatrix);

	return output;
}