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
	Player* player = nullptr;

	std::vector<Bullet*> bullet;
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

	enemy_ = new Enemy(8);
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

	if (Input::IsKeyDown(KEY_INPUT_Z))
	{
		Vector2D pos = Math2D::Add(player->GetPos(),player->GetVelocity());
		Vector2D vel = Math2D::Mul(player->GetVelocity(), 30.0f);
		Bullet* b = new Bullet(pos, vel, GetColor(255, 255, 255), 5.0f, 10.0f);
		bullet.push_back(b);
	}
	if (!bullet.empty())
	{
		for (auto& obj : bullet)
		{
			obj->Update();
		}
	}
	enemy_->Update();
	player->Update();
}

void Stage::Draw()
{
	if (!bullet.empty())
	{
		for (auto& obj : bullet)
		{
			obj->Draw();
		}
	}
	player->Draw();
	enemy_->Draw();
	Vector2D Ppos = player->GetPos();
	DrawFormatString(0, 0, GetColor(255, 255, 255), "%lf : %lf", Ppos.x,Ppos.y);
}

void Stage::Release()
{
	if(player != nullptr)
		delete player;
	if (enemy_ != nullptr)
		delete enemy_;
}

void Stage::DeleteBullet()
{
}

void Stage::ShootBullet()
{
}
