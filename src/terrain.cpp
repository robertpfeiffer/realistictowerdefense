#include <osg/Group>
#include <osg/PositionAttitudeTransform>

#include <terrain.h>

Terrain::Terrain(Map* map) : osg::Group()
{
	_map = map;

	for(long x = 0; x < _map->getWidth(); x++)
	{
		for(long y = 0; y < _map->getHeight(); y++) {
			this->addChild(createTerrainBlock(x, y));
		}
	}
}

osg::Node* Terrain::createTerrainBlock(int x, int y)
{
	osg::PositionAttitudeTransform* terrainBlockTransform = new osg::PositionAttitudeTransform();
	terrainBlockTransform->addChild(_map->getField(x, y));

	osg::Vec3 terrainBlockTranslation(x, -y, 0);
	terrainBlockTransform->setPosition(terrainBlockTranslation);

	return terrainBlockTransform;
}
