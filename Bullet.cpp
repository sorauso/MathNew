#include "Bullet.h"
#include "globals.h"
#include <DxLib.h>

Bullet::Bullet(const Vector2D& pos, const Vector2D& vel, unsigned int color, float radius, float life)
	:Bace(pos,vel,color),radius_(radius),life_(life), isDead(false)
{
}

void Bullet::Update()
{
	float dt = GetDeltaTime();
	//pos_.x = pos_.x + vel_.x * dt;
	//pos_.y = pos_.y + vel_.y * dt;
	pos_ = Math2D::Add(pos_, Math2D::Mul(vel_, dt));

	if (pos_.x < 0)
	{
		if (life_ < 0.0f)
		{
			isDead = true;
		}
		pos_.x = WIN_WIDTH; 
	}
	if (pos_.x > WIN_WIDTH) 
	{
		if (life_ < 0.0f)
		{
			isDead = true;
		}
		pos_.x = 0; 
	}
	if (pos_.y < 0) 
	{
		if (life_ < 0.0f)
		{
			isDead = true;
		}
		pos_.y = WIN_HEIGHT;
	}
	if (pos_.y > WIN_HEIGHT)
	{ 
		if (life_ < 0.0f)
		{
			isDead = true;
		}
		pos_.y = 0;
	}

	life_ = life_ - dt;
}

void Bullet::Draw()
{
	Vector2D sPos = Math2D::World2Screen(pos_);
	DrawCircle((int)sPos.x, (int)sPos.y, radius_, color_, true);
	//DrawCircle(pos_.x, pos_.y, radius_, color_, true);
}

bool Bullet::osDead() const
{
	return life_ <= 0.0f;
}
