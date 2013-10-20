#include <GL/glfw.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include "molecule.h"

#define PI 3.141527

void drawSphere(float x, float y, float z, float rad, std::vector<float> col);
void drawCylinder2p(float x1, float y1, float z1, float x2, float y2, float z2, float rad, float col[3]);
static void Init(int width, int height);
std::vector<float> getColor(std::string &symbol);
float getRadius(std::string &symbol);

int main(int argc, char **argv) {
	int running = GL_TRUE;
	int width = 640;
	int height = 480;
	bool boolper = false;

	int xfirst = 0;
	int yfirst = 0;

	float roty = 0;
	float rotx = 0;
	
	std::vector<Atom> atoms = getAtoms();
	std::vector<Atom> per_atoms = getPeriodicAtoms();
	std::vector<float> center = getCenter();
	float cam_z = -2 * center[2];

	// Initialize GLFW
	if(!glfwInit() ) {
		exit(EXIT_FAILURE);
	} else {
		glutInit(&argc, argv);
	}

	//Open an OpenGL window
	if(!glfwOpenWindow(640,480,0,0,0,0,24,0,GLFW_WINDOW)) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	} else { /* assuming everything worked as expected */
		glfwSetWindowTitle("MolGLFW");
		glEnable(GL_DEPTH_TEST);
	}
	
	float alpha = 0.0;
	
	//Main loop
	while(running) {
		/* set background and lighting */
		Init(width, height);
				
		float col_black[3] = {0.0f, 0.0f, 0.0f};
		float col_white[3] = {1.0f, 1.0f, 1.0f};
		float col_lgrey[3] = {0.7f, 0.7f, 0.7f};
			
		/* camera operations */
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(0, 0, cam_z);

		glRotatef(rotx, 1.0f, 0.0f, 0.0f);
		glRotatef(roty, 0.0f, 1.0f, 0.0f);

		/* translate atoms with respect to their center of mass */
		glTranslatef(-center[0], -center[1], -center[2]);

		if(boolper) {
			/* draw atoms */
			for(unsigned int i=0; i<per_atoms.size(); i++) {
				drawSphere(per_atoms[i].x, per_atoms[i].y, per_atoms[i].z, getRadius(per_atoms[i].el), getColor(per_atoms[i].el));
			}
		
			/* and bonds */
			for(unsigned int i=0; i<per_atoms.size(); i++) {
				for(unsigned int j=i+1; j<per_atoms.size(); j++) {
					if(isBond(per_atoms[i], per_atoms[j])) {
						drawCylinder2p(per_atoms[i].x, per_atoms[i].y, per_atoms[i].z,per_atoms[j].x, per_atoms[j].y, per_atoms[j].z,0.2, col_lgrey);
					}
				}
			}
		} else {
			/* draw atoms */
			for(unsigned int i=0; i<atoms.size(); i++) {
				drawSphere(atoms[i].x, atoms[i].y, atoms[i].z, getRadius(per_atoms[i].el), getColor(per_atoms[i].el));
			}
		
			/* and bonds */
			for(unsigned int i=0; i<atoms.size(); i++) {
				for(unsigned int j=i+1; j<atoms.size(); j++) {
					if(isBond(atoms[i], atoms[j])) {
						drawCylinder2p(atoms[i].x, atoms[i].y, atoms[i].z,atoms[j].x, atoms[j].y, atoms[j].z,0.2, col_lgrey);
					}
				}
			}
		}
		
		glPopMatrix();
		
		if (glfwGetKey(GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
		  cam_z -= .1;
		}
		if (glfwGetKey(GLFW_KEY_KP_ADD) == GLFW_PRESS) {
		  cam_z += .1;
		}
		if (glfwGetKey('A') == GLFW_PRESS) {
			yfirst = 1;
			xfirst = 0;
		  roty -= 5;
		}
		if (glfwGetKey('D') == GLFW_PRESS) {
			yfirst = 1;
			xfirst = 0;
		  roty += 5;
		}
		if (glfwGetKey('W') == GLFW_PRESS) {
			yfirst = 0;
			xfirst = 1;
		  rotx -= 5;
		}
		if (glfwGetKey('S') == GLFW_PRESS) {
			yfirst = 0;
			xfirst = 1;
		  rotx += 5;
		}
		if (glfwGetKey('P') == GLFW_PRESS) {
			boolper = true;
		}
		if (glfwGetKey('L') == GLFW_PRESS) {
			boolper = false;
		}

		glfwSwapBuffers();

		running = !glfwGetKey(GLFW_KEY_ESC) &&
			glfwGetWindowParam(GLFW_OPENED);
	}
	
	// Close window and terminate GLFW
	glfwTerminate();
	
	// Exit Program
	exit (EXIT_SUCCESS);
}

