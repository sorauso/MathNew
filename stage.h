#pragma once
#include <vector>

class Player;
class Stage
{
public:
	Stage();
	~Stage();
	void Init();
	void Update();
	void Draw();
	void Release();
private:
	Player* palyer_;
};