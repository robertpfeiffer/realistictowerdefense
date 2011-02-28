#include<graveyard.h>

Graveyard* Graveyard::instance()
{
	static Graveyard yard;
	return &yard;
}

void Graveyard::burryAll()
{
	while(_killList.size() > 0) {
		world->removeChild(_killList.back());
		_killList.pop_back();
	}
	_killList.clear();
}

void Graveyard::killChild(osg::Group* group, osg::Node* child)
{
	world = group;
	_killList.push_back(child);
}