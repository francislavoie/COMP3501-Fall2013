////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "particle.h"


Particle::Particle() {
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_vertices = 0;
	m_Particles = 0;
}


Particle::Particle(const Particle& other) { }


Particle::~Particle() { }


bool Particle::Initialize(ID3D11Device* device, WCHAR* textureFilename) {
	bool result;

	//m_vertexCount = particleCount;
	//m_indexCount = m_vertexCount;

	//m_Model = new VertexType[m_vertexCount];

	//for (int i = 0; i < m_vertexCount; i++) {
	//	//m_Model[i].pos = position;

	//	//float theta = 2.0f * float(D3DX_PI * float(rand()) / RAND_MAX);

	//	//float R = -1.0f + 2.0f * float(float(rand()) / RAND_MAX);
	//	//float a = sqrt(1 - R*R);

	//	//m_Model[i].vel = D3DXVECTOR3(cos(theta)*a, sin(theta)*a, R);

	//	float r, theta, phi;
	//	m_Model[i].start = D3DXVECTOR3(0, 0, 0);
	//	m_Model[i].pos = position;
	//	m_Model[i].timeElapsed = 0.0f;

	//	if (ring) {
	//		r = 0.1f + (rand() % 10)*0.005;
	//	} else {
	//		r = (rand() % 10000) / 10000.0f - 0.5;
	//	}

	//	r = r * 10;
	//	theta = (rand() % 10000) / 10000.0f * 2 * D3DX_PI;
	//	phi = (rand() % 10000) / 10000.0f * 2 * D3DX_PI;

	//	if (!ring) {
	//		m_Model[i].vel = D3DXVECTOR3(
	//			r * sin(theta) * cos(phi),
	//			r * cos(theta) * cos(phi) + 2.5f,
	//			r * sin(phi)
	//		);
	//	} else {
	//		m_Model[i].vel = D3DXVECTOR3(
	//			r * 6 * sin(theta),
	//			0,
	//			r * 6 * cos(theta)
	//		);
	//	}
	//}

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if (!result) return false;

	// Initialize the particle system.
	result = InitializeParticleSystem();
	if (!result) return false;

	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	result = InitializeBuffers(device);
	if (!result) return false;

	return true;
}


void Particle::Shutdown() {

	// Release the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ShutdownParticleSystem();

	// Release the model texture.
	ReleaseTexture();

	return;
}


