// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <hatchery.h>
#include <world.h>

/**
 * \fn	Hatchery* Hatchery::instance()
 *
 * \brief	Gets the instance (singleton).
 *
 * \return	The hatchery.
 */

Hatchery* Hatchery::instance()
{
	static Hatchery yard;
	return &yard;
}

/**
 * \fn	void Hatchery::releaseAll()
 *
 * \brief	Add all nodes of hatchery to scenegraph.
 */
void Hatchery::releaseAll()
{
	while(_birthList.size() > 0)
	{
		World::instance()->addUpdatableNode(_birthList.back());
		_birthList.pop_back();
	}
}

/**
 * \fn	void Hatchery::enqueueChild(osg::Node* child)
 *
 * \brief	Enqueue child to born after finishing current frame.
 *
 * \param	child	The child.
 */
void Hatchery::enqueueChild(osg::Node* child)
{
	_birthList.push_back(child);
}
