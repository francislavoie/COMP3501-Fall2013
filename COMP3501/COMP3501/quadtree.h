////////////////////////////////////////////////////////////////////////////////
// Filename: quadtreeclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _QUADTREECLASS_H_
#define _QUADTREECLASS_H_


/////////////
// GLOBALS //
/////////////
const int MAX_TRIANGLES = 10000;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "terrain.h"
#include "frustum.h"
#include "shadermanager.h"
#include <vector>


////////////////////////////////////////////////////////////////////////////////
// Class name: QuadTreeClass
////////////////////////////////////////////////////////////////////////////////
class QuadTree {
private:
	struct VertexType {
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct NodeType {
		float positionX, positionZ, width;
		int triangleCount;
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		D3DXVECTOR3* vertexArray;
		NodeType* nodes[4];
	};

public:
	QuadTree();
	QuadTree(const QuadTree&);
	~QuadTree();

	bool Initialize(Terrain*, ID3D11Device*);
	void Shutdown();
	void Render(Frustum*, ID3D11DeviceContext*, ShaderManager*);

	int GetDrawCount();

	bool GetHeightAtPosition(float, float, float&, D3DXVECTOR3&);

private:
	void CalculateMeshDimensions(int, float&, float&, float&);
	void CreateTreeNode(NodeType*, vector<int>*, float, float, float, ID3D11Device*);
	int CountAndAddTrianglesFromVertices(vector<int>*, float, float, float);
	int CountAndAddTriangles(vector<int>*, vector<int>*, float, float, float);
	bool HasTrianglesInNode(vector<int>*, float, float, float);
	bool IsTriangleContained(int, float, float, float);
	void ReleaseNode(NodeType*);
	void RenderNode(NodeType*, Frustum*, ID3D11DeviceContext*, ShaderManager*);

	void FindNode(NodeType*, float, float, float&, D3DXVECTOR3&);
	bool CheckHeightOfTriangle(float, float, float&, D3DXVECTOR3&, float[3], float[3], float[3]);

private:
	int m_triangleCount, m_drawCount;
	VertexType* m_vertexList;
	NodeType* m_parentNode;
};

#endif