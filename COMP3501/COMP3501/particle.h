////////////////////////////////////////////////////////////////////////////////
// Filename: particle.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PARTICLECLASS_H_
#define _PARTICLECLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>

#include <fstream>
#include <string>
using namespace std;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "texture.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: Particle
////////////////////////////////////////////////////////////////////////////////
class Particle {
protected:

	struct VertexType {
		D3DXVECTOR3 pos;
		D3DXVECTOR3 vel;
	};

public:
	Particle();
	Particle(const Particle&);
	~Particle();

	bool Initialize(ID3D11Device*, int, D3DXVECTOR3, bool, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

	void ResetPosition(ID3D11Device*, D3DXVECTOR3);

protected:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	void ReleaseModel();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	Texture* m_Texture;
	VertexType* m_Model;
};

#endif