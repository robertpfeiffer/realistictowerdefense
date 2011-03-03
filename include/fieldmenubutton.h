#include <menubutton.h>

class Field;
class TowerAttributes;

class FieldMenuButton : public MenuButton
{
public:
	FieldMenuButton(Field* f, TowerAttributes* ta);

	virtual void onClick(osgGA::GUIActionAdapter& aa);

private:
	Field* _field;
	TowerAttributes* _towerattributes;
};