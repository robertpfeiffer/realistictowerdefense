#include <menubutton.h>

class Tower;
class TowerAttributes;

class TowerUpgradeButton : public MenuButton
{
public:
	TowerUpgradeButton(Tower* t, TowerAttributes* ta);

	virtual void onClick(osgGA::GUIActionAdapter& aa);
	virtual void onHover(osgGA::GUIActionAdapter& aa);
	virtual void onUnhover();

private:
	Tower* _tower;
	TowerAttributes* _towerattributes;
};