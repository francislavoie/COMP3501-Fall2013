#include "EnemyTank.h"

EnemyTank::EnemyTank()
{
	relicPosition = &D3DXVECTOR3(-1,0,0);
	Tank();
	moveSpeed *= 0.9f;
	turnSpeed *= 0.9f;
}
EnemyTank::EnemyTank(const EnemyTank&){}
EnemyTank::~EnemyTank(){}

void EnemyTank::setRelicPosition(D3DXVECTOR3 *pos)
{
	relicPosition = pos;
}
void EnemyTank::Update(Input* input,float time, float rotation, bool firstPerson, QuadTree *m_QuadTree)
{
	D3DXVECTOR3 normal;
	D3DXVec3Normalize(&normal, &(D3DXVECTOR3(relicPosition->x, 0, relicPosition->z) - D3DXVECTOR3(m_tankState->GetPosition()->x, 0, m_tankState->GetPosition()->z)));
	D3DXVECTOR3 noY = D3DXVECTOR3(m_tankState->GetForward()->x, 0, m_tankState->GetForward()->z);
	float angle = acos(D3DXVec3Dot(&normal, &noY));

	D3DXMATRIX turn;
	D3DXMatrixRotationY(&turn, angle);

	//check which way to rotate
	D3DXVECTOR3 cross;
	D3DXVec3Cross(&cross,&normal,&noY);
	D3DXVec3Normalize(&cross, &cross);
/*	if (angle>0.2)
	{
		if (cross.y > 0)
		{
			turnLeft();
		}
		if (cross.y < 0)
		{
			turnRight();
		}
	}
	moveForward();*/

	Tank::Update(input,time,rotation,firstPerson,m_QuadTree);
}
