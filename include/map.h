#include <string>

class Map
{	
	public:
		Map(const std::string& filename);
		unsigned int GetWidth();
		unsigned int GetHeight();
	private:
		unsigned int Height;
		unsigned int Width;
};