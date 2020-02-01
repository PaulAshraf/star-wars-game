#include <iostream>
#include <windows.h>
#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <random>
#include <string>

//#include <obj.h>

using namespace std;
using std::vector;

int WIDTH = 1280;
int HEIGHT = 720;

GLuint tex;
GLuint tex2;
GLuint tex3; //TO HOLD ASTEROID TEXTURE
GLuint tex4; //TO HOLD level2 TEXTURE
GLuint tex5;

char title[] = "3D Model Loader Sample";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 200;

class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};

Vector Eye(0, 3, 20);
Vector At(0, 0, 0);
Vector Up(0, 1, 0);

// Model Variables
Model_3DS model_tree; //????????????????
Model_3DS model_xwing;

// Textures
GLTexture tex_ground;

//Light Animation Variables
float deltaTime = 0;
float lastFrame = 0;
bool increasing=true;
double zlight = 0;


//global vars
vector<double> bulletX;
vector<double> bulletY;
vector<double> bulletZ;

int health = 100;
int score = 0;

double currX = 0.0;
double currY = 0.0;
double currZ = 10.0;

bool LRBlaster = false;

bool moveLeft = false;
bool moveRight = false;
bool moveUp = false;
bool moveDown = false;

bool thirdPerson = true;
bool level0 = false;
bool gameOver = false;
double SPEED_SLOW = 2;
double SPEED_FAST = 4;
int MIN_RADIUS = 1;
int MAX_RADIUS = 6;
double FARZ = -90;
double NEARZ = 15;
double MAXX = 20;
double MINX = -20;
double MAXY = 10;
double MINY = -10;
int whichSphere = 0;
/*END OF GLOBAL VARIABLES*/

/*ALL THE ASTEROID	 DATA*/

bool hit1 = false;
bool hit2 = false;
bool hit3 = false;
bool hit4 = false;
bool hit5 = false;
bool hit6 = false;
bool hit7 = false;

bool sphere1Thrown = false;
double sphere1Radius = 0;
double sphere1X = 0;
double sphere1Y = 0;
double sphere1Z = 0;

bool sphere2Thrown = false;
double sphere2Radius = 0;
double sphere2X = 0;
double sphere2Y = 0;
double sphere2Z = 0;

bool sphere3Thrown = false;
double sphere3Radius = 0;
double sphere3X = 0;
double sphere3Y = 0;
double sphere3Z = 0;

bool sphere4Thrown = false;
double sphere4Radius = 0;
double sphere4X = 0;
double sphere4Y = 0;
double sphere4Z = 0;

bool sphere5Thrown = false;
double sphere5Radius = 0;
double sphere5X = 0;
double sphere5Y = 0;
double sphere5Z = 0;

bool sphere6Thrown = false;
double sphere6Radius = 0;
double sphere6X = 0;
double sphere6Y = 0;
double sphere6Z = 0;

bool sphere7Thrown = false;
double sphere7Radius = 0;
double sphere7X = 0;
double sphere7Y = 0;
double sphere7Z = 0;

/*        END OF ASTEROID       */


void drawBullets();
void fire();
void moveXWing();
void displayScore(int score);
void displayText(string text,float x,float y);
void displayHealthBar(int health);


//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************************************************************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************************************************************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	//*******************************************************************************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************************************************************************//

	//InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}

