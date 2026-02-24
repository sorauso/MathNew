#pragma once
#include <vector>


class Enemy;
class Player;
class Stage
{
public:
	Stage();
	~Stage();
	void Init();
	void Update();
	void Draw();
	void Release();
private:
	/*Player* palyer_;
	Enemy* enemy_;*/
	void ShootBullet();
	void DeathObject();
	void DereteObject();
	void ObjectHit();

	float score_;
};