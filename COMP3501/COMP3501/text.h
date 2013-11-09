////////////////////////////////////////////////////////////////////////////////
// Filename: textclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTCLASS_H_
#define _TEXTCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "font.h"
#include "fontshader.h"
#include "bitmap.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: TextClass
////////////////////////////////////////////////////////////////////////////////
class Text {
private:
	struct SentenceType {
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		D3DXVECTOR4 color;
	};

	struct VertexType {
		D3DXVECTOR3 pos;
		D3DXVECTOR2 tex;
	};

public:
	Text();
	Text(const Text&);
	~Text();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND, D3DXMATRIX, D3DXVECTOR2, int);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);

	int GetSentenceCount();

	bool SetMousePosition(int, int, ID3D11DeviceContext*);
	bool SetFps(int, int, ID3D11DeviceContext*);
	bool SetCpu(int, int, ID3D11DeviceContext*);
	bool SetRenderCount(int, int, ID3D11DeviceContext*);
	bool SetVector3(char*, D3DXVECTOR3*, int, ID3D11DeviceContext*);

private:
	bool InitializeSentence(SentenceType*, int, ID3D11Device*);
	bool UpdateSentence(SentenceType*, char*, D3DXVECTOR2, D3DXVECTOR4, ID3D11DeviceContext*);
	void ReleaseSentences();
	bool RenderSentence(ID3D11DeviceContext*, SentenceType*, D3DXMATRIX, D3DXMATRIX);
	
private:
	Font* m_Font;
	FontShader* m_FontShader;
	D3DXVECTOR2 m_screen;
	int m_count;
	D3DXMATRIX m_baseViewMatrix;
	SentenceType* m_sentences;
};

#endif