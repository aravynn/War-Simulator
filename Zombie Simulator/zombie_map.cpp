#include "zombie_map.h"

zombie_map::zombie_map(int x, int y) : m_width{ x }, m_height{ y }
{
	
	// generate the armies
	red = army(mr.num(1, 10), mr.num(6, 25), mr.num(5, 10), mr.num(1, 10), mr.num(1, 10));
	blue = army(mr.num(1, 10), mr.num(6, 25), mr.num(5, 10), mr.num(1, 10), mr.num(1, 10));
	
	//generate the intial map. Afer this, use day() to process another day.
	m_spaces.resize(m_width * m_height);
	
	//size_t redinit = mr.num(0, m_width * m_height - 1);
	//size_t blueinit = mr.num(0, m_width * m_height - 1);


	for (size_t i{ 0 }; i < m_spaces.size(); ++i){
		// generate a cell for every space
		
		int r = 0;
		int b = 0;

		int rando = mr.num(1, 250);

		if (rando == 1) {
			r = 255;
		}
		if (rando == 2) {
			b = 255;
		}
		if (rando == 3) {
			r = 255;
			b = 255;
		}


		//if (i == redinit) { r = 255; }
		//if (i == blueinit) { b = 255; }
		
		m_spaces.at(i) = zmap_cell(mr, r, b);
	}
	
	
}

const std::vector<zmap_cell>& zombie_map::getMap()
{
	// return reference to the map to use for later generation. 
	return m_spaces;
}

void zombie_map::day() {
	// process a day for the map. Means all things will move as appropriate. Must use PRE move numbers only, to be converted after. 

	std::vector<zmap_cell> oldSpaces = m_spaces; // stored copy for preventing overflow

	for (int x{ 0 }; x < m_width; ++x) {
		// row of cells
		for (int y{ 0 }; y < m_height; ++y) {

			bool hasRed = false;
			bool hasBlue = false;
			// process the kills, production, and usage within the space.
			if (x > 0) {
				if (oldSpaces.at((x - 1) * m_width + y).GetRedArmy() > 0) { hasRed = true; }
				if (oldSpaces.at((x - 1) * m_width + y).GetBlueArmy() > 0) { hasBlue = true; }
			}

			if (x < m_width - 1) {
				if (oldSpaces.at((x + 1) * m_width + y).GetRedArmy() > 0) { hasRed = true; }
				if (oldSpaces.at((x + 1) * m_width + y).GetBlueArmy() > 0) { hasBlue = true; }
			}

			if (y > 0) {
				if (oldSpaces.at(x * m_width + (y - 1)).GetRedArmy() > 0) { hasRed = true; }
				if (oldSpaces.at(x * m_width + (y - 1)).GetBlueArmy() > 0) { hasBlue = true; }
			}

			if (y < m_height - 1) {
				if (oldSpaces.at(x * m_width + (y + 1)).GetRedArmy() > 0) { hasRed = true; }
				if (oldSpaces.at(x * m_width + (y + 1)).GetBlueArmy() > 0) { hasBlue = true; }
			}
			m_spaces.at(x * m_width + y).processDay(hasRed, hasBlue, red, blue); // setColorCast();

		}
	}
}

