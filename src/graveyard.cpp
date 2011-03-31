// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <graveyard.h>
#include <world.h>

/**
 * @class Graveyard
 * Helper class for Creep removal.
 * This class implements the singleton pattern.
 */


/**	Gets the instance of the Graveyard (singleton).
 */
Graveyard* Graveyard::instance()
{
	static Graveyard yard;
	return &yard;
}

void Graveyard::burryAll()
{
	while(_killList.size() > 0)
	{
		World::instance()->removeChild(_killList.back());
		_killList.pop_back();
	}
}

void Graveyard::killChild(osg::Node* child)
{
	_killList.push_back(child);
}
