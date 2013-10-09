////////////////////////////////////////////////////////////////////////////////
// Filename: positionclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "position.h"


Position::Position() {
	m_frameTime = 0.0f;
	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_leftTurnSpeed  = 0.0f;
	m_rightTurnSpeed = 0.0f;
	m_upTurnSpeed  = 0.0f;
	m_downTurnSpeed = 0.0f;
}


Position::Position(const Position& other) { }


Position::~Position() { }


void Position::SetFrameTime(float time) {
	m_frameTime = time;
	return;
}


void Position::GetRotation(float& x, float& y) {
	x = m_rotationX;
	y = m_rotationY;
	return;
}


void Position::TurnLeft(bool keydown) {
	// If the key is pressed increase the speed at which the camera turns left.  If not slow down the turn speed.
	if(keydown) {
		m_leftTurnSpeed += m_frameTime * 0.01f;
		if(m_leftTurnSpeed > (m_frameTime * 0.15f)) {
			m_leftTurnSpeed = m_frameTime * 0.15f;
		}
	} else {
		m_leftTurnSpeed -= m_frameTime* 0.005f;
		if(m_leftTurnSpeed < 0.0f) {
			m_leftTurnSpeed = 0.0f;
		}
	}

		// Update the rotation using the turning speed.
	if(m_rotationX < 90.0f || m_rotationX > 270.0f)
		m_rotationY -= m_leftTurnSpeed;
	else 
		m_rotationY += m_leftTurnSpeed;

	if(m_rotationY < 0.0f) {
		m_rotationY += 360.0f;
	}

	return;
}


void Position::TurnRight(bool keydown) {
	// If the key is pressed increase the speed at which the camera turns right.  If not slow down the turn speed.
	if(keydown) {
		m_rightTurnSpeed += m_frameTime * 0.01f;
		if(m_rightTurnSpeed > (m_frameTime * 0.15f)) {
			m_rightTurnSpeed = m_frameTime * 0.15f;
		}
	} else {
		m_rightTurnSpeed -= m_frameTime* 0.005f;
		if(m_rightTurnSpeed < 0.0f) {
			m_rightTurnSpeed = 0.0f;
		}
	}

	// Update the rotation using the turning speed.
	if(m_rotationX < 90.0f || m_rotationX > 270.0f)
		m_rotationY += m_rightTurnSpeed;
	else 
		m_rotationY -= m_rightTurnSpeed;

	if(m_rotationY > 360.0f) {
		m_rotationY -= 360.0f;
	}

	return;
}


void Position::TurnUp(bool keydown) {
	// If the key is pressed increase the speed at which the camera turns up.  If not slow down the turn speed.
	if(keydown) {
		m_upTurnSpeed += m_frameTime * 0.01f;
		if(m_upTurnSpeed > (m_frameTime * 0.15f)) {
			m_upTurnSpeed = m_frameTime * 0.15f;
		}
	} else {
		m_upTurnSpeed -= m_frameTime* 0.005f;
		if(m_upTurnSpeed < 0.0f) {
			m_upTurnSpeed = 0.0f;
		}
	}

	// Update the rotation using the turning speed.
	m_rotationX -= m_upTurnSpeed;
	if(m_rotationX < 0.0f) {
		m_rotationX += 360.0f;
	}

	return;
}


void Position::TurnDown(bool keydown) {
	// If the key is pressed increase the speed at which the camera turns down.  If not slow down the turn speed.
	if(keydown) {
		m_downTurnSpeed += m_frameTime * 0.01f;
		if(m_downTurnSpeed > (m_frameTime * 0.15f)) {
			m_downTurnSpeed = m_frameTime * 0.15f;
		}
	} else {
		m_downTurnSpeed -= m_frameTime* 0.005f;
		if(m_downTurnSpeed < 0.0f) {
			m_downTurnSpeed = 0.0f;
		}
	}

	// Update the rotation using the turning speed.
	m_rotationX += m_downTurnSpeed;
	if(m_rotationX > 360.0f) {
		m_rotationX -= 360.0f;
	}

	return;
}