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