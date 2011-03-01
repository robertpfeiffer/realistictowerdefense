#pragma once

#include <osg/Referenced>
#include <osg/Texture2D>

#include <modeldata.h>

class FieldType : public osg::Referenced
{
public:
	FieldType(osg::Texture2D* texture, ModelData* modelData = NULL, bool isBuildable = true);

	inline osg::ref_ptr<ModelData> getModelData() {return _modelData.get();};
	inline osg::ref_ptr<osg::Geode> getGround() {return _ground.get();};
	inline bool isBuildable() {return _isBuildable;};
private:
	osg::ref_ptr<ModelData> _modelData;
	osg::ref_ptr<osg::Geode> _ground;
	bool _isBuildable;
};