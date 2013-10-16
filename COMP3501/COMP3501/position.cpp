////////////////////////////////////////////////////////////////////////////////
// Filename: positionclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "position.h"


Position::Position() {
	m_rotvel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posvel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXQUATERNION(1.0f, 0.0f, 0.0f, 90.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_time = 0.0f;

	m_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_front = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
}


Position::Position(const Position& other) { }


Position::~Position() { }


D3DXVECTOR3 Position::GetRotVel() { return m_rotvel; }
D3DXVECTOR3 Position::GetPosVel() { return m_posvel; }
D3DXVECTOR3 Position::GetPosition() { return m_pos; }
D3DXQUATERNION Position::GetRotation() { return m_rot; }


void Position::SetTime(float t) { m_time = t; }

void Position::SetYaw(float angle) { m_rotvel.x = angle; }
void Position::SetPitch(float angle) { m_rotvel.y = angle; }
void Position::SetRoll(float angle) { m_rotvel.z = angle; }

void Position::SetStrafe(float vel) { m_posvel.x = vel; }
void Position::SetClimb(float vel) { m_posvel.y = vel; }
void Position::SetForward(float vel) { m_posvel.z = vel; }


void Position::Update() {
	// TODO: Add decay speed
	// D3DXVECTOR3 decay;
	// D3DXVec3Normalize(&decay, &m_posvel);
	// m_posvel -= m_decayRate * decay;
	
	// Apply positional velocity
	m_pos += (m_right * m_posvel.x) * m_time;
	m_pos += (m_up * m_posvel.y) * m_time;
	m_pos += (m_front * m_posvel.z) * m_time;

	// Apply rotational velocity
	D3DXQUATERNION rot;
	D3DXQuaternionRotationYawPitchRoll(
		&rot, 
		m_rotvel.x, 
		m_rotvel.y, 
		m_rotvel.z
	);
	m_rot *= rot;
	D3DXQuaternionNormalize(&m_rot, &m_rot);

	// Calculate up, front, left
	D3DXQUATERNION conj, temp;
	D3DXQuaternionConjugate(&conj, &m_rot);

	temp = m_rot * D3DXQUATERNION(0.0f, 1.0f, 0.0f, 0.0f) * conj;
	m_up = D3DXVECTOR3(temp.x, temp.y, temp.z);

	temp = m_rot * D3DXQUATERNION(0.0f, 0.0f, 1.0f, 0.0f) * conj;
	m_front = D3DXVECTOR3(temp.x, temp.y, temp.z);

	temp = m_rot * D3DXQUATERNION(1.0f, 0.0f, 0.0f, 0.0f) * conj;
	m_right = D3DXVECTOR3(temp.x, temp.y, temp.z);
}