#include <osg/Geode>

class InfoBox : public osg::Geode
{
public:
	InfoBox(const std::string title);

	void setPosition(osg::Vec2);
};