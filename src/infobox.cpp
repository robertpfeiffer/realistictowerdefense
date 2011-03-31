#include <infobox.h>

double InfoBox::_padding = 0.8;
double InfoBox::_entryOffset = 2.0;
double InfoBox::_entryIndent = 0.5;
double InfoBox::_entryLineHeight = 2.2;
double InfoBox::_entrySpacing = 1.0;

/**
 * \fn	InfoBox::InfoBox(const std::string title)
 *
 * \brief	Create a info box.
 *
 * \param	title	The title.
 */
InfoBox::InfoBox(const std::string title)
{
	_geode = new osg::Geode();
	_title = createTitle(title);
	_background = new HudBackground(osg::BoundingBox(), _padding);

	this->addChild(_geode);
	_geode->addDrawable(_title);
	_geode->addDrawable(_background);

	updateLayout();
}

/**
 * \fn	osgText::Text* InfoBox::createTitle(const std::string title)
 *
 * \brief	Creates a title aligned to left top.
 *
 * \param	title	The title.
 *
 * \return	The text.
 */
osgText::Text* InfoBox::createTitle(const std::string title)
{
	osgText::Text* text = new osgText::Text();
	text->setCharacterSize(2);
	text->setFont("fonts/DejaVuSans.ttf");
	text->setAxisAlignment(osgText::Text::SCREEN);
	text->setAlignment(osgText::Text::LEFT_TOP);
	text->setColor(osg::Vec4(1.0, 1.0, 0.0, 1.0));
	text->setPosition(osg::Vec3(0.0, 0.0, 0.0));
	text->setText(osgText::String(title));

	return text;
}

/**
 * \fn	osgText::Text* InfoBox::createEntryTitle(const std::string title)
 *
 * \brief	Creates a title aligned to left base line.
 *
 * \param	title	The title.
 *
 * \return	The text.
 */
osgText::Text* InfoBox::createEntryTitle(const std::string title)
{
	osgText::Text* t = new osgText::Text();
	t->setCharacterSize(2);
	t->setFont("fonts/DejaVuSans.ttf");
	t->setAxisAlignment(osgText::Text::SCREEN);
	t->setAlignment(osgText::Text::LEFT_BASE_LINE);
	t->setColor(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	t->setText(osgText::String(title));

	return t;
}

/**
 * \fn	void InfoBox::addEntry(const std::string title, const std::string description)
 *
 * \brief	Adds an entry to the info box.
 *
 * \param	title	   	The title.
 * \param	description	The description.
 */

void InfoBox::addEntry(const std::string title, const std::string description)
{
	osgText::Text* titleText = createEntryTitle(title);
	osgText::Text* descriptionText = createEntryTitle(description);
	this->_entryTitles.push_back(titleText);
	this->_entryDescriptions.push_back(descriptionText);

	double offset = -(_entryOffset + _entryTitles.size() * _entryLineHeight);
	titleText->setPosition(osg::Vec3(_entryIndent, offset, 0.0));
	descriptionText->setPosition(osg::Vec3(0.0, offset, 0.0)); //indent will be computed when layout is done (bounding-box workaround)

	_geode->addDrawable(titleText);
	_geode->addDrawable(descriptionText);
}

/**
 * \fn	void InfoBox::addSpacing()
 *
 * \brief	Adds spacing line to info box.
 */
void InfoBox::addSpacing()
{
	addEntry("", "");
}

/**
 * \fn	void InfoBox::updateLayout()
 *
 * \brief	Updates the layout.
 */
void InfoBox::updateLayout()
{
	//compute indent for descriptions
	osg::BoundingBox titleBound;
	for(unsigned int i=0 ; i < _entryTitles.size() ; i++)
	{
		titleBound.expandBy(_entryTitles[i]->getBound());
	}
	for(unsigned int i=0 ; i < _entryDescriptions.size() ; i++)
	{
		osg::Vec3 pos = _entryDescriptions[i]->getPosition();
		_entryDescriptions[i]->setPosition(osg::Vec3(titleBound.xMax() + _entrySpacing, pos.y(), pos.z()));
	}

	//complete bounding box
	osg::BoundingBox bb;
	bb.expandBy(_title->getBound());
	bb.expandBy(osg::Vec3(20.0, -10.0, 0.0)); // minWidth and Height
	for(unsigned int i=0 ; i < _entryDescriptions.size() ; i++)
	{
		bb.expandBy(_entryDescriptions[i]->getBound());
	}

	_background->setDimensions(bb);
}
