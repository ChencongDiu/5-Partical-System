#include "stdafx.h"

// standard
#include <assert.h>
#include <math.h>
#include <cstdlib>

// glut
#define GLUT_DISABLE_ATEXIT_HACK
#include <gl/glut.h>

using namespace std;
#define pi 3.1415926

//================================
// global variables
//================================
// screen size
int g_screenWidth = 1600;
int g_screenHeight = 900;

// frame index
int g_frameIndex = 0;

// angle for rotation
int g_angle = 0;
double g_xdistance = 0.0;
double g_ydistance = 0.0;

//particles
class particles {
public:
	double x;
	double y;

	float red;
	float green;
	float blue;

	float life;

	float speedx;
	float speedy;
};
int particleNumber = 20000;
particles particle[20000];

float seed = 0.0;
float r = 0;
float g = 0;
float b = 0;
double cpy = 0.0;
double cpx = 0.0;

int irand = 0;
int life = 2;

//================================
// Print String on Screen
//================================
void drawBitmapText(char *string, float x, float y, float z)
{
	char *c;
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
}

//================================
// initialize balls matrix ballsM with initPositions
//================================
void init(void) {
	
}

//================================
// render
//================================
void render() {
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1, 1, 1);
	drawBitmapText("Press Spacebar to Exit", -0.97, 0.9, 0.0);

	for (int i = 0; i < particleNumber; i++) {
		srand(seed);
		seed += 0.1;
		r += 0.1;
		b += 0.002;
		g += 0.009;

		if (r > 1.0) {
			r = float(rand() % 2) - 0.7;
		}
		if (g > 1.0) {
			g = float(rand() % 1) - 0.3;
		}
		if (b > 0.5) {
			b = 0.0;
		}
		if (seed > 1.0) {
			seed = 0.0;
		}
		if (irand > 100) {
			irand = -50;
		}

		glColor4f(particle[i].red, particle[i].green, particle[i].blue, particle[i].life);

		glBegin(GL_TRIANGLE_STRIP);
		glVertex2f(particle[i].x + 0.001, particle[i].y + 0.001);
		glVertex2f(particle[i].x - 0.001, particle[i].y + 0.001);
		glVertex2f(particle[i].x + 0.001, particle[i].y - 0.001);
		glVertex2f(particle[i].x - 0.001, particle[i].y - 0.001);
		glEnd();

		if (particle[i].life <= 0.0) {
			life = 2.5;
			srand((seed + r + g + b) * 500);

			particle[i].x = cpx;
			particle[i].y = cpy;
			particle[i].life = float(rand() % life);
			particle[i].speedy = float((rand() % 50) - 26.0f) / g_screenWidth * 1000 * sin(2 * pi * rand() / (RAND_MAX + 1));
			particle[i].speedx = float((rand() % 50) - 26.0f) / g_screenHeight * 1000 * cos(2 * pi * rand() / (RAND_MAX + 1)) - 10;

			particle[i].red = r;
			particle[i].green = g;
			particle[i].blue = b;
			irand += rand() % 50;
		}

		particle[i].life -= 0.01;
		
		particle[i].y += particle[i].speedx / 4000;
		particle[i].x += particle[i].speedy / 4000;

		++i;
	}

	glutSwapBuffers();
}

//================================
// keyboard input
//================================
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case ' ':
		exit(0);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

//================================
// reshape : update viewport and projection matrix when the window is resized
//================================
void reshape(int w, int h) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, w - 1, h - 1, 0);
	glPopMatrix();
}

//================================
// timer : triggered every 16ms ( about 60 frames per second )
//================================
void timer(int value) {
	// render
	glutPostRedisplay();

	// increase frame index
	g_frameIndex++;

	// reset timer
	// 16 ms per frame ( about 60 frames per second )
	glutTimerFunc(16, timer, 0);
}

//================================
// main
//================================
int main(int argc, char** argv) {
	// create opengL window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(g_screenWidth, g_screenHeight);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Lab5");

	// init
	init();

	// set callback functions
	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(16, timer, 0);

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	glutMainLoop();

	return 0;
}