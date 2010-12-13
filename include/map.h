#include <string>
#include <vector>
#include <SimpleIni.h>
#include <osg/Texture2D>

#define INI_FIELD_GRAS 'G'
#define INI_FIELD_TREE 'T'
#define INI_FIELD_WAY  '+'

class Map
{	
	public:
		Map(const std::string& filename);
		unsigned int getWidth();
		unsigned int getHeight();
		char getField(unsigned int x, unsigned int y);
	private:
		void loadMap();
		void loadTextures();

		unsigned int Height;
		unsigned int Width;
		std::vector<std::string> Fields;
		std::vector<osg::Texture2D*> Textures;
		CSimpleIni Ini;
};