static void Init(int width, int height) { // Initialization routine
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 65.0f, (GLfloat)width/(GLfloat)height, 5.0f, 100.0f );
	glMatrixMode( GL_MODELVIEW );
	glViewport( 0,0, width, height );
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); /* set white background */
	glClearDepth(1.0f);

	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};     // Define highlight properties
	GLfloat mat_shininess[]= {50.0};                   // Define shininess of surface
	GLfloat light_position[] = {1.5, 1.5, 1.5, 1.0}; // Define light source position
	glClearColor(1.0, 1.0, 1.0, 0.0);                  // Set background to gray
	glShadeModel(GL_SMOOTH);                           // Smooth transitions between edges
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);   // Set material properties
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess); // Set material properties
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);   // Set light properties
	glColorMaterial(GL_FRONT,GL_DIFFUSE);                // Set Color Capability
	
	glEnable(GL_COLOR_MATERIAL);		       // Enable color
	glEnable(GL_LIGHTING);		       // Enable lighting for surfaces
	glEnable(GL_LIGHT0);			       // Enable light source
	glEnable(GL_DEPTH_TEST); 		       // Enable depth buffering
	
	glEnable (GL_LINE_SMOOTH);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

	glfwDisable( GLFW_KEY_REPEAT ); // disable KEY REPEAT
}

void drawSphere(float x, float y, float z, float rad, std::vector<float> col) {
	glPushMatrix();
	glTranslatef(x,y,z);
	float color[3];
	color[0] = col[0];
	color[1] = col[1];
	color[2] = col[2];
	glColor3f(color[0], color[1], color[2]);
	glutSolidSphere(rad, 30, 30);
	glPopMatrix();
}

void drawCylinder2p(float x1, float y1, float z1, float x2, float y2, float z2, float rad, float col[3]) {
	float vx = x2 - x1;
	float vy = y2 - y1;
	float vz = z2 - z1;
	
	float height = sqrt(vx*vx + vy*vy + vz*vz);
	
	/* resolve issue for the degenerate case that z1 == z2 */
	vz = vz == 0 ? 0.0001 : vz;
	
	/* define rotation axis and angle */
	float ax = 57.2957795 * acos( vz /height );
	ax = vz < 0.0 ? -ax : ax;
	float rx = -vy * vz;
	float ry = vx*vz;
	
	/* push matrix and load identity */
	glPushMatrix();
	
	/* perform geometry transformations */
	glTranslatef(x1,y1,z1);
	glRotatef(ax, rx, ry, 0.0);
	
	/* place cylinder */
	GLUquadricObj *quadObj = gluNewQuadric();
	glColor3f(col[0], col[1], col[2]);
	gluCylinder(quadObj, rad, rad, height, 30, 10);
	
	/* pop matrix back */
	glPopMatrix();
}

std::vector<float> getColor(std::string &symbol) {
	std::vector<float> color(3,0);

	if(symbol.compare("Rh")==0) {
		color[0] = 0.0f;
		color[1] = 0.0f;
		color[2] = 1.0f;
	} else if(symbol.compare("C")==0) {
		color[0] = 0.0f;
		color[1] = 0.0f;
		color[2] = 0.0f;
	}else if(symbol.compare("O")==0) {
		color[0] = 1.0f;
		color[1] = 0.0f;
		color[2] = 0.0f;
	}else if(symbol.compare("H")==0) {
		color[0] = 1.0f;
		color[1] = 1.0f;
		color[2] = 1.0f;
	}

	return color;
}

float getRadius(std::string &symbol) {
	if(symbol.compare("Rh")==0) {
		return 1.2;
	} else if(symbol.compare("C")==0) {
		return 0.7;
	}else if(symbol.compare("O")==0) {
		return 0.7;
	}else if(symbol.compare("H")==0) {
		return 0.5;
	}

	return 1.0;
}
