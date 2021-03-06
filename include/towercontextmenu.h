#include <contextmenu.h>
#include <osg/ref_ptr>

class Tower;

/**
 * \class	TowerContextMenu
 *
 * \brief	Context menu of twoer. 
 */
class TowerContextMenu : public ContextMenu
{
public:
	TowerContextMenu(Tower* tower);

private:
	void addAllItems();

	osg::ref_ptr<Tower> _tower;
};