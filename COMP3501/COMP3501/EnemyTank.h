#ifndef _ENEMYTANK_H_
#define _ENEMYTANK_H_

#include "tank.h"

class EnemyTank: public Tank 
{
public:
	EnemyTank();
	EnemyTank(const EnemyTank&);
	~EnemyTank();
	void setRelicPosition(D3DXVECTOR3 *);
	void Update(Input*,float, QuadTree *);
	void setTarget(D3DXVECTOR3*);
private:
	D3DXVECTOR3* relicPosition;
	D3DXVECTOR3* targetPosition;

};
#endif