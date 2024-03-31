/*
Name: Adwita Deshpande
Roll no.: 2203303
Branch: Mathematics and computing

Modelling a train using triangles only and rendering after using z-buffer algorithm for hidden surface removal
*/

#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h> 

//global variables
#define PI 3.14159265358979324
static int p = 20; //number of grid columns for cylinder and hemisphere
static int q = 30; //number of grid rows for cylinder and hemisphere
static float* vertices = NULL; //vertex array of the mapped sample on the cylinder.

static float Xangle = 90.0, Yangle = 0.0, Zangle = 0.0; //angles to rotate the scene
float moveLeft = 0.0, moveRight = 0.0, moveBottom = 0.0, moveTop = 0.0, zoomIn = 0.0, zoomOut = 0.0; //distance to move the camera by

bool fill = true; //variable to check if filled or not

//function to fill the vertex array with co-ordinates of the mapped sample points for a cylindrical shape
void fillVertexArray(void)
{
	int i, j, k;
	k = 0;
	for (j = 0; j <= q; j++)
		for (i = 0; i <= p; i++)
		{
			vertices[k++] = cos((-1 + 2 * (float)i / p) * PI);
			vertices[k++] = sin((-1 + 2 * (float)i / p) * PI);
			vertices[k++] = -1 + 2 * (float)j / q;
		}
}

//Given below are functions used to draw basic shapes

//function to draw a plate i.e a solid cylinder using triangles only
void drawPlate()
{
	int  i, j;
	fillVertexArray();
	for (j = 0; j < q; j++)
	{
		glBegin(GL_TRIANGLE_FAN); //use triangle fan for a solid cylinder
		for (i = 0; i <= p; i++)
		{
			glArrayElement((j + 1) * (p + 1) + i);
			glArrayElement(j * (p + 1) + i);
		}
		glEnd();
	}
}

//function to draw a unit cube using triangles only
void drawCube()
{
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(-1, 1, 1);
	glVertex3f(1, 1, 1);
	glVertex3f(-1, -1, 1);
	glVertex3f(1, -1, 1);
	glVertex3f(1, -1, -1);
	glVertex3f(1, 1, 1);
	glVertex3f(1, 1, -1);
	glVertex3f(-1, 1, 1);
	glVertex3f(-1, 1, -1);
	glVertex3f(-1, -1, 1);
	glVertex3f(-1, -1, -1);
	glVertex3f(1, -1, -1);
	glVertex3f(-1, 1, -1);
	glVertex3f(1, 1, -1);
	glEnd();
}

//function to draw a hemisphere of radius R using triangles only
void drawHemisphere(float R) {
	int i, j;
	for (j = 0; j < q; j++)
	{
		// One latitudinal triangle strip.
		glBegin(GL_TRIANGLE_STRIP);
		for (i = 0; i <= p; i++)
		{
			glVertex3f(R * cos((float)(j + 1) / q * PI / 2.0) * cos(2.0 * (float)i / p * PI),
				R * sin((float)(j + 1) / q * PI / 2.0),
				-R * cos((float)(j + 1) / q * PI / 2.0) * sin(2.0 * (float)i / p * PI));
			glVertex3f(R * cos((float)j / q * PI / 2.0) * cos(2.0 * (float)i / p * PI),
				R * sin((float)j / q * PI / 2.0),
				-R * cos((float)j / q * PI / 2.0) * sin(2.0 * (float)i / p * PI));
		}
		glEnd();
	}
}

//function to draw a hollow cylinder using triangles only
void drawCylinder()
{
	int  i, j;

	// Fill the vertex array.
	fillVertexArray();

	// Make the approximating triangular mesh.
	for (j = 0; j < q; j++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (i = 0; i <= p; i++)
		{
			glArrayElement((j + 1) * (p + 1) + i);
			glArrayElement(j * (p + 1) + i);
		}
		glEnd();
	}
}

//function to draw a unit pyramid using triangles only
void pyramid()
{
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(1, 1, 0);
	glVertex3f(-1, 1, 0);
	glVertex3f(-1, -1, 0);
	glVertex3f(0, 0, 1);
	glVertex3f(1, -1, 0);
	glVertex3f(1, 1, 0);
	glVertex3f(-1, -1, 0);
	glEnd();
}


//Now, below are functions used to draw shapes in the figure i.e parts of the train.

