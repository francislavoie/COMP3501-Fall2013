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
		D3DXVECTOR4 col;
		float timeElapsed;
	};

	struct ParticleType {
		D3DXVECTOR3 start;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 vel;
		D3DXVECTOR4 col;
		float timeElapsed;
		bool active;
	};

public:
	Particle();
	Particle(const Particle&);
	~Particle();

	bool Initialize(ID3D11Device*, WCHAR*);
	void Shutdown();
	bool Frame(float, ID3D11DeviceContext*, bool = false, D3DXVECTOR3 = D3DXVECTOR3(0,0,0));
	void Render(ID3D11DeviceContext*);

	ID3D11ShaderResourceView* GetTexture();
	int GetIndexCount();

protected:
	bool InitializeParticleSystem();
	void ShutdownParticleSystem();

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	bool UpdateBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	void EmitParticles(D3DXVECTOR3, float);
	void UpdateParticles(float);
	void KillParticles();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	Texture* m_Texture;
	int m_vertexCount, m_indexCount;
	VertexType* m_vertices;

	int m_particleCount, m_maxParticles;
	ParticleType* m_Particles;
};

#endif