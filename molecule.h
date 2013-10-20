#ifndef _MOLECULE_H
#define _MOLECULE_H

#include "readfile.h"
#include "strfunc.h"
#include<cmath>

struct Atom{
	float x,y,z;		/* for the cartesian coordinates */
	float dx, dy, dz; /* for the direct coordinates */
	std::string el; /* for the element */
};

struct Mat{
	float scale;
	float x1,x2,x3,y1,y2,y3,z1,z2,z3;
};

std::vector<float> getCenter();
std::vector<std::string> getElements();
std::vector<Atom> getAtoms();
std::vector<Atom> getPeriodicAtoms();
float dist(Atom &at1, Atom &at2);
bool isBond(Atom &at1, Atom &at2);

#endif //_MOLECULE_H
