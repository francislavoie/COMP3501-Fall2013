////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "particle.h"


Particle::Particle() {
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_Model = 0;
}


Particle::Particle(const Particle& other) { }


Particle::~Particle() { }


bool Particle::Initialize(ID3D11Device* device, int particleCount, D3DXVECTOR3 position, bool ring, WCHAR* textureFilename) {
	bool result;

	m_vertexCount = particleCount;
	m_indexCount = m_vertexCount;

	m_Model = new VertexType[m_vertexCount];

	for (int i = 0; i < m_vertexCount; i++) {
		//m_Model[i].pos = position;

		//float theta = 2.0f * float(D3DX_PI * float(rand()) / RAND_MAX);

		//float R = -1.0f + 2.0f * float(float(rand()) / RAND_MAX);
		//float a = sqrt(1 - R*R);

		//m_Model[i].vel = D3DXVECTOR3(cos(theta)*a, sin(theta)*a, R);

		float r, theta, phi;
		m_Model[i].pos = position;

		if (ring) {
			r = 0.1f + (rand() % 10)*0.005;
		} else {
			r = (rand() % 10000) / 10000.0f - 0.5;
		}

		r = r * 10;
		theta = (rand() % 10000) / 10000.0f * 2 * D3DX_PI;
		phi = (rand() % 10000) / 10000.0f * 2 * D3DX_PI;

		if (!ring) {
			m_Model[i].vel = D3DXVECTOR3(
				r * sin(theta) * cos(phi),
				r * cos(theta) * cos(phi) + 2.5f,
				r * sin(phi)
			);
		} else {
			m_Model[i].vel = D3DXVECTOR3(
				r * 6 * sin(theta),
				0,
				r * 6 * cos(theta)
			);
		}
	}

	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	result = InitializeBuffers(device);
	if (!result) return false;

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if (!result) return false;

	return true;
}


void Particle::ResetPosition(ID3D11Device* device, D3DXVECTOR3 newpos) {
	for (int i = 0; i < m_vertexCount; i++) {
		m_Model[i].pos = newpos;
	}

	InitializeBuffers(device);
}


void Particle::Shutdown() {

	// Release the model texture.
	ReleaseTexture();

	// Release the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}


void Particle::Render(ID3D11DeviceContext* deviceContext) {
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int Particle::GetIndexCount() {
	return m_indexCount;
}


ID3D11ShaderResourceView* Particle::GetTexture() {
	return m_Texture->GetTexture();
}


bool Particle::InitializeBuffers(ID3D11Device* device) {
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices) return false;

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices) return false;

	// Load the vertex array and index array with data.
	for (i = 0; i < m_vertexCount; i++) {
		vertices[i].pos = m_Model[i].pos;
		vertices[i].vel = m_Model[i].vel;
		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the sub-resource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result)) return false;

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the sub-resource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result)) return false;

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


void Particle::ShutdownBuffers() {
	// Release the index buffer.
	if (m_indexBuffer) {
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer) {
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void Particle::RenderBuffers(ID3D11DeviceContext* deviceContext) {
	unsigned int stride;
	unsigned int offset;
	
	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	return;
}


bool Particle::LoadTexture(ID3D11Device* device, WCHAR* filename) {
	bool result;

	// Create the texture object.
	m_Texture = new Texture;
	if (!m_Texture) return false;

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if (!result) return false;

	return true;
}


void Particle::ReleaseTexture() {
	// Release the texture object.
	if (m_Texture) {
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}


void Particle::ReleaseModel() {
	if (m_Model) {
		delete[] m_Model;
		m_Model = 0;
	}

	return;
}