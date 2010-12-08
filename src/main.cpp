#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osgDB/ReadFile> 
#include <osgViewer/Viewer>
#include <osg/PositionAttitudeTransform>
#include <osgGA/TrackballManipulator>

#include <map.h>

#define TERRAIN_BLOCK_SIZE 10

osg::Geode* CreateColoredTerrainBlock(osg::Vec4 color)
{
	osg::Geode* terrainBlock = new osg::Geode();
	osg::Geometry* terrainBlockGeometry = new osg::Geometry();

	terrainBlock->addDrawable(terrainBlockGeometry);

    osg::Vec3Array* terrainBlockVertices = new osg::Vec3Array;
    terrainBlockVertices->push_back( osg::Vec3( 0, 0, 0) );
    terrainBlockVertices->push_back( osg::Vec3(TERRAIN_BLOCK_SIZE, 0, 0) );
    terrainBlockVertices->push_back( osg::Vec3(TERRAIN_BLOCK_SIZE,TERRAIN_BLOCK_SIZE, 0) );
    terrainBlockVertices->push_back( osg::Vec3( 0,TERRAIN_BLOCK_SIZE, 0) );

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
    for(unsigned int x = 0; x < map.GetWidth(); x++)
	{
		for(unsigned int z = 0; z < map.GetHeight(); z++) {
			osg::PositionAttitudeTransform* terrainBlockTransform = new osg::PositionAttitudeTransform();
			terrain->addChild(terrainBlockTransform);
			if(x == 5)
				terrainBlockTransform->addChild(wayTerrain);
			else
				terrainBlockTransform->addChild(emptyTerrain);
			osg::Vec3 terrainBlockTranslation(x*TERRAIN_BLOCK_SIZE, z*TERRAIN_BLOCK_SIZE, 0);
			terrainBlockTransform->setPosition(terrainBlockTranslation);
		}
	}

    // switch off lighting as we haven't assigned any normals.
    root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

    //The final step is to set up and enter a simulation loop.

    viewer.setSceneData( root );
    //viewer.run();

    viewer.setCameraManipulator(new osgGA::TrackballManipulator());
    viewer.realize();

    while( !viewer.done() )
    {
        viewer.frame();
    }

    return 0;
}
