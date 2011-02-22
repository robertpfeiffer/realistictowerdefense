#include<osg\Node>
#include<osg\Group>

class Graveyard
{
public:
	static Graveyard* instance();

	void burryAll();
	void killChild(osg::Group* group, osg::Node* child);
private:
	std::vector<osg::ref_ptr<osg::Node>> _killList;
};