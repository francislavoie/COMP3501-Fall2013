////////////////////////////////////////////////////////////////////////////////
// Filename: tank.cpp
////////////////////////////////////////////////////////////////////////////////
#include "tank.h"


Tank::Tank() {
	baseTank = 0;
	turret = 0;
	frontRight = FRONTRIGHT;
	frontLeft = FRONTLEFT;
	rearLeft = REARLEFT;
	rearRight = REARRIGHT;
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
	m_tankState->SetFriction(0.005f);
	m_tankState->SetPosition(D3DXVECTOR3(3.0f,0.0f,3.0f));

	// Create turret state
	m_turretState = new State(false, m_tankState);
	if(!m_turretState) return false;

	m_turretState->SetOffset(D3DXVECTOR3(0.0f, 0.45f, 0.0f));

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

void Tank::Update(Input* input,float time, float rotation, bool firstPerson, QuadTree *m_QuadTree){
	m_tankState->SetTime(time);

	int scroll;
	input->GetWheelDelta(scroll);

	if(input->IsKeyPressed(DIK_W)){
		//D3DXVECTOR3 normalForward = D3DXVECTOR3(0,0,0);
		//D3DXVec3Normalize(&normalForward, m_tankState->getForward());
		m_tankState->applyForce(D3DXVECTOR3(0,0,0.0001f));
		//m_tankState->SetForwardVel(0.05f);
	} else if(input->IsKeyPressed(DIK_S)){
		//D3DXVECTOR3 normalForward = D3DXVECTOR3(0,0,0);
		//D3DXVec3Normalize(&normalForward, m_tankState->getForward());
		m_tankState->applyForce(D3DXVECTOR3(0,0,-0.0001f));
		//m_tankState->SetForwardVel(-0.05f);
	} else {
		//m_tankState->SetForwardVel(0.0f);
	}

	/*if(input->IsKeyPressed(DIK_A)) {
		m_tankState->SetStrafeVel(-0.05f);
	} else if(input->IsKeyPressed(DIK_D)) {
		m_tankState->SetStrafeVel(0.05f);
	} else {
		m_tankState->SetStrafeVel(0.0f);
	}*/
	
	if(input->IsKeyPressed(DIK_A)) {
		m_tankState->SetYaw(-0.05f);
	} else if(input->IsKeyPressed(DIK_D)) {
		m_tankState->SetYaw(0.05f);
	} else {
		m_tankState->SetYaw(0.0f);
	}

	m_turretState->SetYaw(rotation);

	D3DXQUATERNION quat = *m_tankState->GetRotation();
	D3DXQUATERNION inverse;
	D3DXQuaternionInverse(&inverse, &quat);
	D3DXQUATERNION temp;
	D3DXVECTOR3 output;
	float garbage;	

	temp	= D3DXQUATERNION(FRONTRIGHT.x,FRONTRIGHT.y,FRONTRIGHT.z,0);
	temp = quat * temp * inverse;
	D3DXQuaternionToAxisAngle(&temp, &output, &garbage);
	frontRight = *m_tankState->GetPosition() + output;

	temp = D3DXQUATERNION(FRONTLEFT.x,FRONTLEFT.y,FRONTLEFT.z,0);
	temp = quat * temp * inverse;
	D3DXQuaternionToAxisAngle(&temp, &output, &garbage);
	frontLeft = *m_tankState->GetPosition() + output;

	temp = D3DXQUATERNION(REARLEFT.x,REARLEFT.y,REARLEFT.z,0);
	temp = quat * temp * inverse;
	D3DXQuaternionToAxisAngle(&temp, &output, &garbage);
	rearLeft = *m_tankState->GetPosition() + output;

	temp = D3DXQUATERNION(REARRIGHT.x,REARRIGHT.y,REARRIGHT.z,0);
	temp = quat * temp * inverse;
	D3DXQuaternionToAxisAngle(&temp, &output, &garbage);
	rearRight = *m_tankState->GetPosition() + output;

	bool result;
	float height;
	D3DXVECTOR3 position = *getTankState()->GetPosition();

	// Get the height of the triangle that is directly underneath the given camera position.
	result =  m_QuadTree->GetHeightAtPosition(position.x, position.z, height);
	if(result) {
		// If there was a triangle under the camera then position the camera just above it by two units.
		getTankState()->SetPosition(D3DXVECTOR3(position.x, height + 2.0f, position.z));
	}
	/*
	result =  m_QuadTree->GetHeightAtPosition(frontRight.x, frontRight.z, height);
	if(result) {
		frontRight = D3DXVECTOR3(frontRight.x, height + 2.0f, frontRight.z);
	}
	else frontRight = D3DXVECTOR3(frontRight.x, 2.0f, frontRight.z);
	
	result =  m_QuadTree->GetHeightAtPosition(frontLeft.x, frontLeft.z, height);
	if(result) {
		frontLeft = D3DXVECTOR3(frontLeft.x, height + 2.0f, frontLeft.z);
	}
	else frontLeft = D3DXVECTOR3(frontLeft.x, 2.0f, frontLeft.z);

	result =  m_QuadTree->GetHeightAtPosition(rearLeft.x, rearLeft.z, height);
	if(result) {
		rearLeft = D3DXVECTOR3(rearLeft.x, height + 2.0f, rearLeft.z);
	}
	else rearLeft = D3DXVECTOR3(rearLeft.x, 2.0f, rearLeft.z);

	result =  m_QuadTree->GetHeightAtPosition(rearRight.x, rearRight.z, height);
	if(result) {
		rearRight = D3DXVECTOR3(rearRight.x, height + 2.0f, rearRight.z);
	}
	else rearRight = D3DXVECTOR3(rearRight.x, 2.0f, rearRight.z);*/
	int count = 4;
	D3DXVECTOR3 normal1,normal2,normal3,normal4;
	result =  m_QuadTree->GetHeightAtPosition2(frontRight.x, frontRight.z, height, normal1);
	if(!result) {
		normal1 = D3DXVECTOR3(0,0,0);
		count--;
	}

	result =  m_QuadTree->GetHeightAtPosition2(frontRight.x, frontRight.z, height, normal2);
	if(!result) {
		normal2 = D3DXVECTOR3(0,0,0); 
		count--;
	}

	result =  m_QuadTree->GetHeightAtPosition2(frontRight.x, frontRight.z, height, normal3);
	if(!result) {
		normal3 = D3DXVECTOR3(0,0,0);
		count--;
	}

	result =  m_QuadTree->GetHeightAtPosition2(frontRight.x, frontRight.z, height, normal4);
	if(!result) {
		normal4 = D3DXVECTOR3(0,0,0);
		count--;
	}
		
	//vector<D3DXVECTOR3> heights;
	//heights.push_back(*getTankState()->GetPosition());
	//heights.push_back(*getTankState()->GetPosition() - D3DXVECTOR3(0,getTankState()->GetPosition()->y,0) + FRONTRIGHT + D3DXVECTOR3(0,0.005f,0));
	//heights.push_back(*getTankState()->GetPosition() - D3DXVECTOR3(0,getTankState()->GetPosition()->y,0) + FRONTLEFT);
	//heights.push_back(*getTankState()->GetPosition() - D3DXVECTOR3(0,getTankState()->GetPosition()->y,0) + REARLEFT);
	//heights.push_back(*getTankState()->GetPosition() - D3DXVECTOR3(0,getTankState()->GetPosition()->y,0) + REARRIGHT);
	/*heights.push_back(frontRight);
	heights.push_back(frontLeft);
	heights.push_back(rearLeft);
	heights.push_back(rearRight);*/
	/*
	std::sort (heights.begin(), heights.end(), VectorSortP());

	D3DXVECTOR3 first = heights.back();
	heights.pop_back();
	
	D3DXVECTOR3 second = heights.back();
	heights.pop_back();

	D3DXVECTOR3 third = heights.back();
	heights.pop_back();

	D3DXVECTOR3 line1 = first - second;

	D3DXVECTOR3 line2 = third - second;

	D3DXVECTOR3 line3;
	D3DXVec3Cross(&line3, &line1, &line2);
	*/
	D3DXVECTOR3 line3;
	if (count != 0)
		line3 = (normal1+normal2+normal3+normal4)/float(count);
	else
		line3 = *m_tankState->getUp();

	float angle = acos(D3DXVec3Dot(&line3, m_tankState->getUp()));// assume normalized vectors /(D3DXVec3Length(&line3)*D3DXVec3Length(m_tankState->getUp())));
	angle /= 50;

	D3DXVECTOR3 cross;
	D3DXVec3Cross(&cross, &line3, m_tankState->getUp());

	D3DXVec3Normalize(&cross,&cross);

	D3DXQUATERNION quaternion;	
	D3DXQuaternionRotationAxis(&quaternion, &cross, -angle);
	m_tankState->SetOrientation(&quaternion);

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