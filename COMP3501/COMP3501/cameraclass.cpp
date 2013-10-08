////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"


CameraClass::CameraClass() {
	m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXQuaternionIdentity(&m_quatOrientation);
	D3DXMatrixIdentity(&m_viewMatrix);

	upToDate = true;
}

CameraClass::CameraClass(D3DXVECTOR3 pos) {
	m_position = pos;

	D3DXQuaternionIdentity(&m_quatOrientation);
	D3DXMatrixIdentity(&m_viewMatrix);
	
	upToDate = false;
}


CameraClass::CameraClass(const CameraClass& other) { }


CameraClass::~CameraClass() { }


void CameraClass::SetPosition(float x, float y, float z) {
	m_position = D3DXVECTOR3(x, y, z);
	upToDate = false;
	return;
}


void CameraClass::SetPosition(D3DXVECTOR3 v) {
	m_position = v;
	upToDate = false;
	return;
}


void CameraClass::SetOrientation(D3DXQUATERNION quatOrient) {
	m_quatOrientation = quatOrient;
	upToDate = false;
	return;
}


void CameraClass::Update() {
	D3DXMATRIX matTranslation;

	D3DXMatrixTranslation(&matTranslation, -m_position.x, -m_position.y , -m_position.z);

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


void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix) {
	if(!upToDate) Update();
	viewMatrix = m_viewMatrix;
	return;
}


const D3DXVECTOR3 CameraClass::GetAxisX() const {	
	D3DXVECTOR3 vAxis;
	vAxis.x = m_viewMatrix._11;
	vAxis.y = m_viewMatrix._21;
	vAxis.z = m_viewMatrix._31;
	return vAxis;
}

const D3DXVECTOR3 CameraClass::GetAxisY() const {
	D3DXVECTOR3 vAxis;
	vAxis.x = m_viewMatrix._12;
	vAxis.y = m_viewMatrix._22;
	vAxis.z = m_viewMatrix._32;
	return vAxis;
}

const D3DXVECTOR3 CameraClass::GetAxisZ() const {
	D3DXVECTOR3 vAxis;
	vAxis.x = m_viewMatrix._13;
	vAxis.y = m_viewMatrix._23;
	vAxis.z = m_viewMatrix._33;
	return vAxis;
}

void CameraClass::ApplyTranslation(float fDistance , eDir ceDir) {
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

bool CameraClass::RotateXAxis(D3DXQUATERNION *pOrientation, float fAngle) { 
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


bool CameraClass::RotateYAxis(D3DXQUATERNION *pOrientation, float fAngle) { 
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


bool CameraClass::RotateZAxis(D3DXQUATERNION *pOrientation, float fAngle) { 
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

void CameraClass::ApplyRotate(float fAngle, eOrient oeOrient) {
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

D3DXVECTOR3* CameraClass::TransformVector(D3DXQUATERNION *pOrientation, D3DXVECTOR3 *pAxis) {
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