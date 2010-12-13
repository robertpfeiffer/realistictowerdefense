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
		long getWidth();
		long getHeight();
		char getField(unsigned int x, unsigned int y);
	private:
		void loadMap();
		void loadTextures();
		void loadFields();

		long Height;
		long Width;
		std::vector<std::string> Fields;
		std::vector<osg::Texture2D*> Textures;
		CSimpleIni Ini;
};