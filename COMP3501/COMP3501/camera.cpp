////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "camera.h"


Camera::Camera() {
	raiseDistance = 1.5f;
	theta = float(D3DX_PI* 3/2);
	m_lookatPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pitch = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	follow = 0;
	CalculatePosition();

	D3DXQuaternionIdentity(&m_quatOrientation);
	D3DXMatrixIdentity(&m_viewMatrix);
	
	minHeight = 0.5f;
	maxHeight = 2.5f;
	angle = float(D3DX_PI*3/8);
	upToDate = false;
	firstPerson = false;
}

Camera::Camera(D3DXVECTOR3 pos) {
	raiseDistance = 1.5f;
	theta = float(D3DX_PI* 3/2);
	m_lookatPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pitch = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	follow = 0;
	CalculatePosition();

	D3DXQuaternionIdentity(&m_quatOrientation);
	D3DXMatrixIdentity(&m_viewMatrix);
	theta = 0.0f;
	minHeight = 0.5f;
	maxHeight = 2.5f;
	angle = float(D3DX_PI*3/8);
	upToDate = false;
	firstPerson = false;
}


Camera::Camera(const Camera& other) { }


Camera::~Camera() { }


void Camera::SetPosition(float x, float y, float z) {
	//m_position = D3DXVECTOR3(x, y, z);
	upToDate = false;
	return;
}


void Camera::SetPosition(D3DXVECTOR3 v) {
	//m_position = v;
	upToDate = false;
	return;
}


void Camera::SetOrientation(D3DXQUATERNION quatOrient) {
	m_quatOrientation = quatOrient;
	upToDate = false;
	return;
}


void Camera::Update() {
	D3DXMATRIX matTranslation;

	CalculatePosition();
		
	D3DXMatrixTranslation(&matTranslation, -m_position.x, -m_position.y , -m_position.z);
	
	D3DXVECTOR3 v3Up, v3Right, v3LookAt = m_lookatPosition - m_position;

	D3DXVec3Normalize( &v3LookAt, &v3LookAt );
	D3DXVec3Cross( &v3Right, &D3DXVECTOR3(0,1,0), &v3LookAt );
	D3DXVec3Normalize( &v3Right, &v3Right );
	D3DXVec3Cross( &v3Up, &v3LookAt, &v3Right );
	D3DXVec3Normalize( &v3Up, &v3Up );

	D3DXMATRIX mRotation;
	D3DXMatrixIdentity( &mRotation );

	mRotation(0,0) = v3Right.x;    
	mRotation(0,1) = v3Up.x;
	mRotation(0,2) = v3LookAt.x;

	mRotation(1,0) = v3Right.y;
	mRotation(1,1) = v3Up.y;
	mRotation(1,2) = v3LookAt.y;

	mRotation(2,0) = v3Right.z;
	mRotation(2,1) = v3Up.z;
	mRotation(2,2) = v3LookAt.z;

	D3DXQuaternionRotationMatrix( &m_quatOrientation, &mRotation );

	// Calculate rotation by taking the conjugate of the quaternion
	D3DXMATRIX matRotation;
	D3DXMatrixRotationQuaternion(
		&matRotation, 
		&D3DXQUATERNION(
			-m_quatOrientation.x, 
			-m_quatOrientation.y, 
			-m_quatOrientation.z,
			m_quatOrientation.w
		)
	);

	// Apply rotation & translation matrix at view matrix
	D3DXMatrixMultiply(&m_viewMatrix, &matTranslation, &matRotation);

	upToDate = true;
}


void Camera::GetViewMatrix(D3DXMATRIX& viewMatrix) {
	if(!upToDate) Update();
	
	D3DXMatrixLookAtLH(&viewMatrix,&m_position,&m_lookatPosition,&D3DXVECTOR3(0,1,0));
	//viewMatrix = m_viewMatrix;
	return;
}


const D3DXVECTOR3 Camera::GetAxisX() const {	
	D3DXVECTOR3 vAxis;
	vAxis.x = m_viewMatrix._11;
	vAxis.y = m_viewMatrix._21;
	vAxis.z = m_viewMatrix._31;
	return vAxis;
}

const D3DXVECTOR3 Camera::GetAxisY() const {
	D3DXVECTOR3 vAxis;
	vAxis.x = m_viewMatrix._12;
	vAxis.y = m_viewMatrix._22;
	vAxis.z = m_viewMatrix._32;
	return vAxis;
}

const D3DXVECTOR3 Camera::GetAxisZ() const {
	D3DXVECTOR3 vAxis;
	vAxis.x = m_viewMatrix._13;
	vAxis.y = m_viewMatrix._23;
	vAxis.z = m_viewMatrix._33;
	return vAxis;
}

void Camera::ApplyTranslation(float fDistance , eDir ceDir) {
	D3DXVECTOR3 vDir;

	switch (ceDir) {
		case MOVE: {
			vDir = GetAxisZ();
			break;
		}
		case STRAFE: {
			vDir = GetAxisX();
			break;
		}
		case UPWARDS: {
			vDir = GetAxisY();
			break;
		}
	}
	
	m_position += vDir * fDistance * 10;

	upToDate = false;

	return;
}

