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
    enum Size
    {
        LARGE,
        MEDIUM,
        SMALL
    };
    Enemy(int segment);
    Enemy(Size size, int segment);
    Enemy(const Vector2D& pos, const Vector2D& vel, Size size, int segment);
    void Draw() override;
    void Update() override;
    bool IsAlive() { return isAlive_; }
    float GetRadius() const{ return radius_; }
    void Kill() { isAlive_ = false; }

    Size GetSize() { return size_; }
    Size CheckSize() const;
    float RandomRadius(Size size);

private:
    void MakeShape();
private:
    int segment_;
    float radius_;
    float angle_;
    float omega_;
    bool isAlive_;
    std::vector<Vector2D>vertex_;

    Size size_;
};

