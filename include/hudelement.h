#include <osg/Geode>
#include <osgText/Text>

class HudElement : public osg::Geode
{
public:
	HudElement(osg::Vec2 position, std::string text);

	void setText(const std::string text);

private:
	void updateBackgroundBounds();

	void setupText(osg::Vec2);
	void setupBackground();

	osg::ref_ptr<osgText::Text> _text;
	osg::ref_ptr<osg::Geometry> _background;

	static double _backgroundPadding;
};