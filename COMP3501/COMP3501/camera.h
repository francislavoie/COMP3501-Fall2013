////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3dx10math.h>
#include "state.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class Camera {
public:
	Camera();
	Camera(D3DXVECTOR3);
	Camera(const Camera&);
	~Camera();

	void SetOrientation(D3DXQUATERNION);
	void SetLookAt(D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR3);

	void SetPosition(D3DXVECTOR3);
	void SetPosition(float, float, float);

	D3DXVECTOR3 GetPosition() const { return m_position; }
	D3DXQUATERNION GetRotation() const { return m_quatOrientation; }
	float getTheta() const {return theta;}

	void GetViewMatrix(D3DXMATRIX&);

	void Pitch(float angle) { ApplyRotate(angle, PITCH); }
	void Roll(float angle)	{ ApplyRotate(angle, ROLL); }
	void Yaw(float angle) { ApplyRotate(angle, YAW); }

	void Move(float distance ) { ApplyTranslation(distance, MOVE); }
	void Strafe(float distance ) { ApplyTranslation(distance, STRAFE); }
	void Up(float distance) { ApplyTranslation(distance, UPWARDS); }
	void Scroll(float);
	void Rotate(float);

	void setFollow(State *, float minHeight = 0.5f, float maxHeight = 2.5f);

	const D3DXVECTOR3 GetAxisZ() const;
	const D3DXVECTOR3 GetAxisY() const;
	const D3DXVECTOR3 GetAxisX() const;
	bool isFirstPerson() {return firstPerson;}
	void toggleFirstPerson() {firstPerson = !firstPerson;}
	void lookUpDown(float);

private:
	bool firstPerson;
	D3DXVECTOR3 m_position;
	D3DXQUATERNION	m_quatOrientation;
	D3DXMATRIX m_viewMatrix;
	D3DXVECTOR3 m_lookatPosition;
	float theta;
	float raiseDistance;
	float minHeight, maxHeight;
	float angle;
	D3DXVECTOR3 pitch;
	State *follow;

	bool upToDate;

	enum eDir { MOVE, STRAFE, UPWARDS };
	enum eOrient { PITCH, ROLL, YAW };

	void Update();
	static bool RotateZAxis(D3DXQUATERNION*, float);
	static bool RotateYAxis(D3DXQUATERNION*, float);
	static bool RotateXAxis(D3DXQUATERNION*, float);
	static D3DXVECTOR3* TransformVector(D3DXQUATERNION*, D3DXVECTOR3*);
	void CalculatePosition();

	void ApplyTranslation(float, eDir); 
	void ApplyRotate(float, eOrient);
};

#endif