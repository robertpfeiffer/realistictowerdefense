#include <contextmenu.h>
#include <osg/ref_ptr>

class Field;

class FieldContextMenu : public ContextMenu
{
public:
	FieldContextMenu(Field* field);

private:
	void addAllItems();

	osg::ref_ptr<Field> _field;
};