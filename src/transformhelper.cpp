#include <transformhelper.h>

#include <osg/Matrix>

/**
 * \fn	osg::Quat TransformHelper::lookAt(osg::Vec3 to)
 *
 * \brief	Convert the direction vector to a quaternion.
 *
 * \param	to	Direction vector.
 *
 * \return	The quaternion class.
 */
osg::Quat TransformHelper::lookAt(osg::Vec3 to)
{
	return lookAt(osg::Vec3(0.0, 0.0, 0.0), to);
}

/**
 * \fn	osg::Quat TransformHelper::lookAt(osg::Vec3 from, osg::Vec3 to)
 *
 * \brief	Calculate the necessary osg::Quat, so the model can rotated correct to look in right direction.
 *
 * \param	from	Position of the model.
 * \param	to  	Target view position of model.
 *
 * \return	The quaternion class.
 */
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