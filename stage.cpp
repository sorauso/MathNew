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
	const unsigned int COLOR_RED = GetColor(255, 0, 0);
	const unsigned int COLOR_GRAY = GetColor(80, 80, 80);
	const unsigned int COLOR_USUBENI = GetColor(255, 100, 100);
	const float DamageRatio = 0.9f;
	const int UpScoreHit = 1;
	const int UpScoreKILL = 5;
	const int RandomVel = 100;
	const int NEXT_TXT_BLINKING_TIME = 40;
	const int AA_BLINKING_TIME = 20;

	unsigned int stageState;
	unsigned int counter;
	unsigned int Score_Time_counter;
	unsigned int Time_Exit_counter;
	unsigned int GamuOverCounter;
	unsigned int ComandFlerm;
	unsigned long long score_;
	unsigned long long Max_score;
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
	Score_Time_counter = 0;
	Time_Exit_counter = 0;
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
	if (score_ > Max_score)
	{
		Max_score = score_;
	}
	counter++;
}

void Stage::GameOverUpadate()
{
	if (Input::IsKeyDown(KEY_INPUT_SPACE))
	{
		stageState = 0;
		counter = 0;
	}
	counter++;
}

void Stage::TitleDraw()
{
	int fontsize = GetFontSize();
	SetFontSize(80);
	DrawString(WIN_WIDTH / 3 + 5, WIN_HEIGHT / 4 + 2, "ASTROIDS", COLOR_GRAY);
	DrawString(WIN_WIDTH / 3, WIN_HEIGHT / 4, "A", COLOR_RED);
	DrawString(WIN_WIDTH / 3 + 41, WIN_HEIGHT / 4, "STROIDS", COLOR_WHIGHT);
	DrawBox(WIN_WIDTH / 3 - 5, WIN_HEIGHT / 4 + 68, WIN_WIDTH/3+counter, WIN_HEIGHT / 4 + 80, COLOR_RED, true);
	DrawBox(WIN_WIDTH / 3 - 5, WIN_HEIGHT / 4 + 80, WIN_WIDTH/3+counter, WIN_HEIGHT / 4 + 82, COLOR_GRAY, true);
	DrawBox(0 , WIN_HEIGHT / 4 + 68, -WIN_WIDTH + counter, WIN_HEIGHT / 4 + 80, COLOR_RED, true);
	DrawBox(0 , WIN_HEIGHT / 4 + 80, -WIN_WIDTH + counter, WIN_HEIGHT / 4 + 82, COLOR_GRAY, true);
	SetFontSize(20);
	int PressColor = COLOR_WHIGHT;
	if (counter % NEXT_TXT_BLINKING_TIME < NEXT_TXT_BLINKING_TIME/2)
	{
		PressColor = COLOR_RED;
	}
	DrawString(WIN_WIDTH / 3, WIN_HEIGHT / 4 + 100, "PRESS SPACE KEY", PressColor);
	int RndColor = COLOR_WHIGHT;
	if(counter > 1360)
	{
		RndColor = GetColor(GetRand(100) + 155, GetRand(100) + 155, GetRand(100) + 155);
	}
	DrawString(WIN_WIDTH / 3, WIN_HEIGHT / 4 + 150, "　　　 　 ／ ⌒ヽ\n⊂二二二（ ＾ω＾）二⊃\n         | 　　 / 　　 　ﾌﾞｰﾝ\n        （　ヽノ\n         ﾉ > ノ\n   三　　レﾚ\n", RndColor);
	SetFontSize(fontsize);
}

void Stage::PlayDraw()
{
	DrawAllObject();
	int fontsize = GetFontSize();
	SetFontSize(30);
	DrawFormatString(0, 0, COLOR_WHIGHT, "SHIELD : %0lld", score_);
	SetFontSize(20);
	DrawFormatString(0, 40, COLOR_WHIGHT, "score : %0lld", Max_score);
	DrawFormatString(0, 70, COLOR_WHIGHT, "time : %.0lf", Timer_);
	SetFontSize(fontsize);
}

