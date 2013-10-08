///////////////////////////////////////////////////////////////////////////////
// Filename: modellistclass.h
///////////////////////////////////////////////////////////////////////////////
#ifndef _MODELLISTCLASS_H_
#define _MODELLISTCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3dx10math.h>
#include <stdlib.h>
#include <time.h>


///////////////////////////////////////////////////////////////////////////////
// Class name: ModelListClass
///////////////////////////////////////////////////////////////////////////////
class ModelListClass {
private:
	struct ModelInfoType {
		D3DXVECTOR4 color;
		D3DXVECTOR3 position;
		D3DXQUATERNION rotation;
		D3DXVECTOR3 velocity;
		bool visible;
		int type;
	};

public:
	ModelListClass();
	ModelListClass(const ModelListClass&);
	~ModelListClass();

	bool Initialize(int);
	void Shutdown();

	int GetModelCount();
	void GetData(int, D3DXVECTOR3&, D3DXVECTOR4&, D3DXQUATERNION&, bool&, int&, float);

	float GetDistance(int, D3DXVECTOR3&);

	void Hide(int);

private:
	int m_modelCount;
	ModelInfoType* m_ModelInfoList;
};

#endif