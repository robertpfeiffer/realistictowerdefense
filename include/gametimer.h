#pragma once

#include <osg/Timer>

/**
 * \class	GameTimer
 *
 * \brief	A game timer, which can paused and change the game speed.
 * 			Implements the singleton pattern.
 */
class GameTimer
{
public:
	static GameTimer* instance();

	void nextFrame();
	double elapsedTime();
	inline void togglePause() {_paused = !_paused;};
	inline void pause() {_paused = true; };
	inline void resume() {_paused = false; };
	inline bool isPaused() { return _paused; };

	void increaseSpeed();
	void normalizeSpeed();
	void decreaseSpeed();
private:
	GameTimer();
	osg::Timer* _timer;
	bool _paused;
	double _elapsedTime;
	osg::Timer_t _offsetTicks;

	int _speed;

	static double _hardLimit;
	static int _defaultSpeed;
};