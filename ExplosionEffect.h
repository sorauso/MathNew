#pragma once
#include "Bace.h"
#include <vector>
class ExplosionEffect :
    public Bace
{
public:
    ExplosionEffect(const Vector2D& pos,int particleCount = 20);
    void Update()override;
    void Draw()override;
    bool IsFinished() const { return IsFinished_; }
    struct Partical
    {
        Vector2D offset;
        Vector2D vel;
        float life;
        float radius;
        unsigned int color;
    };
private:
    bool IsFinished_;
    std::vector<Partical>particles_;
};

