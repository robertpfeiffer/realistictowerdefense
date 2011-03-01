#include <osg/PositionAttitudeTransform>

class TowerAttributes : public osg::Referenced
{
public:
	float cooldown;
	int cost;
	std::string name;

	osg::ref_ptr<osg::PositionAttitudeTransform> model;
};