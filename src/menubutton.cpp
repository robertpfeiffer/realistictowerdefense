#include <menubutton.h>

#include <assetlibrary.h>
#include <osg/BlendFunc>

MenuButton::MenuButton(const std::string texturepath)
{
	init(AssetLibrary::instance()->getTexture(texturepath));
}

MenuButton::MenuButton(osg::Texture2D* texture)
{
	init(texture);
}

void MenuButton::init(osg::Texture2D* texture)
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

	this->setStateSet(createStateSetFromTexture(texture));

	_enabled = false; //avoid optimisation
	setEnabled(true);
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

void MenuButton::setEnabled(bool enabled)
{
	if(enabled == _enabled)
	{
		return;
	}

	_enabled = enabled;

	osg::Vec4Array* colors = new osg::Vec4Array();
	if(_enabled)
	{
		colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else
	{
		colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 0.5f));
	}

	this->setColorArray(colors);
	this->setColorBinding(osg::Geometry::BIND_OVERALL);
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

osg::StateSet* MenuButton::createStateSetFromTexture(osg::Texture2D* texture)
{
	osg::StateSet* stateSet = new osg::StateSet;

	stateSet->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
	stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	stateSet->setAttributeAndModes(new osg::BlendFunc(), osg::StateAttribute::ON);
	stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF); //always on top

	return stateSet;
}

/*
 * The following code is a hack to prevent a menubutton from being deleted by osg.
 * The reason is, that the UserInteractionHandler will try to call onUnhover.
 * If the button is removed before this call would fail.
 * There seems no nice possibility to resolve this, because a MouseEventHandler is not osg::referenced
 * by itself and making it referenced will cause 'ambigious'-errors.
 *
 * GC is prevented using a self-referencing ref_ptr.
 */

void MenuButton::preventGC()
{
	_gcPreventor = this;
}

void MenuButton::allowGC()
{
	_gcPreventor = NULL;
}