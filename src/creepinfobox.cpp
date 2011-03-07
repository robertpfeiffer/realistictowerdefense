#include <creepinfobox.h>

#include <convert.h>
#include <creepattributes.h>

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