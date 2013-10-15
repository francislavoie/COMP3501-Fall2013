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
	Position();
	Position(const Position&);
	~Position();

	D3DXVECTOR3 GetRotVel();
	D3DXVECTOR3 GetPosVel();

	D3DXVECTOR3 GetPosition();
	D3DXQUATERNION GetRotation();

	void SetTime(float);

	void SetYaw(float);
	void SetPitch(float);
	void SetRoll(float);

	void SetStrafe(float);

	void Update();

private:
	float m_time;
	D3DXVECTOR3 m_pos, m_rotvel, m_posvel;
	D3DXQUATERNION m_rot;
};

#endif