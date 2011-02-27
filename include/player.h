#pragma once

class Player
{
public:
	Player() : _lifes(1), _money(0) {};
	inline int getLifes() { return _lifes; };
	inline void setLifes(int value) { _lifes = value; };
	inline void addLife() { _lifes += 1; };
	inline void decreaseLifes() { _lifes -= 1; };

	inline int getMoney() { return _lifes; };
	inline void setMoney(int value) { _money = value; };
	inline void addMoney(int value) { _money += value; };
	inline void decreaseMoney(int value) { _money -= value; };
private:
	int _lifes;
	int _money;
};