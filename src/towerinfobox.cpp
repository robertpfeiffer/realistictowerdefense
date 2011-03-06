#include <towerinfobox.h>

#include <convert.h>
#include <towerattributes.h>

TowerInfoBox::TowerInfoBox(TowerAttributes* attributes) : InfoBox("Tower")
{
	this->addEntry("Name", attributes->name);
	this->addSpacing();
	this->addEntry("Physical Damage", Convert::toString(attributes->projectile.physicalDamage));
	this->addEntry("Magical Damage", Convert::toString(attributes->projectile.magicalDamage));
	this->addSpacing();
	this->addEntry("Range", Convert::toString(attributes->range));
	this->addEntry("Cooldown", Convert::toString(attributes->cooldown));
	this->addSpacing();
	this->addEntry("Cost", Convert::toString(attributes->cost));
}