//function used to draw a wheel of a radius R 
void drawWheel(float R, bool front)
{
	//here the variable from gives the relative position of the wheel i.e in the front or back
	float pos = -1;
	if (front)
		pos = 1;
	glTranslatef(0.0, 0.02 * pos, 0.0);

	//base of wheel
	glPushMatrix();
	glColor3f(0.26, 0.26, 0.26);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glScalef(R, R, 0.04);
	drawPlate();
	glPopMatrix();

	//small 3D knob
	glPushMatrix();
	glColor3f(0.7, 0.7, 0.7);
	glTranslatef(0.0, 0.04 * pos, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glScalef(R / 3, R / 3, 0.02);
	drawPlate();
	glPopMatrix();

	//outline of wheel
	glPushMatrix();
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glScalef(R + 0.02, R + 0.02, 0.005);
	glTranslatef(0.0, -0.04 * pos, 0.0);
	drawPlate();
	glPopMatrix();
}

//function to draw a window
void drawWindow()
{
	//base rectangle
	glPushMatrix();
	glColor3f(0.7, 0.7, 0.7);
	glScalef(0.35, 0.03, 0.25);
	drawCube();
	glPopMatrix();

	//borders
	glColor3f(0.3, 0.3, 0.3);
	glPushMatrix();
	glTranslatef(-0.35, 0.0, 0.0);
	glScalef(0.02, 0.04, 0.25);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.35, 0.0, 0.0);
	glScalef(0.02, 0.04, 0.25);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.25);
	glScalef(0.35, 0.04, 0.02);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, -0.25);
	glScalef(0.35, 0.04, 0.02);
	drawCube();
	glPopMatrix();
}

//function to draw an instance of a wagon of the train
void drawWagon()
{
	glPushMatrix();
	glTranslatef(2.85, 0.0, 0.0);

	//lower connecting things
	glPushMatrix();
	glColor3f(0.8, 0.0, 0.0);
	glTranslatef(-0.1, 0.0, 0.58);
	glScalef(1.1, 0.5, 0.08);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.2, 0.2, 0.2);
	glTranslatef(0.0, 0.0, 0.70);
	glScalef(1, 0.5, 0.05);
	drawCube();
	glPopMatrix();

	//wheels of the wagon
	glPushMatrix();
	glTranslatef(-0.5, 0.5, 0.60);
	drawWheel(0.35, true);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.55, 0.5, 0.60);
	drawWheel(0.35, true);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, -0.5, 0.60);
	drawWheel(0.35, false);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.55, -0.5, 0.60);
	drawWheel(0.35, false);
	glPopMatrix();

	glTranslatef(0.0, 0.0, 0.05);

	//body of the wagon
	glColor3f(0.1, 0.1, 0.1);
	glPushMatrix();
	glTranslatef(0.0, 0, -0.20);
	glScalef(1, 0.5, 0.6);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8, 0.0, 0.0);
	glTranslatef(-0.02, 0, -0.80);
	glScalef(1.05, 0.55, 0.05);
	drawCube();
	glPopMatrix();

	//windows of the wagon
	glPushMatrix();
	glTranslatef(-0.5, 0.5, -0.25);
	drawWindow();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.5, -0.25);
	drawWindow();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, -0.5, -0.25);
	drawWindow();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, -0.5, -0.25);
	drawWindow();
	glPopMatrix();

	glPopMatrix();
}