void throwSphere1() {
	glPushMatrix();
	GLUquadricObj* asteroid1;
	asteroid1 = gluNewQuadric();
	glTranslated(sphere1X, sphere1Y, sphere1Z);
	if(level0)
		glBindTexture(GL_TEXTURE_2D, tex3);
	else
		glBindTexture(GL_TEXTURE_2D, tex5);

	gluQuadricTexture(asteroid1, true);
	gluQuadricNormals(asteroid1, GL_SMOOTH);
	gluSphere(asteroid1, sphere1Radius, 10, 10);
	gluDeleteQuadric(asteroid1);

	glPopMatrix();
	glutPostRedisplay();
}
void throwSphere2() {
	glPushMatrix();
	GLUquadricObj* asteroid2;
	asteroid2 = gluNewQuadric();
	glTranslated(sphere2X, sphere2Y, sphere2Z);
	if (level0)
		glBindTexture(GL_TEXTURE_2D, tex3);
	else
		glBindTexture(GL_TEXTURE_2D, tex5);
	gluQuadricTexture(asteroid2, true);
	gluQuadricNormals(asteroid2, GL_SMOOTH);
	gluSphere(asteroid2, sphere2Radius, 10, 10);
	gluDeleteQuadric(asteroid2);

	glPopMatrix();
	glutPostRedisplay();
}
void throwSphere3() {
	glPushMatrix();
	GLUquadricObj* asteroid3;
	asteroid3 = gluNewQuadric();
	glTranslated(sphere3X, sphere3Y, sphere3Z);
	if (level0)
		glBindTexture(GL_TEXTURE_2D, tex3);
	else
		glBindTexture(GL_TEXTURE_2D, tex5); 
	gluQuadricTexture(asteroid3, true);
	gluQuadricNormals(asteroid3, GL_SMOOTH);
	gluSphere(asteroid3, sphere3Radius, 10, 10);
	gluDeleteQuadric(asteroid3);

	glPopMatrix();
	glutPostRedisplay();
}

void throwSphere4() {
	glPushMatrix();
	GLUquadricObj* asteroid4;
	asteroid4 = gluNewQuadric();
	glTranslated(sphere4X, sphere4Y, sphere4Z);
	if (level0)
		glBindTexture(GL_TEXTURE_2D, tex3);
	else
		glBindTexture(GL_TEXTURE_2D, tex5); 
	gluQuadricTexture(asteroid4, true);
	gluQuadricNormals(asteroid4, GL_SMOOTH);
	gluSphere(asteroid4, sphere4Radius, 10, 10);
	gluDeleteQuadric(asteroid4);

	glPopMatrix();
	glutPostRedisplay();
}

void throwSphere5() {
	glPushMatrix();
	GLUquadricObj* asteroid5;
	asteroid5 = gluNewQuadric();
	glTranslated(sphere5X, sphere5Y, sphere5Z);
	if (level0)
		glBindTexture(GL_TEXTURE_2D, tex3);
	else
		glBindTexture(GL_TEXTURE_2D, tex5);
	gluQuadricTexture(asteroid5, true);
	gluQuadricNormals(asteroid5, GL_SMOOTH);
	gluSphere(asteroid5, sphere5Radius, 10, 10);
	gluDeleteQuadric(asteroid5);

	glPopMatrix();
	glutPostRedisplay();
}
void throwSphere6() {
	glPushMatrix();
	GLUquadricObj* asteroid6;
	asteroid6 = gluNewQuadric();
	glTranslated(sphere6X, sphere6Y, sphere6Z);
	if (level0)
		glBindTexture(GL_TEXTURE_2D, tex3);
	else
		glBindTexture(GL_TEXTURE_2D, tex5); 
	gluQuadricTexture(asteroid6, true);
	gluQuadricNormals(asteroid6, GL_SMOOTH);
	gluSphere(asteroid6, sphere6Radius, 10, 10);
	gluDeleteQuadric(asteroid6);

	glPopMatrix();
	glutPostRedisplay();
}
void throwSphere7() {
	glPushMatrix();
	GLUquadricObj* asteroid7;
	asteroid7 = gluNewQuadric();
	glTranslated(sphere7X, sphere7Y, sphere7Z);
	if (level0)
		glBindTexture(GL_TEXTURE_2D, tex3);
	else
		glBindTexture(GL_TEXTURE_2D, tex5); 
	gluQuadricTexture(asteroid7, true);
	gluQuadricNormals(asteroid7, GL_SMOOTH);
	gluSphere(asteroid7, sphere7Radius, 10, 10);
	gluDeleteQuadric(asteroid7);

	glPopMatrix();
	glutPostRedisplay();
}

