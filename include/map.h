#include <string>
#include <vector>
#include <SimpleIni.h>
#include <osg/Texture2D>

#include <field_node.h>

struct MapPoint{
	long X;
	long Y;
};

class Map
{	
	public:
		Map(const std::string& filename);
		long getWidth();
		long getHeight();
		FieldNode* getFieldBlock(unsigned int x, unsigned int y);
	private:
		void loadMap();
		void loadTextures();
		void loadModels();
		void loadFields();
		void loadFieldBlock(const char* sectionKey, unsigned char fieldBlockIndex);
		void loadCheckPoints();

		long _height;
		long _width;
		std::vector<std::string> _fields;
		std::vector<osg::ref_ptr<osg::Texture2D>> _textures;
		std::vector<osg::ref_ptr<osg::Node>> _models;
		CSimpleIni _ini;
		osg::ref_ptr<FieldNode> _fieldBlocks[MAXBYTE + 1];
		std::vector<MapPoint> _checkpoints;
};