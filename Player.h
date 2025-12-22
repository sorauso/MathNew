#pragma once
#include "Bace.h"
class Player :
    public Bace
{
private:
    Vector2D vertex_[3];
    float angle_;
    float omega_;
    Vector2D dir_;
    float radius_;
public:
    Player();
    Player(Vector2D pos, Vector2D vel, int color,Vector2D dir,float omega ,float radius);
    ~Player();
    void Draw() override;
    void Update() override;
};