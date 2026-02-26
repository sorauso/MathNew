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
	const unsigned int ENEMY_MAX = 1;
	const unsigned int COLOR_WHIGHT = GetColor(255,255,255);
	const float DamageRatio = 0.9f;
	const int UpScoreHit = 1;
	const int UpScoreKILL = 5;
	const int RandomVel = 100;

	unsigned int stageState;
	unsigned int counter;
	unsigned int GamuOverCounter;
	unsigned int ComandFlerm;
	unsigned long long score_;
	float Timer_;
	bool omegaDrast;
	unsigned int comandCounter;

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
	Init();
}

Stage::~Stage()
{
}

void Stage::Init()
{
	objects.clear();

	GamuOverCounter = 0;
	stageState = 0;
	counter = 1;
	ComandFlerm = 0;
	score_ = 100;
	Timer_ = 0.0f;
	omegaDrast = false;
	comandCounter = 0;

	Player* player = new Player(START_POS, START_VEL, START_COLOR, START_DIR, START_OMEGA, START_RADIUS);
	AddObject(player, Bace::ClassName::PLAYER);
}

void Stage::TitleUpdate()
{
	if (Input::IsKeyDown(KEY_INPUT_SPACE))
	{
		Init();
		stageState = 1;
	}
	counter++;
}

void Stage::PlayUpdate()
{
	float dt = GetDeltaTime();
	Timer_ += dt;
	DeathObject();
	Comand();
	ObjectHit();
	UpdateAllObject();
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
			for (int i = 0;i < 2;i++)
			{
				ShootBullet();
			}
		}
	}
	if (counter % 200 == 0)
	{
		Vector2D pos = { (float)GetRand(WIN_WIDTH - 1),0.0f };
		Vector2D vel = { (float)(GetRand(RandomVel * 2) - RandomVel),(float)(GetRand(RandomVel * 2) - RandomVel) };
		Enemy* e = new Enemy(pos, vel, Enemy::Size::LARGE, 8);
		AddObject(e, Bace::ClassName::ENEMY);
	}
	counter++;
}

void Stage::GameOverUpadate()
{
	if (Input::IsKeyDown(KEY_INPUT_SPACE))
	{
		stageState = 0;
	}
	counter++;
}

void Stage::TitleDraw()
{
	int fontsize = GetFontSize();
	SetFontSize(80);
	DrawString(WIN_WIDTH / 3 + 5, WIN_HEIGHT / 4 + 2, "ASTROIDS", GetColor(80, 80, 80));
	DrawString(WIN_WIDTH / 3, WIN_HEIGHT / 4, "A", GetColor(255,0,0));
	DrawString(WIN_WIDTH / 3 + 41, WIN_HEIGHT / 4, "STROIDS", COLOR_WHIGHT);
	DrawBox(WIN_WIDTH / 3 - 5, WIN_HEIGHT / 4 + 68, WIN_WIDTH, WIN_HEIGHT / 4 + 80, GetColor(255, 0, 0), true);
	DrawBox(WIN_WIDTH / 3 - 5, WIN_HEIGHT / 4 + 80, WIN_WIDTH, WIN_HEIGHT / 4 + 82, GetColor(80, 80, 80), true);
	SetFontSize(20);
	int PressColor = GetColor(255, 255, 255);
	if (counter % 40 < 20)
	{
		PressColor = GetColor(255, 0, 0);
	}
	DrawString(WIN_WIDTH / 3, WIN_HEIGHT / 4 + 100, "PRESS SPACE KEY", PressColor);
	DrawString(WIN_WIDTH / 3, WIN_HEIGHT / 4 + 150, "　　　 　 ／ ⌒ヽ\n⊂二二二（ ＾ω＾）二⊃\n         | 　　 / 　　 　ﾌﾞｰﾝ\n        （　ヽノ\n         ﾉ > ノ\n   三　　レﾚ\n", COLOR_WHIGHT);
	SetFontSize(fontsize);
}

