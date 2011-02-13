#include <game_timer.h>

GameTimer* GameTimer::instance()
{
	static GameTimer s_Timer;
	return &s_Timer;
}

GameTimer::GameTimer()
{
	_timer = osg::Timer::instance();
	_elapsedTime = 0;
	_offsetTicks = _timer->tick();
	_paused = false;
	_timer->time_s();
}

double GameTimer::nextFrame()
{
	osg::Timer_t _newOffsetTicks = _timer->tick();
	_elapsedTime = _timer->delta_s(_offsetTicks, _newOffsetTicks);
	_offsetTicks = _newOffsetTicks;

	return _elapsedTime;
}

double GameTimer::elapsedTime()
{
	if (_paused)
	{
		return 0;
	}
	return _elapsedTime;
}