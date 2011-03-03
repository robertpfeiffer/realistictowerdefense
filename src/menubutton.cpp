#include <menubutton.h>

MenuButton::MenuButton(osg::StateSet* ss)
{
	// Declare and assign texture coordinates.
	osg::Vec2Array* texCoords = new osg::Vec2Array(4);
	(*texCoords)[0].set(0.0f,0.0f);
	(*texCoords)[1].set(1.0f,0.0f);
	(*texCoords)[2].set(1.0f,1.0f);
	(*texCoords)[3].set(0.0f,1.0f);
	this->setTexCoordArray(0,texCoords);
	
	this->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));

	this->setStateSet(ss);

	osg::Vec4Array* colors = new osg::Vec4Array();
    colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	this->setColorArray(colors);
	this->setColorBinding(osg::Geometry::BIND_OVERALL);
}

void MenuButton::onClick(osgGA::GUIActionAdapter& aa)
{
  if(this->_onClick != NULL)
      (this->_onClick)(this);
}
