#pragma once

#include <osg/PositionAttitudeTransform>
#include <osgText/Text>
#include <updatablenode.h>

class InSceneText : public osg::PositionAttitudeTransform, public UpdatableNode
{
public:
	InSceneText(osgText::String text, osg::Vec3 position);
	void onUpdate();

private:
	float _distanceLeft;
};