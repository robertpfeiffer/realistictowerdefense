// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include<graveyard.h>

Graveyard* Graveyard::instance()
{
	static Graveyard yard;
	return &yard;
}

void Graveyard::setWorld(osg::Group* world)
{
	_world = world;
}

void Graveyard::burryAll()
{
	while(_killList.size() > 0)
	{
		_world->removeChild(_killList.back());
		_killList.pop_back();
	}
}

void Graveyard::killChild(osg::Node* child)
{
	_killList.push_back(child);
}
