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
	m_Bullet = 0;
	turretLookAt = D3DXVECTOR3(0,0,0);
	yaw = 0;
	pitch = 0;
	forward = 0;
	turn = 0;
	moveSpeed = 0.0001f;
	turnSpeed = 0.004f;
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

	m_Bullet = new Bullet;
	if (!m_Bullet) return false;

	result = m_Bullet->Initialize(m_D3D, hwnd);
	if (!result) return false;

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

	// Release the tank object.
	if (m_Bullet) {
		m_Bullet->Shutdown();
		delete m_Bullet;
		m_Bullet = 0;
	}

	return;
}

void Tank::orientTurret(float deltaX,float deltaY, float time)
{
	float anglelimit = acos(D3DXVec3Dot(&D3DXVECTOR3(0,1,0), m_tankState->GetUp()));

	yaw += deltaX*0.01f;
	if ((deltaY < 0 && pitch > -0.60 + anglelimit) || (deltaY > 0 && pitch <0.60 + anglelimit))
		pitch += deltaY*0.005f;
}

void Tank::Update(Input* input, float time, QuadTree *m_QuadTree){
	int deltaX, deltaY;
	input->GetMouseDelta(deltaX, deltaY);
	
	m_tankState->SetTime(time);
	
	m_tankState->ApplyForce(D3DXVECTOR3(0.0f, 0.0f, forward));
	forward = 0;

	m_tankState->SetYaw(turn * time);
	yaw -= turn * time;	
	turn    = 0;

	
	D3DXVECTOR3 position = *getTankState()->GetPosition(), vgarbage, normal5;
	float height;	

	m_QuadTree->GetHeightAtPosition(position.x, position.z, height, normal5);
	float netforce = -0.00098f;
	float y = m_tankState->GetPosition()->y;
	if (y-(height+17) < 0) {
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

	// Get the height of the triangle that is directly underneath the given tank position.
	//result = m_QuadTree->GetHeightAtPosition(position.x, position.z, height, vgarbage);
	//if(result) {
		// If there was a triangle under the tank then position the tank just above it by one unit.
	//	getTankState()->SetPosition(D3DXVECTOR3(position.x,m_tankState->GetPosition()->y, position.z));
	//}

	int count = 5;
	//D3DXVECTOR3 normal1, normal2, normal3, normal4, normal5;
	//result = m_QuadTree->GetHeightAtPosition(m_frontRight.x, m_frontRight.z, height, normal1);
	//if(!result) {
	//	normal1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//	count--;
	//}

	//result = m_QuadTree->GetHeightAtPosition(m_frontLeft.x, m_frontLeft.z, height, normal2);
	//if(!result) {
	//	normal2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//	count--;
	//}

	//result = m_QuadTree->GetHeightAtPosition(m_rearRight.x, m_rearRight.z, height, normal3);
	//if(!result) {
	//	normal3 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//	count--;
	//}

	//result = m_QuadTree->GetHeightAtPosition(m_rearLeft.x, m_rearLeft.z, height, normal4);
	//if(!result) {
	//	normal4 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//	count--;
	//}

	//result = m_QuadTree->GetHeightAtPosition(m_center.x, m_center.z, height, normal5);
	//if(!result) {
	//	normal5 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//	count--;
	//}
		
	D3DXVECTOR3 line3;
	if (count > 0)
		line3 = normal5;
		//line3 = (normal1+normal2+normal3+normal4+normal5)/float(count);
	else
		line3 = *m_tankState->GetUp();

	float angle = acos(D3DXVec3Dot(&line3, m_tankState->GetUp()));// assume normalized vectors /(D3DXVec3Length(&line3)*D3DXVec3Length(m_tankState->getUp())));
	angle /= 15.0f;// * time;

	//if (angle > 0.015f)
	//{
		D3DXVECTOR3 cross;
		D3DXVec3Cross(&cross, &line3, m_tankState->GetUp());

		D3DXVec3Normalize(&cross, &cross);

		D3DXQUATERNION quaternion;
		D3DXQuaternionRotationAxis(&quaternion, &cross, -angle);
		m_tankState->multiplyOrientation(&quaternion);
	//}
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

	m_Bullet->Update(time);
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

bool Tank::checknResolveTankCollision(Tank* other, D3DXVECTOR3& location)
{
	static int collisions = 0;
	float mindistance = 1.75f;
	State *otherState = other->getTankState();
	D3DXVECTOR3 plane = *otherState->GetPosition()-*m_tankState->GetPosition();
	D3DXVECTOR3 plane2 = (*otherState->GetPosition()+*otherState->GetForward()*0.75f)-(*m_tankState->GetPosition() +  *m_tankState->GetForward()*0.75f);
	float distance = min(D3DXVec3Length(&plane),D3DXVec3Length(&plane2));
	if (distance < mindistance)
	{
		collisions++;
		//ofstream myfile;
		//myfile.open("Debug.txt", ios::out | ios::app);
		D3DXVECTOR3 thisvel,othervel,thisafter,otherafter, thisperp, otherperp;

		D3DXVec3Normalize(&plane,&plane);
		thisvel = m_tankState->GetPosVel();
		othervel = otherState->GetPosVel();

		thisvel = thisvel.x * *m_tankState->GetRight() + thisvel.y * *m_tankState->GetUp() + thisvel.z * *m_tankState->GetForward();
		othervel = othervel.x * *otherState->GetRight() + othervel.y * *otherState->GetUp() + othervel.z * *otherState->GetForward();


		thisperp = plane * D3DXVec3Dot(&plane,&thisvel);
		//thispar = thisvel - thisperp;

		otherperp = plane * D3DXVec3Dot(&plane,&othervel);
		//otherpar = othervel - otherperp;

		//result = (thisperp-otherperp) * 0.5f;

		thisafter = thisvel + D3DXVec3Dot(&(othervel-thisvel),&plane) * plane;
		otherafter = othervel + D3DXVec3Dot(&(thisvel-othervel),&plane) * plane;

		D3DXVECTOR3 temp3,temp4, thisposplus,otherposplus;


		D3DXVECTOR3 thisnorm,othernorm,thisafternorm,otherafternorm;
		float ratio;
		ratio = D3DXVec3Length(&thisvel)/(D3DXVec3Length(&thisvel)+D3DXVec3Length(&othervel));

		D3DXVec3Normalize(&thisafternorm,&thisafter);
		D3DXVec3Normalize(&otherafternorm,&otherafter);
		D3DXVec3Normalize(&thisnorm,&thisvel);
		D3DXVec3Normalize(&othernorm,&othervel);

		thisposplus = (mindistance - distance) * ratio * (thisafternorm - thisnorm);
		otherposplus = (mindistance - distance) * (1.0f - ratio) * (otherafternorm - othernorm);

	/*	temp3 = thisposplus.x * *m_tankState->GetRight();
		temp3 += thisposplus.y * *m_tankState->GetUp();
		temp3 += thisposplus.z * *m_tankState->GetForward();

		temp4 = otherposplus.x * *otherState->GetRight();
		temp4 += otherposplus.y * *otherState->GetUp();
		temp4 += otherposplus.z * *otherState->GetForward();*/

		m_tankState->AddtoPosition( thisposplus);
		otherState->AddtoPosition(otherposplus);

		//m_tankState->SetPosVel(thispar + otherperp);
		//other->getTankState()->SetPosVel(otherpar + thisperp);
		
		m_tankState->SetPosVel(thisafter);
		other->getTankState()->SetPosVel(otherafter);

		//myfile << "Collision #" << collisions << endl;
		//myfile << "thisvel: " << thisvel.x << "," << thisvel.y << "," << thisvel.z << endl;
		//myfile << "othervel: " << othervel.x << "," << othervel.y << "," << othervel.z << endl;
		//myfile << "thisafter: " << thisafter.x << "," << thisafter.y << "," << thisafter.z << endl;
		//myfile << "otherafter: " << otherafter.x << "," << otherafter.y << "," << otherafter.z << endl<<endl;

		//D3DXVec3Normalize(&thisnorm,&thisvel);
		//D3DXVec3Normalize(&othernorm,&othervel);
		//D3DXVec3Normalize(&resultnorm,&result);

		//float ratio = D3DXVec3Length(&thisvel)/(D3DXVec3Length(&thisvel)+D3DXVec3Length(&othervel));

		//m_tankState->AddtoPosition((resultnorm - thisnorm) * distance * ratio);
		//otherState->AddtoPosition((othernorm - resultnorm) * distance * (1-ratio));
		//myfile.close();

		location = *other->getTankState()->GetPosition();

		return true;
	}
	return false;
}

bool Tank::checknResolveBulletCollision(Bullet* other, D3DXVECTOR3& location)
{
	vector<State*> *bullets = other->GetBullets();
	vector<int> *collisions = new vector<int>();

	bool hascollision = false;
	
	for (unsigned int i = 0; i < bullets->size(); i++)
	{
		State *otherState = (*bullets)[i];

		D3DXVECTOR3 plane = *otherState->GetPosition()-*m_tankState->GetPosition();
		D3DXVECTOR3 plane2 = *otherState->GetPosition()-(*m_tankState->GetPosition() +  *m_tankState->GetForward()*0.75f);
		float distance = min(D3DXVec3Length(&plane),D3DXVec3Length(&plane2));
		if (distance < 1)
		{
			collisions->push_back(i);
			D3DXVECTOR3 thisvel,othervel,thisafter,otherafter, thisperp, otherperp;

			D3DXVec3Normalize(&plane,&plane);
			thisvel = m_tankState->GetPosVel();
			othervel = otherState->GetPosVel();

			thisvel = thisvel.x * *m_tankState->GetRight() + thisvel.y * *m_tankState->GetUp() + thisvel.z * *m_tankState->GetForward();
			othervel = othervel.x * *otherState->GetRight() + othervel.y * *otherState->GetUp() + othervel.z * *otherState->GetForward();


			thisperp = plane * D3DXVec3Dot(&plane,&thisvel);
			//thispar = thisvel - thisperp;

			otherperp = plane * D3DXVec3Dot(&plane,&othervel);
			//otherpar = othervel - otherperp;

			//result = (thisperp-otherperp) * 0.5f;

			thisafter = thisvel + D3DXVec3Dot(&(othervel+thisvel),&plane) * plane;
			//otherafter = othervel + D3DXVec3Dot(&(thisvel-othervel),&plane) * plane;

			thisafter.y = 0;	
			thisafter = thisafter * 0.1f;
		
			m_tankState->SetPosVel(thisafter);
			otherState->SetPosVel(D3DXVECTOR3(0,0,0));

			location = *otherState->GetPosition();

			hascollision = true;
		}
	}
	other->removeBullets(collisions);
	return hascollision;
}

void Tank::checknResolveStaticCollision(ModelList *model)
{
	for (int i=0; i< model->GetModelCount(); i++)
	{
		D3DXVECTOR3 otherPosition = model->GetModelPosition(i);

		D3DXVECTOR3 plane = otherPosition-*m_tankState->GetPosition();
		float distance = D3DXVec3Length(&plane);
		if (distance < 6)
		{
			D3DXVECTOR3 thisvel,thisafter, thisperp;

			D3DXVec3Normalize(&plane,&plane);
			thisvel = m_tankState->GetPosVel();

			thisvel = thisvel.x * *m_tankState->GetRight() + thisvel.y * *m_tankState->GetUp() + thisvel.z * *m_tankState->GetForward();


			thisperp = plane * D3DXVec3Dot(&plane,&thisvel);


			thisafter = thisvel - (2*thisperp);

			D3DXVECTOR3 normal1,normal2;
			D3DXVec3Normalize(&normal1,&thisafter);
			D3DXVec3Normalize(&normal2,&thisvel);

			m_tankState->AddtoPosition((6-distance) * (normal1 - normal2));
		
			m_tankState->SetPosVel(thisafter);
		}
		
	}
}