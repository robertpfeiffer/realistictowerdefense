#pragma once

#include <osg/Referenced>
#include <osg/Texture2D>

class FieldType : public osg::Referenced
{
	public:
		FieldType(bool isBuildable, osg::Texture2D* texture, osg::Node* model = NULL);

		inline osg::ref_ptr<osg::Node> getModel() {return _model.get();};
		inline osg::ref_ptr<osg::Geode> getGround() {return _ground.get();};
		inline bool isBuildable() {return _isBuildable;};
	private:
		osg::ref_ptr<osg::Node> _model;
		osg::ref_ptr<osg::Geode> _ground;
		bool _isBuildable;
};