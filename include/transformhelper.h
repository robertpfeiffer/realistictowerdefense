#include <osg/Vec3>
#include <osg/Quat>

/**
 * \class	TransformHelper
 *
 * \brief	Transform helper. 
 */
static class TransformHelper
{
public:
	static osg::Quat lookAt(osg::Vec3 to);
	static osg::Quat lookAt(osg::Vec3 from, osg::Vec3 to);
};