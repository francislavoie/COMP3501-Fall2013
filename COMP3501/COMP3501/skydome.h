////////////////////////////////////////////////////////////////////////////////
// Filename: skydomeclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SKYDOMECLASS_H_
#define _SKYDOMECLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Class name: SkyDomeClass
////////////////////////////////////////////////////////////////////////////////
class SkyDome {
private:
	struct ModelType {
		D3DXVECTOR3 pos;
		D3DXVECTOR2 tex;
		D3DXVECTOR3 nrm;
	};

	struct VertexType {
		D3DXVECTOR3 position;
	};

public:
	SkyDome();
	SkyDome(const SkyDome&);
	~SkyDome();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	D3DXVECTOR4 GetApexColor();
	D3DXVECTOR4 GetCenterColor();

private:
	bool LoadSkyDomeModel(char*);
	bool LoadModelBin(char*);
	void ReleaseSkyDomeModel();

	bool InitializeBuffers(ID3D11Device*);
	void ReleaseBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	
private:
	ModelType* m_model;
	int m_vertexCount, m_indexCount;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	D3DXVECTOR4 m_apexColor, m_centerColor;
};

#endif