//=======================================================================
// Display Function
//=======================================================================
void myDisplay(void)
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	float currentFrame = glutGet(GLUT_ELAPSED_TIME);
	//cout << zlight << "\n";
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	//GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightIntensity1[] = { 0.3, 0.3, 0.3, 0.1f };
	//GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	GLfloat lightPosition1[] = { 0.0f, 10.0f, zlight, 0.0f };
	//glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightIntensity1);


	////XWING//////

	glPushMatrix();
	glTranslatef(currX, currY, currZ);
	glRotated(215, 0, 1, 0);
	if(moveRight)
		glRotated(20, 0, 0, 1);
	if(moveLeft)
		glRotated(-20, 0, 0, 1);
	glScaled(0.5, 0.5, 0.5);
	model_xwing.Draw();
	glPopMatrix();

	/////////

	drawBullets();
	displayScore(score);
	displayHealthBar(health);

	if (sphere1Thrown && !hit1)
		throwSphere1();
	if (sphere2Thrown && !hit2)
		throwSphere2();
	if (sphere3Thrown && !hit3)
		throwSphere3();
	if (sphere4Thrown && !hit4)
		throwSphere4();
	if (sphere5Thrown && !hit5)
		throwSphere5();
	if (sphere6Thrown && !hit6)
		throwSphere6();
	if (sphere7Thrown && !hit7)
		throwSphere7();
		
	///////SKY////////
	
	glPushMatrix();
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glTranslated(0, 0, 0);
	if(level0)
		glBindTexture(GL_TEXTURE_2D, tex);
	else
		glBindTexture(GL_TEXTURE_2D, tex4);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 100, 100, 100);
	gluDeleteQuadric(qobj);
	glPopMatrix();

	//////////////

	glutSwapBuffers();
	glFlush();

}

void collision() {

	if ((((sphere1X - sphere1Radius < currX + 2 && sphere1X + sphere1Radius > currX - 2) || (sphere1X + sphere1Radius > currX - 2 && sphere1X - sphere1Radius < currX + 2)) &&
		(currY > sphere1Y - sphere1Radius && currY < sphere1Y + sphere1Radius) &&
		(currZ<sphere1Z + sphere1Radius && currZ > sphere1Z - sphere1Radius)) && !hit1) {
		health -= 20;
		cout << "you hit 1\n";
		hit1 = true;
		PlaySound(TEXT("fire3.wav"), NULL, SND_ASYNC | SND_FILENAME);

	}
	

	if ((((sphere2X - sphere2Radius < currX + 2 && sphere2X + sphere2Radius > currX - 2) || (sphere2X + sphere2Radius > currX - 2 && sphere2X - sphere2Radius < currX + 2)) &&
		(currY > sphere2Y - sphere2Radius && currY < sphere2Y + sphere2Radius) &&
		(currZ<sphere2Z + sphere2Radius && currZ > sphere2Z - sphere2Radius)) && !hit2) {
		health -= 20;
		cout << "you hit 2\n";
		hit2=true;
		PlaySound(TEXT("fire3.wav"), NULL, SND_ASYNC | SND_FILENAME);

	}
	if ((((sphere3X - sphere3Radius < currX + 2 && sphere3X + sphere3Radius > currX - 2) || (sphere3X + sphere3Radius > currX - 2 && sphere3X - sphere3Radius < currX + 2)) &&
		(currY > sphere3Y - sphere3Radius && currY < sphere3Y + sphere3Radius) &&
		(currZ<sphere3Z + sphere3Radius && currZ > sphere3Z - sphere3Radius)) && !hit3) {
		health -= 20;
		cout << "you hit 3\n";
		hit3 = true;
		PlaySound(TEXT("fire3.wav"), NULL, SND_ASYNC | SND_FILENAME);

	}
	if ((((sphere4X - sphere4Radius < currX + 2 && sphere4X + sphere4Radius > currX - 2) || (sphere4X + sphere4Radius > currX - 2 && sphere4X - sphere4Radius < currX + 2)) &&
		(currY > sphere4Y - sphere4Radius && currY < sphere4Y + sphere4Radius) &&
		(currZ<sphere4Z + sphere4Radius && currZ > sphere4Z - sphere4Radius)) && !hit4) {
		health -= 20;
		cout << "you hit 4\n";
		hit4 = true;
		PlaySound(TEXT("fire3.wav"), NULL, SND_ASYNC | SND_FILENAME);

	}
	if ((((sphere5X - sphere5Radius < currX + 2 && sphere5X + sphere5Radius > currX - 2) || (sphere5X + sphere5Radius > currX - 2 && sphere5X - sphere5Radius < currX + 2)) &&
		(currY > sphere5Y - sphere5Radius && currY < sphere5Y + sphere5Radius) &&
		(currZ<sphere5Z + sphere5Radius && currZ > sphere5Z - sphere5Radius)) && !hit5){
		health -= 20;
		cout << "you hit 5\n";
		hit5= true;
		PlaySound(TEXT("fire3.wav"), NULL, SND_ASYNC | SND_FILENAME);

	}
	if ((((sphere6X - sphere6Radius < currX + 2 && sphere6X + sphere6Radius > currX - 2) || (sphere6X + sphere6Radius > currX - 2 && sphere6X - sphere6Radius < currX + 2)) &&
		(currY > sphere6Y - sphere6Radius && currY < sphere6Y + sphere6Radius) &&
		(currZ<sphere6Z + sphere6Radius && currZ > sphere6Z - sphere6Radius)) && !hit6){
		health -= 20;
		cout << "you hit 6\n";
		hit6 = true;
		PlaySound(TEXT("fire3.wav"), NULL, SND_ASYNC | SND_FILENAME);

	}
	if ((((sphere7X - sphere7Radius < currX + 2 && sphere7X + sphere7Radius > currX - 2) || (sphere7X + sphere7Radius > currX - 2 && sphere7X - sphere7Radius < currX + 2)) &&
		(currY > sphere7Y - sphere7Radius && currY < sphere7Y + sphere7Radius) &&
		(currZ<sphere7Z + sphere7Radius && currZ > sphere7Z - sphere7Radius)) && !hit7)
	{
		health -= 20;
		cout << "you hit 7\n";
		hit7=true;
		PlaySound(TEXT("fire3.wav"), NULL, SND_ASYNC | SND_FILENAME);


	}
	cout << hit1 << " ";
	cout << ((sphere1X - sphere1Radius <= currX + 2 && sphere1X + sphere1Radius >= currX - 2) || (sphere1X + sphere1Radius >= currX - 2 && sphere1X - sphere1Radius <= currX + 2)) << " ";
	cout << (currY >= sphere1Y - sphere1Radius && currY <= sphere1Y + sphere1Radius) << " ";
	cout << (currZ<= sphere1Z + sphere1Radius && currZ >= sphere1Z - sphere1Radius) << " \n";

}

