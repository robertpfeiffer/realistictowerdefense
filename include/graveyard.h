#pragma once

#include<osg/Node>

/**
 * \class	Graveyard
 *
 * \brief	Graveyard for nodes. 
 * 			This class implements the singleton pattern.
 */
class Graveyard
{
public:
	static Graveyard* instance();

	void burryAll();
	void killChild(osg::Node* child);
private:
	Graveyard() {};  //to make sure it is a singleton
	std::vector< osg::ref_ptr<osg::Node> > _killList;
};
