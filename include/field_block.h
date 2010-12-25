#include <osg/Geode>
#include <osg/Texture2D>

class FieldBlock : public osg::Geode
{
	public:
		FieldBlock(bool isBuildable, bool isAccessible, osg::Texture2D* texture);

		bool isBuildable();
		bool isAccessible();

	private:
		bool _isBuildable;
		bool _isAccessible;
		osg::ref_ptr<osg::Texture2D> _texture;
};