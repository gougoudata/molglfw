#include "molecule.h"

std::vector<float> getCenter() {
	std::vector<float> center;
	Mat m;
	
	/* grab contents from file */
	InputFile poscar("POSCAR");
	std::vector<std::string> poscarlines, tempstr, pieces;
	poscar.readFile(poscarlines);

	float scale = atof(poscarlines[1].c_str());
	m.scale = scale;
	pieces = explode(" ",strtrim(poscarlines[2]));
	m.x1 = atof(pieces[0].c_str());
	m.x2 = atof(pieces[1].c_str());
	m.x3 = atof(pieces[2].c_str());
	pieces = explode(" ",strtrim(poscarlines[3]));
	m.y1 = atof(pieces[0].c_str());
	m.y2 = atof(pieces[1].c_str());
	m.y3 = atof(pieces[2].c_str());
	pieces = explode(" ",strtrim(poscarlines[4]));
	m.z1 = atof(pieces[0].c_str());
	m.z2 = atof(pieces[1].c_str());
	m.z3 = atof(pieces[2].c_str());

	float x = m.x1 * 0.5 + m.x2 * 0.5 + m.x3 * 0.5;
	float y = m.y1 * 0.5 + m.y2 * 0.5 + m.y3 * 0.5;
	float z = m.z1 * 0.5 + m.z2 * 0.5 + m.z3 * 0.5;

	center.push_back(x * scale);
	center.push_back(y * scale);
	center.push_back(z * scale);
	
	return center;
}

std::vector<std::string> getElements() {
	std::vector<std::string> symbols;

	/* grab contents from file */
	InputFile potcar("POTCAR");
	std::vector<std::string> potcarlines, tempstr, pieces;
	potcar.readFile(potcarlines);

	/* grab all lines containing the keyword TITEL */
	tempstr = grep(potcarlines, "TITEL");

	/* extract all elements */
	for(unsigned int i=0; i<tempstr.size(); i++) {
		tempstr[i] = strtrim(tempstr[i]);
		pieces = explode(" ", tempstr[i]);		
		symbols.push_back(strtrim(pieces[3]));
	}

	return symbols;
}

std::vector<Atom> getAtoms() {
	std::vector<std::string> elements = getElements();
	std::vector<unsigned int> ellist;
	std::vector<Atom> atoms;
	Mat m;
	
	/* grab contents from file */
	InputFile poscar("POSCAR");
	std::vector<std::string> poscarlines, tempstr, pieces;
	poscar.readFile(poscarlines);

	float scale = atof(poscarlines[1].c_str());
	m.scale = scale;
	pieces = explode(" ",strtrim(poscarlines[2]));
	m.x1 = atof(pieces[0].c_str());
	m.x2 = atof(pieces[1].c_str());
	m.x3 = atof(pieces[2].c_str());
	pieces = explode(" ",strtrim(poscarlines[3]));
	m.y1 = atof(pieces[0].c_str());
	m.y2 = atof(pieces[1].c_str());
	m.y3 = atof(pieces[2].c_str());
	pieces = explode(" ",strtrim(poscarlines[4]));
	m.z1 = atof(pieces[0].c_str());
	m.z2 = atof(pieces[1].c_str());
	m.z3 = atof(pieces[2].c_str());

	pieces = explode(" ",strtrim(poscarlines[5]));
	for(unsigned int i=0; i<pieces.size(); i++) {
		ellist.push_back(atoi(pieces[i].c_str() ) );
	}

	unsigned int cnt = 0;
	for(unsigned int i=0; i<ellist.size(); i++) {
		for(unsigned int j=0; j<ellist[i]; j++) {
			cnt++;
			pieces = explode(" ",strtrim(poscarlines[cnt+6]));
			Atom at;
			at.dx = atof(pieces[0].c_str() );
			at.dy = atof(pieces[1].c_str() );
			at.dz = atof(pieces[2].c_str() );
			at.el = elements[i];

			at.x = m.x1 * at.dx + m.x2 * at.dy + m.x3 * at.dz;
			at.y = m.y1 * at.dx + m.y2 * at.dy + m.y3 * at.dz;
			at.z = m.z1 * at.dx + m.z2 * at.dy + m.z3 * at.dz;

			at.x *= scale;
			at.y *= scale;
			at.z *= scale;

			atoms.push_back(at);
		}
	}

	return atoms;
}

