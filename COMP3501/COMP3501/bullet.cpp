#include "bullet.h"

Bullet::Bullet() {
	
}

Bullet::Bullet(const Bullet& other) { }


Bullet::~Bullet() { }


bool Bullet::Initialize(D3D* m_D3D, HWND hwnd) {
	bool result;

	// Create the model object.
	m_bullet = new Model;
	if (!m_bullet) return false;

	// Initialize the model object.
	result = m_bullet->Initialize(m_D3D->GetDevice(), "data/bullet.bin", L"data/bulletrust.dds");
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void Bullet::Shutdown() {

	for (State* state : m_bulletList) {
		delete state;
		state = 0;
	}

	// Release the bullet object.
	if (m_bullet) {
		m_bullet->Shutdown();
		delete m_bullet;
		m_bullet = 0;
	}

	return;
}


void Bullet::Update(Input* input, float time, State* turret) {

	if (input->IsMousePressed(MOUSE_LEFT)) {
		State* state = new State(false);

		D3DXQUATERNION quat = *turret->GetRotation(), inverse, temp;
		D3DXQuaternionInverse(&inverse, &quat);
		float garbage;
		D3DXVECTOR3 output, offset = bulletOffsets[rand() % (sizeof(bulletOffsets) / sizeof(*bulletOffsets))];

		temp = quat * D3DXQUATERNION(offset.x, offset.y, offset.z, 0.0f) * inverse;
		D3DXQuaternionToAxisAngle(&temp, &output, &garbage);

		state->SetPosition(*turret->GetPosition() + output);
		state->SetOrientation(turret->GetRotation());
		state->SetFriction(0.0f);
		state->ApplyForce(D3DXVECTOR3(0.0f, 0.0f, 0.005f));
		m_bulletList.push_back(state);
	}

	//vector<State> removelist;

	for (State* state : m_bulletList) {
		state->SetTime(time);
		
		// Remove bullets outside of field
		//if (state.GetPosition()->x > 512.0f || state.GetPosition()->x < 0.0f || state.GetPosition()->z > 512.0f || state.GetPosition()->z < 0.0f)
		//	removelist.push_back(state);

		state->Update();
	}

	//set_difference(m_bulletList.begin(), m_bulletList.end(), removelist.begin(), removelist.end(), std::back_inserter(m_bulletList));
}

int Bullet::GetIndexCount() {
	return m_bullet->GetIndexCount();
}

void Bullet::Render(ID3D11DeviceContext* device) {
	m_bullet->Render(device);
	return;
}

void Bullet::removeBullets(vector<int>* removals)
{
	for (int i=removals->size() -1; i >-1; i--)
	{
		m_bulletList.erase(m_bulletList.begin()+(*removals)[i]);
	}
}