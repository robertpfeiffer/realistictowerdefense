#include <string>
#include <SimpleIni.h>

class Map
{	
	public:
		Map(const std::string& filename);
		unsigned int getWidth();
		unsigned int getHeight();
	private:
		void loadMap();

		unsigned int Height;
		unsigned int Width;
		CSimpleIni Ini;
};