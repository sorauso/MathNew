#include "ExplosionEffect.h"
#include "DxLib.h"


namespace
{
	const float PARTICLE_LIFE = 10.0f;
	const float PARTICLE_RADIUS = 1.0f;
	const int SPEED_MAX = 3;
	const float PARTICLE_SPEED[SPEED_MAX] = {50.0f,100.0f,150.0f};
	const float PARTICLE_DECAY = 0.99f;
}

ExplosionEffect::ExplosionEffect(const Vector2D& pos, int particleCount)
	:Bace(pos, {0.0f,0.0f},GetColor(255,255,255))
{
	particles_.clear();
	IsFinished_ = false;

	for (int i = 0;i < particleCount;i++)
	{
		float num = (float)GetRand(360) * (Math2D::PI / 180);
		Partical par;
		par.offset = { 0.0f,0.0f };
		par.vel = Math2D::Mul(Math2D::FromAngle(num), PARTICLE_SPEED[GetRand(SPEED_MAX-1)]);
		par.life = PARTICLE_LIFE;
		par.radius = PARTICLE_RADIUS;
		par.color = GetCharaColor();

		particles_.push_back(par);
	}
}

void ExplosionEffect::Update()
{
	float dt = GetDeltaTime();
	bool allDead = true;

	for (auto& par : particles_)
	{
		if (par.life > 0.0f)
		{
			allDead = false;

			par.offset = Math2D::Add(par.offset,Math2D::Mul( par.vel,dt));

			par.vel = Math2D::Mul(par.vel, PARTICLE_DECAY);

			par.life -= dt;
			if (par.life < 0.0f)
			{
				par.life = 0.0f;
			}
		}
	}
	if (allDead)
	{
		IsFinished_ = true;
	}
}

void ExplosionEffect::Draw()
{
	float dt = GetDeltaTime();
	if (IsFinished_)return;
	for (auto& par : particles_)
	{
		if (par.life > 0.0f)
		{
			Vector2D drawPos = Math2D::Add(GetPos(), par.offset);
			Vector2D screenPos = Math2D::World2Screen(drawPos);
			if (par.life < PARTICLE_LIFE/2)
			{
				par.color -= 10 * dt;
			}
			DrawCircle((int)screenPos.x, (int)screenPos.y, par.radius, par.color);
		}
	 }
}
