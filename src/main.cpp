#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osgDB/ReadFile> 
#include <osgViewer/Viewer>
#include <osg/PositionAttitudeTransform>
#include <osgGA/TerrainManipulator>

#include <osg/Billboard>
#include <osg/BlendFunc>
#include <osg/AlphaFunc>
#include <osgDB/Registry>
#include <osg/PositionAttitudeTransform>

#include <map.h>

#define TERRAIN_BLOCK_SIZE 10

//elevation im bogenmass
#define MIN_ELEVATION 0.1

#define WHEEL_ZOOM_FACTOR -0.1

osg::Drawable* createPin(const float & scale, osg::StateSet* bbState)
{
   // Standard size shrub
   float width = 1.5f;
   float height = 3.0f;

   // Scale width and height based on 'scale' parameter
   width *= scale;
   height *= scale;

   // Declare and initialize geometry
   osg::Geometry* geometry = new osg::Geometry;

   // Declare an array of vertices, assign values so we can create a
   // quadrilateral centered relative to the Z axis
   osg::Vec3Array* verts = new osg::Vec3Array(4);
   (*verts)[0] = osg::Vec3(-width/2.0f, 0, 0);
   (*verts)[1] = osg::Vec3( width/2.0f, 0, 0);
   (*verts)[2] = osg::Vec3( width/2.0f, 0, height);
   (*verts)[3] = osg::Vec3(-width/2.0f, 0, height);
   geometry->setVertexArray(verts);

   // Declare and assign texture coordinates.
   osg::Vec2Array* texCoords = new osg::Vec2Array(4);
   (*texCoords)[0].set(0.0f,0.0f);
   (*texCoords)[1].set(1.0f,0.0f);
   (*texCoords)[2].set(1.0f,1.0f);
   (*texCoords)[3].set(0.0f,1.0f);
   geometry->setTexCoordArray(0,texCoords);

   // Add a primitive set (QUADS) to the geometry
   geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));

   // Make sure the geometry has the correct state
   geometry->setStateSet(bbState);


   osg::Vec4Array* colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
   geometry->setColorArray(colors);
   geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

   return geometry;
} 

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

 osg::Billboard* pinBillBoard = new osg::Billboard();
   world->addChild(pinBillBoard);
	world->addChild(terrain);
	
	osg::Geode* emptyTerrain = CreateEmptyTerrainBlock();
	osg::Geode* wayTerrain = CreateWayTerrainBlock();

	Map map("maps/default.map");
    for(unsigned int x = 0; x < map.GetWidth(); x++)
	{
		for(unsigned int y = 0; y < map.GetHeight(); y++) {
			osg::PositionAttitudeTransform* terrainBlockTransform = new osg::PositionAttitudeTransform();
			terrain->addChild(terrainBlockTransform);
			if(x == map.GetWidth()/2)
				terrainBlockTransform->addChild(wayTerrain);
			else
				terrainBlockTransform->addChild(emptyTerrain);
			osg::Vec3 terrainBlockTranslation(x*TERRAIN_BLOCK_SIZE, y*TERRAIN_BLOCK_SIZE, 0);
			terrainBlockTransform->setPosition(terrainBlockTranslation);
		}
	}

  

 

    // switch off lighting as we haven't assigned any normals.
    root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);


  pinBillBoard->setMode(osg::Billboard::AXIAL_ROT);
   pinBillBoard->setAxis(osg::Vec3(0.0f,0.0f,1.0f));
   pinBillBoard->setNormal(osg::Vec3(0.0f,-1.0f,0.0f));

    osg::Texture2D *ocotilloTexture = new osg::Texture2D;
   ocotilloTexture->setImage(osgDB::readImageFile("kingpin.png"));

   osg::StateSet* billBoardStateSet = new osg::StateSet;

   billBoardStateSet->setTextureAttributeAndModes
     (0, ocotilloTexture, osg::StateAttribute::ON);

   osg::BlendFunc *blendFunc = new osg::BlendFunc;
   billBoardStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

   billBoardStateSet->setAttributeAndModes( blendFunc, osg::StateAttribute::ON );
   billBoardStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

   osg::Drawable* shrub1Drawable = createPin( 10.0f, billBoardStateSet);
   osg::Drawable* shrub2Drawable = createPin( 20.0f, billBoardStateSet);
   osg::Drawable* shrub3Drawable = createPin( 12.0f, billBoardStateSet);

   // Add these drawables to our billboard at various positions
   pinBillBoard->addDrawable( shrub1Drawable , osg::Vec3(12,3,8) );
   pinBillBoard->addDrawable( shrub2Drawable , osg::Vec3(10,18,8));
   pinBillBoard->addDrawable( shrub3Drawable , osg::Vec3(6,10,8) );


    //The final step is to set up and enter a simulation loop.

    viewer.setSceneData( root );

    osgGA::TerrainManipulator* manipulator = new osgGA::TerrainManipulator();
	manipulator->setRotationMode(osgGA::TerrainManipulator::ELEVATION_AZIM);
	manipulator->setWheelZoomFactor(WHEEL_ZOOM_FACTOR);
	manipulator->setAllowThrow(false);

	viewer.setCameraManipulator(manipulator);
    viewer.realize();

    while( !viewer.done() )
    {
      LimitElevation(manipulator);
      viewer.frame();

    }

    return 0;
}