void hit() {
	for (int i = 0; i < bulletX.size(); i++) {
		
		
		if (bulletX[i] > sphere1X - sphere1Radius && bulletX[i] < sphere1X + sphere1Radius &&
			bulletY[i] > sphere1Y - sphere1Radius && bulletY[i] < sphere1Y + sphere1Radius &&
			bulletZ[i] > sphere1Z - sphere1Radius && bulletZ[i] < sphere1Z + sphere1Radius) {
			hit1 = true;
			score += 20;
			bulletX.erase(bulletX.begin() + i);
			bulletY.erase(bulletY.begin() + i);
			bulletZ.erase(bulletZ.begin() + i);
			if(bulletX.size() != 0)
				i--;
			return;
		}
		if (bulletX[i] > sphere2X - sphere2Radius && bulletX[i] < sphere2X + sphere2Radius &&
			bulletY[i] > sphere2Y - sphere2Radius && bulletY[i] < sphere2Y + sphere2Radius &&
			bulletZ[i] > sphere2Z - sphere2Radius && bulletZ[i] < sphere2Z + sphere2Radius) {
			hit2 = true;
			score += 20;
			bulletX.erase(bulletX.begin() + i);
			bulletY.erase(bulletY.begin() + i);
			bulletZ.erase(bulletZ.begin() + i);
			if (bulletX.size() != 0)
				i--;
			return;
		}
		if (bulletX[i] > sphere3X - sphere3Radius && bulletX[i] < sphere3X + sphere3Radius &&
			bulletY[i] > sphere3Y - sphere3Radius && bulletY[i] < sphere3Y + sphere3Radius &&
			bulletZ[i] > sphere3Z - sphere3Radius && bulletZ[i] < sphere3Z + sphere3Radius) {
			hit3 = true;
			score += 20;
			bulletX.erase(bulletX.begin() + i);
			bulletY.erase(bulletY.begin() + i);
			bulletZ.erase(bulletZ.begin() + i);
			if (bulletX.size() != 0)
				i--;
			return;
		}
		if (bulletX[i] > sphere4X - sphere4Radius && bulletX[i] < sphere4X + sphere4Radius &&
			bulletY[i] > sphere4Y - sphere4Radius && bulletY[i] < sphere4Y + sphere4Radius &&
			bulletZ[i] > sphere4Z - sphere4Radius && bulletZ[i] < sphere4Z + sphere4Radius) {
			hit4 = true;
			score += 20;
			bulletX.erase(bulletX.begin() + i);
			bulletY.erase(bulletY.begin() + i);
			bulletZ.erase(bulletZ.begin() + i);
			if (bulletX.size() != 0)
				i--;
			return;
		}
		if (bulletX[i] > sphere5X - sphere5Radius && bulletX[i] < sphere5X + sphere5Radius &&
			bulletY[i] > sphere5Y - sphere5Radius && bulletY[i] < sphere5Y + sphere5Radius &&
			bulletZ[i] > sphere5Z - sphere5Radius && bulletZ[i] < sphere5Z + sphere5Radius) {
			hit5 = true;
			score += 20;
			bulletX.erase(bulletX.begin() + i);
			bulletY.erase(bulletY.begin() + i);
			bulletZ.erase(bulletZ.begin() + i);
			if (bulletX.size() != 0)
				i--;
			return;
		}
		if (!level0) {
			if (bulletX[i] > sphere6X - sphere6Radius && bulletX[i] < sphere6X + sphere6Radius &&
				bulletY[i] > sphere6Y - sphere6Radius && bulletY[i] < sphere6Y + sphere6Radius &&
				bulletZ[i] > sphere6Z - sphere6Radius && bulletZ[i] < sphere6Z + sphere6Radius) {
				hit6 = true;
				score += 20;
				bulletX.erase(bulletX.begin() + i);
				bulletY.erase(bulletY.begin() + i);
				bulletZ.erase(bulletZ.begin() + i);
				if (bulletX.size() != 0)
					i--;
				return;
			}
			if (bulletX[i] > sphere7X - sphere7Radius && bulletX[i] < sphere7X + sphere7Radius &&
				bulletY[i] > sphere7Y - sphere7Radius && bulletY[i] < sphere7Y + sphere7Radius &&
				bulletZ[i] > sphere7Z - sphere7Radius && bulletZ[i] < sphere7Z + sphere7Radius) {
				hit7 = true;
				score += 20;
				bulletX.erase(bulletX.begin() + i);
				bulletY.erase(bulletY.begin() + i);
				bulletZ.erase(bulletZ.begin() + i);
				if (bulletX.size() != 0)
					i--;
				return;
			}
		}
	}
}

