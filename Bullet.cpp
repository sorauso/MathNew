#include "Bullet.h"
#include "globals.h"
#include <DxLib.h>

Bullet::Bullet(const Vector2D& pos, const Vector2D& vel, unsigned int color, float radius, float life)
	:Bace(pos,vel,color),radius_(radius),life_(life)
{
}

void Bullet::Update()
{
	float dt = GetDeltaTime();
	pos_ = Math2D::Add(pos_, Math2D::Mul(vel_, dt));
}

void Bullet::Draw()
{
	Vector2D sPos = Math2D::World2Screen(pos_);
	DrawCircleAA(sPos.x, sPos.y, radius_, color_, true);
}
