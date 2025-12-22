#include "Bace.h"

Bace::Bace()
	:pos_(0,0), vel_(0,0), color_(0xffffff)
{
}

Bace::Bace(Vector2D pos, Vector2D vel, int color)
	:pos_(pos),vel_(vel),color_(color)
{
}

Bace::~Bace()
{
}

void Bace::Draw()
{
}

void Bace::Update()
{
}