void glutTimer(int value) {
	if (increasing) {
		zlight += 2;
		if (zlight >= 20) {
			increasing = false;
		}
	}
	else {
		zlight -= 2;
		if (zlight <= -20) {
			increasing = true;
		}
	}
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(MIN_RADIUS * 10, MAX_RADIUS * 10);
	std::uniform_int_distribution<> dis2(MINY * 10, MAXY * 10);
	std::uniform_int_distribution<> dis3(MINX * 10, MAXX * 10);
	if (level0) {
		if (whichSphere == 0 && !sphere1Thrown) {
			int randsize = dis(gen);
			sphere1Radius = (double)randsize / 10;
			int randX = dis3(gen);
			sphere1X = (double)randX / 10;
			int randY = dis2(gen);
			sphere1Y = (double)randY / 10;
			sphere1Z = FARZ;
			sphere1Thrown = true;
		}
		if (whichSphere == 25 && !sphere2Thrown) {
			int randsize = dis(gen);
			sphere2Radius = (double)randsize / 10;
			int randX = dis3(gen);
			sphere2X = (double)randX / 10;
			int randY = dis2(gen);
			sphere2Y = (double)randY / 10;
			sphere2Z = FARZ;
			sphere2Thrown = true;
		}
		if (whichSphere == 50 && !sphere3Thrown) {
			int randsize = dis(gen);
			sphere3Radius = (double)randsize / 10;
			int randX = dis3(gen);
			sphere3X = (double)randX / 10;
			int randY = dis2(gen);
			sphere3Y = (double)randY / 10;
			sphere3Z = FARZ;
			sphere3Thrown = true;
		}
		if (whichSphere == 75 && !sphere4Thrown) {
			int randsize = dis(gen);
			sphere4Radius = (double)randsize / 10;
			int randX = dis3(gen);
			sphere4X = (double)randX / 10;
			int randY = dis2(gen);
			sphere4Y = (double)randY / 10;
			sphere4Z = FARZ;
			sphere4Thrown = true;
		}
		if (whichSphere == 100 && !sphere5Thrown) {
			int randsize = dis(gen);
			sphere5Radius = (double)randsize / 10;
			int randX = dis3(gen);
			sphere5X = (double)randX / 10;
			int randY = dis2(gen);
			sphere5Y = (double)randY / 10;
			sphere5Z = FARZ;
			sphere5Thrown = true;
		}

		if (sphere1Thrown) {
			sphere1Z += SPEED_SLOW;
			if (sphere1Z >= NEARZ) {
				sphere1Thrown = false;
				hit1 = false;
			}
		}
		if (sphere2Thrown) {
			sphere2Z += SPEED_SLOW;
			if (sphere2Z >= NEARZ) {
				sphere2Thrown = false;
				hit2 = false;
			}
		}
		if (sphere3Thrown) {
			sphere3Z += SPEED_SLOW;
			if (sphere3Z >= NEARZ) {
				sphere3Thrown = false;
				hit3 = false;
			}
		}
		if (sphere4Thrown) {
			sphere4Z += SPEED_SLOW;
			if (sphere4Z >= NEARZ) {
				sphere4Thrown = false;
				hit4 = false;
			}
		}
		if (sphere5Thrown) {
			sphere5Z += SPEED_SLOW;
			if (sphere5Z >= NEARZ) {
				sphere5Thrown = false;
				hit5 = false;
			}
		}
	}
	else {
		if (whichSphere == 0 && !sphere1Thrown) {
			int randsize = dis(gen);
			sphere1Radius = (double)randsize / 10;
			int randX = dis3(gen);
			sphere1X = (double)randX / 10;
			int randY = dis2(gen);
			sphere1Y = (double)randY / 10;
			sphere1Z = FARZ;
			sphere1Thrown = true;
		}
		if (whichSphere == 10 && !sphere2Thrown) {
			int randsize = dis(gen);
			sphere2Radius = (double)randsize / 10;
			int randX = dis3(gen);
			sphere2X = (double)randX / 10;
			int randY = dis2(gen);
			sphere2Y = (double)randY / 10;
			sphere2Z = FARZ;
			sphere2Thrown = true;
		}
		if (whichSphere == 20 && !sphere3Thrown) {
			int randsize = dis(gen);
			sphere3Radius = (double)randsize / 10;
			int randX = dis3(gen);
			sphere3X = (double)randX / 10;
			int randY = dis2(gen);
			sphere3Y = (double)randY / 10;
			sphere3Z = FARZ;
			sphere3Thrown = true;
		}
		if (whichSphere == 30 && !sphere4Thrown) {
			int randsize = dis(gen);
			sphere4Radius = (double)randsize / 10;
			int randX = dis3(gen);
			sphere4X = (double)randX / 10;
			int randY = dis2(gen);
			sphere4Y = (double)randY / 10;
			sphere4Z = FARZ;
			sphere4Thrown = true;
		}
		if (whichSphere == 40 && !sphere5Thrown) {
			int randsize = dis(gen);
			sphere5Radius = (double)randsize / 10;
			int randX = dis3(gen);
			sphere5X = (double)randX / 10;
			int randY = dis2(gen);
			sphere5Y = (double)randY / 10;
			sphere5Z = FARZ;
			sphere5Thrown = true;
		}
		if (whichSphere == 50 && !sphere6Thrown) {
			int randsize = dis(gen);
			sphere6Radius = (double)randsize / 10;
			int randX = dis3(gen);
			sphere6X = (double)randX / 10;
			int randY = dis2(gen);
			sphere6Y = (double)randY / 10;
			sphere6Z = FARZ;
			sphere6Thrown = true;
		}
		if (whichSphere == 60 && !sphere7Thrown) {
			int randsize = dis(gen);
			sphere7Radius = (double)randsize / 10;
			int randX = dis3(gen);
			sphere7X = (double)randX / 10;
			int randY = dis2(gen);
			sphere7Y = (double)randY / 10;
			sphere7Z = FARZ;
			sphere7Thrown = true;
		}

		if (sphere1Thrown) {
			sphere1Z += SPEED_FAST;
			if (sphere1Z >= NEARZ) {
				sphere1Thrown = false;
				hit1 = false;
			}
		}
		if (sphere2Thrown) {
			sphere2Z += SPEED_FAST;
			if (sphere2Z >= NEARZ){
				sphere2Thrown = false;
				hit2 = false;
			}
		}
		if (sphere3Thrown) {
			sphere3Z += SPEED_FAST;
			if (sphere3Z >= NEARZ) {
				sphere3Thrown = false;
				hit3 = false;
			}
		}
		if (sphere4Thrown) {
			sphere4Z += SPEED_FAST;
			if (sphere4Z >= NEARZ) {
				sphere4Thrown = false;
				hit4 = false;
			}
		}
		if (sphere5Thrown) {
			sphere5Z += SPEED_FAST;
			if (sphere5Z >= NEARZ) {
				sphere5Thrown = false;
				hit5 = false;
			}
		}
		if (sphere6Thrown) {
			sphere6Z += SPEED_FAST;
			if (sphere6Z >= NEARZ) {
				sphere6Thrown = false;
				hit6 = false;
			}
		}
		if (sphere7Thrown) {
			sphere7Z += SPEED_FAST;
			if (sphere7Z >= NEARZ) {
				sphere7Thrown = false;
				hit7 = false;
			}
		}
	}
	whichSphere++;
	if (level0)
		whichSphere %= 125;
	else
		whichSphere %= 70;
	glutTimerFunc(100, glutTimer, 1);
	glutPostRedisplay();
}

