#include <updatecallback.h>
#include <updatablenode.h>

/**
 * \fn	void UpdateCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
 *
 * \brief	Call update method on update traversing.
 */
void UpdateCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
	UpdatableNode* myNode = dynamic_cast<UpdatableNode*>(node);
	if (myNode != NULL)
	{
		myNode->onUpdate();
	}
	traverse(node, nv);
}