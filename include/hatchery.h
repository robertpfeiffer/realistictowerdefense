#pragma once

#include<osg/Node>

/**
 * \class	Hatchery
 *
 * \brief	Hatchery. 
 */
class Hatchery
{
public:
	static Hatchery* instance();

	void releaseAll();
	void enqueueChild(osg::Node* child);
private:
	std::vector< osg::ref_ptr<osg::Node> > _birthList;
};
