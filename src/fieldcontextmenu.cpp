#include <fieldcontextmenu.h>

#include <field.h>
#include <tower.h>
#include <world.h>

FieldContextMenu::FieldContextMenu(Field* field)
{
	_field = field;

	addAllItems();
}

//callback for menuitem (needs to be replaced with something more generic)
void build_tower(osg::ref_ptr<MenuButton> button)
{
	Field* f = (dynamic_cast<Field*> (button->getParent(0)->getParent(0)));
	
	if(f != NULL){
		f->setBuilding(new Tower(f->getPosition(), World::instance()->getMap()->getTowerAttributes()->front()));
	}
}

void FieldContextMenu::addAllItems()
{
	if (_field->isBuildable()){
		this->addEntry(build_tower, "tower.png");
		this->addEntry(build_tower, "tower.png");
		this->addEntry(build_tower, "tower.png");
		this->addEntry(build_tower, "tower.png");
	}
}