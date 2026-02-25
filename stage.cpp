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
	const unsigned int COLOR_WHIGHT = GetColor(255,255,255);

	Player* player = nullptr;

	/*std::vector<Bullet*> bullet;
	std::vector<Enemy*>enemys;*/
	std::vector<ExplosionEffect*>effects;

	std::vector<Bace*>objects;
	void AddObject(Bace* obj, Bace::ClassName Name)
	{
		obj->SetName(Name);
		objects.push_back(obj);
	}
	void UpdateAllObject()
	{
		for (auto& itr : objects)
		{
			if (itr != nullptr)
			{
				itr->Update();
			}
		}
	}
	void DrawAllObject()
	{
		for (auto& itr : objects)
		{
			if (itr != nullptr)
			{
				itr->Draw();
			}
		}
	}
}

Stage::Stage()
{
	counter = 0;
	score_ = 0;
	Timer_ = 0.0f;
	omegaDrast = false;
	comandCounter = 0;
}

Stage::~Stage()
{
}

void Stage::Init()
{
	player = new Player(START_POS, START_VEL, START_COLOR, START_DIR, START_OMEGA, START_RADIUS);
	AddObject(player, Bace::ClassName::PLAYER);

	for (int i = 0;i < ENEMY_MAX;i++)
	{
		Enemy* e = new Enemy(Enemy::Size::LARGE,8);
		//enemys.push_back(e);
		AddObject(e,Bace::ClassName::ENEMY);
	}
}

void Stage::Update()
{
	float dt = GetDeltaTime();
	Timer_ += dt;
	DeathObject();
	Comand();
	if (omegaDrast == false)
	{
		if (Input::IsKeyDown(KEY_INPUT_SPACE))
		{
			ShootBullet();
		}
	}
	else
	{
		if (Input::IsKeepKeyDown(KEY_INPUT_SPACE))
		{
			for (int i = 0;i < 5;i++)
			{
				ShootBullet();
			}
		}
	}
	
	ObjectHit();
	UpdateAllObject();
	
}

void Stage::Draw()
{
	DrawAllObject();
	int fontsize = GetFontSize();
	SetFontSize(30);
	DrawFormatString(0, 0, COLOR_WHIGHT, "SCORE : %0lld", score_);
	SetFontSize(20);
	DrawFormatString(0, 40, COLOR_WHIGHT, "TIME : %.0lf", Timer_);
	SetFontSize(fontsize);
	DrawFormatString(0, 100, COLOR_WHIGHT, "SCORE : %d", comandCounter);
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
	float r = 1.0f;
	float Life = 0.5f;
	Bullet* b = new Bullet(pos, vel, COLOR_WHIGHT, r, Life);
	//bullet.push_back(b);
	AddObject(b, Bace::ClassName::BULLET);
}

void Stage::DeathObject()
{
	for (auto& obj : objects)
	{
		if (obj->GetName() == Bace::ClassName::BULLET)
		{
			Bullet* b = (Bullet*)(obj);
			if (b->IsDead() == true)
			{
				delete b;
				obj = nullptr;
			}
		}
	}
	DereteObject();

	for(auto& obj : objects)
	{
		if (obj->GetName() == Bace::ClassName::ENEMY)
		{
			Enemy* e = (Enemy*)(obj);
			if (e->IsAlive() == false)
			{
				delete e;
				obj = nullptr;
			}
		}
	}
	DereteObject();

	for (auto& obj : objects)
	{
		if (obj->GetName() == Bace::ClassName::EFFECT)
		{
			ExplosionEffect* e = (ExplosionEffect*)(obj);
			if (e->IsFinished() == true)
			{
				delete e;
				obj = nullptr;
			}
		}
	}
	DereteObject();
}

void Stage::DereteObject()
{
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
	
	bullet.clear();
	enemys.clear();

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
	if (enemys.size() < ENEMY_MAX)
	{
		Vector2D pos = { (float)GetRand(WIN_WIDTH - 1),0.0f };
		Vector2D vel = { (float)(GetRand(200) - 100),(float)(GetRand(200) - 100) };
		Enemy* e = new Enemy(pos,vel,Enemy::Size::LARGE, 8);
		AddObject(e, Bace::ClassName::ENEMY);
	}
	
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
					score_ += 1;
					Enemy::Size size = enemys[i]->CheckSize();
					Vector2D Evel = enemys[i]->GetVel();
					enemys[i]->Kill();
					bobj->Kill();
					if (size == Enemy::Size::SMALL)
					{
						score_ += 5;
						ExplosionEffect* eddect = new ExplosionEffect(Epos);
						//effects.push_back(eddect);
						AddObject(eddect, Bace::ClassName::EFFECT);
					}
					else if (size == Enemy::Size::MEDIUM)
					{
						for (int i = 0;i < GetRand(3) + 2;i++)
						{
							Enemy* e = new Enemy(Epos, Evel, Enemy::Size::SMALL, 8);
							//enemys.push_back(e);
							AddObject(e, Bace::ClassName::ENEMY);
						}
					}
					else if (size == Enemy::Size::LARGE)
					{
						for (int i = 0;i < GetRand(3) + 2;i++)
						{
							Enemy* e = new Enemy(Epos, Evel, Enemy::Size::MEDIUM, 8);
							//enemys.push_back(e);
							AddObject(e, Bace::ClassName::ENEMY);
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
			score_ -= 2;
			ExplosionEffect* eddect = new ExplosionEffect(Ppos, 1);
			eddect->SetColor(255,0,0);
			//effects.push_back(eddect);
			AddObject(eddect, Bace::ClassName::EFFECT);
		}


	}

}

void Stage::Comand()
{
	if (counter > 30)
	{
		comandCounter = 0;
		counter = 0;
	}
	if (comandCounter > 0)
	{
		counter++;
	}
	switch (comandCounter)
	{
	case 0:
		if (Input::IsKeyDown(KEY_INPUT_D))
		{
			comandCounter = 1;
		}
		break;
	case 1:
		if (Input::IsKeyDown(KEY_INPUT_S))
		{
			comandCounter = 2;
		}
		break;
	case 2:
		if (Input::IsKeyDown(KEY_INPUT_A))
		{
			comandCounter = 3;
		}
		break;
	case 3:
		if (Input::IsKeyDown(KEY_INPUT_D))
		{
			comandCounter = 4;
		}
		break;
	case 4:
		if (Input::IsKeyDown(KEY_INPUT_S))
		{
			comandCounter = 5;
		}
		break;
	case 5:
		if (Input::IsKeyDown(KEY_INPUT_A))
		{
			comandCounter = 6;
		}
		break;
	case 6:
		if (Input::IsKeyDown(KEY_INPUT_B))
		{
			comandCounter = 7;
		}
		break;
	case 7:
		omegaDrast = true;
	default:
		break;
	}
}
