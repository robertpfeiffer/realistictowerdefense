// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#include <creepcallback.h>
#include <creep.h>

void CreepCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
	Creep* myCreep = dynamic_cast<Creep*>(node);
	if (myCreep != NULL)
	{
		myCreep->OnUpdate();
	}
	traverse(node, nv);
}
