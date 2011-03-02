#include <osg/TextureCubeMap>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osg/ClearNode>
#include <osg/TexEnv>
#include <osg/TexGen>
#include <osg/TexMat>
#include <osg/Depth>
#include <osg/ShapeDrawable>
#include <osgUtil/CullVisitor>

#include <skybox.h>

// Update texture matrix for cubemaps
struct TexMatCallback : public osg::NodeCallback
{
public:

    TexMatCallback(osg::TexMat& tm) :
        _texMat(tm)
    {
    }

    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
        if (cv)
        {
            const osg::Matrix& MV = *(cv->getModelViewMatrix());
            const osg::Matrix R = osg::Matrix::rotate( osg::DegreesToRadians(112.0f), 0.0f,0.0f,1.0f)*
                                  osg::Matrix::rotate( osg::DegreesToRadians(90.0f), 1.0f,0.0f,0.0f);

            osg::Quat q = MV.getRotate();
            const osg::Matrix C = osg::Matrix::rotate( q.inverse() );

            _texMat.setMatrix( C*R );
        }

        traverse(node,nv);
    }

    osg::TexMat& _texMat;
};


class MoveEarthySkyWithEyePointTransform : public osg::Transform
{
public:
    /** Get the transformation matrix which moves from local coords to world coords.*/
    virtual bool computeLocalToWorldMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const 
    {
        osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
        if (cv)
        {
            osg::Vec3 eyePointLocal = cv->getEyeLocal();
            matrix.preMultTranslate(eyePointLocal);
        }
        return true;
    }

    /** Get the transformation matrix which moves from world coords to local coords.*/
    virtual bool computeWorldToLocalMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const
    {
        osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
        if (cv)
        {
            osg::Vec3 eyePointLocal = cv->getEyeLocal();
            matrix.postMultTranslate(-eyePointLocal);
        }
        return true;
    }
};

osg::TextureCubeMap* SkyBox::readCubeMap()
{
    osg::TextureCubeMap* cubemap = new osg::TextureCubeMap;
    #define CUBEMAP_FILENAME(face) "textures/Cubemap_snow/" #face ".jpg"

    osg::Image* imagePosX = osgDB::readImageFile(CUBEMAP_FILENAME(posx));
    osg::Image* imageNegX = osgDB::readImageFile(CUBEMAP_FILENAME(negx));
    osg::Image* imagePosY = osgDB::readImageFile(CUBEMAP_FILENAME(posy));
    osg::Image* imageNegY = osgDB::readImageFile(CUBEMAP_FILENAME(negy));
    osg::Image* imagePosZ = osgDB::readImageFile(CUBEMAP_FILENAME(posz));
    osg::Image* imageNegZ = osgDB::readImageFile(CUBEMAP_FILENAME(negz));

    if (imagePosX && imageNegX && imagePosY && imageNegY && imagePosZ && imageNegZ)
    {
        cubemap->setImage(osg::TextureCubeMap::POSITIVE_X, imagePosX);
        cubemap->setImage(osg::TextureCubeMap::NEGATIVE_X, imageNegX);
        cubemap->setImage(osg::TextureCubeMap::POSITIVE_Y, imagePosY);
        cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Y, imageNegY);
        cubemap->setImage(osg::TextureCubeMap::POSITIVE_Z, imagePosZ);
        cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Z, imageNegZ);

        cubemap->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
        cubemap->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
        cubemap->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);

        cubemap->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
        cubemap->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
    }

    return cubemap;
}

SkyBox::SkyBox()
{
	osg::StateSet* stateset = new osg::StateSet();

    osg::TexEnv* te = new osg::TexEnv;
    te->setMode(osg::TexEnv::REPLACE);
    stateset->setTextureAttributeAndModes(0, te, osg::StateAttribute::ON);

    osg::TexGen *tg = new osg::TexGen;
    tg->setMode(osg::TexGen::NORMAL_MAP);
    stateset->setTextureAttributeAndModes(0, tg, osg::StateAttribute::ON);

    osg::TexMat *tm = new osg::TexMat;
    stateset->setTextureAttribute(0, tm);

    osg::TextureCubeMap* skymap = readCubeMap();
    stateset->setTextureAttributeAndModes(0, skymap, osg::StateAttribute::ON);

    stateset->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
    stateset->setMode( GL_CULL_FACE, osg::StateAttribute::OFF );

    // clear the depth to the far plane.
    osg::Depth* depth = new osg::Depth;
    depth->setFunction(osg::Depth::ALWAYS);
    depth->setRange(1.0,1.0);   
    stateset->setAttributeAndModes(depth, osg::StateAttribute::ON );

    stateset->setRenderBinDetails(-1,"RenderBin");

    osg::Drawable* drawable = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),1));

    osg::Geode* geode = new osg::Geode;
    geode->setCullingActive(false);
    geode->setStateSet( stateset );
    geode->addDrawable(drawable);


    osg::Transform* transform = new MoveEarthySkyWithEyePointTransform;
    transform->setCullingActive(false);
    transform->addChild(geode);

    this->setCullCallback(new TexMatCallback(*tm));
    this->addChild(transform);
}