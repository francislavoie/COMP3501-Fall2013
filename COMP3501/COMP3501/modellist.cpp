///////////////////////////////////////////////////////////////////////////////
// Filename: modellistclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "modellist.h"

ModelList::ModelList() {
	m_ModelInfoList = 0;
}


ModelList::ModelList(const ModelList& other) { }


ModelList::~ModelList() { }


bool ModelList::Initialize(int numModels, QuadTree* quadTree) {
	int i;
	float red, green, blue;
	float height;
	D3DXVECTOR3 garbage;

	// Store the number of models.
	m_modelCount = numModels;

	// Create a list array of the model information.
	m_ModelInfoList = new ModelInfoType[m_modelCount];
	if(!m_ModelInfoList) return false;

	// Seed the random generator with the current time.
	srand((unsigned int)time(NULL));

	// Go through all the models and randomly generate the model color and position.
	for(i=0; i<m_modelCount; i++) {
		// Generate a random color for the model.
		red = (float)rand() / RAND_MAX / 2.0f + 0.5f;
		green = (float)rand() / RAND_MAX / 2.0f + 0.5f;
		blue = (float)rand() / RAND_MAX / 2.0f + 0.5f;

		m_ModelInfoList[i].color = D3DXVECTOR4(red, green, blue, 1.0f);

		// Generate a random position in front of the viewer for the mode.
		m_ModelInfoList[i].position.x = (float)(rand() % 512);
		m_ModelInfoList[i].position.z = (float)(rand() % 512);
		
		quadTree->GetHeightAtPosition(m_ModelInfoList[i].position.x, m_ModelInfoList[i].position.z, height, garbage);

		m_ModelInfoList[i].position.y = height;
		
		//m_ModelInfoList[i].velocity.x = ((float)(rand() % 10) / 50.0f) - 0.1f;
		//m_ModelInfoList[i].velocity.y = ((float)(rand() % 10) / 50.0f) - 0.1f;
		//m_ModelInfoList[i].velocity.z = ((float)(rand() % 10) / 50.0f) - 0.1f;

		//m_ModelInfoList[i].velocity.x = (((float)rand()-(float)rand())/RAND_MAX) * 0.005f;
		//m_ModelInfoList[i].velocity.y = (((float)rand()-(float)rand())/RAND_MAX) * 0.005f;
		//m_ModelInfoList[i].velocity.z = (((float)rand()-(float)rand())/RAND_MAX) * 0.005f;

		D3DXQuaternionRotationYawPitchRoll(
			&m_ModelInfoList[i].rotation,
			float((rand()) / (RAND_MAX / (D3DX_PI * 2))),
			float((rand()) / (RAND_MAX / (D3DX_PI * 2))),
			float((rand()) / (RAND_MAX / (D3DX_PI * 2)))
		);

		D3DXQuaternionNormalize(&m_ModelInfoList[i].rotation, &m_ModelInfoList[i].rotation);

		m_ModelInfoList[i].velocity.x = 0.0f;
		m_ModelInfoList[i].velocity.y = 0.0f;
		m_ModelInfoList[i].velocity.z = 0.0f;

		m_ModelInfoList[i].visible = true;

		m_ModelInfoList[i].type = rand() % 1;
	}

	return true;
}


void ModelList::Shutdown() {
	// Release the model information list.
	if(m_ModelInfoList) {
		delete [] m_ModelInfoList;
		m_ModelInfoList = 0;
	}

	return;
}


int ModelList::GetModelCount() {
	return m_modelCount;
}

D3DXVECTOR3 ModelList::GetModelPosition(int index)
{
	return m_ModelInfoList[index].position;
}

void ModelList::GetData(int index, D3DXVECTOR3& position, D3DXVECTOR4& color, D3DXQUATERNION& rotation, bool& visible, int& type, float time) {
	position = m_ModelInfoList[index].position;
	color = m_ModelInfoList[index].color;
	visible = m_ModelInfoList[index].visible;

	D3DXQUATERNION rot;

	D3DXQuaternionRotationYawPitchRoll(
		&rot, 
		m_ModelInfoList[index].velocity.x * time, 
		m_ModelInfoList[index].velocity.y * time, 
		m_ModelInfoList[index].velocity.z * time
	);

	m_ModelInfoList[index].rotation *= rot;

	D3DXQuaternionNormalize(&m_ModelInfoList[index].rotation, &m_ModelInfoList[index].rotation);

	rotation = m_ModelInfoList[index].rotation;

	type = m_ModelInfoList[index].type;

	return;
}


void ModelList::Hide(int index) {
	m_ModelInfoList[index].visible = false;
}


float ModelList::GetDistance(int index, D3DXVECTOR3& other) {
	D3DXVECTOR3 diff = m_ModelInfoList[index].position - other;

	return sqrt(diff.x*diff.x + diff.y*diff.y + diff.z*diff.z);
}