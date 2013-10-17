////////////////////////////////////////////////////////////////////////////////
// Filename: positionclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _POSITIONCLASS_H_
#define _POSITIONCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <math.h>
#include <d3dx10math.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: PositionClass
////////////////////////////////////////////////////////////////////////////////
class Position {
public:
	Position(bool, Position* = 0);
	Position(const Position&);
	~Position();

	D3DXVECTOR3 GetRotVel();
	D3DXVECTOR3 GetPosVel();

	D3DXVECTOR3 GetPosition();
	D3DXQUATERNION GetRotation();

	void SetOffset(D3DXVECTOR3);

	void SetTime(float);

	void SetYaw(float);
	void SetPitch(float);
	void SetRoll(float);

	void SetStrafeVel(float);
	void SetClimbVel(float);
	void SetForwardVel(float);

	void Update();

private:
	float m_time;

	bool m_rotvel_on;

	Position *m_follow;

	D3DXVECTOR3 m_pos, m_rotvel, m_posvel, m_offset,
				m_up, m_front, m_right;

	D3DXQUATERNION m_rot;
};

#endif