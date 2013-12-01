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
#include "quadtree.h"
#include "shadermanager.h"
#include "input.h"
#include "state.h"
#include "tank.h"
#include "terrain.h"
#include "skydome.h"


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN_WINDOWED = false;
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

	bool Initialize(D3DXVECTOR2, HWND);
	void Shutdown();

	bool Frame(int, int, float, Input*);
	bool Render(float);

private:
	D3DXVECTOR3 chasePosition;
	int spaceBuffer;
	D3D* m_D3D;
	Camera* m_Camera;

	Model* m_Model;
	Model* m_Model2;
	Model* m_Chase;
	Model* m_Spotlight;

	Bitmap* m_Bitmap;
	MouseCursor* m_Cursor;

	ShaderManager* m_ShaderManager;
	Light* m_Light;

	Text* m_Text;
	ModelList* m_ModelList;
	Frustum* m_Frustum;

	Tank* m_Tank;

	Terrain* m_Terrain;
	QuadTree* m_QuadTree;

	SkyDome* m_SkyDome;
};

#endif