//=======================================================================
// Keyboard Function
//=======================================================================
void myKeyboardUp(unsigned char button, int x, int y)
{
	switch (button)
	{
	case 'm':
		thirdPerson = !thirdPerson;
			break;
	case 'f':
		fire();
		break;
	case 'w':
		moveUp = false;
		break;
	case 'a':
		moveLeft = false;
		break;
	case 's':
		moveDown = false;
		break;
	case 'd':
		moveRight = false;
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

void myKeyboard(unsigned char button, int x, int y)
{
	switch (button)
	{

	
	case 'w':
		moveUp = true;
		break;
	case 'a':
		moveLeft = true;
		break;
	case 's':
		moveDown = true;
		break;
	case 'd':
		moveRight = true;
		break;
	
	default:
		break;
	}

	glutPostRedisplay();
}

//=======================================================================
// Motion Function
//=======================================================================
void myMotion(int x, int y)
{

	y = HEIGHT - y;
	
	double newX = (x - (double)WIDTH / 2.0) * (10.0 / ((double)WIDTH / 2.0));
	double newY = (y - (double)HEIGHT / 2.0) * (10.0 / ((double)HEIGHT / 2.0));

	//cout << newX << " " << newY << "\n";

	At.x = newX;
	At.y = newY;

	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters

	GLfloat light_position3[] = { 0.0f, 5.0f, 0.0f, 1.0f };
	GLfloat lightIntensity3[] = { 1, 0, 0, 1.0f };

	glLightfv(GL_LIGHT3, GL_POSITION, light_position3);
	glLightfv(GL_LIGHT3, GL_SPECULAR, lightIntensity3);

	glutPostRedisplay();	//Re-draw scene 
}

//=======================================================================
// Mouse Function
//=======================================================================
void myMouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		fire();
	}

	
}

