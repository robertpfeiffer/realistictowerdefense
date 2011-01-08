#include <constants.h>
#include <world.h>

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
#include <constants.h>

osg::Drawable* World::createPin(const float & scale, osg::StateSet* bbState)
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

osg::Billboard* World::addBillBoards()
{
	osg::Billboard* pinBillBoard = new osg::Billboard();

	pinBillBoard->setMode(osg::Billboard::AXIAL_ROT);
	pinBillBoard->setAxis(osg::Vec3(0.0f,0.0f,1.0f));
	pinBillBoard->setNormal(osg::Vec3(0.0f,-1.0f,0.0f));

	osg::Texture2D *pinTexture = new osg::Texture2D;
	pinTexture->setImage(osgDB::readImageFile("textures/pin.png"));
	pinTexture->setMaxAnisotropy(AF_LEVEL);

	osg::Texture2D *kingpinTexture = new osg::Texture2D;
	kingpinTexture->setImage(osgDB::readImageFile("textures/kingpin.png"));
	kingpinTexture->setMaxAnisotropy(AF_LEVEL);
 
	osg::StateSet* billBoardStateSet = new osg::StateSet;
	billBoardStateSet->setTextureAttributeAndModes
		(0, pinTexture, osg::StateAttribute::ON);
	osg::BlendFunc *blendFunc = new osg::BlendFunc;
	billBoardStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	billBoardStateSet->setAttributeAndModes( blendFunc, osg::StateAttribute::ON );
	billBoardStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);


	osg::StateSet* kingbillBoardStateSet = new osg::StateSet;
	kingbillBoardStateSet->setTextureAttributeAndModes
		(0, kingpinTexture, osg::StateAttribute::ON);
	kingbillBoardStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
		kingbillBoardStateSet->setAttributeAndModes( blendFunc, osg::StateAttribute::ON );
	kingbillBoardStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);


	osg::Drawable* shrub1Drawable = createPin( 0.4f, billBoardStateSet);
	osg::Drawable* shrub2Drawable = createPin( 0.4f, billBoardStateSet);
	osg::Drawable* shrub3Drawable = createPin( 0.6f, kingbillBoardStateSet);

	// Add these drawables to our billboard at various positions
	pinBillBoard->addDrawable( shrub1Drawable , osg::Vec3(12,-3,0) );
	pinBillBoard->addDrawable( shrub2Drawable , osg::Vec3(10,-18,0));
	pinBillBoard->addDrawable( shrub3Drawable , osg::Vec3(6,-10,0) );

	return pinBillBoard;
}

osg::Node* World::createTerrainBlockFromMap(Map* map, int x, int y)
{
	osg::PositionAttitudeTransform* terrainBlockTransform = new osg::PositionAttitudeTransform();
	terrainBlockTransform->addChild(map->getField(x, y));

	osg::Vec3 terrainBlockTranslation(x, -y, 0);
	terrainBlockTransform->setPosition(terrainBlockTranslation);

	return terrainBlockTransform;
}

World::World() : osg::Group()
{
	osg::Group* terrain = new osg::Group();

	Map map("maps/default.map");
    for(long x = 0; x < map.getWidth(); x++)
	{
		for(long y = 0; y < map.getHeight(); y++) {
			terrain->addChild(createTerrainBlockFromMap(&map, x, y));
		}
	}

	this->addChild(addBillBoards());
	this->addChild(terrain);
}