//drawing the main scene
void drawScene(void)
{
	vertices = new float[3 * (p + 1) * (q + 1)];

	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //enable depth for hidden surface removal

	glLoadIdentity();
	gluLookAt(0.0, 0.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	//condition for fill and outline
	if (fill)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glColor3f(0.0, 0.0, 0.0);

	//move scene
	glTranslatef(moveRight + moveLeft, moveBottom + moveTop, zoomOut + zoomIn);

	//rotate scene.
	glRotatef(Zangle, 0.0, 0.0, 1.0);
	glRotatef(Yangle, 0.0, 1.0, 0.0);
	glRotatef(Xangle, 1.0, 0.0, 0.0);
	glTranslatef(-1, 0, 0);
	glScalef(0.5, 0.5, 0.5);

	//engine of train
	glPushMatrix();
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glScalef(0.5, 0.5, 0.7);
	drawCylinder();
	glPopMatrix();

	glPushMatrix();
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glScalef(0.53, 0.53, 0.01);
	glTranslatef(0.0, 0.0, 70.0);
	drawPlate();
	glPopMatrix();

	//draw 3d rings
	glColor3f(0.2, 0.2, 0.2);
	glPushMatrix();
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glScalef(0.55, 0.55, 0.04);
	glTranslatef(0.0, 0.0, -17.5);
	drawPlate();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.20, -0.5, -0.5);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glScalef(0.3, 0.3, 0.02);
	drawPlate();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.20, 0.5, -0.5);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glScalef(0.3, 0.3, 0.02);
	drawPlate();
	glPopMatrix();

	glColor3f(0.7, 0.7, 0.7);
	glPushMatrix();
	glTranslatef(1.20, 0.5, -0.5);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glScalef(0.25, 0.25, 0.04);
	drawPlate();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.20, -0.5, -0.5);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glScalef(0.25, 0.25, 0.04);
	drawPlate();
	glPopMatrix();

	//head of the train
	glPushMatrix();
	glColor3f(0.2, 0.2, 0.2);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	glTranslatef(0.0, 0.95, 0.0);
	drawHemisphere(0.07);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.7, 0.7, 0.7);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	glTranslatef(0.0, 0.70, 0.0);
	glScalef(1.0, 0.6, 1.0);
	drawHemisphere(0.5);
	glPopMatrix();

	glPushMatrix();
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glScalef(0.53, 0.53, 0.02);
	glTranslatef(0.0, 0.0, -10);
	drawPlate();
	glPopMatrix();

	glPushMatrix();
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glScalef(0.53, 0.53, 0.02);
	glTranslatef(0.0, 0.0, 10);
	drawPlate();
	glPopMatrix();

	//chimney
	glPushMatrix();
	glColor3f(0.4, 0.4, 0.4);
	glTranslatef(-0.5, 0.0, -0.60);
	glScalef(0.075, 0.075, 0.1);
	drawCylinder();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 0.0, -0.75);
	glScalef(0.1, 0.1, 0.1);
	pyramid();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(1.2, 0, -0.25);
	glScalef(0.5, 0.5, 0.75);
	drawCube();
	glPopMatrix();

	//draw roof of engine
	glPushMatrix();
	glColor3f(0.8, 0.0, 0.0);
	glTranslatef(0.5, 0.0, 0.58);
	glScalef(1.2, 0.5, 0.08);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.2, 0, -1.0);
	glScalef(0.60, 0.60, 0.06);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.68, 0.0, 0.575);
	glRotatef(135, 0.0, 1.0, 0.0);
	glScalef(0.06, 0.5, 0.06);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.20, 0.0, -1.05);
	glRotatef(90, 0, 0, -1);
	glScalef(0.65, 0.65, -0.3);
	pyramid();
	glPopMatrix();


	//draw wheels of the engine
	glPushMatrix();
	glTranslatef(-0.3, 0.5, 0.65);
	drawWheel(0.3, true);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.3, -0.5, 0.65);
	drawWheel(0.3, false);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.2, 0.5, 0.50);
	drawWheel(0.45, true);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.2, -0.5, 0.50);
	drawWheel(0.45, false);
	glPopMatrix();

	//draw 1st wagon 
	drawWagon();

	//draw 1st wagon 
	glPushMatrix();
	glTranslatef(2.30, 0.0, 0.0);
	drawWagon();
	glPopMatrix();

	//draw connecting rod at the bottom
	glPushMatrix();
	glTranslatef(2.9, 0.0, 0.7);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glScalef(0.08, 0.08, 3.5);
	drawCylinder();
	glPopMatrix();

	glutSwapBuffers();
}

//window reshape
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)w / (float)h, 1.0, 50.0);
	glMatrixMode(GL_MODELVIEW);
}

//keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 'x':
		Xangle += 5.0;
		if (Xangle > 360.0) Xangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'X':
		Xangle -= 5.0;
		if (Xangle < 0.0) Xangle += 360.0;
		glutPostRedisplay();
		break;
	case 'y':
		Yangle += 5.0;
		if (Yangle > 360.0) Yangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'Y':
		Yangle -= 5.0;
		if (Yangle < 0.0) Yangle += 360.0;
		glutPostRedisplay();
		break;
	case 'z':
		Zangle += 5.0;
		if (Zangle > 360.0) Zangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'Z':
		Zangle -= 5.0;
		if (Zangle < 0.0) Zangle += 360.0;
		glutPostRedisplay();
		break;
	case '+':
		zoomIn += 0.1;
		glutPostRedisplay();
		break;
	case '-':
		zoomOut -= 0.1;
		glutPostRedisplay();
		break;
	case 'r':
		Zangle = 0.0;
		Xangle = 90.0;
		Yangle = 0.0;
		moveRight = 0.0; moveTop = 0.0; moveBottom = 0.0; moveLeft = 0.0; zoomIn = 0.0; zoomOut = 0.0;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

//callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT) moveLeft -= 0.1;
	if (key == GLUT_KEY_RIGHT) moveRight += 0.1;
	if (key == GLUT_KEY_DOWN) moveTop -= 0.1;
	if (key == GLUT_KEY_UP) moveBottom += 0.1;
	glutPostRedisplay();
}

//the right button menu callback function.
void rightMenu(int id)
{
	if (id == 7)
	{
		Zangle = 0.0;
		Xangle = 90.0;
		Yangle = 0.0;
		moveRight = 0.0; moveTop = 0.0; moveBottom = 0.0; moveLeft = 0.0; zoomIn = 0.0; zoomOut = 0.0;
		glutPostRedisplay();
	}
	if (id == 8) exit(0);
}

//first submenu
void fillMenu(int id)
{
	if (id == 1)
	{
		fill = false;
		glutPostRedisplay();
	}
	if (id == 2)
	{
		fill = true;
		glutPostRedisplay();
	}
}

//second submenu
void viewMenu(int id)
{
	if (id == 3)
	{
		Zangle = 0.0;
		Xangle = 180.0;
		Yangle = 0.0;
		moveRight = 0.0; moveTop = 0.0; moveBottom = 0.0; moveLeft = 0.0; zoomIn = 0.0; zoomOut = 0.0;
		glutPostRedisplay();
	}
	if (id == 4)
	{
		Zangle = 0.0;
		Xangle = 90.0;
		Yangle = 90.0;
		moveRight = 0.0; moveTop = 0.0; moveBottom = 0.0; moveLeft = 0.0; zoomIn = 0.0; zoomOut = 0.0;
		glutPostRedisplay();
	}
	if (id == 5)
	{
		Zangle = 0.0;
		Xangle = 90.0;
		Yangle = 0.0;
		moveRight = 0.0; moveTop = 0.0; moveBottom = 0.0; moveLeft = 0.0; zoomIn = 0.0; zoomOut = 0.0;
		glutPostRedisplay();
	}
	if (id == 6)
	{
		Zangle = 0.0;
		Xangle = 0.0;
		Yangle = 0.0;
		moveRight = 0.0; moveTop = 0.0; moveBottom = 0.0; moveLeft = 0.0; zoomIn = 0.0; zoomOut = 0.0;
		glutPostRedisplay();
	}
}

//function to create menu.
void makeMenu(void)
{
	int sub_menu1 = glutCreateMenu(fillMenu);
	glutAddMenuEntry("Outline", 1);
	glutAddMenuEntry("Filled", 2);

	int sub_menu2 = glutCreateMenu(viewMenu);
	glutAddMenuEntry("Top", 3);
	glutAddMenuEntry("Bottom", 6);
	glutAddMenuEntry("Side", 4);
	glutAddMenuEntry("Front", 5);


	glutCreateMenu(rightMenu);
	glutAddSubMenu("Fill", sub_menu1);
	glutAddSubMenu("View", sub_menu2);
	glutAddMenuEntry("Reset", 7);
	glutAddMenuEntry("Quit", 8);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//initialization function
void setup(void)
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glClearColor(1.0, 1.0, 1.0, 0.0);
	makeMenu();
}

//function to output interaction instructions to the C++ window
void printInteraction(void)
{
	std::cout << "Interaction:" << std::endl;
	std::cout << "Press left/right/up/down arrow keys to move the train" << std::endl
		<< "Press +/- arrow keys to zoom in and out" << std::endl
		<< "Press x, X, y, Y, z, Z to turn the cylinder." << std::endl
		<< "Right click for menu" << std::endl
		<< "Press r to return to original position" << std::endl;
}

//main function
int main(int argc, char** argv)
{
	printInteraction();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(700, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("cylinder.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}


/*
References:
Computer Graphics through OpenGl by Sumant Guha
*/