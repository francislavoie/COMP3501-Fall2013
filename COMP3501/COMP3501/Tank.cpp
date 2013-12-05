////////////////////////////////////////////////////////////////////////////////
// Filename: tank.cpp
////////////////////////////////////////////////////////////////////////////////
#include "tank.h"
#include <iostream>
#include <fstream>


Tank::Tank() {
	baseTank = 0;
	turret = 0;
	m_frontRight = FRONTRIGHT;
	m_frontLeft = FRONTLEFT;
	m_rearLeft = REARLEFT;
	m_rearRight = REARRIGHT;
	turretLookAt = D3DXVECTOR3(0,0,0);
	yaw = 0;
	pitch = 0;
	forward = 0;
	turn = 0;
	moveSpeed = 0.005f;
	turnSpeed = 0.001f;
}


Tank::Tank(const Tank& other) { }


Tank::~Tank() { }


bool Tank::Initialize(D3D* m_D3D, HWND hwnd, QuadTree *m_QuadTree) {
	bool result;

	// Create the model object.
	baseTank = new Model;
	if(!baseTank) return false;

	// Initialize the model object.
	result = baseTank->Initialize(m_D3D->GetDevice(), "data/tank.bin", L"data/camo.dds");
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	turret = new Model;
	if(!turret) return false;

	// Initialize the model object.
	result = turret->Initialize(m_D3D->GetDevice(), "data/turret.bin", L"data/camo.dds");
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create tank state
	m_tankState = new State(true);
	if(!m_tankState) return false;
	m_tankState->SetFriction(0.005f);

	// Create turret state
	m_turretState = new State(false, m_tankState);
	if(!m_turretState) return false;
	
	float height;
	D3DXVECTOR3 vgarbage, temp = D3DXVECTOR3((rand() % 312) + 100.0f,0,(rand() % 312) + 100.0f);
	m_QuadTree->GetHeightAtPosition(temp.x, temp.z, height, vgarbage);
	m_tankState->SetPosition(D3DXVECTOR3(temp.x, height+2, temp.z));
	m_turretState->SetOffset(D3DXVECTOR3(0.0f, 0.22f, 0.0f));
	turretLookAt = *m_turretState->GetForward() * RADIUS;

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

void Tank::Update(Input* input,float time, float rotation, QuadTree *m_QuadTree){
	int mouseX, mouseY, deltaX, deltaY;
	input->GetMouseLocation(mouseX, mouseY);
	input->GetMouseDelta(deltaX, deltaY);
	
	m_tankState->SetTime(time);

	int scroll;
	input->GetWheelDelta(scroll);

	m_tankState->ApplyForce(D3DXVECTOR3(0.0f, 0.0f, forward));
	yaw += deltaX*0.01f;
	m_tankState->SetYaw(turn * time);
	yaw -= turn * time;

	forward = 0;
	turn    = 0;

	//m_turretState->SetYaw(deltaX*0.01f);
	
	/*D3DXVECTOR3 *forward = m_turretState->GetForward();*/
	if ((deltaY < 0 && pitch > -0.75) || (deltaY > 0 && pitch <0.75))
		pitch += deltaY*0.005f;
	
	D3DXVECTOR3 position = *getTankState()->GetPosition(), vgarbage;
	float height;	

	m_QuadTree->GetHeightAtPosition(position.x, position.z, height, vgarbage);
	float netforce = -0.00098f;
	float y = m_tankState->GetPosition()->y;
	if (y-(height+17)<0)
	{
		netforce -= (y-(height+17)) * 0.000065f;
	}

	m_tankState->ApplyForce(D3DXVECTOR3(0,netforce,0));

	//m_turretState->SetPitch(deltaY*0.01f);*/

	//m_turretState->SetYaw(rotation);

	D3DXQUATERNION quat = *m_tankState->GetRotation();
	D3DXQUATERNION inverse;
	D3DXQuaternionInverse(&inverse, &quat);
	D3DXQUATERNION temp;
	D3DXVECTOR3 output;
	
	float garbage;

	temp = quat * D3DXQUATERNION(FRONTRIGHT.x, FRONTRIGHT.y, FRONTRIGHT.z, 0.0f) * inverse;
	D3DXQuaternionToAxisAngle(&temp, &output, &garbage);
	m_frontRight = *m_tankState->GetPosition() + output;

	temp = quat * D3DXQUATERNION(FRONTLEFT.x, FRONTLEFT.y, FRONTLEFT.z, 0.0f) * inverse;
	D3DXQuaternionToAxisAngle(&temp, &output, &garbage);
	m_frontLeft = *m_tankState->GetPosition() + output;

	temp = quat * D3DXQUATERNION(REARLEFT.x, REARLEFT.y, REARLEFT.z, 0.0f) * inverse;
	D3DXQuaternionToAxisAngle(&temp, &output, &garbage);
	m_rearLeft = *m_tankState->GetPosition() + output;

	temp = quat * D3DXQUATERNION(REARRIGHT.x, REARRIGHT.y, REARRIGHT.z, 0.0f) * inverse;
	D3DXQuaternionToAxisAngle(&temp, &output, &garbage);
	m_rearRight = *m_tankState->GetPosition() + output;

	temp = quat * D3DXQUATERNION(CENTER.x, CENTER.y, CENTER.z, 0.0f) * inverse;
	D3DXQuaternionToAxisAngle(&temp, &output, &garbage);
	m_center = *m_tankState->GetPosition() + output;

	bool result;
	

	// Get the height of the triangle that is directly underneath the given tank position.
	result = m_QuadTree->GetHeightAtPosition(position.x, position.z, height, vgarbage);
	if(result) {
		// If there was a triangle under the tank then position the tank just above it by one unit.
		getTankState()->SetPosition(D3DXVECTOR3(position.x,m_tankState->GetPosition()->y, position.z));
	}

	int count = 5;
	D3DXVECTOR3 normal1, normal2, normal3, normal4, normal5;
	result = m_QuadTree->GetHeightAtPosition(m_frontRight.x, m_frontRight.z, height, normal1);
	if(!result) {
		normal1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		count--;
	}

	result = m_QuadTree->GetHeightAtPosition(m_frontLeft.x, m_frontLeft.z, height, normal2);
	if(!result) {
		normal2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		count--;
	}

	result = m_QuadTree->GetHeightAtPosition(m_rearRight.x, m_rearRight.z, height, normal3);
	if(!result) {
		normal3 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		count--;
	}

	result = m_QuadTree->GetHeightAtPosition(m_rearLeft.x, m_rearLeft.z, height, normal4);
	if(!result) {
		normal4 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		count--;
	}

	result = m_QuadTree->GetHeightAtPosition(m_center.x, m_center.z, height, normal5);
	if(!result) {
		normal5 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		count--;
	}
		
	D3DXVECTOR3 line3;
	if (count > 0)
		line3 = (normal1+normal2+normal3+normal4+normal5)/float(count);
	else
		line3 = *m_tankState->GetUp();

	float angle = acos(D3DXVec3Dot(&line3, m_tankState->GetUp()));// assume normalized vectors /(D3DXVec3Length(&line3)*D3DXVec3Length(m_tankState->getUp())));
	angle /= 15.0f;// * time;

	if (angle > 0.0175f)
	{
		D3DXVECTOR3 cross;
		D3DXVec3Cross(&cross, &line3, m_tankState->GetUp());

		D3DXVec3Normalize(&cross, &cross);

		D3DXQUATERNION quaternion;
		D3DXQuaternionRotationAxis(&quaternion, &cross, -angle);
		m_tankState->multiplyOrientation(&quaternion);
	}
	m_tankState->Update();




	D3DXQUATERNION orien;

	D3DXVECTOR3 slope, forward, turretProj;
	D3DXVec3Cross(&slope, m_tankState->GetUp(), &D3DXVECTOR3(0, 1, 0));
	D3DXVec3Normalize(&slope, &slope);
	D3DXVec3Cross(&forward, m_tankState->GetUp(), &slope);
	D3DXVec3Normalize(&forward, &forward);
	D3DXVec3Cross(&turretProj, m_tankState->GetUp(), m_turretState->GetForward());
	D3DXVec3Normalize(&turretProj, &turretProj);
	D3DXVec3Cross(&turretProj, m_tankState->GetUp(), &turretProj);
	D3DXVec3Normalize(&turretProj, &turretProj);

	float projangle = acos(abs(D3DXVec3Dot(&turretProj, &forward)));
	if (D3DXVec3Dot(&turretProj, &forward) < 0) projangle = float(D3DX_PI - projangle);

	float slopeangle = acos(abs(D3DXVec3Dot(&D3DXVECTOR3(0, 1, 0), &forward)));
	slopeangle = float(D3DX_PI/2 - slopeangle);

	float pitchOffset = (1 - cos(projangle)) * slopeangle;

	D3DXQuaternionRotationYawPitchRoll(&orien, yaw, pitch - pitchOffset, 0);
	orien = orien * *m_tankState->GetRotation();
	m_turretState->SetOrientation(&orien);
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

void Tank::checknResolveTankCollision(Tank* other)
{
	static int collisions = 0;
	State *otherState = other->getTankState();
	D3DXVECTOR3 plane = *otherState->GetPosition()-*m_tankState->GetPosition();
	float distance = D3DXVec3Length(&plane);
	if (distance < 2)
	{
		collisions++;
		ofstream myfile;
		myfile.open("Debug.txt", ios::out | ios::app);
		D3DXVECTOR3 thisvel,othervel,thisafter,otherafter, resultnorm, thisperp, otherperp;

		D3DXVec3Normalize(&plane,&plane);
		thisvel = m_tankState->GetPosVel();
		othervel = otherState->GetPosVel();



		thisperp = plane * D3DXVec3Dot(&plane,&thisvel);
		//thispar = thisvel - thisperp;

		otherperp = plane * D3DXVec3Dot(&plane,&othervel);
		//otherpar = othervel - otherperp;

		//result = (thisperp-otherperp) * 0.5f;
		//float ratio;
		//ratio = D3DXVec3Length(&thisvel)/(D3DXVec3Length(&thisvel)+D3DXVec3Length(&othervel));

		//D3DXVec3Normalize(&resultnorm,&result);
		//D3DXVec3Normalize(&thisnorm,&thisvel);
		//D3DXVec3Normalize(&othernorm,&othervel);

		//m_tankState->AddtoPosition(distance * ratio * (resultnorm - thisnorm));//newVelocity - distance1 * sphere->getVelocity());
		//otherState->AddtoPosition(distance * (1-ratio) * (-resultnorm + othernorm));//newVelocity - distance1 * sphere->getVelocity());'

		//m_tankState->SetPosVel(thispar + otherperp);
		//other->getTankState()->SetPosVel(otherpar + thisperp);
		thisafter = thisvel + D3DXVec3Dot(&(othervel-thisvel),&plane) * plane;
		otherafter = othervel + D3DXVec3Dot(&(thisvel-othervel),&plane) * plane;
		m_tankState->SetPosVel(thisafter);
		other->getTankState()->SetPosVel(otherafter);

		myfile << "Collision #" << collisions << endl;
		myfile << "thisvel: " << thisvel.x << "," << thisvel.y << "," << thisvel.z << endl;
		myfile << "othervel: " << othervel.x << "," << othervel.y << "," << othervel.z << endl;
		myfile << "thisafter: " << thisafter.x << "," << thisafter.y << "," << thisafter.z << endl;
		myfile << "otherafter: " << otherafter.x << "," << otherafter.y << "," << otherafter.z << endl<<endl;


		//D3DXVec3Normalize(&thisnorm,&thisvel);
		//D3DXVec3Normalize(&othernorm,&othervel);
		//D3DXVec3Normalize(&resultnorm,&result);

		//float ratio = D3DXVec3Length(&thisvel)/(D3DXVec3Length(&thisvel)+D3DXVec3Length(&othervel));

		//m_tankState->AddtoPosition((resultnorm - thisnorm) * distance * ratio);
		//otherState->AddtoPosition((othernorm - resultnorm) * distance * (1-ratio));
		myfile.close();
	}
}