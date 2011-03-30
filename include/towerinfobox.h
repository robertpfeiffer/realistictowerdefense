#pragma once
#include <infobox.h>

class TowerAttributes;

/**
 * \class	TowerInfoBox
 *
 * \brief	Tower information box. 
 */
class TowerInfoBox : public InfoBox
{
public:
	TowerInfoBox(TowerAttributes* attributes);
};