bool Camera::RotateXAxis(D3DXQUATERNION *pOrientation, float fAngle) { 
	bool bSuccess = false;

	if(pOrientation) {
		D3DXQUATERNION Rotation;

		D3DXQuaternionRotationAxis(	
			&Rotation, 
			TransformVector(
				pOrientation, 
				&D3DXVECTOR3(1.0f, 0.0f, 0.0f)
			), fAngle);

		*pOrientation *= Rotation;

		bSuccess = true;
	}

	return bSuccess;
}


bool Camera::RotateYAxis(D3DXQUATERNION *pOrientation, float fAngle) { 
	bool bSuccess = false;

	if(pOrientation) {
		D3DXQUATERNION Rotation;

		D3DXQuaternionRotationAxis(
			&Rotation, 
			TransformVector(
				pOrientation, 
				&D3DXVECTOR3(0.0f, 1.0f, 0.0f)
			), fAngle);

		*pOrientation *= Rotation;

		bSuccess = true;
	}

	return bSuccess;
}


bool Camera::RotateZAxis(D3DXQUATERNION *pOrientation, float fAngle) { 
	bool bSuccess = false;

	if(pOrientation) {
		D3DXQUATERNION Rotation;
		
		D3DXQuaternionRotationAxis(	
			&Rotation, 
			TransformVector(
				pOrientation, 
				&D3DXVECTOR3(0.0f, 0.0f, 1.0f)
			), fAngle);

		*pOrientation *= Rotation;

		bSuccess = true;
	}

	return bSuccess;
}

void Camera::ApplyRotate(float fAngle, eOrient oeOrient) {
	switch( oeOrient ) {
		case PITCH: {
			RotateXAxis(&m_quatOrientation, fAngle);
			break;
		}
		case YAW: {
			RotateYAxis(&m_quatOrientation, fAngle);
			break;
		}
		case ROLL: {
			RotateZAxis(&m_quatOrientation, fAngle);
			break;
		}
	}

	D3DXQuaternionNormalize(&m_quatOrientation, &m_quatOrientation);

	upToDate = false;

	return;
}

D3DXVECTOR3* Camera::TransformVector(D3DXQUATERNION *pOrientation, D3DXVECTOR3 *pAxis) {
	D3DVECTOR vNewAxis;
	D3DXMATRIX matRotation;

	// Build a matrix from the quaternion.
	D3DXMatrixRotationQuaternion(&matRotation, pOrientation); 

	// Transform the queried axis vector by the matrix.
	vNewAxis.x = (pAxis->x * matRotation._11) + (pAxis->y * matRotation._21) + (pAxis->z * matRotation._31) + matRotation._41; 
	vNewAxis.y = (pAxis->x * matRotation._12) + (pAxis->y * matRotation._22) + (pAxis->z * matRotation._32) + matRotation._42;
	vNewAxis.z = (pAxis->x * matRotation._13) + (pAxis->y * matRotation._23) + (pAxis->z * matRotation._33) + matRotation._43;

	memcpy(pAxis, &vNewAxis, sizeof(vNewAxis)); // Copy axis.
	
	return pAxis;
}

void Camera::CalculatePosition()
{
	/*float radius = tan(angle)*raiseDistance;
	float x = cos(theta)*radius;
	float y = raiseDistance;
	float z = sin(theta)*radius;*/
	if (follow != 0)
	{
		m_position = *follow->GetForward() * -2.5f + *follow->GetPosition() + D3DXVECTOR3(0,0.75f,0);
		//D3DXVECTOR3 look;
		//D3DXVec3Normalize(&look, &D3DXVECTOR3(-x,0,-z)); 
		m_lookatPosition = *follow->GetPosition() + *follow->GetForward() * 50;//m_position + look*50;
	}
}

void Camera::Scroll(float scroll)
{
	raiseDistance -= scroll*0.001f;
	if (raiseDistance < minHeight)
		raiseDistance = minHeight;
	else if (raiseDistance > maxHeight)
		raiseDistance = maxHeight;
	//else
		//m_lookatPosition += D3DXVECTOR3(0, scroll*0.0002f, 0);
	upToDate = false;
}

void Camera::Rotate(float rot)
{
	theta += rot*0.01f;
	upToDate = false;
}

void Camera::lookUpDown(float rot)
{
	if (rot > 0 && pitch.y < 100) 
		pitch += D3DXVECTOR3(0,rot*0.35f,0);
	if (rot < 0 && pitch.y > -100) 
		pitch += D3DXVECTOR3(0,rot*0.35f,0);
	upToDate = false;
}

void Camera::setFollow(State* followState, float _minHeight, float _maxHeight)
{
	minHeight = _minHeight;
	maxHeight = _maxHeight;
	if(minHeight == maxHeight) angle = float(D3DX_PI);
	else angle = float(D3DX_PI*3/8);
	follow = followState;
}