////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "model.h"


Model::Model() {
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_Model = 0;
}


Model::Model(const Model& other) { }


Model::~Model() { }


bool Model::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename) {
	bool result;

	std::string fn = modelFilename;

	// Load in the model data
	if(fn.substr(fn.find_last_of(".") + 1) == "obj") {
		result = LoadModelObj(modelFilename);
		if(!result) return false;
	} else if (fn.substr(fn.find_last_of(".") + 1) == "txt") {
		result = LoadModel(modelFilename);
		if(!result) return false;
	} else if (fn.substr(fn.find_last_of(".") + 1) == "bin") {
		result = LoadModelBin(modelFilename);
		if(!result) return false;
	}

	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	result = InitializeBuffers(device);
	if(!result) return false;

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if(!result) return false;

	return true;
}


void Model::Shutdown() {

	// Release the model texture.
	ReleaseTexture();

	// Release the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}


void Model::Render(ID3D11DeviceContext* deviceContext) {
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int Model::GetIndexCount() {
	return m_indexCount;
}


ID3D11ShaderResourceView* Model::GetTexture() {
	return m_Texture->GetTexture();
}


bool Model::InitializeBuffers(ID3D11Device* device) {
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices) return false;

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices) return false;

	// Load the vertex array and index array with data.
	for(i=0; i<m_vertexCount; i++) {
		vertices[i].pos = m_Model[i].pos;
		vertices[i].tex = m_Model[i].tex;
		vertices[i].nrm = m_Model[i].nrm;
		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
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
	if(FAILED(result)) return false;

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
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
	if(FAILED(result)) return false;

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}


void Model::ShutdownBuffers() {
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


void Model::RenderBuffers(ID3D11DeviceContext* deviceContext) {
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


bool Model::LoadTexture(ID3D11Device* device, WCHAR* filename) {
	bool result;

	// Create the texture object.
	m_Texture = new Texture;
	if(!m_Texture) return false;

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if(!result) return false;

	return true;
}


void Model::ReleaseTexture() {
	// Release the texture object.
	if(m_Texture) {
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}


bool Model::LoadModel(char* filename) {
	ifstream fin;
	char input;
	int i;

	// Open the model file.
	fin.open(filename);
	
	// If it could not open the file then exit.
	if(fin.fail()) return false;

	// Read up to the value of vertex count.
	fin.get(input);
	while(input != ':') fin.get(input);

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_Model = new VertexType[m_vertexCount];
	if(!m_Model) return false;

	// Read up to the beginning of the data.
	fin.get(input);
	while(input != ':') fin.get(input);
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for(i=0; i<m_vertexCount; i++) {
		fin >> m_Model[i].pos.x >> m_Model[i].pos.y >> m_Model[i].pos.z;
		fin >> m_Model[i].tex.x >> m_Model[i].tex.y;
		fin >> m_Model[i].nrm.x >> m_Model[i].nrm.y >> m_Model[i].nrm.z;
	}

	// Close the model file.
	fin.close();

	return true;
}


bool Model::LoadModelObj(char* filename) {
	ifstream fin;
	char input, input2;
 
	// Open the model file.
	fin.open(filename);
	   
	// If it could not open the file then exit.
	if(fin.fail()) return false;
 
	ArrayList<D3DXVECTOR3> *vertices = new ArrayList<D3DXVECTOR3>();
	ArrayList<D3DXVECTOR2> *vertexTextures = new ArrayList<D3DXVECTOR2>();
	ArrayList<D3DXVECTOR3> *vertexNormals = new ArrayList<D3DXVECTOR3>();

	struct VertexIndices {
		int vIndex;
		int tIndex;
		int nIndex;
	};
 
	ArrayList<VertexIndices> *vertexTotals = new ArrayList<VertexIndices>();
 
	fin.get(input);
	while(!fin.eof()) {
		if(input == 'v') {
			fin.get(input);
 
			// Read in the vertices.
			if(input == ' ') {
				D3DXVECTOR3 *vector = new D3DXVECTOR3;
				fin >> vector->x >> vector->y >> vector->z;
				vertices->add(vector);
				// Invert the Z vertex to change to left hand system.
				vertices->get(vertices->count()-1)->z = vertices->get(vertices->count()-1)->z * -1.0f;
			}
 
			// Read in the texture UV coordinates.
			if(input == 't') {
				D3DXVECTOR2 *vector = new D3DXVECTOR2;
				fin >> vector->x >> vector->y;
				vertexTextures->add(vector);
				// Invert the V texture coordinates to left hand system.
				vertexTextures->get(vertexTextures->count()-1)->y = vertexTextures->get(vertexTextures->count()-1)->y * -1.0f;
			}
 
			// Read in the normals.
			if(input == 'n') {
				D3DXVECTOR3 *vector = new D3DXVECTOR3;
				fin >> vector->x >> vector->y >> vector->z;
				vertexNormals->add(vector);
				// Invert the Z normal to change to left hand system.
				vertexNormals->get(vertexNormals->count()-1)->z = vertexNormals->get(vertexNormals->count()-1)->z * -1.0f;
			}
		}
 
		// Read in the faces.
		if(input == 'f') {
			fin.get(input);
			if(input == ' ') {
				// Read the face data in backwards to convert it to a left hand system from right hand system.
				VertexIndices *vertex1 = new VertexIndices(), *vertex2 = new VertexIndices(), *vertex3 = new VertexIndices();

				fin >> vertex3->vIndex >> input2 >> vertex3->tIndex >> input2 >> vertex3->nIndex
					>> vertex2->vIndex >> input2 >> vertex2->tIndex >> input2 >> vertex2->nIndex
					>> vertex1->vIndex >> input2 >> vertex1->tIndex >> input2 >> vertex1->nIndex;
 
				vertex1->vIndex--; vertex1->tIndex--; vertex1->nIndex--;
				vertex2->vIndex--; vertex2->tIndex--; vertex2->nIndex--;
				vertex3->vIndex--; vertex3->tIndex--; vertex3->nIndex--;
 
				vertexTotals->add(vertex1);
				vertexTotals->add(vertex2);
				vertexTotals->add(vertex3);
			}
		}
 
		// Read in the remainder of the line.
		while(input != '\n') fin.get(input);
 
		// Start reading the beginning of the next line.
		fin.get(input);
	}
 
	// Close the file.
	fin.close();
 
	// Now put all the stuff into m_model
	m_Model = new VertexType[vertexTotals->count()];
 
	for (int i = 0; i < vertexTotals->count(); i++) {
		m_Model[i].pos = D3DXVECTOR3(
			vertices->get(vertexTotals->get(i)->vIndex)->x,
			vertices->get(vertexTotals->get(i)->vIndex)->y,
			vertices->get(vertexTotals->get(i)->vIndex)->z
		);
		m_Model[i].tex = D3DXVECTOR2(
			vertexTextures->get(vertexTotals->get(i)->tIndex)->x,
			vertexTextures->get(vertexTotals->get(i)->tIndex)->y
		);
		m_Model[i].nrm = D3DXVECTOR3(
			vertexNormals->get(vertexTotals->get(i)->nIndex)->x,
			vertexNormals->get(vertexTotals->get(i)->nIndex)->y,
			vertexNormals->get(vertexTotals->get(i)->nIndex)->z
		);
	}
 
	m_vertexCount = vertexTotals->count();
	m_indexCount = m_vertexCount;
 
	return true;
}


bool Model::LoadModelBin(char* filename) {
	FILE* f;
	fopen_s(&f, filename, "rb");
	if(f == NULL) return false;

	fseek(f, 0, SEEK_END);
	long fileSize = ftell(f);
	fseek(f, 0, SEEK_SET);
	m_Model = new VertexType[fileSize/sizeof(VertexType)];

	fread(m_Model, sizeof(VertexType), fileSize/sizeof(VertexType), f);

	fclose(f);

	m_vertexCount = fileSize/sizeof(VertexType);
	m_indexCount = m_vertexCount;

	return true;
}


void Model::ReleaseModel() {
	if(m_Model) {
		delete [] m_Model;
		m_Model = 0;
	}

	return;
}