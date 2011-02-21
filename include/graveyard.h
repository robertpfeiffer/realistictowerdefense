#include<osg\Node>

class Graveyard
{
public:
	static Graveyard* instance();

	void burryAll();
	void killFrom(osg::Node* node, osg::Group* group);

private:
	Graveyard();
};