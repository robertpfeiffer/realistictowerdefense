#include <transformhelper.h>

#include <osg/Matrix>

osg::Quat TransformHelper::lookAt(osg::Vec3 to)
{
	return lookAt(osg::Vec3(0.0, 0.0, 0.0), to);
}

osg::Quat TransformHelper::lookAt(osg::Vec3 from, osg::Vec3 to)
{
	osg::Quat quad;
    osg::Matrix matrix;
	matrix.makeLookAt(from, to, osg::Z_AXIS);
	matrix *= matrix.rotate(osg::DegreesToRadians(90.0), osg::Vec3(1.0, 0.0, 0.0));
	matrix *= matrix.rotate(osg::DegreesToRadians(-90.0), osg::Vec3(0.0, 0.0, 1.0));
	matrix.get(quad);
	return quad.inverse();
}