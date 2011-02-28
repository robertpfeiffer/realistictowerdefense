#include<osg/Node>
#include<osg/Group>

class Graveyard
{
public:
	static Graveyard* instance();

	void setWorld(osg::Group* world);

	void burryAll();
	void killChild(osg::Node* child);
private:
	std::vector< osg::ref_ptr<osg::Node> > _killList;
	osg::Group* _world;
};
