#include <fieldcontextmenu.h>
#include <field.h>
#include <towerbuildbutton.h>
#include <tower.h>
#include <world.h>
 
FieldContextMenu::FieldContextMenu(Field* field)
{
	_field = field;

	addAllItems();
}

void FieldContextMenu::addAllItems()
{
	if (_field->isBuildable()){
		std::vector< osg::ref_ptr<TowerAttributes> >::iterator it;
		for(it = World::instance()->getMap()->getTowerAttributes()->begin();
			it != World::instance()->getMap()->getTowerAttributes()->end();
			it++)
		{
			this->addEntry(new TowerBuildButton(_field, it->get()));
		}
	}
}
