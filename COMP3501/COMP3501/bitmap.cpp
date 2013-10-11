////////////////////////////////////////////////////////////////////////////////
// Filename: bitmapclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "bitmap.h"


Bitmap::Bitmap() {
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
}


Bitmap::Bitmap(const Bitmap& other) { }


Bitmap::~Bitmap() { }


bool Bitmap::Initialize(ID3D11Device* device, D3DXVECTOR2 screen, WCHAR* textureFilename, D3DXMATRIX baseViewMatrix, D3DXVECTOR2 size) {
	bool result;

	// Store the screen size.
	m_screen = screen;

	// Store the base view matrix.
	m_baseViewMatrix = baseViewMatrix;

	// Store the size in pixels that this bitmap should be rendered at.
	m_size = size;

	// Initialize the previous rendering position to negative one.
	m_prevPos = D3DXVECTOR2(0.0f, 0.0f);

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result) return false;

	// Load the texture for this bitmap.
	result = LoadTexture(device, textureFilename);
	if(!result) return false;

	return true;
}


void Bitmap::Shutdown() {
	// Release the bitmap texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}


bool Bitmap::Render(ID3D11DeviceContext* deviceContext, D3DXVECTOR2 size) {
	bool result;

	// Re-build the dynamic vertex buffer for rendering to possibly a different location on the screen.
	result = UpdateBuffers(deviceContext, size);
	if(!result) return false;

	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return true;
}


int Bitmap::GetIndexCount() {
	return m_indexCount;
}


ID3D11ShaderResourceView* Bitmap::GetTexture() {
	return m_Texture->GetTexture();
}


bool Bitmap::InitializeBuffers(ID3D11Device* device) {
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Set the number of vertices in the vertex array.
	m_vertexCount = 6;

	// Set the number of indices in the index array.
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices) return false;

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices) return false;

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	// Load the index array with data.
	for(i=0; i<m_indexCount; i++) {
		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the sub resource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result)) return false;

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the sub resource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result)) return false;

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}


void Bitmap::ShutdownBuffers() {
	// Release the index buffer.
	if(m_indexBuffer) {
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer) {
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


bool Bitmap::UpdateBuffers(ID3D11DeviceContext* deviceContext, D3DXVECTOR2 position) {
	float left, right, top, bottom;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;
	
	// If the position we are rendering this bitmap to has not changed then don't update the vertex buffer since it
	// currently has the correct parameters.
	if((position == m_prevPos)) return true;
	
	// If it has changed then update the position it is being rendered to.
	m_prevPos = position;

	// Calculate the screen coordinates of the left side of the bitmap.
	left = (float)((m_screen.x / 2) * -1) + (float)position.x;

	// Calculate the screen coordinates of the right side of the bitmap.
	right = left + (float)m_size.x;

	// Calculate the screen coordinates of the top of the bitmap.
	top = (float)(m_screen.y / 2) - (float)position.y;

	// Calculate the screen coordinates of the bottom of the bitmap.
	bottom = top - (float)m_size.y;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices) return false;

	// Load the vertex array with data.
	// First triangle.
	vertices[0].pos = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	vertices[0].tex = D3DXVECTOR2(0.0f, 0.0f);

	vertices[1].pos = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].tex = D3DXVECTOR2(1.0f, 1.0f);

	vertices[2].pos = D3DXVECTOR3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].tex = D3DXVECTOR2(0.0f, 1.0f);

	// Second triangle.
	vertices[3].pos = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	vertices[3].tex = D3DXVECTOR2(0.0f, 0.0f);

	vertices[4].pos = D3DXVECTOR3(right, top, 0.0f);  // Top right.
	vertices[4].tex = D3DXVECTOR2(1.0f, 0.0f);

	vertices[5].pos = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].tex = D3DXVECTOR2(1.0f, 1.0f);

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result)) return false;

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(m_vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	return true;
}


void Bitmap::RenderBuffers(ID3D11DeviceContext* deviceContext) {
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
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


bool Bitmap::LoadTexture(ID3D11Device* device, WCHAR* filename) {
	bool result;

	// Create the texture object.
	m_Texture = new Texture;
	if(!m_Texture) return false;

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if(!result) return false;

	return true;
}


void Bitmap::ReleaseTexture() {
	// Release the texture object.
	if(m_Texture) {
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}


D3DXMATRIX Bitmap::GetViewMatrix() {
	return m_baseViewMatrix;
}


D3DXVECTOR2 Bitmap::GetCenter() {
	return m_screen / 2 - m_size / 2;
}
