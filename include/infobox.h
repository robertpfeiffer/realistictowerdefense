#pragma once
#include <osg/PositionAttitudeTransform>
#include <osgText/Text>

#include <hudbackground.h>

class InfoBox : public osg::PositionAttitudeTransform
{
public:
	InfoBox(const std::string title);

	void addEntry(const std::string, const std::string);
	void addSpacing();

	friend class Hud;

private:
	osgText::Text* createTitle(const std::string);
	osgText::Text* createEntryTitle(const std::string);
	osgText::Text* createEntryDescription(const std::string);
	void updateLayout();

	std::vector< osg::ref_ptr<osgText::Text> > _entryTitles;
	std::vector< osg::ref_ptr<osgText::Text> > _entryDescriptions;

	osg::ref_ptr<osg::Geode> _geode;
	osg::ref_ptr<HudBackground> _background;
	osg::ref_ptr<osgText::Text> _title;

	static double _padding;
	static double _entryOffset;
	static double _entryIndent;
	static double _entryLineHeight;
	static double _entrySpacing;
};