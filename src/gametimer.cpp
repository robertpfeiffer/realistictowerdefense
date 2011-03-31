// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <gametimer.h>
#include <algorithm>

double GameTimer::_hardLimit = 0.5;
int GameTimer::_defaultSpeed = 2;

GameTimer* GameTimer::instance()
{
	static GameTimer timer;
	return &timer;
}

GameTimer::GameTimer()
{
	_timer = osg::Timer::instance();
	_elapsedTime = 0;
	_offsetTicks = _timer->tick();
	_paused = false;
	_speed = _defaultSpeed;
}

void GameTimer::nextFrame()
{
	osg::Timer_t _newOffsetTicks = _timer->tick();
	_elapsedTime = _timer->delta_s(_offsetTicks, _newOffsetTicks);
	_offsetTicks = _newOffsetTicks;
}

double GameTimer::elapsedTime()
{
	if (_paused)
	{
		return 0;
	}

	return std::min(_speed * _elapsedTime, _hardLimit);
}

void GameTimer::increaseSpeed()
{
	_speed *= 2;
}

void GameTimer::normalizeSpeed()
{
	_speed = _defaultSpeed;
}

void GameTimer::decreaseSpeed()
{
	if (_speed > 1)
	{
		_speed /= 2;
	}
}
