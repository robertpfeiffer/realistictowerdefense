#include <menubutton.h>

class Field;
class TowerAttributes;

class TowerBuildButton : public MenuButton
{
public:
	TowerBuildButton(Field* f, TowerAttributes* ta);

	virtual void onClick(osgGA::GUIActionAdapter& aa);

private:
	Field* _field;
	TowerAttributes* _towerattributes;
};