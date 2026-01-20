#include "Enemy.h"
#include "globals.h"
#include <DxLib.h>

Enemy::Enemy(int segment)
	:Bace(),segment_(segment)
{
	pos_ = { (float)GetRand(WIN_WIDTH - 1),(float)GetRand(WIN_HEIGHT - 1) };
	vel_ = { (float)(GetRand(200) - 100),(float)(GetRand(200) - 100) };
	color_ = GetColor(255, 255, 255);
	if (segment_ < 8)
	{
		segment_ = 8;
	}
	radius_ = (float)(GetRand(50) + 25.0f);
	vertex_.resize(segment_);
}

void Enemy::Draw()
{
	std::vector<Vector2D> scrVertex(segment_);
	for (int i = 0; i < segment_; i++)
	{
		scrVertex[i] = Math2D::World2Screen(vertex_[i]);
	}
	Vector2D End = scrVertex[0];
	for (int i = 1; i < segment_; i++)
	{
		Vector2D Start = scrVertex[i];
		DrawLineAA((int)Start.x, (int)Start.y, (int)End.x, (int)End.y, color_, 1.0f);
	}
}

void Enemy::Update()
{
}

void Enemy::MakeShape()
{
	for (int i = 0; i < segment_; i++)
	{
		float angle = (2.0f * DX_PI / segment_) * i;
		vertex_[i] = { radius_ * cosf(angle),radius_ * sinf(angle) };
		vertex_[i] = Math2D::Add(vertex_[i], pos_);
	}
}
