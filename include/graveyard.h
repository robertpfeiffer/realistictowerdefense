#pragma once

#include<osg/Node>

class Graveyard
{
public:
	static Graveyard* instance();

	void burryAll();
	void killChild(osg::Node* child);
private:
	std::vector< osg::ref_ptr<osg::Node> > _killList;
};
