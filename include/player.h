#pragma once
#include <osg/Referenced>

class Player : public osg::Referenced
{
public:
	inline int getLives() { return _lives; };
	inline void setLives(int value) { _lives = value; };
	inline void addLife() { _lives--; };
	inline void takeLife() { _lives++; };

	inline int getMoney() { return _money; };
	inline void setMoney(int value) { _money = value; };
	inline void addMoney(int value) { _money += value; };
	inline void decreaseMoney(int value) { _money -= value; };
private:
	int _lives;
	int _money;
};