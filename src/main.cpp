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

#include <osg/Billboard>
#include <osg/BlendFunc>
#include <osg/AlphaFunc>
#include <osgDB/Registry>
#include <osg/PositionAttitudeTransform>

#include <map.h>

#define TERRAIN_BLOCK_SIZE 1

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


//elevation im bogenmass
#define MIN_ELEVATION 0.1

#define WHEEL_ZOOM_FACTOR -0.1

osg::Geode* CreateTerrainBlock()
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

	osg::Vec2Array* texcoords = new osg::Vec2Array(4);
	(*texcoords)[0].set(0.0, 0.0);
	(*texcoords)[1].set(1.0, 0.0f);
	(*texcoords)[2].set(1.0, 1.0);
	(*texcoords)[3].set(0.0, 1.0);
   terrainBlockGeometry->setTexCoordArray(0,texcoords);

	return terrainBlock;
}

osg::Geode* CreateEmptyTerrainBlock()
{
	osg::Geode* geode = CreateTerrainBlock();

	osg::Image* grasImage = osgDB::readImageFile("textures/gras.jpg");
	osg::Texture2D* grasTexture = new osg::Texture2D(grasImage);
	grasTexture->setDataVariance(osg::Object::STATIC);

	osg::StateSet* state = new osg::StateSet();
	state->setTextureAttributeAndModes(0, grasTexture, osg::StateAttribute::ON);
	geode->setStateSet(state);

	return geode;
}

osg::Geode* CreateWayTerrainBlock()
{
	osg::Geode* geode = CreateTerrainBlock();

	osg::Image* grasImage = osgDB::readImageFile("textures/way.jpg");
	osg::Texture2D* grasTexture = new osg::Texture2D(grasImage);
	grasTexture->setDataVariance(osg::Object::STATIC);

	osg::StateSet* state = new osg::StateSet();
	state->setTextureAttributeAndModes(0, grasTexture, osg::StateAttribute::ON);
	geode->setStateSet(state);

	return geode;
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
	manipulator->setVerticalAxisFixed(true);
	manipulator->setWheelZoomFactor(WHEEL_ZOOM_FACTOR);
	manipulator->setAllowThrow(false);

	viewer.setCameraManipulator(manipulator);

	//activate 4x multisample
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
