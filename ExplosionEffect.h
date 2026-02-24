#pragma once
#include "Bace.h"
#include <vector>
class ExplosionEffect :
    public Bace
{
private:
    int color[3];
public:
    ExplosionEffect(const Vector2D& pos,int particleCount = 20);
    void Update()override;
    void Draw()override;
    bool IsFinished() const { return IsFinished_; }
    void SetColor(int R, int G, int B) { color[0] = R;color[1] = G;color[2] = B;}
    struct Partical
    {
        Vector2D offset;
        Vector2D vel;
        float life;
        float radius;
        float alpha;
    };
private:
    bool IsFinished_;
    std::vector<Partical>particles_;
};

