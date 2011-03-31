#pragma once
#include <osg/Geometry>

/**
 * \class	HudBackground
 *
 * \brief	Hud background. 
 */
class HudBackground : public osg::Geometry
{
public:
	HudBackground(osg::BoundingBox dimensions, double padding);

	void setDimensions(osg::BoundingBox);

private:
	double _padding;
};