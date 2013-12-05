////////////////////////////////////////////////////////////////////////////////
// Filename: bullet.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _BULLETCLASS_H_
#define _BULLETCLASS_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "model.h"
#include "state.h"
#include "d3d.h"
#include "input.h"

const D3DXVECTOR3 bulletOffsets[] = {
	D3DXVECTOR3(-0.53078f, 0.23084f, 0.33679f),
	D3DXVECTOR3(-0.53078f, 0.24220f, 0.24328f),
	D3DXVECTOR3(-0.43978f, 0.23084f, 0.33679f),
	D3DXVECTOR3(-0.43978f, 0.24220f, 0.24328f),
	D3DXVECTOR3(-0.34878f, 0.23084f, 0.33679f),
	D3DXVECTOR3(-0.34878f, 0.24220f, 0.24328f),
	D3DXVECTOR3(-0.25778f, 0.23084f, 0.33679f),
	D3DXVECTOR3(-0.25778f, 0.24220f, 0.24328f),
	D3DXVECTOR3(0.26221f, 0.23084f, 0.33488f),
	D3DXVECTOR3(0.26221f, 0.24220f, 0.24137f),
	D3DXVECTOR3(0.35321f, 0.23084f, 0.33488f),
	D3DXVECTOR3(0.35321f, 0.24220f, 0.24137f),
	D3DXVECTOR3(0.44421f, 0.23084f, 0.33488f),
	D3DXVECTOR3(0.44421f, 0.24220f, 0.24137f),
	D3DXVECTOR3(0.53521f, 0.23084f, 0.33488f),
	D3DXVECTOR3(0.53521f, 0.24220f, 0.24137f)
};

////////////////////////////////////////////////////////////////////////////////
// Class name: Bullet
////////////////////////////////////////////////////////////////////////////////
class Bullet {

public:
	Bullet();
	Bullet(const Bullet &);
	~Bullet();

	bool Initialize(D3D*, HWND);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	virtual void Update(Input*, float, State*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture() { return m_bullet->GetTexture(); }

	vector<State*>* GetBullets() { return &m_bulletList; }
	void removeBullets(vector<int>*);

protected:
	Model *m_bullet;
	vector<State*> m_bulletList;
};

#endif