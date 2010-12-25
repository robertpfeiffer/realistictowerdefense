#include <osg/Geode>
#include <osg/Texture2D>

class FieldBlock : public osg::Group
{
	public:
		FieldBlock(bool isBuildable, bool isAccessible, osg::Texture2D* texture, osg::Node* model = NULL);

		bool isBuildable();
		bool isAccessible();

	private:
		bool _isBuildable;
		bool _isAccessible;
		osg::ref_ptr<osg::Node> _model;
		osg::ref_ptr<osg::Geode> _ground;
};