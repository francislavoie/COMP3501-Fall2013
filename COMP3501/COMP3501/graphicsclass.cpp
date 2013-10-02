////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass() {
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_LightShader = 0;
	m_Light = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other) { }


GraphicsClass::~GraphicsClass() { }


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd) {
	bool result;

		
	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D) return false;

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result) {
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera) return false;

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -20.0f);
	
	// Create the model object.
	m_Model = new ModelClass;
	if(!m_Model) return false;

	// Initialize the model object.
	result = m_Model->Initialize(m_D3D->GetDevice(), "data/asteroid2.txt", L"data/rocky.dds");
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if(!m_LightShader) return false;

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light) return false;

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	return true;
}


void GraphicsClass::Shutdown() {

	// Release the light object.
	if(m_Light) {
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if(m_LightShader) {
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the model object.
	if(m_Model) {
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if(m_Camera) {
		delete m_Camera;
		m_Camera = 0;
	}

	if(m_D3D) {
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool GraphicsClass::Frame() {
	bool result;

	//static float rotation = 0.0f;
	static float time = 0.0f;

	// Update the rotation variable each frame.
	//rotation += (float)D3DX_PI * 0.005f;
	//if(rotation > 360.0f) rotation -= 360.0f;

	time += 1;
	
	// Render the graphics scene.
	result = Render(time);
	if(!result) return false;

	return true;
}


bool GraphicsClass::Render(float time) {
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	D3DXMATRIX scale, rotate, rotateZ, robotPosition, parent;

	// Start going to the right
	static int direction = 1;

	// Start moving, state false means rotating
	static bool state = true;

	// Start facing right
	static float bodyrotation = (float)D3DX_PI/2;
	static float x = 0.0f;

	// Set the patrol range (left bound, right bound)
	float armrotation, left = -10.0f, right = 10.0f;
	bool result;
	
	// Calculate the cumulative
	armrotation = (float)D3DX_PI * 0.03f * time;

	// If turning around, change rotation angle
	if(!state) bodyrotation -= direction * (float)D3DX_PI * 0.015f;

	// If moving, add to position
	if(state) x += direction * 0.2f;

	// Scale and rotate body
	D3DXMatrixScaling(&scale, 0.8f, 0.3f, 0.3f);
	D3DXMatrixRotationY(&rotate, bodyrotation);

	// Arm Z rotation using sin and divide to limit the range
	D3DXMatrixRotationZ(&rotateZ, sin(armrotation)/3.0f);

	// Translate based on calculated x pos
	D3DXMatrixTranslation(&robotPosition, x, 0.0f, 0.0f);

	// Once reaching either side, change state and fix pos and direction
	if(x > right || x < left) {
		direction *= -1;
		x = direction==1 ? left : right;
		state = false;
	}

	// Once finishing rotate, fix angle and change state
	if(bodyrotation < (float)D3DX_PI/2 || bodyrotation > (float)(3*D3DX_PI)/2) {
		bodyrotation = direction==1 ? (float)D3DX_PI/2 : (float)(3*D3DX_PI)/2;
		state = true;
	}


	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	m_Model->Render(m_D3D->GetDeviceContext());

	// Multiply pos with rotation of the entire robot
	worldMatrix = rotate * robotPosition;

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
				       m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), 
				       m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if(!result) return false;

	int armCount = 8;

	parent = worldMatrix;

	for(int i = 0; i < armCount; i++) {

		D3DXMATRIX translate, backtranslate, local, rotateZ;
		
		D3DXMatrixTranslation(&backtranslate, -0.8f, 0.0f, 0.0f);

		D3DXMatrixTranslation(&translate, 1.6f, 0.0f, 0.0f);

		D3DXMatrixRotationZ(&rotateZ, sin(armrotation)/3.0f);

		local = scale * translate * rotateZ * backtranslate * parent;

		parent = translate * rotateZ * parent;

		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), local, viewMatrix, projectionMatrix, 
						   m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), 
						   m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
		if(!result) return false;
	}

	parent = worldMatrix;

	for(int i = 0; i < armCount; i++) {

		D3DXMATRIX translate, backtranslate, local, rotateZ;
		
		D3DXMatrixTranslation(&backtranslate, 0.8f, 0.0f, 0.0f);

		D3DXMatrixTranslation(&translate, -1.6f, 0.0f, 0.0f);

		D3DXMatrixRotationZ(&rotateZ, sin(armrotation)/3.0f);

		local = scale * translate * rotateZ * backtranslate * parent;

		parent = translate * rotateZ * parent;

		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), local, viewMatrix, projectionMatrix, 
						   m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), 
						   m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
		if(!result) return false;
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}