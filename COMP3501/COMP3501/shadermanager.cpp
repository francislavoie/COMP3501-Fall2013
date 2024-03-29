////////////////////////////////////////////////////////////////////////////////
// Filename: ShaderManager.cpp
////////////////////////////////////////////////////////////////////////////////
#include "ShaderManager.h"


ShaderManager::ShaderManager() {
	m_TextureShader = 0;
	m_LightShader = 0;
	m_BumpMapShader = 0;
	m_ColorShader = 0;
	m_TerrainShader = 0;
	m_SkyDomeShader = 0;
	m_ParticleShader = 0;
}


ShaderManager::ShaderManager(const ShaderManager& other) { }


ShaderManager::~ShaderManager() { }


bool ShaderManager::Initialize(ID3D11Device* device, HWND hwnd) {
	bool result;

	// Create the texture shader object.
	m_TextureShader = new TextureShader;
	if(!m_TextureShader) return false;

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(device, hwnd);
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShader;
	if(!m_LightShader) return false;

	// Initialize the light shader object.
	result = m_LightShader->Initialize(device, hwnd);
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the bump map shader object.
	m_BumpMapShader = new BumpMapShader;
	if(!m_BumpMapShader) return false;

	// Initialize the bump map shader object.
	result = m_BumpMapShader->Initialize(device, hwnd);
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the bump map shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	m_ColorShader = new ColorShader;
	if(!m_ColorShader) return false;

	// Initialize the color shader object.
	result = m_ColorShader->Initialize(device, hwnd);
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain shader object.
	m_TerrainShader = new TerrainShader;
	if(!m_TerrainShader) return false;

	// Initialize the terrain shader object.
	result = m_TerrainShader->Initialize(device, hwnd);
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the sky dome shader object.
	m_SkyDomeShader = new SkyDomeShader;
	if(!m_SkyDomeShader) return false;

	// Initialize the sky dome shader object.
	result = m_SkyDomeShader->Initialize(device, hwnd);
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the sky dome shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the particle shader object.
	m_ParticleShader = new ParticleShader;
	if (!m_ParticleShader) return false;

	// Initialize the particle shader object.
	result = m_ParticleShader->Initialize(device, hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the particle shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void ShaderManager::Shutdown() {
	// Release the particle shader object.
	if (m_ParticleShader) {
		m_ParticleShader->Shutdown();
		delete m_ParticleShader;
		m_ParticleShader = 0;
	}

	// Release the sky dome shader object.
	if(m_SkyDomeShader) {
		m_SkyDomeShader->Shutdown();
		delete m_SkyDomeShader;
		m_SkyDomeShader = 0;
	}
	
	// Release the terrain shader object.
	if(m_TerrainShader) {
		m_TerrainShader->Shutdown();
		delete m_TerrainShader;
		m_TerrainShader = 0;
	}
	
	// Release the color shader object.
	if(m_ColorShader) {
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// Release the bump map shader object.
	if(m_BumpMapShader) {
		m_BumpMapShader->Shutdown();
		delete m_BumpMapShader;
		m_BumpMapShader = 0;
	}

	// Release the light shader object.
	if(m_LightShader) {
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the texture shader object.
	if(m_TextureShader) {
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	return;
}


bool ShaderManager::RenderTexture(ID3D11DeviceContext* device, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture) {
	bool result;
	
	// Render the model using the texture shader.
	result = m_TextureShader->Render(device, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture);
	if(!result) return false;

	return true;
}


bool ShaderManager::RenderLight(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, 
										   ID3D11ShaderResourceView* texture, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambient, D3DXVECTOR4 diffuse, 
										   D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specular, float specularPower) {
	bool result;
	
	// Render the model using the light shader.
	result = m_LightShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture, lightDirection, ambient, diffuse, cameraPosition, specular, specularPower);
	if(!result) return false;

	return true;
}


bool ShaderManager::RenderBumpMap(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, 
											 ID3D11ShaderResourceView* colorTexture, ID3D11ShaderResourceView* normalTexture, D3DXVECTOR3 lightDirection, 
											 D3DXVECTOR4 diffuse) {
	bool result;

	// Render the model using the bump map shader.
	result = m_BumpMapShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, colorTexture, normalTexture, lightDirection, diffuse);
	if(!result) return false;

	return true;
}


bool ShaderManager::RenderColor(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix) {
	bool result;

	// Render the model using the color shader.
	result = m_ColorShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix);
	if(!result) return false;

	return true;
}


bool ShaderManager::RenderParticle(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
	D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, D3DXVECTOR3 cameraPosition, D3DXVECTOR3 cameraUp, float time) {
	bool result;

	// Render the model using the color shader.
	result = m_ParticleShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture, cameraPosition, cameraUp, time);
	if (!result) return false;

	return true;
}


void ShaderManager::RenderTerrain(ID3D11DeviceContext* deviceContext, int indexCount) {
	// Render the model using the terrain shader.
	m_TerrainShader->Render(deviceContext, indexCount);

	return;
}

bool ShaderManager::SetTerrainParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, 
										 D3DXMATRIX projectionMatrix, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 lightDirection, 
										 ID3D11ShaderResourceView* texture) {
	return m_TerrainShader->SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, ambientColor, diffuseColor, lightDirection, texture);
}

bool ShaderManager::RenderSkyDome(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, 
								  D3DXMATRIX projectionMatrix, D3DXVECTOR4 apexColor, D3DXVECTOR4 centerColor) {
	bool result;

	// Render the model using the sky dome shader.
	result = m_SkyDomeShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, apexColor, centerColor);
	if(!result) return false;

	return true;
}
