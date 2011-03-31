#include <creepinfobox.h>

#include <convert.h>
#include <creepattributes.h>

/**
 * \fn	CreepInfoBox::CreepInfoBox(CreepAttributes* attributes)
 *
 * \brief	Create a info box for a creep.
 *
 * \param	attributes	If non-null, the attributes.
 */
CreepInfoBox::CreepInfoBox(CreepAttributes* attributes) : InfoBox("Creep")
{
	this->addEntry("Name", attributes->name);
	this->addSpacing();
	this->addEntry("Max. Health", Convert::toString(attributes->maxHealth));
	this->addEntry("Armor", Convert::toString(attributes->armor));
	this->addEntry("Magic Resistance", Convert::toString(attributes->magicResistance));
	this->addSpacing();
	this->addEntry("Speed", Convert::toString(attributes->speed));
}