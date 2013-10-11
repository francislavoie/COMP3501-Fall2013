////////////////////////////////////////////////////////////////////////////////
// Filename: bitmapclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "mousecursor.h"


MouseCursor::MouseCursor() {
	m_pos = D3DXVECTOR2(0.0f, 0.0f);
}


void MouseCursor::SetPosition(D3DXVECTOR2 pos) {

	// If it has changed then update the position it is being rendered to.
	m_prevPos = m_pos;

	m_pos = pos;

	return;
}


bool MouseCursor::Render(ID3D11DeviceContext* deviceContext) {
	bool result;
	
	// Re-build the dynamic vertex buffer for rendering to possibly a different location on the screen.
	result = UpdateBuffers(deviceContext, m_pos);
	if(!result) return false;

	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return true;
}