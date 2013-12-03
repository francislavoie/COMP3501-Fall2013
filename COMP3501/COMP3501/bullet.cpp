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


void Bullet::Update(Input* input, float time, State* turret) {

	if (input->IsMousePressed(MOUSE_LEFT)) {
		// TODO: Create new Bullet
		// Position should be current turret pos + rand(bulletOffsets)
		//State* state = new State(false);
		//state->SetPosition(*turret->GetPosition() + bulletOffsets[rand() % (sizeof(bulletOffsets) / sizeof(*bulletOffsets))]);
		//state->SetOrientation(turret->GetRotation());
		//m_bulletList.push_back(state);
	}

	for (State state : m_bulletList) {
		state.SetTime(time);
		
		// Move

		state.Update();
	}


	//m_tankState->ApplyForce(D3DXVECTOR3(0.0f, 0.0f, forward));
}