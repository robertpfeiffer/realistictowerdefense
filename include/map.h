#include <string>
#include <vector>
#include <SimpleIni.h>

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

		unsigned int Height;
		unsigned int Width;
		std::vector<std::string> Fields;
		CSimpleIni Ini;
};