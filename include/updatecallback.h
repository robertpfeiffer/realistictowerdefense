#include <osgDB/Callbacks>
#include <osg/Node>

class UpdateCallback : public osg::NodeCallback
{
public:
	void operator()(osg::Node* node, osg::NodeVisitor* nv);
};
