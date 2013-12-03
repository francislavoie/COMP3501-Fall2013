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
	void Update(Input*,float, float, bool, QuadTree *);
private:
	D3DXVECTOR3* relicPosition;

};
#endif