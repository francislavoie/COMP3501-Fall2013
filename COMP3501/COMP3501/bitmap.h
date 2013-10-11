////////////////////////////////////////////////////////////////////////////////
// Filename: bitmapclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _BITMAPCLASS_H_
#define _BITMAPCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "texture.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: BitmapClass
////////////////////////////////////////////////////////////////////////////////
class Bitmap {
protected:
	struct VertexType {
		D3DXVECTOR3 pos;
	    D3DXVECTOR2 tex;
	};

public:
	Bitmap();
	Bitmap(const Bitmap&);
	~Bitmap();

	bool Initialize(ID3D11Device*, D3DXVECTOR2, WCHAR*, D3DXMATRIX, D3DXVECTOR2);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, D3DXVECTOR2);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
	D3DXMATRIX GetViewMatrix();

	D3DXVECTOR2 GetCenter();

protected:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext*, D3DXVECTOR2);
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

protected:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	D3DXMATRIX m_baseViewMatrix;
	Texture* m_Texture;
	D3DXVECTOR2 m_prevPos, m_screen, m_size;
};

#endif