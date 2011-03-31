#include <convert.h>

using namespace rapidxml;

#ifndef WIN32
	#include <strings.h>
	#include <string.h>
	#define _strcmpi strcasecmp
#endif

/**
 * \fn	std::string Convert::toString(int i)
 *
 * \brief	Convert an integer value into a string representation.
 *
 * \param	i	The integer value.
 *
 * \return	A string representation of the integer value.
 */
std::string Convert::toString(int i)
{
	std::stringstream out;
	out << i;
	return out.str();
}

/**
 * \fn	std::string Convert::toString(long l)
 *
 * \brief	Convert an long value into a string representation.
 *
 * \param	l	The long value.
 *
 * \return	A string representation of the long value.
 */
std::string Convert::toString(long l)
{
	std::stringstream out;
	out << l;
	return out.str();
}

/**
 * \fn	std::string Convert::toString(double d)
 *
 * \brief	Convert an double value into a string representation.
 *
 * \param	d	The double value.
 *
 * \return	A string representation of the double value.
 */
std::string Convert::toString(double d)
{
	std::stringstream out;
	out << d;
	return out.str();
}

/**
 * \fn	bool Convert::attrToBool(xml_attribute<>* attr, bool defaultValue)
 *
 * \brief	Convert an XML attribute to bool.
 *
 * \param	attr	If non-null, the attribute.
 * \param	defaultValue	default value.
 *
 * \return	The detected value if it succeeds, else the default value.
 */
bool Convert::attrToBool(xml_attribute<>* attr, bool defaultValue)
{
	if (attr == NULL) return defaultValue;
	char* str = attr->value();

	if (strcmp(str, "0") == 0) return false;
	if (strcmp(str, "1") == 0) return true;

	if (_strcmpi(str, "false") == 0) return false;
	if (_strcmpi(str, "true") == 0) return true;

	return defaultValue;
}

/**
 * \fn	long Convert::attrToLong(xml_attribute<>* attr, long defaultValue)
 *
 * \brief	Convert an XML attribute to long.
 *
 * \param	attr	If non-null, the attribute.
 * \param	defaultValue	default value.
 *
 * \return	The detected value if it succeeds, else the default value.
 */
long Convert::attrToLong(xml_attribute<>* attr, long defaultValue)
{
	if (attr == NULL) return defaultValue;
	char* str = attr->value();

	long val = atol(str);
	if (val == 0)
	{
		if (strcmp(str, "0") != 0) return defaultValue;
	}
	return val;	
}

/**
 * \fn	double Convert::attrToDouble(xml_attribute<>* attr, double defaultValue)
 *
 * \brief	Convert an XML attribute to double.
 *
 * \param	attr	If non-null, the attribute.
 * \param	defaultValue	default value.
 *
 * \return	The detected value if it succeeds, else the default value.
 */
double Convert::attrToDouble(xml_attribute<>* attr, double defaultValue)
{
	if (attr == NULL) return defaultValue;
	char* str = attr->value();

	double val = atof(str);
	if (val == 0.0)
	{
		if (strcmp(str, "0.0") != 0) return defaultValue;
	}
	return val;	
}
