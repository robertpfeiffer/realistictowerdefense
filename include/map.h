#pragma once

#include <string>
#include <vector>
#include <queue>
#include <osg/Texture2D>
#include <rapidxml.hpp>

class Wave;

#include <player.h>
#include <modeldata.h>
#include <field.h>
#include <skyboxattributes.h>
#include <towerattributes.h>

using namespace rapidxml;

struct MapPoint{
	long X;
	long Y;
};

class Map : public osg::Referenced
{	
public:
	Map();
	Map(const std::string& filename);
	long getWidth();
	long getHeight();

	Field* getField(unsigned int x, unsigned int y);
	std::vector<MapPoint>* getCheckpoints();
	osg::Texture2D* getStrataTexture();
	inline Player* getPlayer() { return &_player; };
	inline std::queue< osg::ref_ptr<Wave> >* getWaves() { return &_waves; };
	inline std::vector< osg::ref_ptr<TowerAttributes> >* getTowerAttributes() { return &_towers; };
	inline SkyBoxAttributes* getSkyBoxAttributes() { return &_skyBoxAttributes; } ;
private:
	bool _attrToBool(xml_attribute<>* attr, bool defaultValue) const;
	long _attrToLong(xml_attribute<>* attr, long defaultValue) const;
	float _attrToFloat(xml_attribute<>* attr, float defaultValue) const;
	void _reset();
	void _cleanup();
	void _load(const std::string& filename);				// +
	void _loadPlayer(xml_node<> *node);						// - +
	void _loadWaves(xml_node<> *node);						// - +
	void _loadTowers(xml_node<> *node);						// - +
	TowerAttributes* _getTowerAttributes(xml_node<> *node);	// - - *
	void _loadSkyBox(xml_node<> *node);						// - 
	void _loadTerrain(xml_node<> *node);					// - +
	void _loadStrata(xml_node<> *node);						// - - +
	void _loadFieldTypes(xml_node<> *node);					// - - +
	ModelData* _readModel(xml_node<> *node);				// - - - +
	void _loadGrid(xml_node<> *node);						// - - +		
	void _loadCheckPoints(xml_node<> *node);				// +
	
	osg::Texture2D* _getTexture(xml_attribute<> *attr);

	osg::Texture2D* _getTexture(const char* filename);
	osg::Node* _getModel(const char* filename);

	osg::ref_ptr<osg::Texture2D> _strata;

	xml_document<> _xml;
	std::vector<MapPoint> _checkpoints;
	osg::ref_ptr<FieldType> _fieldTypes[256];
	Player _player;

	long _height;
	long _width;
	std::queue< osg::ref_ptr<Wave> > _waves;
	std::vector< std::vector< osg::ref_ptr<Field> > > _fields;

	std::vector< osg::ref_ptr<TowerAttributes> > _towers;

	SkyBoxAttributes _skyBoxAttributes;
};