void Stage::PlayDraw()
{
	DrawAllObject();
	int fontsize = GetFontSize();
	SetFontSize(30);
	DrawFormatString(0, 0, COLOR_WHIGHT, "SCORE : %0lld", score_);
	SetFontSize(20);
	DrawFormatString(0, 40, COLOR_WHIGHT, "TIME : %.0lf", Timer_);
	SetFontSize(fontsize);
}

void Stage::GameOverDraw()
{
	int fontsize = GetFontSize();
	SetFontSize(80);
	
	DrawString(WIN_WIDTH / 3 + sin(counter/2) * 10, WIN_HEIGHT / 4 + cos(counter/2) * 10, "GAMU_OVER", GetColor(255,100,100));
	DrawString(WIN_WIDTH / 3, WIN_HEIGHT / 4, "GAMU_OVER", GetColor(255,0,0));
	SetFontSize(20);
	int PressColor = GetColor(255, 255, 255);
	if (counter % 40 < 20)
	{
		PressColor = GetColor(255, 0, 0);
	}
	DrawString(WIN_WIDTH / 3, WIN_HEIGHT / 4 + 100, "PRESS SPACE KEY", PressColor);
	if (counter % 40 < 20)
	{
		DrawString(WIN_WIDTH / 3 - 50, WIN_HEIGHT / 4 + 150, "　　　　ｵ､ｵ、ｵﾜﾀｰｵﾜｵﾜｵﾜﾀｰ♪\n　　　　＼　　　　ｵｵｵｵﾜﾀｰｵﾜｵｵﾜｵﾜﾀ／\n　  　　　　　　♪＼(^o^)　♪\n　　   　　　　 ＿  ) 　> ＿ ｷｭｯｷｭ♪\n　    　　　　／.◎。／◎｡／|\n　 ＼(^o^)／.|￣￣￣￣￣|　 | 　＼(^o^)／\n 　  )　 )  .|　　　　　|／　　　ノ　ノ\n(((  >￣ > )))　＼(^o^)／　 ((　<￣<　)))\n   　　　　　 　 　)　 )\n　　　　　　 (((　 >￣ > ))))\n", GetColor(GetRand(100) + 155, GetRand(100) + 155, GetRand(100) + 155));
	}
	else
	{
		DrawString(WIN_WIDTH / 3 - 50, WIN_HEIGHT / 4 + 150, "　　　　ｵ､ｵ、ｵﾜﾀｰｵﾜｵﾜｵﾜﾀｰ♪\n　　　　＼　　　　ｵｵｵｵﾜﾀｰｵﾜｵｵﾜｵﾜﾀ／\n　  　　　　　♪   (^o^)／　♪\n　　   　　　　 ＿ < 　( ＿ ｷｭｷｭｯ♪\n　    　　　　／.◎。／◎｡／|\n　＼(^o^ )／.|￣￣￣￣￣|　 | 　＼( ^o^)／\n 　 (　 (   .|　　　　　|／　　　  ) 　)\n((( <￣ < )))　＼(^o^)／　     ((　>￣>　)))\n   　　　　　 　 (　 (\n　　　　　　(((  <￣ < ))))\n", GetColor(GetRand(100) + 155, GetRand(100) + 155, GetRand(100) + 155));
	}
	SetFontSize(fontsize);
}

void Stage::Update()
{
	if (stageState == 0)
	{
		TitleUpdate();
	}
	else if (stageState == 1)
	{
		PlayUpdate();
	}
	else if(stageState == 2)
	{
		GameOverUpadate();
	}

}

void Stage::Draw()
{
	if (stageState == 0)
	{
		TitleDraw();
	}
	else if (stageState == 1)
	{
		PlayDraw();
	}
	else if(stageState == 2)
	{
		GameOverDraw();
	}
}

void Stage::Release()
{
}

