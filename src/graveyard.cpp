#include<graveyard.h>

Graveyard* Graveyard::instance()
{
	static Graveyard yard;
	return &yard;
}

void Graveyard::burryAll()
{
	_killList.clear();
}

void Graveyard::killChild(osg::Group* group, osg::Node* child)
{
	_killList.push_back(child);
	group->removeChild(child);
}