std::vector<Atom> getPeriodicAtoms() {
	std::vector<std::string> elements = getElements();
	std::vector<unsigned int> ellist;
	std::vector<Atom> atoms;
	Mat m;
	
	/* grab contents from file */
	InputFile poscar("POSCAR");
	std::vector<std::string> poscarlines, tempstr, pieces;
	poscar.readFile(poscarlines);

	float scale = atof(poscarlines[1].c_str());
	m.scale = scale;
	pieces = explode(" ",strtrim(poscarlines[2]));
	m.x1 = atof(pieces[0].c_str());
	m.x2 = atof(pieces[1].c_str());
	m.x3 = atof(pieces[2].c_str());
	pieces = explode(" ",strtrim(poscarlines[3]));
	m.y1 = atof(pieces[0].c_str());
	m.y2 = atof(pieces[1].c_str());
	m.y3 = atof(pieces[2].c_str());
	pieces = explode(" ",strtrim(poscarlines[4]));
	m.z1 = atof(pieces[0].c_str());
	m.z2 = atof(pieces[1].c_str());
	m.z3 = atof(pieces[2].c_str());

	pieces = explode(" ",strtrim(poscarlines[5]));
	for(unsigned int i=0; i<pieces.size(); i++) {
		ellist.push_back(atoi(pieces[i].c_str() ) );
	}

	/* additional loop for periodicity */
	float ix=0, iy=0;
	for(unsigned int k=0; k<9; k++) {
		switch(k) {
			case 0:
				ix = 1;
				iy = 0;
				break;
			case 1:
				ix = -1;
				iy = 0;
				break;
			case 2:
				ix = 0;
				iy = 1;
				break;
			case 3:
				ix = 0;
				iy = -1;
				break;
			case 4:
				ix = 1;
				iy = 1;
				break;
			case 5:
				ix = 1;
				iy = -1;
				break;
			case 6:
				ix = -1;
				iy = 1;
				break;
			case 7:
				ix = -1;
				iy = -1;
				break;
			case 8:
				ix = 0;
				iy = 0;
			break;
		}

		unsigned int cnt = 0;
		for(unsigned int i=0; i<ellist.size(); i++) {
			for(unsigned int j=0; j<ellist[i]; j++) {
				cnt++;
				pieces = explode(" ",strtrim(poscarlines[cnt+6]));
				Atom at;
				at.dx = atof(pieces[0].c_str() ) + ix;
				at.dy = atof(pieces[1].c_str() ) + iy;
				at.dz = atof(pieces[2].c_str() );
				at.el = elements[i];

				at.x = m.x1 * at.dx + m.x2 * at.dy + m.x3 * at.dz;
				at.y = m.y1 * at.dx + m.y2 * at.dy + m.y3 * at.dz;
				at.z = m.z1 * at.dx + m.z2 * at.dy + m.z3 * at.dz;

				at.x *= scale;
				at.y *= scale;
				at.z *= scale;

				atoms.push_back(at);
			}
		}
	}

	return atoms;
}

float dist(Atom &at1, Atom &at2) {
	float v1 = at1.x - at2.x;
	float v2 = at1.y - at2.y;
	float v3 = at1.z - at2.z;

	return sqrt(v1*v1 + v2*v2 + v3*v3);
}

bool isBond(Atom &at1, Atom &at2) {
	float d = dist(at1, at2);

	if(d > 3.0) {
		return false;
	}

	std::string s1 = "Rh";
	std::string s2 = "Rh";
	if( ((at1.el.compare(s1)==0) && (at2.el.compare(s2)==0)) || ((at1.el.compare(s2)==0) && (at2.el.compare(s1)==0)) ) {
		if(d < 3.0) {
			return true;
		}
	}
	s1 = "Rh";
	s2 = "C";
	if( ((at1.el.compare(s1)==0) && (at2.el.compare(s2)==0)) || ((at1.el.compare(s2)==0) && (at2.el.compare(s1)==0)) ) {
		if(d < 2.5) {
			return true;
		}
	}
	s1 = "Rh";
	s2 = "H";
	if( ((at1.el.compare(s1)==0) && (at2.el.compare(s2)==0)) || ((at1.el.compare(s2)==0) && (at2.el.compare(s1)==0)) ) {
		if(d < 1.5) {
			return true;
		}
	}
	s1 = "C";
	s2 = "H";
	if( ((at1.el.compare(s1)==0) && (at2.el.compare(s2)==0)) || ((at1.el.compare(s2)==0) && (at2.el.compare(s1)==0)) ) {
		if(d < 1.5) {
			return true;
		}
	}

	return false;
}
