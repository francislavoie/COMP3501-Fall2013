////////////////////////////////////////////////////////////////////////////////
// Filename: stateclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _STATECLASS_H_
#define _STATECLASS_H_


//////////////
// INCLUDES //
//////////////
#include <math.h>
#include <d3dx10math.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: StateClass
////////////////////////////////////////////////////////////////////////////////
class State {
public:
	State(bool, State* = 0);
	State(const State&);
	~State();

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
	void SetDecayRate(float rate) {decayRate = rate;}
	void applyForce(D3DXVECTOR3);
	void setMaxSpeed(float speed) {maxSpeed = speed;}
	D3DXVECTOR3* getForward() {return &m_front;}

	void Update();

private:
	float m_time;
	float decayRate; //% decay rate
	float maxSpeed;
	D3DXVECTOR3 acceleration;

	bool m_rotvel_on;

	State *m_follow;

	D3DXVECTOR3 m_pos, m_rotvel, m_posvel, m_offset,
				m_up, m_front, m_right;

	D3DXQUATERNION m_rot;
};

#endif