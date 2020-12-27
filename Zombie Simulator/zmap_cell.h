#pragma once

#include "random.h"
#include <vector>
#include "army.h"

class zmap_cell
{
private:
	int m_armyRed{ 0 };
	int m_armyBlue{ 0 };
	int m_resource{ 0 };
	int m_regen{ 0 };
	bool m_exists = true; // cell does not exist, do nothing with it.


public: 
	zmap_cell() {} // default constructor does nothing.
	zmap_cell(random<int>& r, int red = 0, int blue = 0); // call an external 
	zmap_cell(bool NotReal);
	~zmap_cell();

	int GetRedArmy() { return m_armyRed; }
	int GetBlueArmy() { return m_armyBlue; }
	int GetResource() { return m_resource; }
	
	bool exists() { return m_exists; }
	
	void processDay(bool hasRed, bool hasBlue, army& red, army& blue);

	std::vector<unsigned char> getColorCast();

};

