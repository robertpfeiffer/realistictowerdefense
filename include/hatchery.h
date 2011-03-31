#pragma once

#include<osg/Node>

/**
 * \class	Hatchery
 *
 * \brief	Hatchery.
 * 			At end of a frame the nodes will be added to the scenegraph.
 * 			This class implements the singleton pattern. 
 */
class Hatchery
{
public:
	static Hatchery* instance();

	void releaseAll();
	void enqueueChild(osg::Node* child);
private:
	Hatchery() {}; //to make sure it is a singleton
	std::vector< osg::ref_ptr<osg::Node> > _birthList;
};
