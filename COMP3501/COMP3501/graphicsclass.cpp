////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"

GraphicsClass::GraphicsClass() {
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_LightShader = 0;
	m_TextureShader = 0;
	m_Light = 0;
	m_Bitmap = 0;
	m_Cursor = 0;
	m_Text = 0;
	m_ModelList = 0;
	m_Frustum = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other) { }


GraphicsClass::~GraphicsClass() { }


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd) {
	bool result;
	D3DXMATRIX baseViewMatrix;
		
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

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Create the text object.
	m_Text = new TextClass;
	if(!m_Text) return false;

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -20.0f);
	
	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if(!m_TextureShader) return false;

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	m_Model = new ModelClass;
	if(!m_Model) return false;

	// Initialize the model object.
	result = m_Model->Initialize(m_D3D->GetDevice(), "data/asteroid1.txt", L"data/rocky.dds");
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

	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	if(!m_Bitmap) return false;

	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"data/seafloor.dds", baseViewMatrix, 100, 100);
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_Cursor = new MouseCursor;
	if(!m_Cursor) return false;

	// Initialize the bitmap object.
	result = m_Cursor->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"data/cursor.dds", baseViewMatrix, 25, 25);
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Create the model list object.
	m_ModelList = new ModelListClass;
	if(!m_ModelList) return false;

	// Initialize the model list object.
	result = m_ModelList->Initialize(200);
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the model list object.", L"Error", MB_OK);
		return false;
	}

	// Create the frustum object.
	m_Frustum = new FrustumClass;
	if(!m_Frustum) return false;

	return true;
}


void GraphicsClass::Shutdown() {

	// Release the frustum object.
	if(m_Frustum) {
		delete m_Frustum;
		m_Frustum = 0;
	}

	// Release the model list object.
	if(m_ModelList) {
		m_ModelList->Shutdown();
		delete m_ModelList;
		m_ModelList = 0;
	}

	// Release the text object.
	if(m_Text) {
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the bitmap object.
	if(m_Bitmap) {
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	// Release the bitmap object.
	if(m_Cursor) {
		m_Cursor->Shutdown();
		delete m_Cursor;
		m_Cursor = 0;
	}

	// Release the texture shader object.
	if(m_TextureShader) {
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

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


bool GraphicsClass::Frame(int mouseX, int mouseY, int fps, int cpu, float frameTime, float rotationX, float rotationY) {
	bool result;

	// Set the frames per second.
	result = m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	if(!result) return false;

	// Set the cpu usage.
	result = m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());
	if(!result) return false;

	// Set the location of the mouse.
	//result = m_Text->SetMousePosition(mouseX, mouseY, m_D3D->GetDeviceContext());
	//if(!result) return false;

	m_Cursor->setPosition(mouseX, mouseY);

	// Set the position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -20.0f);

	m_Camera->SetRotation(rotationX, rotationY, 0.0f);

	return true;
}


bool GraphicsClass::Render(float time) {
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	D3DXMATRIX scale, rotate, rotateZ, robotPosition, parent;
	bool renderModel, result;
	int modelCount, renderCount, index;
	float positionX, positionY, positionZ, radius;
	D3DXVECTOR4 color;
	static int direction = 1;
	static bool state = true;
	static float bodyrotation = (float)D3DX_PI/2.0f;
	static float x = 0.0f;
	static float armrotation = 0.0f;
	float left = -10.0f, right = 10.0f;

	armrotation += (float)D3DX_PI * 0.001f * time;

	if(!state) bodyrotation -= direction * (float)D3DX_PI * 0.001f * time;
	if(state) x += direction * 0.01f * time;

	D3DXMatrixScaling(&scale, 0.8f, 0.3f, 0.3f);
	D3DXMatrixRotationY(&rotate, bodyrotation);

	D3DXMatrixRotationZ(&rotateZ, sin(armrotation)/3.0f);

	D3DXMatrixTranslation(&robotPosition, x, 0.0f, 0.0f);

	if(x > right || x < left) {
		direction *= -1;
		x = direction==1 ? left : right;
		state = false;
	}

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
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Construct the frustum.
	m_Frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);

	// Get the number of models that will be rendered.
	modelCount = m_ModelList->GetModelCount();

	// Initialize the count of models that have been rendered.
	renderCount = 0;

	// Go through all the models and render them only if they can be seen by the camera view.
	for(index=0; index<modelCount; index++) {
		// Get the position and color of the sphere model at this index.
		m_ModelList->GetData(index, positionX, positionY, positionZ, color);

		// Set the radius of the sphere to 1.0 since this is already known.
		radius = 1.0f;

		// Check if the sphere model is in the view frustum.
		renderModel = m_Frustum->CheckSphere(positionX, positionY, positionZ, radius);

		// If it can be seen then render it, if not skip this model and check the next sphere.
		if(renderModel) {
			// Move the model to the location it should be rendered at.
			D3DXMatrixTranslation(&worldMatrix, positionX, positionY, positionZ); 

			// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
			m_Model->Render(m_D3D->GetDeviceContext());

			// Render the model using the light shader.
			m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
					   m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), color, 
				       m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			// Reset to the original world matrix.
			m_D3D->GetWorldMatrix(worldMatrix);

			// Since this model was rendered then increase the count for this frame.
			renderCount++;
		}
	}

	// Set the number of models that was actually rendered this frame.
	result = m_Text->SetRenderCount(renderCount, m_D3D->GetDeviceContext());
	if(!result) return false;

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

	D3DXMatrixIdentity(&worldMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_Bitmap->Render(m_D3D->GetDeviceContext(), 100, 100);
	if(!result) return false;

	// Render the bitmap with the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, m_Bitmap->GetViewMatrix(), orthoMatrix, m_Bitmap->GetTexture());
	if(!result) return false;

	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if(!result) return false;

	result = m_Cursor->Render(m_D3D->GetDeviceContext());
	if(!result) return false;

	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Cursor->GetIndexCount(), worldMatrix, m_Cursor->GetViewMatrix(), orthoMatrix, m_Cursor->GetTexture());
	if(!result) return false;

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}