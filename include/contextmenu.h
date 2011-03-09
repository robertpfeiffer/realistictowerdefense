// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
#pragma once

#include <osg/Billboard>

class MenuButton;

class ContextMenu : public osg::Billboard
{
public:
	ContextMenu();
	void addEntry(void (* _onClick)(osg::ref_ptr<MenuButton>), const std::string texturepath);
	void addEntry(MenuButton* button);

private:
	int _numberOfEntries;

	osg::Vec2 positionForNextButton();
};
