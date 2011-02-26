#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osgGA/TerrainManipulator>

#include <osg/Billboard>
#include <osg/BlendFunc>
#include <osg/AlphaFunc>
#include <osgDB/Registry>

#include <constants.h>
#include <map.h>
#include <terrain.h>
#include <world.h>

osg::Drawable* World::createTestPin(const float & scale, osg::StateSet* bbState)
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

osg::Node* World::createTestBillboard()
{
	osg::Billboard* pinBillBoard = new osg::Billboard();

	pinBillBoard->setMode(osg::Billboard::AXIAL_ROT);
	pinBillBoard->setAxis(osg::Vec3(0.0f,0.0f,1.0f));
	pinBillBoard->setNormal(osg::Vec3(0.0f,-1.0f,0.0f));

	osg::Texture2D *kingpinTexture = new osg::Texture2D;
	kingpinTexture->setImage(osgDB::readImageFile("textures/kingpin.png"));
	kingpinTexture->setMaxAnisotropy(AF_LEVEL);
 
	osg::BlendFunc *blendFunc = new osg::BlendFunc;
	osg::StateSet* kingbillBoardStateSet = new osg::StateSet;
	kingbillBoardStateSet->setTextureAttributeAndModes
		(0, kingpinTexture, osg::StateAttribute::ON);
	kingbillBoardStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
		kingbillBoardStateSet->setAttributeAndModes( blendFunc, osg::StateAttribute::ON );
	kingbillBoardStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

	osg::Drawable* shrub3Drawable = createTestPin( 0.6f, kingbillBoardStateSet);

	pinBillBoard->addDrawable( shrub3Drawable , osg::Vec3(0,0,0) );

	return pinBillBoard;
}

void World::createPath()
{
	std::vector<OpenSteer::Vec3> pathPoints = std::vector<OpenSteer::Vec3>();
	std::vector<MapPoint>* checkpoints = _map->getCheckpoints();
	pathPoints.resize(_map->getCheckpoints()->size());
	for(unsigned int i=0;i < _map->getCheckpoints()->size(); i++)
	{
		if(i==0)
		{
			_spawnPosition = osg::Vec3((float) ((*checkpoints)[i].X), 0.0, (float) ((*checkpoints)[i].Y));
		}
		pathPoints[i] = OpenSteer::Vec3((float) ((*checkpoints)[i].X), 0.0, (float) -((*checkpoints)[i].Y));
	}
	
	_path = new OpenSteer::PolylineSegmentedPathwaySingleRadius(pathPoints.size(), pathPoints.data(), 0.5, false);
}

void World::spawnCreep(osg::Node* style)
{
	Creep* myCreep = new Creep(*_proximities, _spawnPosition, _path, new CreepEventHandler(this));
	CreepAttributes attr = CreepAttributes();
	attr.speed = 100;
	myCreep->setCreepStats(&attr);
	myCreep->addChild(style);
	this->addChild(myCreep);
	myCreep->setUpdateCallback(new CreepCallback());
}

World::World(const std::string mapFilename) : osg::Group()
{
	_map = new Map(mapFilename);
	_proximities = new ProximityDatabase();

	createPath();

	spawnCreep(createTestBillboard());

	this->addChild(new Terrain(_map));
}

World::~World()
{
	delete _path;
	//delete _proximities; //uncomment for crash -> crash because memory is still in use by creeps
}