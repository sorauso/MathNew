#include "stage.h"
#include "Math2.h"
#include "Player.h"
#include <DxLib.h>
#include "globals.h"
#include "Input.h"

namespace
{
	const Vector2D START_POS = { WIN_WIDTH / 2,WIN_HEIGHT / 2 };
	const Vector2D START_VEL = { 0.0f,0.0f };
	const Vector2D START_DIR = { 0.0f,-1.0f };
	const float START_RADIUS = 30.0f;
	const float START_OMEGA = 2.0f;
	const unsigned int START_COLOR = 0xfffffffff;
	Player* player = nullptr;
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
}

void Stage::Update()
{
	player->Update();
}

void Stage::Draw()
{
	player->Draw();
}

void Stage::Release()
{
	if(player != nullptr)
	delete player;
}
