#pragma once
#include "Bace.h"
class Bullet :
	public Bace
{
public:
	Bullet(const Vector2D& pos,const Vector2D& vel,unsigned int color,float radius,float life);
	void Update() override;
	void Draw() override;

	bool osDead() const { return life_ <= 0.0f; }
	float GetRadius() const { return radius_; }
 private:
	float radius_;
	float life_;
};