///////////////////////////////////////////////////////////////////////////////
// Filename: textclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "text.h"


Text::Text() {
	m_Font = 0;
	m_FontShader = 0;

	m_sentences = 0;
}

Text::Text(const Text& other) { }

Text::~Text() { }


bool Text::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, D3DXMATRIX baseViewMatrix, D3DXVECTOR2 screen, int count) {
	bool result;
	
	// Store the screen size.
	m_screen = screen;

	// Store the base view matrix.
	m_baseViewMatrix = baseViewMatrix;

	m_count = count;

	// Create the font object.
	m_Font = new Font;
	if(!m_Font) return false;

	// Initialize the font object.
	result = m_Font->Initialize(device, "data/fontdata.txt", L"data/font.dds");
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
		return false;
	}

	// Create the font shader object.
	m_FontShader = new FontShader;
	if(!m_FontShader) return false;

	// Initialize the font shader object.
	result = m_FontShader->Initialize(device, hwnd);
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	m_sentences = new SentenceType[m_count];

	for(int i = 0; i < m_count; i++) {
		result = InitializeSentence(&m_sentences[i], 32, device);
		if(!result) return false;
	}

	return true;
}


void Text::Shutdown() {
	// Release the first sentence.
	ReleaseSentences();

	// Release the font shader object.
	if(m_FontShader) {
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the font object.
	if(m_Font) {
		m_Font->Shutdown();
		delete m_Font;
		m_Font = 0;
	}

	return;
}


bool Text::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix) {
	bool result;

	for(int i = 0; i < m_count; i++) {
		// Draw the first sentence.
		result = RenderSentence(deviceContext, &m_sentences[i], worldMatrix, orthoMatrix);
		if(!result) return false;
	}

	return true;
}


bool Text::InitializeSentence(SentenceType* sentence, int maxLength, ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Initialize the sentence buffers to null.
	sentence->vertexBuffer = 0;
	sentence->indexBuffer = 0;

	// Set the maximum length of the sentence.
	sentence->maxLength = maxLength;

	// Set the number of vertices in the vertex array.
	sentence->vertexCount = 6 * maxLength;

	// Set the number of indexes in the index array.
	sentence->indexCount = sentence->vertexCount;

	// Create the vertex array.
	vertices = new VertexType[sentence->vertexCount];
	if(!vertices) return false;

	// Create the index array.
	indices = new unsigned long[sentence->indexCount];
	if(!indices) return false;

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * sentence->vertexCount));

	// Initialize the index array.
	for(i=0; i<sentence->indexCount; i++) {
		indices[i] = i;
	}

	// Set up the description of the dynamic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * sentence->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the sub resource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &sentence->vertexBuffer);
	if(FAILED(result)) return false;

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * sentence->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the sub resource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &sentence->indexBuffer);
	if(FAILED(result)) return false;

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	// Release the index array as it is no longer needed.
	delete [] indices;
	indices = 0;

	return true;
}


bool Text::UpdateSentence(SentenceType* sentence, char* text, D3DXVECTOR2 position, D3DXVECTOR4 color, ID3D11DeviceContext* deviceContext) {

	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;


	// Store the color of the sentence.
	sentence->color = color;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(text);

	// Check for possible buffer overflow.
	if(numLetters > sentence->maxLength) return false;

	// Create the vertex array.
	vertices = new VertexType[sentence->vertexCount];
	if(!vertices) return false;

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * sentence->vertexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((m_screen.x / 2) * -1) + position.x);
	drawY = (float)((m_screen.y / 2) - position.y);

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result)) return false;

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * sentence->vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(sentence->vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	return true;
}


void Text::ReleaseSentences() {
	for(int i = 0; i < m_count; i++) {
		if(&m_sentences[i]) {
			// Release the sentence vertex buffer.
			if(m_sentences[i].vertexBuffer) {
				m_sentences[i].vertexBuffer->Release();
				m_sentences[i].vertexBuffer = 0;
			}

			// Release the sentence index buffer.
			if(m_sentences[i].indexBuffer) {
				m_sentences[i].indexBuffer->Release();
				m_sentences[i].indexBuffer = 0;
			}
		}
	}

	delete [] m_sentences;

	return;
}


bool Text::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix) {
	unsigned int stride, offset;
	D3DXVECTOR4 pixelColor;
	bool result;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create a pixel color vector with the input sentence color.
	pixelColor = sentence->color;

	// Render the text using the font shader.
	result = m_FontShader->Render(deviceContext, sentence->indexCount, worldMatrix, m_baseViewMatrix, orthoMatrix, m_Font->GetTexture(), pixelColor);
	if(!result) false;

	return true;
}


bool Text::SetMousePosition(int mouseX, int mouseY, ID3D11DeviceContext* deviceContext) {
	char tempString[16];
	char mouseString[16];
	bool result;

	// Convert the mouseX integer to string format.
	_itoa_s(mouseX, tempString, 10);

	// Setup the mouseX string.
	strcpy_s(mouseString, "Mouse X: ");
	strcat_s(mouseString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(&m_sentences[1], mouseString, D3DXVECTOR2(20, 20), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), deviceContext);
	if(!result) return false;

	// Convert the mouseY integer to string format.
	_itoa_s(mouseY, tempString, 10);

	// Setup the mouseY string.
	strcpy_s(mouseString, "Mouse Y: ");
	strcat_s(mouseString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(&m_sentences[2], mouseString, D3DXVECTOR2(20, 40), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), deviceContext);
	if(!result) return false;

	return true;
}


bool Text::SetFps(int fps, int index, ID3D11DeviceContext* deviceContext) {
	char tempString[16];
	char fpsString[16];
	D3DXVECTOR4 color;
	bool result;
	
	// Truncate the fps to below 10,000.
	if(fps > 9999) fps = 9999;

	// Convert the fps integer to string format.
	_itoa_s(fps, tempString, 10);

	// Setup the fps string.
	strcpy_s(fpsString, "Fps: ");
	strcat_s(fpsString, tempString);

	// If fps is 60 or above set the fps color to green.
	if(fps >= 60) color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	// If fps is below 60 set the fps color to yellow.
	if(fps < 60) color = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);

	// If fps is below 30 set the fps color to red.
	if(fps < 30) color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(&m_sentences[index], fpsString, D3DXVECTOR2(20, 20), color, deviceContext);
	if(!result) return false;

	return true;
}


bool Text::SetCpu(int cpu, int index, ID3D11DeviceContext* deviceContext) {
	char tempString[16];
	char cpuString[16];
	bool result;

	// Convert the cpu integer to string format.
	_itoa_s(cpu, tempString, 10);

	// Setup the cpu string.
	strcpy_s(cpuString, "Cpu: ");
	strcat_s(cpuString, tempString);
	strcat_s(cpuString, "%");

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(&m_sentences[index], cpuString, D3DXVECTOR2(20, 40), D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f), deviceContext);
	if(!result) return false;

	return true;
}


bool Text::SetRenderCount(int count, int index, ID3D11DeviceContext* deviceContext) {
	char tempString[16];
	char countString[20];
	bool result;

	// Convert the cpu integer to string format.
	_itoa_s(count, tempString, 10);

	// Setup the cpu string.
	strcpy_s(countString, "Render Count: ");
	strcat_s(countString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(&m_sentences[index], countString, D3DXVECTOR2(20, 60), D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f), deviceContext);
	if(!result) return false;

	return true;
}


int Text::GetSentenceCount() {
	return m_count;
}