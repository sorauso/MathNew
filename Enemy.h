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
private:
    void MakeShape();
private:
    int segment_;
    float radius_;
    std::vector<Vector2D>vertex_;
};