void Stage::ShootBullet()
{
	Player* player = nullptr;
	for (auto& obj : objects)
	{
		if (obj->GetName() == Bace::ClassName::PLAYER)
		{
			player = (Player*)obj;
		}
	}
	if (player != nullptr)
	{
		Vector2D pos = Math2D::Add(player->GetPos(), player->GetVelocity());
		Vector2D vel = Math2D::Mul(player->GetVelocity(), 30.0f);
		float r = 1.0f;
		float Life = 0.5f;
		Bullet* b = new Bullet(pos, vel, COLOR_WHIGHT, r, Life);
		//bullet.push_back(b);
		AddObject(b, Bace::ClassName::BULLET);
	}
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

	for (auto& obj : objects)
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

	for (auto& obj : objects)
	{
		if (obj->GetName() == Bace::ClassName::PLAYER)
		{
			Player* p = (Player*)(obj);
			if (p->IsAlive() == false)
			{
				delete p;
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
	Player* player = nullptr;
	
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
		else if (obj->GetName() == Bace::ClassName::PLAYER)
		{
			player = (Player*)obj;
		}
	}
	if (enemys.size() < ENEMY_MAX + (Timer_/10))
	{
		Vector2D pos = { (float)GetRand(WIN_WIDTH - 1),0.0f };
		Vector2D vel = { (float)(GetRand(RandomVel * 2) - RandomVel),(float)(GetRand(RandomVel * 2) - RandomVel) };
		Enemy* e = new Enemy(pos,vel,Enemy::Size::LARGE, 8);
		AddObject(e, Bace::ClassName::ENEMY);
	}
	if (player == nullptr)
	{
		GamuOverCounter++;
		if (GamuOverCounter > 200)
		{
			stageState = 2;
		}
	}
	else
	{
		GamuOverCounter = 0;
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
						score_ += UpScoreHit;
						Enemy::Size size = enemys[i]->CheckSize();
						Vector2D Evel = enemys[i]->GetVel();
						enemys[i]->Kill();
						bobj->Kill();
						if (size == Enemy::Size::SMALL)
						{
							score_ += UpScoreKILL;
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
			if (omegaDrast == true)
			{
				player->SetCharaColor(GetColor(255, 0, 0));
			}
			Vector2D Ppos = player->GetPos();
			if (score_ <= 0)
			{
				ExplosionEffect* eddect = new ExplosionEffect(Ppos, 20);
				eddect->SetColor(255, 0, 0);
				//effects.push_back(eddect);
				AddObject(eddect, Bace::ClassName::EFFECT);
				player->Kill();
			}
			float Dist = Math2D::Length(Math2D::Sub(Epos, Ppos));
			float Radius = Eradiuse + player->GetCrideRadius();
			if (Dist < Radius)
			{
				score_ *= DamageRatio;
				ExplosionEffect* eddect = new ExplosionEffect(Ppos, 1);
				eddect->SetColor(255, 0, 0);
				//effects.push_back(eddect);
				AddObject(eddect, Bace::ClassName::EFFECT);
			}
		}
	}
}

void Stage::Comand()
{
	if (ComandFlerm > 10)
	{
		comandCounter = 0;
		ComandFlerm = 0;
	}
	if (comandCounter > 0)
	{
		ComandFlerm++;
	}
	switch (comandCounter)
	{
	case 0:
		if (Input::IsKeyDown(KEY_INPUT_D))
		{
			comandCounter = 1;
			ComandFlerm = 0;
		}
		break;
	case 1:
		if (Input::IsKeyDown(KEY_INPUT_S))
		{
			comandCounter = 2;
			ComandFlerm = 0;
		}
		break;
	case 2:
		if (Input::IsKeyDown(KEY_INPUT_A))
		{
			comandCounter = 3;
			ComandFlerm = 0;
		}
		break;
	case 3:
		if (Input::IsKeyDown(KEY_INPUT_D))
		{
			comandCounter = 4;
			ComandFlerm = 0;
		}
		break;
	case 4:
		if (Input::IsKeyDown(KEY_INPUT_S))
		{
			comandCounter = 5;
			ComandFlerm = 0;
		}
		break;
	case 5:
		if (Input::IsKeyDown(KEY_INPUT_A))
		{
			comandCounter = 6;
			ComandFlerm = 0;
		}
		break;
	case 6:
		if (Input::IsKeyDown(KEY_INPUT_B))
		{
			comandCounter = 7;
			ComandFlerm = 0;
		}
		break;
	case 7:
		omegaDrast = true;
		break;
	default:
		break;
	}
}
