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

void EnemyTank::setTarget(D3DXVECTOR3 *target)
{
	targetPosition = target;
}

void EnemyTank::Update(Input* input,float time, QuadTree *m_QuadTree)
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
	if (angle>0.2)
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
	moveForward();
	int deltaX = 0,deltaY = 0;
	if (targetPosition)
	{
		int yDiff = m_tankState->GetPosition()->y - targetPosition->y;
		if ( yDiff < -0.0000001)
		{
			//deltaY = -2;
		}
		else if  (yDiff > 0.0000001)
		{
			//deltaY = 2;
		}
		D3DXVECTOR3 normal1;
		D3DXVec3Normalize(&normal1, &(D3DXVECTOR3(targetPosition->x, 0, targetPosition->z) - D3DXVECTOR3(m_tankState->GetPosition()->x, 0, m_tankState->GetPosition()->z)));
		D3DXVECTOR3 noY1 = D3DXVECTOR3(m_turretState->GetForward()->x, 0, m_turretState->GetForward()->z);
		float angle1 = acos(D3DXVec3Dot(&normal1, &noY1));

		D3DXMATRIX turn1;
		D3DXMatrixRotationY(&turn1, angle1);

		//check which way to rotate
		D3DXVECTOR3 cross1;
		D3DXVec3Cross(&cross1,&normal1,&noY1);
		D3DXVec3Normalize(&cross1, &cross1);
		if (angle1 > 0.2)
		{
			if (cross1.y > 0)
			{
				deltaX = -2;
			}
			if (cross1.y < 0)
			{
				deltaX = 2;
			}
		}
		orientTurret(deltaX,deltaY,time);
	}

	static int countdown = 250 + (rand()%500);
	countdown -= time;
	if (countdown < 0)
	{
		shoot();
		countdown = 250 + (rand()%500);
	}

	Tank::Update(input,time,m_QuadTree);
}
