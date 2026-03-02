#include "Player.h"
#include "DxLib.h"
#include "globals.h"
#include "Input.h"
#include "Bullet.h"

namespace
{
	
	const float DAMP = 0.995f;
	const float ACC = 100.0f;
}

Player::Player()
	:Bace(), radius_(20), angle_(0), omega_(0), dir_(0, -1)
{
	vertex_[0] = { 0,0 };
	vertex_[1] = { 0,0 };
	vertex_[2] = { 0,0 };
	CridRadius_ = 15.0f;
	isAlive_ = true;
	counter = 0;
}

Player::Player(Vector2D pos, Vector2D vel, int color,Vector2D dir, float omega, float radius)
	:Bace(pos, vel, color,Bace::ClassName::PLAYER), radius_(radius), angle_(0), omega_(omega), dir_(dir)
{
	vertex_[0] = { 0,0 };
	vertex_[1] = { 0,0 };
	vertex_[2] = { 0,0 };
	CridRadius_ = 15.0f;
	isAlive_ = true;
	counter = 0;
}

Player::~Player()
{
}

void Player::Draw()
{
	if (IsAlive() == false)
	{
		return;
	}
	Vector2D scrPos[3];
	scrPos[0] = Math2D::World2Screen(vertex_[0]);
	scrPos[1] = Math2D::World2Screen(vertex_[1]);
	scrPos[2] = Math2D::World2Screen(vertex_[2]);
	DrawTriangleAA(
		scrPos[0].x, scrPos[0].y,
		scrPos[1].x, scrPos[1].y,
		scrPos[2].x, scrPos[2].y,
		color_, false, 1.1f);
	if (Input::IsKeepKeyDown(KEY_INPUT_LSHIFT))
	{
		if (counter % 2 == (rand()%2))
		{
			Vector2D pos = pos_;
			pos = Math2D::Mul(Math2D::Sub(pos, vertex_[0]), 1.1f);
			pos = Math2D::Add(pos, pos_);
			pos = Math2D::World2Screen(pos);
			DrawCircle(pos.x, pos.y, 2, 0xffffff, true);
		}
	}
	
	
}

void Player::Update()
{
	counter++;
	if (IsAlive() == false)
	{
		return;
	}
	const float PI = 3.14599236;
	Vector2D p[3];
	p[0] = { cos(90.0f * (PI / 180)), sin(90.0f * (PI / 180)) };
	p[1] = { cos(-60.0f * (PI / 180)), sin(-60.0f * (PI / 180)) };
	p[2] = { cos(240.0f * (PI / 180)), sin(240.0f * (PI / 180)) };

	p[0].x = radius_ * p[0].x;p[0].y = radius_ * p[0].y;
	p[1].x = radius_ * p[1].x;p[1].y = radius_ * p[1].y;
	p[2].x = radius_ * p[2].x;p[2].y = radius_ * p[2].y;

	vertex_[0] = { pos_.x + p[0].x,pos_.y + p[0].y };
	vertex_[1] = { pos_.x + p[1].x,pos_.y + p[1].y };
	vertex_[2] = { pos_.x + p[2].x,pos_.y + p[2].y };

	if (Input::IsKeepKeyDown(KEY_INPUT_A))
	{
		angle_ = angle_ + omega_ * GetDeltaTime();
	}
	if (Input::IsKeepKeyDown(KEY_INPUT_D))
	{
		angle_ = angle_ - omega_ * GetDeltaTime();
	}

    
	Mat2 toOrigin = Math2D::Translation({ -pos_.x,-pos_.y });
	/*for (int i = 0;i < 3;i++)
	{
		vertex_[i] = Math2D::TransformPoint(vertex_[i], toOrigin);	
	}*/

	Mat2 rotMat = Math2D::Rotation(angle_);
	/*for (int i = 0;i < 3;i++)
	{
		vertex_[i] = Math2D::TransformPoint(vertex_[i], rotMat);
	}*/

	Mat2 toPos = Math2D::Translation({pos_.x,pos_.y});
	/*for (int i = 0;i < 3;i++)
	{
		vertex_[i] = Math2D::TransformPoint(vertex_[i], toPos);
	}*/

	Mat2 tmp =  Math2D::Multiply(rotMat, toOrigin);
	Mat2 M = Math2D::Multiply(toPos, tmp);
	for (int i = 0;i < 3;i++)
	{
		vertex_[i] = Math2D::TransformPoint(vertex_[i], M);
	}

	dir_ = Math2D::FromAngle(angle_ + PI / 2.0f);

	if (Input::IsKeepKeyDown(KEY_INPUT_LSHIFT))
	{
		vel_.x = vel_.x + dir_.x * ACC * GetDeltaTime();
		vel_.y = vel_.y + dir_.y * ACC * GetDeltaTime();
	}

	pos_.x = pos_.x + vel_.x * GetDeltaTime();
	pos_.y = pos_.y + vel_.y * GetDeltaTime();

	vel_ = Math2D::Mul(vel_, DAMP);

	if (pos_.x < 0) {pos_.x = WIN_WIDTH;}
	if (pos_.x > WIN_WIDTH) { pos_.x = 0; }
	if (pos_.y < 0) { pos_.y = WIN_HEIGHT; }
	if (pos_.y > WIN_HEIGHT) { pos_.y = 0; }
}

Vector2D Player::GetVelocity()
{
	return Math2D::Sub(vertex_[0], pos_);
}
