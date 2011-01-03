#include <field.h>

Field::Field(FieldType* fieldType)
{
	_fieldType = fieldType;

	this->addChild(fieldType);
}

bool Field::setBuilding()
{
	FieldType* fieldType = _fieldType.get();
	if (!fieldType->isBuildable())
	{
		return false;
	}

	//TODO: now add building

	return true;
}