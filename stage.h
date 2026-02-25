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
	void TitleUpdate();
	void PlayUpdate();
	void GameOverUpadate();

	void TitleDraw();
	void PlayDraw();
	void GameOverDraw();
	void Update();
	void Draw();
	void Release();
private:
	void ShootBullet();
	void DeathObject();
	void DereteObject();
	void ObjectHit();
	void Comand();
};