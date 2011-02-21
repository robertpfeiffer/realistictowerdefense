#include<graveyard.h>

Graveyard::Graveyard()
{
	_killList = new std::vector<osg::ref_ptr<osg::Node>>();
}

Graveyard::~Graveyard()
{
	burryAll();
	delete _killList;
}

Graveyard* Graveyard::instance()
{
	static Graveyard yard;
	return &yard;
}

void Graveyard::burryAll()
{
	_killList->clear();
}

void Graveyard::killChild(osg::Group* group, osg::Node* child)
{
	_killList->push_back(osg::ref_ptr<osg::Node>(child));
	group->removeChild(child);
}