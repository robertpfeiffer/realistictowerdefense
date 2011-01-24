#include <field.h>

Field::Field(FieldType* fieldType) : _isBuildable(fieldType->isBuildable()), _ground(fieldType->getGround()), _model(fieldType->getModel())
{
	this->addChild(_ground.get());

	if (_model != NULL)
	{
		this->addChild(_model.get());
	}
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