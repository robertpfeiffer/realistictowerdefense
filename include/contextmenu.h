// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#pragma once

#include <osg/Billboard>
#include <menubutton.h>
#include <constants.h>

class ContextMenu : public osg::Billboard
{
public:
	ContextMenu();
	void addEntry(void (* _onClick)(osg::ref_ptr<MenuButton>), const std::string texturepath);

private:
	int _numberOfEntries;

	MenuButton* createMenuItem(const std::string texturepath);
};
