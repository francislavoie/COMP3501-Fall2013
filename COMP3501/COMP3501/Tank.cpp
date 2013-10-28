////////////////////////////////////////////////////////////////////////////////
// Filename: tank.cpp
////////////////////////////////////////////////////////////////////////////////
#include "tank.h"


Tank::Tank() {
	baseTank = 0;
	turret = 0;
}


Tank::Tank(const Tank& other) { }


Tank::~Tank() { }


bool Tank::Initialize(D3D* m_D3D, HWND hwnd) {
	bool result;

	// Create the model object.
	baseTank = new Model;
	if(!baseTank) return false;

	// Initialize the model object.
	result = baseTank->Initialize(m_D3D->GetDevice(), "data/tank.obj", L"data/bulletrust.dds");
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}


	// Create the model object.
	turret = new Model;
	if(!turret) return false;

	// Initialize the model object.
	result = turret->Initialize(m_D3D->GetDevice(), "data/turret.obj", L"data/bulletrust.dds");
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create tank state
	m_tankState = new State(true);
	if(!m_tankState) return false;

	// Create turret state
	m_turretState = new State(false, m_tankState);
	if(!m_turretState) return false;

	m_turretState->SetOffset(D3DXVECTOR3(0.0f, 0.48f, 0.0f));

	return true;
}


void Tank::Shutdown() {

	// Release the state object.
	if(m_tankState) {
		delete m_tankState;
		m_tankState = 0;
	}

	// Release the state object.
	if(m_turretState) {
		delete m_turretState;
		m_turretState = 0;
	}

	// Release the Tank object.
	if(baseTank) {
		baseTank->Shutdown();
		delete baseTank;
		baseTank = 0;
	}

	// Release the turret object.
	if(turret) {
		turret->Shutdown();
		delete turret;
		turret = 0;
	}	

	return;
}

void Tank::Update(Input* input,float time, float rotation){
	m_tankState->SetTime(time);

	int scroll;
	input->GetWheelDelta(scroll);

	if(input->IsKeyPressed(DIK_W)){
		m_tankState->SetForwardVel(0.05f);
	} else if(input->IsKeyPressed(DIK_S)){
		m_tankState->SetForwardVel(-0.05f);
	} else {
		m_tankState->SetForwardVel(0.0f);
	}

	if(input->IsKeyPressed(DIK_A)) {
		m_tankState->SetStrafeVel(-0.05f);
	} else if(input->IsKeyPressed(DIK_D)) {
		m_tankState->SetStrafeVel(0.05f);
	} else {
		m_tankState->SetStrafeVel(0.0f);
	}

	if(input->IsKeyPressed(DIK_Q)) {
		m_tankState->SetYaw(-0.05f);
	} else if(input->IsKeyPressed(DIK_E)) {
		m_tankState->SetYaw(0.05f);
	} else {
		m_tankState->SetYaw(0.0f);
	}

	m_turretState->SetYaw(rotation);

	m_tankState->Update();
	m_turretState->Update();
}


void Tank::RenderTank(ID3D11DeviceContext* device) {
	baseTank->Render(device);

	return;
}

void Tank::RenderTurret(ID3D11DeviceContext* device) {
	turret->Render(device);

	return;
}


int Tank::GetTankIndexCount() {
	return baseTank->GetIndexCount();
}

int Tank::GetTurretIndexCount() {
	return turret->GetIndexCount();
}