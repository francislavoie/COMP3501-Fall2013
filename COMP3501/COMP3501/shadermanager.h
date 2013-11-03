////////////////////////////////////////////////////////////////////////////////
// Filename: shadermanager.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _ShaderManager_H_
#define _ShaderManager_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3d.h"

#include "textureshader.h"
#include "lightshader.h"
#include "bumpmapshader.h"
#include "colorshader.h"
#include "terrainshader.h"
#include "skydomeshader.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: ShaderManager
////////////////////////////////////////////////////////////////////////////////
class ShaderManager {
public:
	ShaderManager();
	ShaderManager(const ShaderManager&);
	~ShaderManager();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();

	bool RenderTexture(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);
	bool RenderLight(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, D3DXVECTOR4, float);
	bool RenderBumpMap(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4);
	bool RenderColor(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	
	bool SetTerrainParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, ID3D11ShaderResourceView*);
	void RenderTerrain(ID3D11DeviceContext*, int);

	bool RenderSkyDome(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXVECTOR4, D3DXVECTOR4);

private:
	TextureShader* m_TextureShader;
	LightShader* m_LightShader;
	BumpMapShader* m_BumpMapShader;
	ColorShader* m_ColorShader;
	TerrainShader* m_TerrainShader;
	SkyDomeShader* m_SkyDomeShader;
};

#endif