bool Particle::Frame(float frameTime, ID3D11DeviceContext* device, bool emit, D3DXVECTOR3 position) {
	bool result;

	// Release old particles.
	KillParticles();

	// Emit new particles.
	if(emit) EmitParticles(position, frameTime);

	// Update the position of the particles.
	UpdateParticles(frameTime);

	// Update the dynamic vertex buffer with the new position of each particle.
	result = UpdateBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
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

bool Particle::InitializeParticleSystem() {
	int i;

	// Set the maximum number of particles allowed in the particle system.
	m_maxParticles = 10000;

	// Create the particle list.
	m_Particles = new ParticleType[m_maxParticles];
	if (!m_Particles) return false;

	// Initialize the particle list.
	for (i = 0; i < m_maxParticles; i++) {
		m_Particles[i].active = false;
		m_Particles[i].timeElapsed = 0;
	}

	// Initialize the current particle count to zero since none are emitted yet.
	m_particleCount = 0;

	return true;
}


void Particle::ShutdownParticleSystem() {
	// Release the particle list.
	if (m_Particles) {
		delete[] m_Particles;
		m_Particles = 0;
	}

	return;
}


bool Particle::InitializeBuffers(ID3D11Device* device) {
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	m_vertexCount = m_maxParticles;
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	m_vertices = new VertexType[m_vertexCount];
	if (!m_vertices) return false;

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices) return false;

	// Initialize vertex array to zeros at first.
	memset(m_vertices, 0, (sizeof(VertexType)* m_vertexCount));

	// Load the vertex array and index array with data.
	for (i = 0; i < m_indexCount; i++) {
		//m_vertices[i].pos = m_Particles[i].pos;
		//m_vertices[i].col = m_Particles[i].col;
		//m_vertices[i].timeElapsed = m_Particles[i].timeElapsed;
		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the sub-resource structure a pointer to the vertex data.
	vertexData.pSysMem = m_vertices;
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


void Particle::EmitParticles(D3DXVECTOR3 emitPos, float frameTime) {
	bool found;
	int index, i, j;
	D3DXVECTOR3 velocity;
	D3DXVECTOR4 color;

	for (int p = 0; p < 50; p++) {
		if ((m_particleCount < (m_maxParticles - 1))) {
			m_particleCount++;

			float r = (((float)rand() - (float)rand()) / RAND_MAX) - 0.5f;
			r = r * 10;
			float theta = (((float)rand() - (float)rand()) / RAND_MAX) * D3DX_PI;
			float phi = (((float)rand() - (float)rand()) / RAND_MAX) * 2.0f * D3DX_PI;

			velocity = D3DXVECTOR3(
				r * sin(theta) * cos(phi),
				r * cos(theta) * cos(phi) + 2.5f,
				r * sin(phi)
			);

			color = D3DXVECTOR4(
				(((float)rand() - (float)rand()) / RAND_MAX) + 0.5f,
				(((float)rand() - (float)rand()) / RAND_MAX) + 0.5f,
				(((float)rand() - (float)rand()) / RAND_MAX) + 0.5f,
				1.0f
			);

			index = m_particleCount - 1;

			// Now insert it into the particle array in the correct depth order.
			m_Particles[index].start = emitPos;
			m_Particles[index].pos = emitPos;
			m_Particles[index].col = color;
			m_Particles[index].vel = velocity;
			m_Particles[index].timeElapsed = 0.0f;
			m_Particles[index].active = true;
		}
		else {
			break;
		}
	}

	return;
}


void Particle::UpdateParticles(float time) {
	for (int i = 0; i < m_particleCount; i++) {
		m_Particles[i].timeElapsed += time / 1000.0f;
		m_Particles[i].pos = m_Particles[i].start + m_Particles[i].vel * m_Particles[i].timeElapsed;
		m_Particles[i].pos.y += (-0.98) *  m_Particles[i].timeElapsed * m_Particles[i].timeElapsed;
	}
}


void Particle::KillParticles() {
	int i, j;
	
	// Kill all the particles that have gone below a certain height range.
	for (i = 0; i < m_maxParticles; i++) {
		if ((m_Particles[i].active == true) && (m_Particles[i].timeElapsed > 3.0f)) {
			m_Particles[i].active = false;
			m_particleCount--;

			// Now shift all the live particles back up the array to erase the destroyed particle and keep the array sorted correctly.
			for (j = i; j < m_maxParticles - 1; j++) {
				m_Particles[j].start = m_Particles[j + 1].start;
				m_Particles[j].pos = m_Particles[j + 1].pos;
				m_Particles[j].col = m_Particles[j + 1].col;
				m_Particles[j].vel = m_Particles[j + 1].vel;
				m_Particles[j].timeElapsed = m_Particles[j + 1].timeElapsed;
				m_Particles[j].active = m_Particles[j + 1].active;
			}
		}
	}

	return;
}


bool Particle::UpdateBuffers(ID3D11DeviceContext* deviceContext)
{
	int index, i;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;


	// Initialize vertex array to zeros at first.
	memset(m_vertices, 0, (sizeof(VertexType)* m_vertexCount));

	// Now build the vertex array from the particle list array.
	index = 0;

	for (i = 0; i < m_particleCount; i++) {
		m_vertices[index].pos = m_Particles[i].pos;
		m_vertices[index].timeElapsed = m_Particles[i].timeElapsed;
		m_vertices[index].col = m_Particles[i].col;
		index++;
	}

	// Lock the vertex buffer.
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)m_vertices, (sizeof(VertexType)* m_vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(m_vertexBuffer, 0);

	return true;
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