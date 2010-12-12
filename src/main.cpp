#include <osg/DisplaySettings>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/Texture2D>
#include <osgDB/ReadFile> 
#include <osgViewer/Viewer>
#include <osg/PositionAttitudeTransform>
#include <osgGA/TerrainManipulator>

#include <map.h>

#define TERRAIN_BLOCK_SIZE 1

//elevation im bogenmass
#define MIN_ELEVATION 0.1

#define WHEEL_ZOOM_FACTOR -0.1

osg::Geode* CreateColoredTerrainBlock(osg::Vec4 color)
{
	osg::Geode* terrainBlock = new osg::Geode();
	osg::Geometry* terrainBlockGeometry = new osg::Geometry();

	terrainBlock->addDrawable(terrainBlockGeometry);

    osg::Vec3Array* terrainBlockVertices = new osg::Vec3Array;
    terrainBlockVertices->push_back( osg::Vec3( 0, 0, 0) );
    terrainBlockVertices->push_back( osg::Vec3(TERRAIN_BLOCK_SIZE, 0, 0) );
    terrainBlockVertices->push_back( osg::Vec3(TERRAIN_BLOCK_SIZE, TERRAIN_BLOCK_SIZE, 0) );
    terrainBlockVertices->push_back( osg::Vec3( 0, TERRAIN_BLOCK_SIZE, 0) );

    terrainBlockGeometry->setVertexArray(terrainBlockVertices);

    osg::DrawElementsUInt* terrainBlockPrimitiveSet = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
    terrainBlockPrimitiveSet->push_back(3);
    terrainBlockPrimitiveSet->push_back(2);
    terrainBlockPrimitiveSet->push_back(1);
    terrainBlockPrimitiveSet->push_back(0);
    terrainBlockGeometry->addPrimitiveSet(terrainBlockPrimitiveSet);

    osg::Vec4Array* terrainBlockColors = new osg::Vec4Array;
    terrainBlockColors->push_back(color);

    terrainBlockGeometry->setColorArray(terrainBlockColors);
	terrainBlockGeometry->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);

	return terrainBlock;
}

osg::Geode* CreateEmptyTerrainBlock()
{
	return CreateColoredTerrainBlock(osg::Vec4(0.0f, 0.7f, 0.0f, 1.0f));
}

osg::Geode* CreateWayTerrainBlock()
{
	return CreateColoredTerrainBlock(osg::Vec4(0.3f, 0.2f, 0.1f, 1.0f));
}

void LimitElevation(osgGA::TerrainManipulator* manipulator)
{
	if(manipulator->getElevation() > -MIN_ELEVATION) {
		manipulator->setElevation(MIN_ELEVATION);
	}
}

int main()
{
    osgViewer::Viewer viewer;
    osg::Group* root = new osg::Group();
	 osg::Group* world = new osg::Group();
	  osg::Group* terrain = new osg::Group();
	 osg::Group* hud = new osg::Group();

	root->addChild(world);
	root->addChild(hud);

	world->addChild(terrain);
	
	osg::Geode* emptyTerrain = CreateEmptyTerrainBlock();
	osg::Geode* wayTerrain = CreateWayTerrainBlock();

	Map map("maps/default.map");
    for(unsigned int x = 0; x < map.getWidth(); x++)
	{
		for(unsigned int y = 0; y < map.getHeight(); y++) {
			osg::PositionAttitudeTransform* terrainBlockTransform = new osg::PositionAttitudeTransform();
			terrain->addChild(terrainBlockTransform);
			switch(map.getField(x, y))
			{
				case INI_FIELD_WAY:
					terrainBlockTransform->addChild(wayTerrain);
					break;
				default:
					terrainBlockTransform->addChild(emptyTerrain);
					break;
			}
			osg::Vec3 terrainBlockTranslation(x*TERRAIN_BLOCK_SIZE, y*TERRAIN_BLOCK_SIZE, 0);
			terrainBlockTransform->setPosition(terrainBlockTranslation);
		}
	}

    // switch off lighting as we haven't assigned any normals.
    root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

    //The final step is to set up and enter a simulation loop.

    viewer.setSceneData( root );

    osgGA::TerrainManipulator* manipulator = new osgGA::TerrainManipulator();
	manipulator->setVerticalAxisFixed(true);
	manipulator->setWheelZoomFactor(WHEEL_ZOOM_FACTOR);
	manipulator->setAllowThrow(false);

	viewer.setCameraManipulator(manipulator);

	//activate 4x multisample (works only in debug modus :? )
	osg::DisplaySettings* displaySettings = new osg::DisplaySettings;
    displaySettings->setNumMultiSamples(4);
	viewer.setDisplaySettings(displaySettings);

    viewer.realize();

    while( !viewer.done() )
    {
		LimitElevation(manipulator);
        viewer.frame();
    }

    return 0;
}
