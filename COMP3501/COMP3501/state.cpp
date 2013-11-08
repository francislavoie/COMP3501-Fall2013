////////////////////////////////////////////////////////////////////////////////
// Filename: positionclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "state.h"


State::State(bool rotvel_on, State* follow) {
	m_rotvel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posvel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXQuaternionIdentity(&m_rot);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_acceleration = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_friction = 0.0f;
	m_time = 0.0f;

	m_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_front = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	m_offset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_offsetRot = m_offset;

	m_follow = follow;

	m_rotvel_on = rotvel_on;
}


State::State(const State& other) { }


State::~State() { }


D3DXVECTOR3 State::GetRotVel() { return m_rotvel; }
D3DXVECTOR3 State::GetPosVel() { return m_posvel; }
D3DXVECTOR3 State::GetOffset() { return m_offset; }

D3DXVECTOR3 *State::GetPosition() { return &m_pos; }
D3DXQUATERNION *State::GetRotation() { return &m_rot; }



void State::SetOffset(D3DXVECTOR3 offset) { 
	m_offset = offset; 
	m_offsetRot = m_offset;
}

void State::SetTime(float t) { m_time = t; }

void State::SetYaw(float angle) { m_rotvel.x = angle; }
void State::SetPitch(float angle) { m_rotvel.y = angle; }
void State::SetRoll(float angle) { m_rotvel.z = angle; }

void State::SetStrafeVel(float vel) { m_posvel.x = vel; }
void State::SetClimbVel(float vel) { m_posvel.y = vel; }
void State::SetForwardVel(float vel) { m_posvel.z = vel; }

void State::SetOrientation(D3DXQUATERNION *orien) { 
	m_rot *= *orien;
}

void State::applyForce(D3DXVECTOR3 force) {
	m_acceleration += force;
}

void State::Update() {
	// TODO: Add decay speed
	m_acceleration -= m_posvel * m_friction;
	
	D3DXVECTOR3 prevVelocity = m_posvel;
	m_posvel += m_acceleration * m_time;

	// Apply positional velocity
	if(!m_follow) {
		m_pos += (m_right * prevVelocity.x) * m_time + (1/2) * (m_right * m_acceleration.x) * pow(m_time, 2);
		m_pos += (m_up * prevVelocity.y) * m_time + (1/2) * (m_right * m_acceleration.y) * pow(m_time, 2);
		m_pos += (m_front * prevVelocity.z) * m_time + (1/2) * (m_right * m_acceleration.z) * pow(m_time, 2);
	} else {
		D3DXQUATERNION inverse, temp; 
		D3DXQuaternionInverse(&inverse, m_follow->GetRotation());
		temp = *m_follow->GetRotation() * D3DXQUATERNION(m_offset.x, m_offset.y, m_offset.z, 0.0f) * inverse;
		m_offsetRot = D3DXVECTOR3(temp.x, temp.y, temp.z);
		m_pos = *m_follow->GetPosition() + m_offsetRot;
	}

	m_acceleration = D3DXVECTOR3(0,0,0);

	// Apply rotational velocity
	D3DXQUATERNION rot;
	D3DXQuaternionRotationYawPitchRoll(
		&rot, 
		m_rotvel.x, 
		m_rotvel.y, 
		m_rotvel.z
	);

	if(m_rotvel_on) m_rot *= rot;
	else m_rot = rot;

	if (m_follow)
	{
		m_rot *= *m_follow->GetRotation();
	}

	D3DXQuaternionNormalize(&m_rot, &m_rot);

	// Calculate up, front, left
	m_up = D3DXVECTOR3(
		2 * (m_rot.x * m_rot.y - m_rot.w * m_rot.z), 
		1 - 2 * (m_rot.x * m_rot.x + m_rot.z * m_rot.z),
		2 * (m_rot.y * m_rot.z + m_rot.w * m_rot.x)
	);

	m_front = D3DXVECTOR3(
		2 * (m_rot.x * m_rot.z + m_rot.w * m_rot.y), 
		2 * (m_rot.y * m_rot.z - m_rot.w * m_rot.x),
		1 - 2 * (m_rot.x * m_rot.x + m_rot.y * m_rot.y)
	);
	
	m_right = D3DXVECTOR3(
		1 - 2 * (m_rot.y * m_rot.y + m_rot.z * m_rot.z),
		2 * (m_rot.x * m_rot.y + m_rot.w * m_rot.z),
		2 * (m_rot.x * m_rot.z - m_rot.w * m_rot.y)
	);;
}
