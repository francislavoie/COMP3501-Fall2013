////////////////////////////////////////////////////////////////////////////////
// Filename: bitmapclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MOUSECURSORCLASS_H_
#define _MOUSECURSORCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: MouseCursor
////////////////////////////////////////////////////////////////////////////////
class MouseCursor {
private:
	struct VertexType {
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
	};

public:
	MouseCursor();
	MouseCursor(const MouseCursor&);
	~MouseCursor();

	bool Initialize(ID3D11Device*, int, int, WCHAR*, D3DXMATRIX, int, int);
	void Shutdown();
	bool Render(ID3D11DeviceContext*);

	void setPosition(int, int);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
	D3DXMATRIX GetViewMatrix();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext*);
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	D3DXMATRIX m_baseViewMatrix;
	TextureClass* m_Texture;
	int m_screenWidth, m_screenHeight;
	int m_bitmapWidth, m_bitmapHeight;
	int m_previousPosX, m_previousPosY;

	int m_posX, m_posY;
};

#endif