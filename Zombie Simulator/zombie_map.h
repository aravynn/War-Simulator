#pragma once

#include <vector>

#include "zmap_cell.h"
#include "random.h"
#include <math.h>
#include "army.h"


class zombie_map
{
private: 
	int m_width{ 100 }; // map width, default to 100
	int m_height{ 100 }; // map height, default to 100
	random<int> mr; // random number generator, external object.
	std::vector<zmap_cell> m_spaces; // vector of all map spaces.

	army red;
	army blue;



public:
	zombie_map(int x, int y);
	void day();

	const std::vector<zmap_cell> & getMap();

	int getWidth() { return m_width; }
	int getHeight() { return m_height; }
};

