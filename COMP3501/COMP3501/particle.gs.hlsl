////////////////////////////////////////////////////////////////////////////////
// Filename: particle.gs.hlsl
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
struct GeometryInputType {
	float4 position : POSITION;
	float3 viewDirection : NORMAL;
};

struct PixelInputType {
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 viewDirection : NORMAL;
};

////////////////////////////////////////////////////////////////////////////////
// Geometry Shader
////////////////////////////////////////////////////////////////////////////////
[maxvertexcount(4)]
void ParticleGeometryShader(point GeometryInputType gin[1], inout TriangleStream<PixelInputType> triStream) {
	float4 v[4];
	float3 pos;

	float hHeight = 0.5;
	float hWidth = 0.5;

	float3 side = normalize(cross(cameraUp, gin[0].viewDirection)), up = normalize(cross(gin[0].viewDirection, side));

	pos = gin[0].position - hWidth * side - hHeight * up;
	v[0] = float4(pos.x, pos.y, pos.z, 1);
	pos = gin[0].position + hWidth * side - hHeight * up;
	v[1] = float4(pos.x, pos.y, pos.z, 1);
	pos = gin[0].position - hWidth * side + hHeight * up;
	v[2] = float4(pos.x, pos.y, pos.z, 1);
	pos = gin[0].position + hWidth * side + hHeight * up;
	v[3] = float4(pos.x, pos.y, pos.z, 1);
	

	//v[0] = float4(gin[0].position.x - hWidth, gin[0].position.y + hHeight, gin[0].position.z, 1);
	//v[1] = float4(gin[0].position.x + hWidth, gin[0].position.y + hHeight, gin[0].position.z, 1);
	//v[2] = float4(gin[0].position.x - hWidth, gin[0].position.y - hHeight, gin[0].position.z, 1);
	//v[3] = float4(gin[0].position.x + hWidth, gin[0].position.y - hHeight, gin[0].position.z, 1);


	PixelInputType gout;

	for (int i = 0; i < 4; i++) {
		gout.position = mul(v[i], viewMatrix);
		gout.position = mul(gout.position, projectionMatrix);
		gout.tex = float2(floor(i / 2)*0.5, (float)(i % 2)*0.5);
		gout.viewDirection = gin[0].viewDirection;
		triStream.Append(gout);
	}
}