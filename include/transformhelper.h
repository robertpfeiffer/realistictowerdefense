#include <osg/Vec3>
#include <osg/Quat>

class TransformHelper
{
public:
	static osg::Quat lookAt(osg::Vec3 to);
	static osg::Quat lookAt(osg::Vec3 from, osg::Vec3 to);
};