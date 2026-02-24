#pragma once
#include "Math2.h"
#include <string>

class Bace
{
public:
	enum ClassName
	{
		PLAYER,
		BULLET,
		ENEMY,
		EFFECT
	};
protected:
	Vector2D pos_;
	Vector2D vel_;
	int color_;
	int shape_;
	ClassName objName_;
public:
	Bace();
	Bace(Vector2D pos, Vector2D vel, int color,ClassName objName);
	~Bace();
	virtual void Draw();
	virtual void Update();
public:
	
	void SetPos(const Vector2D& pos) { pos_ = pos; }
	Vector2D GetPos() const { return pos_; }
	void SetVel(const Vector2D& vel) { vel_ = vel; }
	Vector2D GetVel() const { return vel_; }
	void GetCharaColor(const unsigned int color) { color_ = color; }
	unsigned int GetCharaColor() const { return color_; }
	void SetShape(const int shape) { shape_ = shape; }
	int GetShape() const { return shape_; }
	void SetName(const ClassName Name) { objName_ = Name; }
	ClassName GetName()const { return objName_; }
};