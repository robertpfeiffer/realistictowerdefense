#include <menubutton.h>

#include <assetlibrary.h>
#include <osg/BlendFunc>

MenuButton::MenuButton(const std::string texturepath)
{
	this->setPosition(osg::Vec2(0.0, 0.0));

	// Declare and assign texture coordinates.
	osg::Vec2Array* texCoords = new osg::Vec2Array(4);
	(*texCoords)[0].set(0.0f,0.0f);
	(*texCoords)[1].set(1.0f,0.0f);
	(*texCoords)[2].set(1.0f,1.0f);
	(*texCoords)[3].set(0.0f,1.0f);
	this->setTexCoordArray(0,texCoords);
	
	this->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));

	this->setStateSet(createStateSetFromTexturePath(texturepath));

	osg::Vec4Array* colors = new osg::Vec4Array();
    colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	this->setColorArray(colors);
	this->setColorBinding(osg::Geometry::BIND_OVERALL);
}

void MenuButton::setPosition(osg::Vec2 pos)
{
	osg::Vec3Array* verts = new osg::Vec3Array(4);
	(*verts)[0] = osg::Vec3( pos.x() - 0.5, 0, pos.y() - 0.5);
	(*verts)[1] = osg::Vec3( pos.x() + 0.5, 0, pos.y() - 0.5);
	(*verts)[2] = osg::Vec3( pos.x() + 0.5, 0, pos.y() + 0.5);
	(*verts)[3] = osg::Vec3( pos.x() - 0.5, 0, pos.y() + 0.5);
	this->setVertexArray(verts);
}

void MenuButton::setCallback(void (* callback)(osg::ref_ptr<MenuButton>))
{
	this->_onClick = callback;
}

void MenuButton::onClick(osgGA::GUIActionAdapter& aa)
{
  if(this->_onClick != NULL)
      (this->_onClick)(this);
}

osg::StateSet* MenuButton::createStateSetFromTexturePath(const std::string texturepath)
{
	osg::StateSet* stateSet = new osg::StateSet;

	stateSet->setTextureAttributeAndModes(0, AssetLibrary::instance()->getTexture(texturepath), osg::StateAttribute::ON);
	stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	stateSet->setAttributeAndModes(new osg::BlendFunc(), osg::StateAttribute::ON);
	stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF); //always on top

	return stateSet;
}