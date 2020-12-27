#include "zmap_cell.h"

zmap_cell::zmap_cell(random<int> &r, int red, int blue) {
	// randomly generate the cell contents for the starter value. 
	//random<int> r;

	m_armyRed = red;
	m_armyBlue = blue;
	m_resource = r.num(0, 255);
	m_regen = r.num(0, 8) - 3;
}

zmap_cell::zmap_cell(bool NotReal) : m_exists{ NotReal } {
	// nothing else needs to be done, this is for placeholder purposes only.
	// since all values are at 0, any overflow shouldn't cause an issue.
}

zmap_cell::~zmap_cell() {
	
}

std::vector<unsigned char> zmap_cell::getColorCast() {
	// return the cell colors, zombies as R, humans as B, and supply as G

	// we'll assume a max of 256 per space for coloring. if there is more than that number, then the color will simply be at MAX. 
	std::vector<unsigned char> ret; 

	ret.push_back((m_armyRed > 255 ? 255 : (m_armyRed < 0 ? 0 : m_armyRed)));
	
	ret.push_back((m_resource > 255 ? 255 : (m_resource < 0 ? 0 : m_resource)));

	ret.push_back((m_armyBlue > 255 ? 255 : (m_armyBlue < 0 ? 0 : m_armyBlue)));
	
	return ret;
}

void zmap_cell::processDay(bool hasRed, bool hasBlue, army& red, army& blue) {
	// check if adjacent squares have red or blue, if they don't have units of that color already.
	if(hasRed && m_armyRed == 0){
		m_armyRed = 1;
	}
	if (hasBlue && m_armyBlue == 0) {
		m_armyBlue = 1;
	}

	// add armies as per growth
	int addRed{ 0 };
	int addBlue{ 0 };
	int consumed{ 0 };

	if (m_armyRed > 0) {
		int maxgrowth = m_resource / red.consume;
		addRed += (maxgrowth >= red.growth ? maxgrowth : red.growth);
		consumed += addRed;
	}
	if (m_armyBlue > 0) {
		int maxgrowth = m_resource / blue.consume;
		addBlue += (maxgrowth >= blue.growth ? maxgrowth : blue.growth);
		consumed += addBlue;
	}
	
	if (consumed <= m_resource) {
		//Resources are withing acceptable limits. 
		m_armyRed += addRed;
		m_armyBlue += addBlue;
		m_resource -= consumed;
	}

	// perform fighting for this day
	int redStr = red.attack * m_armyRed;
	int blueStr = blue.attack * m_armyBlue;

	m_armyRed -= blueStr / red.health;
	m_armyBlue -= redStr / blue.health;
	
	//if (m_armyBlue > 0 && m_armyBlue > m_armyRed && blueStr > redStr) { m_armyBlue += blueStr / (red.health * 2); }
	//if (m_armyRed > 0 && m_armyRed > m_armyBlue && redStr > blueStr) { m_armyRed += redStr / (blue.health * 2); }

	// apply army decay.
	if (m_armyRed > 0) {
		m_armyRed -= red.decay;
	}
	
	if (m_armyBlue > 0) {
		m_armyBlue -= blue.decay;
	}

	// set to zero
	if (m_armyBlue < 0) { m_armyBlue = 0; }
	if (m_armyRed < 0) { m_armyRed = 0; }
	
	if (m_resource < 255) { m_resource += m_regen; }
}