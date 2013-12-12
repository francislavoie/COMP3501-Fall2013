////////////////////////////////////////////////////////////////////////////////
// Filename: tank.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TANKCLASS_H_
#define _TANKCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <vector>
#include <algorithm>

using namespace std;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "model.h"
#include "state.h"
#include "d3d.h"
#include "input.h"
#include "timer.h"
#include "quadtree.h"
#include "bullet.h"
#include "modellist.h"

struct VectorSortP {
	bool operator()(const D3DXVECTOR3 a, const D3DXVECTOR3 b) const {
		return a.y < b.y;
	}
};

const float RADIUS = 50;
const D3DXVECTOR3 FRONTRIGHT = D3DXVECTOR3(0.55614f, 0.0f, 1.35747f);
const D3DXVECTOR3 FRONTLEFT = D3DXVECTOR3(-0.55106f, 0.0f, 1.35747f);
const D3DXVECTOR3 REARLEFT = D3DXVECTOR3(-0.55826f, 0.0f, -0.53326f);
const D3DXVECTOR3 REARRIGHT = D3DXVECTOR3(0.56334f, 0.0f, -0.53326f);
const D3DXVECTOR3 CENTER = D3DXVECTOR3(0.0f, 0.0f, 0.94537f);

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class Tank {

public:
	Tank();
	Tank(const Tank &);
	~Tank();

	bool Initialize(D3D*, HWND, QuadTree *);
	void Shutdown();
	void RenderTank(ID3D11DeviceContext*);
	void RenderTurret(ID3D11DeviceContext*);
	virtual void Update(Input*,float, QuadTree *);
	State *getTankState() {return m_tankState;}
	State *getTurretState() {return m_turretState;}

	int GetTankIndexCount();
	int GetTurretIndexCount();
	ID3D11ShaderResourceView* GetTankTexture() { return baseTank->GetTexture(); }
	ID3D11ShaderResourceView* GetTurretTexture() { return turret->GetTexture(); }

	float GetPitch() { return pitch; }
	float GetYaw() { return yaw; }

	void turnLeft(){turn = -turnSpeed;}
	void turnRight(){turn = turnSpeed;}

	void moveForward(){forward = moveSpeed;}
	void moveBack(){forward = -moveSpeed;}

	void orientTurret(float,float,float);

	void checknResolveTankCollision(Tank*);
	void checknResolveBulletCollision(Bullet*);
	void checknResolveStaticCollision(ModelList *);
	Bullet *getBullets() {return m_Bullet;}

protected:
	Model *baseTank, *turret;
	State *m_tankState, *m_turretState;
	D3DXVECTOR3 turretLookAt;
	float yaw, pitch;
	float turn, forward;
	float moveSpeed;
	float turnSpeed;

	D3DXVECTOR3 m_frontRight, m_frontLeft, m_rearRight, m_rearLeft, m_center;
	Bullet *m_Bullet;
};

#endif