////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphics.h"

Graphics::Graphics() {
	m_D3D = 0;
	m_Camera = 0;

	m_Model = 0;
	m_Model2 = 0;
	m_Chase = 0;
	m_Spotlight = 0;
	m_Tank = 0;

	m_ShaderManager = 0;
	m_Light = 0;
	m_Bitmap = 0;
	m_Cursor = 0;
	m_Text = 0;
	m_ModelList = 0;
	m_Frustum = 0;
	m_QuadTree = 0;
	m_SkyDome = 0;
	chasePosition = D3DXVECTOR3(50,2,50);
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
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, baseViewMatrix, screen, 20);
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
	result = m_Model->Initialize(m_D3D->GetDevice(), "data/asteroid1.bin", L"data/Am13.dds");
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	m_Model2 = new Model;
	if(!m_Model2) return false;

	// Initialize the model object.
	result = m_Model2->Initialize(m_D3D->GetDevice(), "data/asteroid2.bin", L"data/Am8.dds");
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Chase = new Model;
	if(!m_Chase) return false;

	// Initialize the model object.
	result = m_Chase->Initialize(m_D3D->GetDevice(), "data/cube.txt", L"data/Am8.dds");
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}


	m_Spotlight = new Model;
	if (!m_Spotlight) return false;

	// Initialize the model object.
	result = m_Spotlight->Initialize(m_D3D->GetDevice(), "data/cylinder.bin", L"data/brightlight.dds");
	if (!result) {
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
	m_Light->SetAmbientColor(0.05f, 0.05f, 0.05f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(-0.5f, -1.0f, 0.0f);
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
	result = m_Terrain->Initialize(m_D3D->GetDevice(), "data/terrain04.bmp", L"data/dirt01.dds");
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	// Create the quad tree object.
	m_QuadTree = new QuadTree;
	if(!m_QuadTree) return false;

	// Initialize the quad tree object.
	result = m_QuadTree->Initialize(m_Terrain, m_D3D->GetDevice());
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the quad tree object.", L"Error", MB_OK);
		return false;
	}

	// Create the sky dome object.
	m_SkyDome = new SkyDome;
	if(!m_SkyDome) return false; 

	// Initialize the sky dome object.
	result = m_SkyDome->Initialize(m_D3D->GetDevice());
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the sky dome object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void Graphics::Shutdown() {

	// Release the sky dome object.
	if(m_SkyDome) {
		m_SkyDome->Shutdown();
		delete m_SkyDome;
		m_SkyDome = 0;
	}

	// Release the quad tree object.
	if(m_QuadTree) {
		m_QuadTree->Shutdown();
		delete m_QuadTree;
		m_QuadTree = 0;
	}

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

	if (m_Spotlight) {
		m_Spotlight->Shutdown();
		delete m_Spotlight;
		m_Spotlight = 0;
	}

	if(m_Chase) {
		m_Chase->Shutdown();
		delete m_Chase;
		m_Chase = 0;
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

	result = m_Text->SetVector3("Tank Position", m_Tank->getTankState()->GetPosition(), 3, m_D3D->GetDeviceContext());
	if(!result) return false;

	input->GetMouseLocation(mouseX, mouseY);
	input->GetMouseDelta(deltaX, deltaY);
		
	int scroll;
	input->GetWheelDelta(scroll);
	m_Camera->Scroll(float(scroll));
	m_Camera->Rotate(-float(deltaX));
	m_Camera->lookUpDown(-float(deltaY));

	m_Cursor->SetPosition(D3DXVECTOR2(float(mouseX), float(mouseY)));

	float rotation = -float(m_Camera->getTheta() + D3DX_PI/2);

	if (input->IsKeyPressed(DIK_SPACE)){
		if (m_Camera->isFirstPerson()) 
			m_Camera->setFollow(m_Tank->getTurretState());
		else
			m_Camera->setFollow(m_Tank->getTurretState(), 0.1f, 0.1f);
		m_Camera->toggleFirstPerson();
	}
	
	if(input->IsKeyPressed(DIK_W)){
		m_Tank->moveForward();
	}
	if(input->IsKeyPressed(DIK_S)){
		m_Tank->moveBack();
	}

	if(input->IsKeyPressed(DIK_A)){
		m_Tank->turnLeft();
	}
	if(input->IsKeyPressed(DIK_D)){
		m_Tank->turnRight();
	}
	D3DXVECTOR3 normal;
	D3DXVec3Normalize(&normal, &(D3DXVECTOR3(chasePosition.x, 0, chasePosition.z) - D3DXVECTOR3(m_Tank->getTankState()->GetPosition()->x, 0, m_Tank->getTankState()->GetPosition()->z)));
	D3DXVECTOR3 noY = D3DXVECTOR3(m_Tank->getTankState()->GetForward()->x, 0, m_Tank->getTankState()->GetForward()->z);
	float angle = acos(D3DXVec3Dot(&normal, &noY));

	D3DXMATRIX turn;
	D3DXMatrixRotationY(&turn, angle);

	//check which way to rotate
	D3DXVECTOR3 cross;
	D3DXVec3Cross(&cross,&normal,&noY);
	D3DXVec3Normalize(&cross, &cross);
	if (angle>0.2)
	{
		if (cross.y > 0)
		{
			m_Tank->turnLeft();
		}
		if (cross.y < 0)
		{
			m_Tank->turnRight();
		}
	}

	if (angle < 0.75)
	{
		m_Tank->moveForward();
	}
		
	//else if (angle < -0.15)
		//m_Tank->turnLeft();

	m_Tank->Update(input, time, rotation, m_Camera->isFirstPerson(), m_QuadTree);

	result = m_Text->SetFloat("Pitch Angle", m_Tank->GetPitch() * float(180 / D3DX_PI), 4, m_D3D->GetDeviceContext());
	if(!result) return false;

	result = m_Text->SetFloat("Yaw Angle", m_Tank->GetYaw() * float(180 / D3DX_PI), 5, m_D3D->GetDeviceContext());
	if (!result) return false;

	result = m_Text->SetFloat("Time", angle, 6, m_D3D->GetDeviceContext());
	if(!result) return false;

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
	D3DXVECTOR3 position, cameraPosition;
	D3DXQUATERNION rotation;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	////////////////////////////////////////////////////////////////////////////
	//			Sky Dome DRAWING
	////////////////////////////////////////////////////////////////////////////
	// Get the position of the camera.
	cameraPosition = m_Camera->GetPosition();
	// Translate the sky dome to be centered around the camera position.
	D3DXMatrixTranslation(&worldMatrix, cameraPosition.x, cameraPosition.y, cameraPosition.z);
	// Turn off back face culling.
	m_D3D->TurnOffCulling();
	// Turn off the Z buffer.
	m_D3D->TurnZBufferOff();

	// Render the sky dome using the sky dome shader.
	m_SkyDome->Render(m_D3D->GetDeviceContext());
	m_ShaderManager->RenderSkyDome(m_D3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
		m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor());

	// Turn back face culling back on.
	m_D3D->TurnOnCulling();
	// Turn the Z buffer back on.
	m_D3D->TurnZBufferOn();
	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);
	////////////////////////////////////////////////////////////////////////////
	//			Sky Dome DRAWING
	////////////////////////////////////////////////////////////////////////////


	// Construct the frustum.
	m_Frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);

	// Set the terrain shader parameters that it will use for rendering.
	result = m_ShaderManager->SetTerrainParameters(m_D3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), 
		m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Terrain->GetTexture());
	if(!result) return false;

	// Render the terrain using the quad tree and terrain shader.
	m_QuadTree->Render(m_Frustum, m_D3D->GetDeviceContext(), m_ShaderManager);

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
					m_ShaderManager->RenderLight(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), local, viewMatrix, projectionMatrix, 
						m_Model->GetTexture(), m_Light->GetDirection(), D3DXVECTOR4(color.x * 0.15f, color.y * 0.15f, color.z * 0.15f, 1.0f), color, 
						m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
				} else {
					m_Model2->Render(m_D3D->GetDeviceContext());
					m_ShaderManager->RenderLight(m_D3D->GetDeviceContext(), m_Model2->GetIndexCount(), local, viewMatrix, projectionMatrix, 
						m_Model2->GetTexture(), m_Light->GetDirection(), D3DXVECTOR4(color.x * 0.15f, color.y * 0.15f, color.z * 0.15f, 1.0f), color, 
						m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
				}
	
				
				// Since this model was rendered then increase the count for this frame.
				renderCount++;
			}
		}
	}

	// Set the number of models that was actually rendered this frame.
	result = m_Text->SetRenderCount(m_QuadTree->GetDrawCount(), 2, m_D3D->GetDeviceContext());
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
	result = m_ShaderManager->RenderLight(m_D3D->GetDeviceContext(), m_Tank->GetTankIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
		m_Tank->GetTankTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), 
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if(!result) return false;

	D3DXMatrixRotationQuaternion(&rotationMatrix, m_Tank->getTurretState()->GetRotation());
	
	m_D3D->GetWorldMatrix(worldMatrix);
	D3DXMatrixTranslation(&translationMatrix, m_Tank->getTurretState()->GetPosition()->x, m_Tank->getTurretState()->GetPosition()->y, m_Tank->getTurretState()->GetPosition()->z);
	worldMatrix = rotationMatrix * translationMatrix * worldMatrix;

	m_Tank->RenderTurret(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLight(m_D3D->GetDeviceContext(), m_Tank->GetTurretIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
		m_Tank->GetTurretTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), 
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if(!result) return false;

	m_D3D->GetWorldMatrix(worldMatrix);
	////////////////////////////////////////////////////////////////////////////
	//			Tank DRAWING
	////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////
	//			Chase Object
	///////////////////////////////////////////////////////////////////////////
<<<<<<< HEAD
=======
	static D3DXVECTOR3 chasePosition = D3DXVECTOR3((rand() % 312) + 100.0f, -10.0f, (rand() % 312) + 100.0f);
	if (chasePosition.y == -10.0f) {
		float height;
		D3DXVECTOR3 vgarbage;
		m_QuadTree->GetHeightAtPosition(chasePosition.x, chasePosition.z, height, vgarbage);
		chasePosition.y = height + 2.0f;
	}
>>>>>>> f6aa2b9177d7a2e051dae52e3978e42fa0347067
	static float chaserotate = 0.0f;
	chaserotate += time / 1000.0f;
	D3DXMATRIX localWorldMatrix, scaleMatrix;
	

	if (D3DXVec3Length(&(*m_Tank->getTankState()->GetPosition()-chasePosition)) < sqrt(10))	{
		float height;
		D3DXVECTOR3 vgarbage;
		chasePosition = D3DXVECTOR3((rand() % 312) + 100.0f, 2.0f, (rand() % 312) + 100.0f);
		m_QuadTree->GetHeightAtPosition(chasePosition.x, chasePosition.z, height, vgarbage);
		chasePosition.y += height;
	}
	
	D3DXMatrixRotationY(&worldMatrix, chaserotate);
	D3DXMatrixTranslation(&localWorldMatrix, chasePosition.x, chasePosition.y, chasePosition.z);
	D3DXMatrixMultiply(&localWorldMatrix, &worldMatrix, &localWorldMatrix);

	m_Chase->Render(m_D3D->GetDeviceContext());
	m_D3D->GetWorldMatrix(worldMatrix);

	m_ShaderManager->RenderLight(m_D3D->GetDeviceContext(), m_Chase->GetIndexCount(), localWorldMatrix, viewMatrix, projectionMatrix, 
		m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), 
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if(!result) return false;

	D3DXMatrixScaling(&scaleMatrix, 2.0f, 100.0f, 2.0f);
	D3DXMatrixTranslation(&localWorldMatrix, chasePosition.x, 0.0f, chasePosition.z);
	localWorldMatrix = worldMatrix * scaleMatrix * localWorldMatrix;

	m_Spotlight->Render(m_D3D->GetDeviceContext());
	m_D3D->GetWorldMatrix(worldMatrix);

	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	m_ShaderManager->RenderTexture(m_D3D->GetDeviceContext(), m_Spotlight->GetIndexCount(), localWorldMatrix, viewMatrix, projectionMatrix, m_Spotlight->GetTexture());
	if (!result) return false;

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();
	////////////////////////////////////////////////////////////////////////////
	//			Chase Object
	///////////////////////////////////////////////////////////////////////////


	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_Bitmap->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetCenter());
	if(!result) return false;

	// Render the bitmap with the texture shader.
	result = m_ShaderManager->RenderTexture(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, m_Bitmap->GetViewMatrix(), orthoMatrix, m_Bitmap->GetTexture());
	if(!result) return false;

	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if(!result) return false;

	result = m_Cursor->Render(m_D3D->GetDeviceContext());
	if(!result) return false;

	result = m_ShaderManager->RenderTexture(m_D3D->GetDeviceContext(), m_Cursor->GetIndexCount(), worldMatrix, m_Cursor->GetViewMatrix(), orthoMatrix, m_Cursor->GetTexture());
	if(!result) return false;

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}