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
#include <skyboxattributes.h>

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

osg::Image* SkyBox::getImageFromTexture(osg::Texture2D* texture)
{
	return (texture != NULL) ? texture->getImage() : NULL;
}

osg::TextureCubeMap* SkyBox::readCubeMap(SkyBoxAttributes* attributes)
{
    osg::TextureCubeMap* cubemap = new osg::TextureCubeMap;
	
    cubemap->setImage(osg::TextureCubeMap::POSITIVE_X, getImageFromTexture(attributes->texturePosX));
    cubemap->setImage(osg::TextureCubeMap::NEGATIVE_X, getImageFromTexture(attributes->textureNegX));
    cubemap->setImage(osg::TextureCubeMap::POSITIVE_Y, getImageFromTexture(attributes->texturePosY));
    cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Y, getImageFromTexture(attributes->textureNegY));
    cubemap->setImage(osg::TextureCubeMap::POSITIVE_Z, getImageFromTexture(attributes->texturePosZ));
    cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Z, getImageFromTexture(attributes->textureNegZ));

    cubemap->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
    cubemap->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
    cubemap->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);

    cubemap->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
    cubemap->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);

    return cubemap;
}

SkyBox::SkyBox(SkyBoxAttributes* attributes)
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

    osg::TextureCubeMap* skymap = readCubeMap(attributes);
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