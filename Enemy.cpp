#include "Enemy.h"
#include "globals.h"
#include <DxLib.h>

namespace
{
	const int SEGMENT_MAX = 8;
	const float MIN_RADIUS = 20.0f;
	const float MAX_OMEGA = 2.0f;

	const float SMALL_RADIUS_MAX = 40.0f;
	const float SMALL_RADIUS_MIN = 20.0f;
	const float MEDIUM_RADIUS_MAX = 80.0f;
	const float MEDIUM_RADIUS_MIN = 60.0f;
	const float LARG_RADIUS_MAX = 120.0f;
	const float LARG_RADIUS_MIN = 100.0f;
}

Enemy::Enemy(int segment)
	:Bace(),segment_(segment),isAlive_(true)
{
	pos_ = { (float)GetRand(WIN_WIDTH - 1),(float)GetRand(WIN_HEIGHT - 1) };
	vel_ = { (float)(GetRand(200) - 100),(float)(GetRand(200) - 100) };
	color_ = GetColor(255, 255, 255);
	if (segment_ < SEGMENT_MAX)
	{
		segment_ = SEGMENT_MAX;
	}
	radius_ = (float)(GetRand(50) + MIN_RADIUS);

	size_ = CheckSize();

	vertex_.resize(segment_);
	angle_ = 0.0f;
	omega_ = ((float)GetRand(20) + 1)/10;
	counter = 0;
	MakeShape();
}

Enemy::Enemy(Size size, int segment)
	:Bace(), segment_(segment), isAlive_(true),size_(size)
{
	pos_ = { (float)GetRand(WIN_WIDTH - 1),(float)GetRand(WIN_HEIGHT - 1) };
	vel_ = { (float)(GetRand(200) - 100),(float)(GetRand(200) - 100) };
	color_ = GetColor(255, 255, 255);
	if (segment_ < SEGMENT_MAX)
	{
		segment_ = SEGMENT_MAX;
	}
	radius_ = RandomRadius(size_);
	vertex_.resize(segment_);
	angle_ = 0.0f;
	omega_ = ((float)GetRand(20) + 1) / 10;
	counter = 0;
	MakeShape();
}

Enemy::Enemy(const Vector2D& pos, const Vector2D& vel, Size size, int segment)
	:Bace(pos,vel,0xffffff, Bace::ClassName::ENEMY), segment_(segment), isAlive_(true), size_(size)
{
	vel_ = Math2D::Add(vel_,{ (float)(GetRand(200) - 100),(float)(GetRand(200) - 100) });

	if (segment_ < SEGMENT_MAX)
	{
		segment_ = SEGMENT_MAX;
	}
	radius_ = RandomRadius(size_);
	vertex_.resize(segment_);
	angle_ = 0.0f;
	omega_ = ((float)GetRand(20) + 1) / 10;
	counter = 0;
	MakeShape();
}

void Enemy::Draw()
{
	if (IsAlive() == false)
	{
		return;
	}
	std::vector<Vector2D> scrVertex(segment_);
	for (int i = 0; i < segment_; i++)
	{
		Mat2 rotMat = Math2D::Rotation(angle_);
		scrVertex[i] = Math2D::TransformPoint(vertex_[i], rotMat);
		scrVertex[i] = Math2D::Add(scrVertex[i], pos_);
		scrVertex[i] = Math2D::World2Screen(scrVertex[i]);
	}
	for (int i = 0; i < segment_; i++)
	{
		Vector2D End = scrVertex[(i + 1) % segment_];
		Vector2D Start = scrVertex[i];
		DrawLineAA(Start.x, Start.y, End.x, End.y, color_, 1.0f);
	}
	//DrawCircleAA(pos_.x, WIN_HEIGHT - pos_.y, radius_,20, 0xfffff,0);
	
}

void Enemy::Update()
{
	if (IsAlive() == false)
	{
		return;
	}
	float dt = GetDeltaTime();
	pos_ = Math2D::Add(pos_, Math2D::Mul(vel_, dt));
	if (pos_.x < 0) { pos_.x = WIN_WIDTH; }
	if (pos_.x > WIN_WIDTH) { pos_.x = 0; }
	if (pos_.y < 0) { pos_.y = WIN_HEIGHT; }
	if (pos_.y > WIN_HEIGHT) { pos_.y = 0; }
	angle_ = angle_ + omega_ * dt;
	counter += dt;
}

Enemy::Size Enemy::CheckSize() const
{
	if (radius_ <= SMALL_RADIUS_MAX)
	{
		return Size::SMALL;
	}
	else if (radius_ <= MEDIUM_RADIUS_MAX)
	{
		return Size::MEDIUM;
	}
	else
	{
		return Size::LARGE;
	}
}

float Enemy::RandomRadius(Size size)
{
	switch (size)
	{
	case Size::SMALL:
		return (float)(GetRand(SMALL_RADIUS_MAX - SMALL_RADIUS_MIN) + SMALL_RADIUS_MIN);
		break;
	case Size::MEDIUM:
		return (float)(GetRand(MEDIUM_RADIUS_MAX - MEDIUM_RADIUS_MIN) + MEDIUM_RADIUS_MIN);
		break;
	case Size::LARGE:
		return (float)(GetRand(LARG_RADIUS_MAX - LARG_RADIUS_MIN) + LARG_RADIUS_MIN);
		break;
	default:
		return 1.0f;
		break;
	}
}

void Enemy::MakeShape()
{
	for (int i = 0; i < segment_; i++)
	{
		float angle = (2.0f * (float)DX_PI / segment_) * i;
		float r2 = radius_ / 2.0f;
		float length = r2 + r2 * (float)(GetRand(100)) / 100.0f;
		vertex_[i] = { length * cosf(angle),length * sinf(angle) };
	}
}