void Stage::GameOverDraw()
{
	int fontsize = GetFontSize();
	SetFontSize(80);
	
	DrawString(WIN_WIDTH / 3 + sin(counter/2) * 10, WIN_HEIGHT / 4 + cos(counter/2) * 10, "GAMU_OVER", COLOR_USUBENI);
	DrawString(WIN_WIDTH / 3, WIN_HEIGHT / 4, "GAMU_OVER", COLOR_RED);
	SetFontSize(20);


	const int AA_POSITION_X = WIN_WIDTH / 3 - 50;
	const int AA_POSITION_Y = WIN_HEIGHT / 2 + 80;
	const int MOVA_AA_POSITION_X = WIN_WIDTH / 2 - 50;
	const int MOVA_AA_POSITION_Y = WIN_HEIGHT / 3 + 70;
	const int SCORE_POSITION_X = WIN_WIDTH / 3;
	const int SCORE_TIME_POSITION_Y = WIN_HEIGHT / 3 + 50;
	const int TIME_POSITION_X = WIN_WIDTH / 2 - 30;
	int counterFirst = (WIN_WIDTH + 100) - MOVA_AA_POSITION_X;
	if (counter < counterFirst)
	{
		DrawFormatString(SCORE_POSITION_X, SCORE_TIME_POSITION_Y, COLOR_WHIGHT, "score : %0lld", Max_score);
		DrawFormatString(WIN_WIDTH+20-counter+100, SCORE_TIME_POSITION_Y, COLOR_WHIGHT, "time : %0lld", (int)Timer_);
	}
	else if((Max_score + Score_Time_counter < Max_score + (int)Timer_))
	{
		Score_Time_counter++;
		DrawFormatString(SCORE_POSITION_X, SCORE_TIME_POSITION_Y, COLOR_WHIGHT, "score : %0lld", Max_score + Score_Time_counter);
		DrawFormatString(TIME_POSITION_X, SCORE_TIME_POSITION_Y, COLOR_WHIGHT, "time : %0lld", (int)(Timer_) - Score_Time_counter);
	}
	else
	{
		Time_Exit_counter++;
		DrawFormatString(SCORE_POSITION_X, SCORE_TIME_POSITION_Y, COLOR_WHIGHT, "score : %0lld", Max_score + (int)Timer_);
		DrawFormatString(TIME_POSITION_X + Time_Exit_counter, SCORE_TIME_POSITION_Y, COLOR_WHIGHT, "time : 0");
	}
	int PressColor = COLOR_WHIGHT;
	if (counter % NEXT_TXT_BLINKING_TIME < NEXT_TXT_BLINKING_TIME/2)
	{
		PressColor = COLOR_RED;
	}
	DrawString(WIN_WIDTH / 3, WIN_HEIGHT / 2 + 30, "PRESS SPACE KEY", PressColor);
	unsigned int RndColor = GetColor(GetRand(100) + 155, GetRand(100) + 155, GetRand(100) + 155);
	if (counter % AA_BLINKING_TIME < AA_BLINKING_TIME/2)
	{
		DrawString(AA_POSITION_X, AA_POSITION_Y, "　　　　ｵ､ｵ、ｵﾜﾀｰｵﾜｵﾜｵﾜﾀｰ♪\n　　　　＼　　　　ｵｵｵｵﾜﾀｰｵﾜｵｵﾜｵﾜﾀ／\n　  　　　　　　♪＼(^o^)　♪\n　　   　　　　 ＿  ) 　> ＿ ｷｭｯｷｭ♪\n　    　　　　／.◎。／◎｡／|\n　 ＼(^o^)／.|￣￣￣￣￣|　 | 　＼(^o^)／\n 　  )　 )  .|　　　　　|／　　　ノ　ノ\n(((  >￣ > )))　＼(^o^)／　 ((　<￣<　)))\n   　　　　　 　 　)　 )\n　　　　　　 (((　 >￣ > ))))\n", GetColor(GetRand(100) + 155, GetRand(100) + 155, GetRand(100) + 155));
		if (counter < counterFirst)
		{
			DrawString(WIN_WIDTH - counter+100, MOVA_AA_POSITION_Y, "  ＼( ^o^)／\n 　 )　  )  \n (((> ￣ > )))\n", RndColor);
		}
		else if ((Max_score + Score_Time_counter < Max_score + (int)Timer_))
		{
			DrawString(MOVA_AA_POSITION_X, MOVA_AA_POSITION_Y, "  ＼( ^o^)／\n 　 )　  )  \n (((> ￣ > )))\n", RndColor);

		}
		else
		{
			DrawString(MOVA_AA_POSITION_X + Time_Exit_counter, MOVA_AA_POSITION_Y, "  ＼( ^o^)／\n 　 )　  )  \n (((> ￣ > )))\n", RndColor);

		}
	}
	else
	{
		DrawString(AA_POSITION_X, AA_POSITION_Y, "　　　　ｵ､ｵ、ｵﾜﾀｰｵﾜｵﾜｵﾜﾀｰ♪\n　　　　＼　　　　ｵｵｵｵﾜﾀｰｵﾜｵｵﾜｵﾜﾀ／\n　  　　　　　♪   (^o^)／　♪\n　　   　　　　 ＿ < 　( ＿ ｷｭｷｭｯ♪\n　    　　　　／.◎。／◎｡／|\n　＼(^o^ )／.|￣￣￣￣￣|　 | 　＼( ^o^)／\n 　 (　 (   .|　　　　　|／　　　  ) 　)\n((( <￣ < )))　＼(^o^)／　     ((　>￣>　)))\n   　　　　　 　 (　 (\n　　　　　　(((  <￣ < ))))\n", RndColor);
		if (counter < counterFirst)
		{
			DrawString(WIN_WIDTH - counter+100, MOVA_AA_POSITION_Y, " ＼(^o^ )／\n 　(　  ( \n(((< ￣ < )))", RndColor);
		}
		else if ((Max_score + Score_Time_counter < Max_score + (int)Timer_))
		{
			DrawString(MOVA_AA_POSITION_X, MOVA_AA_POSITION_Y, " ＼(^o^ )／\n 　(　  ( \n(((< ￣ < )))", RndColor);

		}
		else
		{
			DrawString(MOVA_AA_POSITION_X + Time_Exit_counter, MOVA_AA_POSITION_Y, " ＼(^o^ )／\n 　(　  ( \n(((< ￣ < )))", RndColor);
		}
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
			counter = 0;
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
						Max_score += UpScoreHit;
						Enemy::Size size = enemys[i]->CheckSize();
						Vector2D Evel = enemys[i]->GetVel();
						enemys[i]->Kill();
						bobj->Kill();
						if (size == Enemy::Size::SMALL)
						{
							score_ += UpScoreKILL;
							Max_score += UpScoreKILL;
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
				player->SetCharaColor(COLOR_RED);
			}
			Vector2D Ppos = player->GetPos();
			if (score_ <= 0)
			{
				for (int i = 0;i < 10;i++)
				{
					ExplosionEffect* eddect = new ExplosionEffect(Ppos, 20);
					eddect->SetColor(255, 0, 0);
					//effects.push_back(eddect);
					AddObject(eddect, Bace::ClassName::EFFECT);
				}
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
			for (int j = 0;j < enemys.size();j++)
			{
				//enemys[i]->Update();
				if (enemys[j] == enemys[i])
				{
					continue;
				}
				Vector2D E2pos = enemys[j]->GetPos();
				float E2radiuse = enemys[j]->GetRadius();
				if (!enemys[j]->IsAlive())
				{
					continue;
				}
				float Dist = Math2D::Length(Math2D::Sub(Epos, E2pos));
				float Radius = Eradiuse + E2radiuse;
				if (Dist < Radius)
				{
					
				}
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
