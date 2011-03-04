#include <osg/Geode>
#include <osgText/Text>

class HudElement : public osg::Geode
{
public:
	HudElement(osg::Vec2 position, std::string text);

	void setText(const std::string text);

private:
	osg::ref_ptr<osgText::Text> _text;
};