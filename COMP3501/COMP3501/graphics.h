////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3d.h"
#include "camera.h"
#include "model.h"
#include "bitmap.h"
#include "mousecursor.h"
#include "textureshader.h"
#include "lightshader.h"
#include "light.h"
#include "text.h"
#include "modellist.h"
#include "frustum.h"

#include "input.h"


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN_WINDOWED = true;
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class Graphics {
public:
	Graphics();
	Graphics(const Graphics&);
	~Graphics();

	bool Initialize(int, int, HWND);
	void Shutdown();

	bool Frame(int, int, float, Input*);
	bool Render(float);

private:
	D3D* m_D3D;
	Camera* m_Camera;

	Model* m_Model;
	Model* m_Model2;

	Model* m_Bullet;
	Model* m_Turret;

	Bitmap* m_Bitmap;
	MouseCursor* m_Cursor;
	TextureShader* m_TextureShader;
	LightShader* m_LightShader;
	Light* m_Light;
	Text* m_Text;
	ModelList* m_ModelList;
	Frustum* m_Frustum;

	D3DXVECTOR3 bulletVelocity, bulletPosition;
	D3DXQUATERNION bulletRotation;

	D3DXVECTOR3 turretVelocity, turretPosition;
	D3DXQUATERNION turretRotation;
};

#endif