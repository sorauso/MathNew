#pragma once
#include "Bace.h"
#include <vector>

namespace
{
    const int SEGMENT_NUM = 8;

}

class Enemy :
    public Bace
{
public:
    Enemy(int segment);
    void Draw() override;
    void Update() override;
    bool IsAlive() { return isAlive_; }
    Vector2D GetPos() { return pos_; }
    float GetRadius() { return radius_; }
    void Kill() { isAlive_ = false; }
private:
    void MakeShape();
private:
    int segment_;
    float radius_;
    float angle_;
    float omega_;
    bool isAlive_;
    std::vector<Vector2D>vertex_;
};

