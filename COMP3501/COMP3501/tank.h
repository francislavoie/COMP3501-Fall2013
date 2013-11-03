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

struct VectorSortP {
    bool operator()(const D3DXVECTOR3 a, const D3DXVECTOR3 b) const {
		return a.y < b.y;
    }
};

const D3DXVECTOR3 FRONTRIGHT = D3DXVECTOR3(0.55614, 0, 1.35747);
const D3DXVECTOR3 FRONTLEFT = D3DXVECTOR3(-0.55106, 0, 1.35747);
const D3DXVECTOR3 REARLEFT= D3DXVECTOR3(-0.55826, 0, -0.53326);
const D3DXVECTOR3 REARRIGHT = D3DXVECTOR3(0.56334, 0, -0.53326);

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class Tank {

private:
	Model *baseTank, *turret;
	State *m_tankState, *m_turretState;

public:
	Tank();
	Tank(const Tank &);
	~Tank();

	bool Initialize(D3D*, HWND);
	void Shutdown();
	void RenderTank(ID3D11DeviceContext*);
	void RenderTurret(ID3D11DeviceContext*);
	void Update(Input*,float, float, bool, QuadTree *);
	State *getTankState() {return m_tankState;}
	State *getTurretState() {return m_turretState;}

	int GetTankIndexCount();
	int GetTurretIndexCount();
	ID3D11ShaderResourceView* GetTankTexture() {return baseTank->GetTexture();}
	ID3D11ShaderResourceView* GetTurretTexture() {return turret->GetTexture();}
	
	D3DXVECTOR3 frontRight, frontLeft, rearRight, rearLeft;

};

#endif