void displayText(string text, float x, float y) {
	glPushMatrix();
	glRasterPos2f(x, y);
	glDisable(GL_LIGHTING);
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(1, 1, 1);
	for (int i = 0; i < text.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
	}
	glPopAttrib();
	glDisable(GL_LIGHTING);
	glPopMatrix();
}

void displayScore(int score) {
	string str = to_string(score);
	displayText(str, At.x-12, At.y+6);
}

void displayHealthBar(int health) {

	glPushMatrix();
	glDisable(GL_LIGHTING);
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(0, 1, 0);
	glTranslatef(At.x-7, At.y+6 + 1, 0);
	glScalef(10 * ((double)health / 100.0), 1, 0.001);
	glutSolidCube(1);
	glPopAttrib();
	glEnable(GL_LIGHTING);
	glPopMatrix();


	glFlush();
}


//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{
	//?????????????????????????
	model_tree.Load("Models/tree/Tree1.3ds");
	
	//Loading models
	model_xwing.Load("Models/xwing/star wars x-wing.3DS");

	// Loading texture files
	loadBMP(&tex3, "Textures/asteroid.bmp", true);
	loadBMP(&tex, "Textures/milkyway.bmp", true);
	loadBMP(&tex2, "Textures/laser.bmp", true);
	loadBMP(&tex4, "Textures/andromeda.bmp", true);
	loadBMP(&tex5, "Textures/lava.bmp", true);


}

