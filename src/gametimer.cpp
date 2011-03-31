// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <gametimer.h>
#include <algorithm>

double GameTimer::_hardLimit = 0.5;
int GameTimer::_defaultSpeed = 2;

/**
 * \fn	GameTimer* GameTimer::instance()
 *
 * \brief	Gets the instance of the game timer (singleton).
 *
 * \return	The game timer.
 */
GameTimer* GameTimer::instance()
{
	static GameTimer timer;
	return &timer;
}

/**
 * \fn	GameTimer::GameTimer()
 *
 * \brief	Default constructor.
 */

GameTimer::GameTimer()
{
	_timer = osg::Timer::instance();
	_elapsedTime = 0;
	_offsetTicks = _timer->tick();
	_paused = false;
	_speed = _defaultSpeed;
}

/**
 * \fn	void GameTimer::nextFrame()
 *
 * \brief	Calculate new elapsed time since last frame.
 */

void GameTimer::nextFrame()
{
	osg::Timer_t _newOffsetTicks = _timer->tick();
	_elapsedTime = _timer->delta_s(_offsetTicks, _newOffsetTicks);
	_offsetTicks = _newOffsetTicks;
}

/**
 * \fn	double GameTimer::elapsedTime()
 *
 * \brief	Gets the elapsed time.
 *
 * \author	Sun Black
 * \date	31.03.2011
 *
 * \return	0 if game is paused, else elapsed game time.
 */
double GameTimer::elapsedTime()
{
	if (_paused)
	{
		return 0;
	}

	return std::min(_speed * _elapsedTime, _hardLimit);
}

/**
 * \fn	void GameTimer::increaseSpeed()
 *
 * \brief	Increase game speed.
 */

void GameTimer::increaseSpeed()
{
	_speed *= 2;
}

/**
 * \fn	void GameTimer::normalizeSpeed()
 *
 * \brief	Reset game speed.
 */
void GameTimer::normalizeSpeed()
{
	_speed = _defaultSpeed;
}

/**
 * \fn	void GameTimer::decreaseSpeed()
 *
 * \brief	Decrease game speed.
 */
void GameTimer::decreaseSpeed()
{
	if (_speed > 1)
	{
		_speed /= 2;
	}
}
