#pragma once

#include <osg/Timer>

class GameTimer
{
	public:
		static GameTimer* instance();

		GameTimer();

		double nextFrame();
		double elapsedTime();
		inline void togglePause() {_paused = !_paused;};
		inline void pause() {_paused = true; };
		inline void resume() {_paused = false; };
		inline bool isPaused() { return _paused; };
	private:
		osg::Timer* _timer;
		bool _paused;
		double _elapsedTime;
		osg::Timer_t _offsetTicks;
};