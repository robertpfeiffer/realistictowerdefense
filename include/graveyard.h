#pragma once

#include<osg/Node>

/**
 * \class	Graveyard
 *
 * \brief	Graveyard for nodes. 
 */
class Graveyard
{
public:
	static Graveyard* instance();

	void burryAll();
	void killChild(osg::Node* child);
private:
	std::vector< osg::ref_ptr<osg::Node> > _killList;
};
