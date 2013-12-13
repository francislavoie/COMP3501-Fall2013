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
	float padding;
	float3 cameraUp;
	float time;
};

//////////////
// TYPEDEFS //
//////////////
struct GeometryInputType {
	float4 position : POSITION;
	float3 viewDirection : NORMAL;
	float4 col : COLOR;
	float2 time : TEXCOORD0;
};

struct PixelInputType {
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 viewDirection : NORMAL;
	float4 col : COLOR;
	float2 time : TEXCOORD1;
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

	float3 side = normalize(cross(cameraUp, gin[0].viewDirection));
	float3 up = normalize(cross(gin[0].viewDirection, side));

	pos = gin[0].position.xyz - hWidth * side - hHeight * up;
	v[0] = float4(pos.x, pos.y, pos.z, 1);
	pos = gin[0].position.xyz + hWidth * side - hHeight * up;
	v[1] = float4(pos.x, pos.y, pos.z, 1);
	pos = gin[0].position.xyz - hWidth * side + hHeight * up;
	v[2] = float4(pos.x, pos.y, pos.z, 1);
	pos = gin[0].position.xyz + hWidth * side + hHeight * up;
	v[3] = float4(pos.x, pos.y, pos.z, 1);
	

	//v[0] = float4(gin[0].position.x - hWidth, gin[0].position.y + hHeight, gin[0].position.z, 1);
	//v[1] = float4(gin[0].position.x + hWidth, gin[0].position.y + hHeight, gin[0].position.z, 1);
	//v[2] = float4(gin[0].position.x - hWidth, gin[0].position.y - hHeight, gin[0].position.z, 1);
	//v[3] = float4(gin[0].position.x + hWidth, gin[0].position.y - hHeight, gin[0].position.z, 1);


	PixelInputType gout;

	for (uint i = 0; i < 4; i++) {
		gout.position = mul(v[i], viewMatrix);
		gout.position = mul(gout.position, projectionMatrix);
		gout.tex = float2((float)floor(i / 2), (float)(i % 2));
		gout.viewDirection = gin[0].viewDirection;
		gout.col = gin[0].col;
		gout.time = gin[0].time;
		triStream.Append(gout);
	}
}