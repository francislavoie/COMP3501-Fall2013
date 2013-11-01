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

using namespace std;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "model.h"
#include "state.h"
#include "d3d.h"
#include "input.h"
#include "timer.h"


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
	void Update(Input*,float, float, bool);
	State *getTankState() {return m_tankState;}
	State *getTurretState() {return m_turretState;}

	int GetTankIndexCount();
	int GetTurretIndexCount();
	ID3D11ShaderResourceView* GetTankTexture() {return baseTank->GetTexture();}
	ID3D11ShaderResourceView* GetTurretTexture() {return turret->GetTexture();}


};

#endif