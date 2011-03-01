#include <convert.h>

std::string Convert::toString(int i)
{
	std::stringstream out;
	out << i;
	return out.str();
}