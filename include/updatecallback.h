#pragma once
#include <osgDB/Callbacks>
#include <osg/Node>

/**
 * \class	UpdateCallback
 *
 * \brief	Update callback. 
 */
class UpdateCallback : public osg::NodeCallback
{
public:
	void operator()(osg::Node* node, osg::NodeVisitor* nv);
};
