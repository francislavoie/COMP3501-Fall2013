////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphics.h"

Graphics::Graphics() {
	m_D3D = 0;
	m_Camera = 0;

	m_Model = 0;
	m_Model2 = 0;
	m_Tank = 0;

	m_ShaderManager = 0;
	m_Light = 0;
	m_Bitmap = 0;
	m_Cursor = 0;
	m_Text = 0;
	m_ModelList = 0;
	m_Frustum = 0;
	firstPerson = false;
}


Graphics::Graphics(const Graphics& other) { }


Graphics::~Graphics() { }


bool Graphics::Initialize(D3DXVECTOR2 screen, HWND hwnd)
{
	bool result;
	D3DXMATRIX baseViewMatrix;
		
	// Create the Direct3D object.
	m_D3D = new D3D;
	if(!m_D3D) return false;

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screen, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result) {
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new Camera;
	if(!m_Camera) return false;
	//m_Camera->setPosition(0,0,0);

	// Initialize a base view matrix for 2D user interface rendering.	
	D3DXMatrixIdentity(&baseViewMatrix);
	D3DXMatrixTranslation(&baseViewMatrix, 0,0,1);

	// Create the text object.
	m_Text = new Text;
	if(!m_Text) return false;

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, baseViewMatrix, screen, 4);
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -20.0f);
	
	// Create the shader manager object.
	m_ShaderManager = new ShaderManager;
	if(!m_ShaderManager) return false;

	// Initialize the shader manager object.
	result = m_ShaderManager->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the shader manager object.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	m_Model = new Model;
	if(!m_Model) return false;

	// Initialize the model object.
	result = m_Model->Initialize(m_D3D->GetDevice(), "data/asteroid1.txt", L"data/Am13.dds");
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	m_Model2 = new Model;
	if(!m_Model2) return false;

	// Initialize the model object.
	result = m_Model2->Initialize(m_D3D->GetDevice(), "data/asteroid2.txt", L"data/Am8.dds");
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	m_Tank = new Tank;
	if(!m_Tank) return false;

	// Initialize the model object.
	result = m_Tank->Initialize(m_D3D, hwnd);
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the tank object.", L"Error", MB_OK);
		return false;
	}

	m_Camera->setFollow(m_Tank->getTurretState());

	// Create the light object.
	m_Light = new Light;
	if(!m_Light) return false;

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, -1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	// Create the bitmap object.
	m_Bitmap = new Bitmap;
	if(!m_Bitmap) return false;

	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screen, L"data/crosshairs.dds", baseViewMatrix, D3DXVECTOR2(50, 50));
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_Cursor = new MouseCursor;
	if(!m_Cursor) return false;

	// Initialize the bitmap object.
	result = m_Cursor->Initialize(m_D3D->GetDevice(), screen, L"data/cursor.dds", baseViewMatrix, D3DXVECTOR2(25, 25));
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Create the model list object.
	m_ModelList = new ModelList;
	if(!m_ModelList) return false;

	// Initialize the model list object.
	result = m_ModelList->Initialize(500);
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the model list object.", L"Error", MB_OK);
		return false;
	}

	// Create the frustum object.
	m_Frustum = new Frustum;
	if(!m_Frustum) return false;

	// Create the terrain object.
	m_Terrain = new Terrain;
	if(!m_Terrain) return false;

	// Initialize the terrain object.
	result = m_Terrain->Initialize(m_D3D->GetDevice(), "data/heightmap01.bmp");
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void Graphics::Shutdown() {

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

	// Release the shader manager object.
	if(m_ShaderManager) {
		m_ShaderManager->Shutdown();
		delete m_ShaderManager;
		m_ShaderManager = 0;
	}

	// Release the terrain object.
	if(m_Terrain) {
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
	}

	// Release the light object.
	if(m_Light) {
		delete m_Light;
		m_Light = 0;
	}

	// Release the tank object.
	if(m_Tank) {
		m_Tank->Shutdown();
		delete m_Tank;
		m_Tank = 0;
	}

	// Release the model object.
	if(m_Model2) {
		m_Model2->Shutdown();
		delete m_Model2;
		m_Model2 = 0;
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


bool Graphics::Frame(int fps, int cpu, float time, Input* input) {
	bool result;
	int mouseX, mouseY, deltaX, deltaY;

	// Set the frames per second.
	result = m_Text->SetFps(fps, 0, m_D3D->GetDeviceContext());
	if(!result) return false;

	// Set the cpu usage.
	result = m_Text->SetCpu(cpu, 1, m_D3D->GetDeviceContext());
	if(!result) return false;

	result = m_Text->SetVector3(m_Tank->getTankState()->GetPosition(), 3, m_D3D->GetDeviceContext());

	input->GetMouseLocation(mouseX, mouseY);
	input->GetMouseDelta(deltaX, deltaY);

	
	int scroll;
	input->GetWheelDelta(scroll);
	m_Camera->Scroll(float(scroll));
	m_Camera->Rotate(-float(deltaX));

	m_Cursor->SetPosition(D3DXVECTOR2(float(mouseX), float(mouseY)));

	float rotation = -float(m_Camera->getTheta() + D3DX_PI/2);

	m_Tank->Update(input, time, rotation, firstPerson);	

	if (input->IsKeyPressed(DIK_SPACE))
		if (firstPerson)
		{
			m_Camera->setFollow(m_Tank->getTurretState());
			firstPerson = false;
		}
		else
		{
			m_Camera->setFollow(m_Tank->getTurretState(), 0.1f, 0.1f);
			firstPerson = true;
		}

	/*
	// Mouse controls
	m_Camera->Yaw(deltaX * 0.005f);
	m_Camera->Pitch(deltaY * 0.005f);

	// Keyboard camera controls
	if(input->IsKeyPressed(DIK_W)) m_Camera->Pitch(-0.005f * time);
	if(input->IsKeyPressed(DIK_S)) m_Camera->Pitch(0.005f * time);
	if(input->IsKeyPressed(DIK_A)) m_Camera->Yaw(-0.005f * time);
	if(input->IsKeyPressed(DIK_D)) m_Camera->Yaw(0.005f * time);
	if(input->IsKeyPressed(DIK_Q)) m_Camera->Roll(0.005f * time);
	if(input->IsKeyPressed(DIK_E)) m_Camera->Roll(-0.005f * time);
	
	// Movement controls
	if(input->IsKeyPressed(DIK_LSHIFT))
		m_Camera->Move(0.01f * time);
	else
		m_Camera->Move(0.002f * time);

	// Stop movement
	if(input->IsKeyPressed(DIK_LCONTROL))
		m_Camera->Move(-0.002f * time);

	D3DXMATRIX viewMatrix;
	m_Camera->GetViewMatrix(viewMatrix);

	// Bullet controls
	if(input->IsKeyPressed(DIK_SPACE) || input->IsMousePressed(MOUSE_LEFT)) {
		bulletVelocity = m_Camera->GetAxisZ() * 0.05f;
		bulletPosition = m_Camera->GetPosition() + 50*bulletVelocity - 0.5f * m_Camera->GetAxisY();
		bulletRotation = m_Camera->GetRotation();

		turretVelocity = m_Camera->GetAxisZ() * 0.05f;
		turretPosition = m_Camera->GetPosition() + 50*bulletVelocity - 0.02f * m_Camera->GetAxisY();
		turretRotation = m_Camera->GetRotation();
	} else {
		bulletPosition += bulletVelocity * time;
		turretPosition += turretVelocity * time;
	}*/

	return true;
}


bool Graphics::Render(float time) {
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool renderModel, result, visible;
	int modelCount, renderCount, index, modelType;
	float radius;
	D3DXVECTOR4 color;
	D3DXVECTOR3 position;
	D3DXQUATERNION rotation;

	////////////////////////////////////////////////////////////////////////////
	//			ROBOT MATH
	////////////////////////////////////////////////////////////////////////////
	D3DXMATRIX scale, rotate, rotateZ, robotPosition, parent;
	static int direction = 1;
	static bool state = true;
	static float bodyrotation = (float)D3DX_PI/2.0f;
	static float x = 0.0f;
	static float armrotation = 0.0f;
	float left = -10.0f, right = 10.0f;
	int armCount = 8;

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
	////////////////////////////////////////////////////////////////////////////
	//			ROBOT MATH
	////////////////////////////////////////////////////////////////////////////


	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);


	// Render the terrain buffers.
	m_Terrain->Render(m_D3D->GetDeviceContext());

	// Render the model using the color shader.
	result = m_ShaderManager->RenderColorShader(m_D3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if(!result) return false;

	// Construct the frustum.
	m_Frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);

	// Get the number of models that will be rendered.
	modelCount = m_ModelList->GetModelCount();

	// Initialize the count of models that have been rendered.
	renderCount = 0;

	// Go through all the models and render them only if they can be seen by the camera view.
	for(index=0; index < modelCount; index++) {
		// Get the position and color of the sphere model at this index.
		m_ModelList->GetData(index, position, color, rotation, visible, modelType, time);

		// If the model is visible, display it
		if (visible) {
			// Set the radius of the sphere to 1.0 since this is already known.
			radius = 1.0f;
	
			// Check if the asteroid model is in the view frustum.
			renderModel = m_Frustum->CheckSphere(position.x, position.y, position.z, radius);
	
			// Check if asteroid collides with ship
			if(m_ModelList->GetDistance(index, m_Camera->GetPosition()) < radius + 2.0f) {
				m_ModelList->Hide(index);
			}
	
			// Check if asteroid collides with tank
			/*if(m_ModelList->GetDistance(index, tankPosition) < radius + 0.15f) {
				m_ModelList->Hide(index);
			}*/
	
			// If it can be seen then render it, if not skip this model and check the next sphere.
			if(renderModel) {
				D3DXMATRIX local, rot, tran;
	
				// Rotate the model
				D3DXMatrixRotationQuaternion(&rot, &rotation);
	
				// Move the model to the location it should be rendered at.
				D3DXMatrixTranslation(&tran, position.x, position.y, position.z); 
	
				// Compose the transformations
				local = rot * tran * worldMatrix;
	
				// Draw model based on type
				if(modelType == 0) {
					m_Model->Render(m_D3D->GetDeviceContext());
					m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), local, viewMatrix, projectionMatrix, 
						m_Model->GetTexture(), m_Light->GetDirection(), D3DXVECTOR4(color.x * 0.15f, color.y * 0.15f, color.z * 0.15f, 1.0f), color, 
						m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
				} else {
					m_Model2->Render(m_D3D->GetDeviceContext());
					m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model2->GetIndexCount(), local, viewMatrix, projectionMatrix, 
						m_Model2->GetTexture(), m_Light->GetDirection(), D3DXVECTOR4(color.x * 0.15f, color.y * 0.15f, color.z * 0.15f, 1.0f), color, 
						m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
				}
	
				
				// Since this model was rendered then increase the count for this frame.
				renderCount++;
			}
		}
	}

	// Set the number of models that was actually rendered this frame.
	result = m_Text->SetRenderCount(renderCount, 2, m_D3D->GetDeviceContext());
	if(!result) return false;

	////////////////////////////////////////////////////////////////////////////
	//			Tank DRAWING
	////////////////////////////////////////////////////////////////////////////
	D3DXMATRIX translationMatrix, scalingMatrix, rotationMatrix;
	D3DXMatrixRotationQuaternion(&rotationMatrix, m_Tank->getTankState()->GetRotation());
	
	m_D3D->GetWorldMatrix(worldMatrix);
	D3DXMatrixTranslation(&translationMatrix, m_Tank->getTankState()->GetPosition()->x, m_Tank->getTankState()->GetPosition()->y, m_Tank->getTankState()->GetPosition()->z);
	worldMatrix = rotationMatrix * translationMatrix * worldMatrix;
	
	m_Tank->RenderTank(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Tank->GetTankIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
		m_Tank->GetTankTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), 
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if(!result) return false;

	D3DXMatrixRotationQuaternion(&rotationMatrix, m_Tank->getTurretState()->GetRotation());
	
	m_D3D->GetWorldMatrix(worldMatrix);
	D3DXMatrixTranslation(&translationMatrix, m_Tank->getTurretState()->GetPosition()->x, m_Tank->getTurretState()->GetPosition()->y, m_Tank->getTurretState()->GetPosition()->z);
	worldMatrix = rotationMatrix * translationMatrix * worldMatrix;

	m_Tank->RenderTurret(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Tank->GetTurretIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
		m_Tank->GetTurretTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), 
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if(!result) return false;

	m_D3D->GetWorldMatrix(worldMatrix);
	////////////////////////////////////////////////////////////////////////////
	//			Tank DRAWING
	////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////
	//			ROBOT DRAWING
	////////////////////////////////////////////////////////////////////////////
	m_Model->Render(m_D3D->GetDeviceContext());

	// Multiply position with rotation of the entire robot
	worldMatrix = rotate * robotPosition;

	// Render the model using the light shader.
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
		m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), 
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if(!result) return false;

	parent = worldMatrix;
	for(int i = 0; i < armCount; i++) {
		D3DXMATRIX translate, backtranslate, local, rotateZ;
		D3DXMatrixTranslation(&backtranslate, -0.8f, 0.0f, 0.0f);
		D3DXMatrixTranslation(&translate, 1.6f, 0.0f, 0.0f);
		D3DXMatrixRotationZ(&rotateZ, sin(armrotation)/3.0f);
		local = scale * translate * rotateZ * backtranslate * parent;
		parent = translate * rotateZ * parent;
		result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), local, viewMatrix, projectionMatrix, 
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
		result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), local, viewMatrix, projectionMatrix, 
			m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), 
			m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
		if(!result) return false;
	}

	m_D3D->GetWorldMatrix(worldMatrix);
	////////////////////////////////////////////////////////////////////////////
	//			ROBOT DRAWING
	////////////////////////////////////////////////////////////////////////////

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_Bitmap->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetCenter());
	if(!result) return false;

	// Render the bitmap with the texture shader.
	result = m_ShaderManager->RenderTextureShader(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, m_Bitmap->GetViewMatrix(), orthoMatrix, m_Bitmap->GetTexture());
	if(!result) return false;

	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if(!result) return false;

	result = m_Cursor->Render(m_D3D->GetDeviceContext());
	if(!result) return false;

	result = m_ShaderManager->RenderTextureShader(m_D3D->GetDeviceContext(), m_Cursor->GetIndexCount(), worldMatrix, m_Cursor->GetViewMatrix(), orthoMatrix, m_Cursor->GetTexture());
	if(!result) return false;

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}