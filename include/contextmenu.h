// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#pragma once

#include <osg/Billboard>
#include <menubutton.h>
#include <constants.h>

class Contextmenu : public osg::Billboard
{
public:
Contextmenu();
   int numberOfEntries;
   void addEntry(void (* _onClick)(osg::ref_ptr<MenuButton>),
					  const std::string texturepath);
   MenuButton* createMenuItem(osg::StateSet* state, int offset);
};
