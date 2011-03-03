#include <menubutton.h>

class Tower;
class TowerAttributes;

class TowerUpgradeButton : public MenuButton
{
public:
	TowerUpgradeButton(Tower* t, TowerAttributes* ta);

	virtual void onClick(osgGA::GUIActionAdapter& aa);

private:
	Tower* _tower;
	TowerAttributes* _towerattributes;
};