void moveXWing() {
	if (moveUp)
		currY += 0.1;
	if (moveDown)
		currY -= 0.1;
	if (moveLeft)
		currX -= 0.1;
	if (moveRight)
		currX += 0.1;

	if (currX > 15)
		currX = 15;
	if (currX < -15)
		currX = -15;
	if (currY > 15)
		currY = 15;
	if (currY < -15)
		currY = -15;
}

void win() {
	cout << "You win! \n Your score is " << score;
	exit(0);
}

void lose() {
	cout << "You lose! \n Good luck next time :D";
	exit(0);
}

void Anim() {
	for (int i = 0; i < bulletZ.size(); i++) {
		bulletZ[i] -= 0.5;
	}
	if (score >= 200 && level0) {
		level0 = false;
		score = 0;
	}
	if (score >= 200 && !level0) {
		win();
	}

	if (health <= 0) {
		lose();
	}

	collision();
	moveXWing();
	hit();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	if (thirdPerson) {
		Eye.x = currX;
		Eye.y = currY + 3.0;
	}
	else {
		Eye.x = currX;
		Eye.y = currY + 3.0;

	}
	At.x = currX;
	At.y = currY;
	if(!thirdPerson)
	gluLookAt(Eye.x, Eye.y, Eye.z-10, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	else
		gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);

	glutPostRedisplay();

}

void fire() {

	PlaySound(TEXT("fire2.wav"), NULL, SND_ASYNC | SND_FILENAME);
	
	if (LRBlaster) {
		bulletX.push_back(currX-2.5);
	}
	else {
		bulletX.push_back(currX+2.5);
	}
	if (moveRight) {
		if(LRBlaster)
			bulletY.push_back(currY+1.2);
		else
			bulletY.push_back(currY-1.2);
	}
	else {
		if (moveLeft) {
			if (LRBlaster)
				bulletY.push_back(currY - 1.2);
			else
				bulletY.push_back(currY + 1.2);
		}
		else {
			bulletY.push_back(currY);
		}
	}
	bulletZ.push_back(currZ);

	LRBlaster = !LRBlaster;
}

void drawBullets() {
	for (int i = 0; i < bulletX.size(); i++) {
		
		glPushMatrix();

		GLUquadricObj* qobj;
		qobj = gluNewQuadric();
		
		glTranslated(bulletX[i], bulletY[i], bulletZ[i]);
		glBindTexture(GL_TEXTURE_2D, tex2);
		gluQuadricTexture(qobj, true);
		gluQuadricNormals(qobj, GL_SMOOTH);
		gluCylinder(qobj, 0.1f, 0.1f, 1, 32, 32);
		gluDeleteQuadric(qobj);

		glPopMatrix();

	}

}

//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(10, 10);

	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);

	glutKeyboardUpFunc(myKeyboardUp);
	glutKeyboardFunc(myKeyboard);

	glutPassiveMotionFunc(myMotion);

	glutMouseFunc(myMouse);

	glutTimerFunc(100, glutTimer, 1);

	glutIdleFunc(Anim);

	myInit();

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_NORMALIZE);
	//glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);
	glutMainLoop();
}