////////////////////////////////////////////////////////////////////////////////
// Filename: bitmapclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "mousecursor.h"


MouseCursor::MouseCursor() {
	m_posX = 0;
	m_posY = 0;
}


MouseCursor::MouseCursor(const MouseCursor& other) { }


MouseCursor::~MouseCursor() { }


void MouseCursor::SetPosition(int x, int y) {

	// If it has changed then update the position it is being rendered to.
	m_previousPosX = m_posX;
	m_previousPosY = m_posY;

	m_posX = x;
	m_posY = y;

	return;
}


bool MouseCursor::Render(ID3D11DeviceContext* deviceContext) {
	bool result;
	
	// Re-build the dynamic vertex buffer for rendering to possibly a different location on the screen.
	result = UpdateBuffers(deviceContext, m_posX, m_posY);
	if(!result) return false;

	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return true;
}