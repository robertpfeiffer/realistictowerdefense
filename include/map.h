#include <string>
#include <vector>
#include <SimpleIni.h>
#include <osg/Texture2D>

#include <field_block.h>

class Map
{	
	public:
		Map(const std::string& filename);
		long getWidth();
		long getHeight();
		FieldBlock* getFieldBlock(unsigned int x, unsigned int y);
	private:
		void loadMap();
		void loadTextures();
		void loadModels();
		void loadFields();
		void loadFieldBlock(const char* sectionKey, unsigned char fieldBlockIndex);

		long _height;
		long _width;
		std::vector<std::string> _fields;
		std::vector<osg::ref_ptr<osg::Texture2D>> _textures;
		std::vector<osg::ref_ptr<osg::Node>> _models;
		CSimpleIni _ini;
		osg::ref_ptr<FieldBlock> _fieldBlocks[MAXBYTE];
};