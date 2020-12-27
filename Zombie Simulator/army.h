#pragma once

// army struct for storing the basic information for all units.

struct army {
	int decay{ 1 };
	int growth{ 10 };
	int consume{ 1 };
	int attack{ 2 };
	int health{ 2 };

	army() {}

	army(int dec, int gro, int con, int att, int hea) : decay{ dec }, growth { gro }, consume{ con }, attack{ att }, health{ hea } {
		// generate the basic information.
	}
};