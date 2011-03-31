#pragma once
#include <infobox.h>

class CreepAttributes;

/**
 * \class	CreepInfoBox
 *
 * \brief	Creep information box. 
 */
class CreepInfoBox : public InfoBox
{
public:
	CreepInfoBox(CreepAttributes* attributes);
};