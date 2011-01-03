#include <string>
#include <vector>
#include <SimpleIni.h>
#include <osg/Texture2D>

#include <field.h>

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

		Field* getField(unsigned int x, unsigned int y);
	private:
		void loadMap();
		void loadTextures();
		void loadModels();
		void loadFields();
		void loadFieldTypes(const char* sectionKey, unsigned char fieldTypeIndex);
		void loadCheckPoints();

		void createFields();

		long _height;
		long _width;
		std::vector<std::vector<osg::ref_ptr<Field>>> _fields;
		std::vector<osg::ref_ptr<osg::Texture2D>> _textures;
		std::vector<osg::ref_ptr<osg::Node>> _models;
		CSimpleIni _ini;
		osg::ref_ptr<FieldType> _fieldTypes[MAXBYTE + 1];
		std::vector<MapPoint> _checkpoints;
};