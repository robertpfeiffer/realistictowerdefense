// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <graveyard.h>
#include <world.h>

/**
 * \fn	Graveyard* Graveyard::instance()
 *
 * \brief	Gets the instance (singleton).
 *
 * \return	The graveyard.
 */
Graveyard* Graveyard::instance()
{
	static Graveyard yard;
	return &yard;
}

/**
 * \fn	void Graveyard::burryAll()
 *
 * \brief	Remove all nodes of graveyard from scenegraph at end of frame.
 */
void Graveyard::burryAll()
{
	while(_killList.size() > 0)
	{
		World::instance()->removeChild(_killList.back());
		_killList.pop_back();
	}
}

/**
 * \fn	void Graveyard::killChild(osg::Node* child)
 *
 * \brief	Kill child.
 *
 * \param	child	The child.
 */
void Graveyard::killChild(osg::Node* child)
{
	_killList.push_back(child);
}
