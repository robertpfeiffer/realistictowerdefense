#pragma once

#include <osg/PositionAttitudeTransform>
#include <osgText/Text>
#include <updatablenode.h>

/**
 * \class	InSceneText
 *
 * \brief	In scene text. 
 */
class InSceneText : public osg::PositionAttitudeTransform, public UpdatableNode
{
public:
	InSceneText(const std::string text, osg::Vec3 position);
	void onUpdate();
	void setColor(osg::Vec3 color);

private:
	float _distanceLeft;
	osg::ref_ptr<osgText::Text> _text;

	static float _totalDistance;
	static float _stepSize;
};