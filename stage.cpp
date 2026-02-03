#include "stage.h"
#include "Math2.h"
#include "Player.h"
#include <DxLib.h>
#include "globals.h"
#include "Input.h"
#include "Bullet.h"
#include "Enemy.h"

namespace
{
	const Vector2D START_POS = { WIN_WIDTH / 2,WIN_HEIGHT / 2 };
	const Vector2D START_VEL = { 0.0f,0.0f };
	const Vector2D START_DIR = { 0.0f,-1.0f };
	const float START_RADIUS = 30.0f;
	const float START_OMEGA = 2.0f;
	const unsigned int START_COLOR = 0xfffffffff;
	const unsigned int ENEMY_MAX = 10;

	Player* player = nullptr;

	std::vector<Bullet*> bullet;
	std::vector<Enemy*>enemys;
}

Stage::Stage()
{
}

Stage::~Stage()
{
}

void Stage::Init()
{
	player = new Player(START_POS, START_VEL, START_COLOR, START_DIR, START_OMEGA, START_RADIUS);

	for (int i = 0;i < ENEMY_MAX;i++)
	{
		Enemy* e = new Enemy(Enemy::Size::LARGE,8);
		enemys.push_back(e);
	}
}

void Stage::Update()
{
	for (auto it = bullet.begin(); it != bullet.end();)
	{
		if ((*it)->osDead() == true)
		{
			it = bullet.erase(it);
		}
		else
		{
			it++;
		}
	}
	for (auto it = enemys.begin(); it != enemys.end();)
	{
		if ((*it)->IsAlive() == false)
		{
			it = enemys.erase(it);
		}
		else
		{
			it++;
		}
	}

	if (Input::IsKeyDown(KEY_INPUT_Z))
	{
		Vector2D pos = Math2D::Add(player->GetPos(),player->GetVelocity());
		Vector2D vel = Math2D::Mul(player->GetVelocity(), 30.0f);
		Bullet* b = new Bullet(pos, vel, GetColor(255, 255, 255), 1.0f, 0.5f);
		bullet.push_back(b);
	}
	if (!bullet.empty())
	{
		for (auto& obj : bullet)
		{
			obj->Update();
		}
	}
	if (!enemys.empty())
	{
		for (int i = 0;i<enemys.size();i++)
		{
			enemys[i]->Update();
			Vector2D Epos = enemys[i]->GetPos();
			float Eradiuse = enemys[i]->GetRadius();
			if (!enemys[i]->IsAlive())
			{
				continue;
			}
			if (!bullet.empty())
			{
				for (auto& bobj : bullet)
				{
					Vector2D Bpos = bobj->GetPos();
					float Dist = Math2D::Length(Math2D::Sub(Epos, Bpos));
					if (Dist < Eradiuse)
					{
						Enemy::Size size = enemys[i]->CheckSize();
						Vector2D Evel = enemys[i]->GetVel();
						enemys[i]->Kill();
						bobj->Kill();
						if (size == Enemy::Size::SMALL)
						{

						}
						else if(size == Enemy::Size::MEDIUM)
						{
							for (int i = 0;i < GetRand(2) + 2;i++)
							{
								Enemy* e = new Enemy(Epos, Evel, Enemy::Size::SMALL, 8);
								enemys.push_back(e);
							}
						}
						else if (size == Enemy::Size::LARGE)
						{
							for (int i = 0;i < GetRand(2) + 2;i++)
							{
								Enemy* e = new Enemy(Epos, Evel, Enemy::Size::MEDIUM, 8);
								enemys.push_back(e);
							}
						}
					}
				}
			}
		}
	}
	
	if (enemys.size() < ENEMY_MAX)
	{
		Enemy* e = new Enemy(Enemy::Size::LARGE, 8);
		enemys.push_back(e);
	}

	player->Update();
}

void Stage::Draw()
{
	if (!enemys.empty())
	{
		for (int i = 0;i < enemys.size();i++)
		{
			enemys[i]->Draw();
		}
	}
	if (!bullet.empty())
	{
		for (auto& obj : bullet)
		{
			obj->Draw();
		}
	}
	player->Draw();
	Vector2D Ppos = player->GetPos();
	DrawFormatString(0, 0, GetColor(255, 255, 255), "%lf : %lf", Ppos.x,Ppos.y);
}

void Stage::Release()
{
	if(player != nullptr)
		delete player;
}

void Stage::DeleteBullet()
{
}

void Stage::ShootBullet()
{
}
