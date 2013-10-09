////////////////////////////////////////////////////////////////////////////////
// Filename: positionclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _POSITIONCLASS_H_
#define _POSITIONCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <math.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: PositionClass
////////////////////////////////////////////////////////////////////////////////
class Position {
public:
	Position();
	Position(const Position&);
	~Position();

	void SetFrameTime(float);
	void GetRotation(float&, float&);

	void TurnLeft(bool);
	void TurnRight(bool);
	void TurnUp(bool);
	void TurnDown(bool);

private:
	float m_frameTime;
	float m_rotationX, m_rotationY;
	float m_leftTurnSpeed, m_rightTurnSpeed;
	float m_upTurnSpeed, m_downTurnSpeed;
};

#endif