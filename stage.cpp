#include "Bace.h"
#include "stage.h"
#include "Math2.h"
#include "Player.h"
#include <DxLib.h>
#include "globals.h"
#include "Input.h"
#include "Bullet.h"
#include "Enemy.h"
#include "ExplosionEffect.h"

namespace
{
	const Vector2D START_POS = { WIN_WIDTH / 2,WIN_HEIGHT / 2 };
	const Vector2D START_VEL = { 0.0f,0.0f };
	const Vector2D START_DIR = { 0.0f,-1.0f };
	const float START_RADIUS = 30.0f;
	const float START_OMEGA = 2.5f;
	const unsigned int START_COLOR = 0xfffffffff;
	const unsigned int ENEMY_MAX = 10;

	Player* player = nullptr;

	/*std::vector<Bullet*> bullet;
	std::vector<Enemy*>enemys;*/
	std::vector<ExplosionEffect*>effects;

	std::vector<Bace*>objects;
	void AddObject(Bace* obj)
	{
		objects.push_back(obj);
	}
	void UpdateAllObject()
	{
		for (auto& itr : objects)
		{
			itr->Update();
		}
	}
	void DrawAllObject()
	{
		for (auto& itr : objects)
		{
			itr->Draw();
		}
	}
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
	AddObject(player);

	for (int i = 0;i < ENEMY_MAX;i++)
	{
		Enemy* e = new Enemy(Enemy::Size::LARGE,8);
		//enemys.push_back(e);
		AddObject(e);
	}
}

void Stage::Update()
{
	
	DeathObject();
	if (Input::IsKeyDown(KEY_INPUT_SPACE))
	{
		ShootBullet();
	}
	
	ObjectHit();
	UpdateAllObject();
}

void Stage::Draw()
{
	DrawAllObject();
	Vector2D Ppos = player->GetPos();
	DrawFormatString(0, 0, GetColor(255, 255, 255), "%lf : %lf", Ppos.x,Ppos.y);
}

void Stage::Release()
{
	if(player != nullptr)
		delete player;
}

void Stage::ShootBullet()
{
	Vector2D pos = Math2D::Add(player->GetPos(), player->GetVelocity());
	Vector2D vel = Math2D::Mul(player->GetVelocity(), 30.0f);
	Bullet* b = new Bullet(pos, vel, GetColor(255, 255, 255), 1.0f, 0.5f);
	//bullet.push_back(b);
	AddObject(b);
}

void Stage::DeathObject()
{
	for (auto obj : objects)
	{
		if (obj->GetName() == Bace::ClassName::BULLET)
		{

		}
		else if (obj->GetName() == Bace::ClassName::ENEMY)
		{

		}
	}
	for (auto it = objects.begin(); it != objects.end();)
	{
		if (*it == nullptr)
		{
			it = objects.erase(it);
		}
		else
		{
			it++;
		}
	}
	
}

void Stage::ObjectHit()
{
	std::vector<Bullet*> bullet;
	std::vector<Enemy*>enemys;
	for (auto &obj : objects)
	{
		if (obj->GetName() == Bace::ClassName::BULLET)
		{
			Bullet* b = (Bullet*)obj;
			bullet.push_back(b);
		}
		else if (obj->GetName() == Bace::ClassName::ENEMY)
		{
			Enemy* e = (Enemy*)obj;
			enemys.push_back(e);
		}
	}
	//if (!enemys.empty())
	//{
		for (int i = 0;i < enemys.size();i++)
		{
			//enemys[i]->Update();
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
							ExplosionEffect* eddect = new ExplosionEffect(Epos);
							//effects.push_back(eddect);
							AddObject(eddect);
						}
						else if (size == Enemy::Size::MEDIUM)
						{
							for (int i = 0;i < GetRand(2) + 2;i++)
							{
								Enemy* e = new Enemy(Epos, Evel, Enemy::Size::SMALL, 8);
								//enemys.push_back(e);
								AddObject(e);
							}
						}
						else if (size == Enemy::Size::LARGE)
						{
							for (int i = 0;i < GetRand(2) + 2;i++)
							{
								Enemy* e = new Enemy(Epos, Evel, Enemy::Size::MEDIUM, 8);
								//enemys.push_back(e);
								AddObject(e);
							}
						}
					}
				}
			}
			Vector2D Ppos = player->GetPos();
			float Dist = Math2D::Length(Math2D::Sub(Epos, Ppos));
			float Radius = Eradiuse + player->GetCrideRadius();
			if (Dist < Radius)
			{
				ExplosionEffect* eddect = new ExplosionEffect(Ppos, 1);
				//effects.push_back(eddect);
				AddObject(eddect);
			}


		}
	//}
}
