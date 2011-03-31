#include <fieldcontextmenu.h>

#include <field.h>
#include <map.h>
#include <towerbuildbutton.h>
#include <tower.h>
#include <world.h>

/**
 * \fn	FieldContextMenu::FieldContextMenu(Field* field)
 *
 * \brief	Create a context menu for a field.
 *
 * \param	field	The field.
 */
FieldContextMenu::FieldContextMenu(Field* field)
{
	_field = field;

	addAllItems();
}

/**
 * \fn	void FieldContextMenu::addAllItems()
 *
 * \brief	Add build buttons for towers.
 */
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
