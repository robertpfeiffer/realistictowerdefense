// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <hatchery.h>

/**
 * @class Hatchery
 * Helper class for Creep spawning.
 * This class implements the singleton pattern.
 */

#include <world.h>

/**	Gets the instance of the Hatchery (singleton).
 */
Hatchery* Hatchery::instance()
{
	static Hatchery yard;
	return &yard;
}

void Hatchery::releaseAll()
{
	while(_birthList.size() > 0)
	{
		World::instance()->addUpdatableNode(_birthList.back());
		_birthList.pop_back();
	}
}

void Hatchery::enqueueChild(osg::Node* child)
{
	_birthList.push_back(child);
}
