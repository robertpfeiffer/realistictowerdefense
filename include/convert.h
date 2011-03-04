#include <string>
#include <sstream>
#include "rapidxml.hpp"

using namespace rapidxml;

class Convert
{
public:
	static std::string toString(int i);
	static bool Convert::attrToBool(xml_attribute<>* attr, bool defaultValue);
	static long Convert::attrToLong(xml_attribute<>* attr, long defaultValue);
	static double Convert::attrToDouble(xml_attribute<>* attr, double defaultValue);
};