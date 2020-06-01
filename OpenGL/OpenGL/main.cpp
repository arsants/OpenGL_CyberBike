#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <glut.h>

using namespace std;

static int useRGB = 1;
static int useLighting = 1;
static int useQuads = 1;

#define GRAY	0
#define RED	    1
#define GREEN	2
#define SILVER	3
#define SUPER_DARK_GRAY	4
#define ORANGE	5
#define YELLOW	6
#define BLACK	7
#define WHITE	8
#define DARK_GRAY	9

static float colors[10][4] =
{
  {0.8, 0.8, 0.8, 1.0},
  {0.8, 0.0, 0.0, 1.0},
  {0.27, 0.64, 0.27, 1.0},
  {0.8, 0.8, 0.8, 1.0},
  {0.25, 0.25, 0.25, 1.0},
  {1, 0.65, 0.0, 1.0},
  {0.8, 0.8, 0.0, 1.0},
  {0.0, 0.0, 0.0, 0.6},
  {1.0, 1.0, 1.0, 1.0},
  {0.5, 0.5, 0.5, 1.0}
};

static float lightPos[4] =
{ 1.0, 5.5, 5.0, 1.0 };
static float lightAmb[4] =
{ 0.2, 0.2, 0.2, 1.0 };
static float lightDiff[4] =
{ 0.8, 0.8, 0.8, 1.0 };
static float lightSpec[4] =
{ 0.4, 0.4, 0.4, 1.0 };


static void setColor(int c)
{
	glMaterialfv(GL_FRONT_AND_BACK,
		GL_AMBIENT_AND_DIFFUSE, &colors[c][0]);
}

static void drawChessPanel(int w, int h, int evenColor, int oddColor)
{
	static int initialized = 0;
	static int usedLighting = 0;
	static GLuint checklist = 0;

	if (!initialized || (usedLighting != useLighting)) {
		static float square_normal[4] =
		{ 0.0, 0.0, 1.0, 0.0 };
		static float square[4][4];
		int i, j;

		if (!checklist) {
			checklist = glGenLists(1);
		}
		glNewList(checklist, GL_COMPILE_AND_EXECUTE);

		if (useQuads) {
			glNormal3fv(square_normal);
			glBegin(GL_QUADS);
		}
		for (j = 0; j < h; ++j) {
			for (i = 0; i < w; ++i) {
				square[0][0] = -1.0 + 2.0 / w * i;
				square[0][1] = -1.0 + 2.0 / h * (j + 1);
				square[0][2] = 0.0;
				square[0][3] = 1.0;

				square[1][0] = -1.0 + 2.0 / w * i;
				square[1][1] = -1.0 + 2.0 / h * j;
				square[1][2] = 0.0;
				square[1][3] = 1.0;

				square[2][0] = -1.0 + 2.0 / w * (i + 1);
				square[2][1] = -1.0 + 2.0 / h * j;
				square[2][2] = 0.0;
				square[2][3] = 1.0;

				square[3][0] = -1.0 + 2.0 / w * (i + 1);
				square[3][1] = -1.0 + 2.0 / h * (j + 1);
				square[3][2] = 0.0;
				square[3][3] = 1.0;

				if (i & 1 ^ j & 1) {
					setColor(oddColor);
				}
				else {
					setColor(evenColor);
				}

				if (!useQuads) {
					glBegin(GL_POLYGON);
				}
				glVertex4fv(&square[0][0]);
				glVertex4fv(&square[1][0]);
				glVertex4fv(&square[2][0]);
				glVertex4fv(&square[3][0]);
				if (!useQuads) {
					glEnd();
				}
			}
		}

		if (useQuads) {
			glEnd();
		}
		glEndList();

		initialized = 0;
		usedLighting = useLighting;
	}
	else {
		glCallList(checklist);
	}
}

static void drawBackGround(int gridX, int gridY, int firstColor, int SecondColor, double scaleFactor)
{
	glPushMatrix();
	glTranslatef(0.0, 0, 0.0);
	glRotatef(-90.0, 1, 0, 0);
	glScalef(scaleFactor, scaleFactor, scaleFactor);
	drawChessPanel(gridX, gridY, firstColor, SecondColor);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 4, -4);
	glScalef(scaleFactor, scaleFactor, scaleFactor);
	drawChessPanel(gridX, gridY, firstColor, SecondColor);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4.0, 4, 0);
	glRotatef(-90.0, 0, 1, 0);
	glScalef(scaleFactor, scaleFactor, scaleFactor);
	drawChessPanel(gridX, gridY, firstColor, SecondColor);
	glPopMatrix();
}

void draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawBackGround(10, 10, WHITE, WHITE, 15);

	static double pos[3] = {0, 0, -3};

	//Переднее колесо
	glPushMatrix();
	setColor(SUPER_DARK_GRAY);
	glTranslatef(pos[0]+0, pos[0] + 2, pos[0] + 0);
	glScaled(0.4, 0.4, 0.4);
	glRotatef(90, 0, 0, 1);
	glutSolidTorus(0.8, 1.2, 20, 50);
	glPopMatrix();


	//Передняя сошка
	glPushMatrix();
	setColor(SILVER);
	glTranslatef(pos[0] +0 , pos[0] + 2, pos[0] - 0.45);
	glScaled(0.4, 0.4, 0.4);
	glRotatef(90, 0, 0, 1);
	gluCylinder(gluNewQuadric(), 0.2, 0.2, 2.3, 30, 30);
	glPopMatrix();

	//Правая передняя
	glPushMatrix();
	setColor(SILVER);
	glTranslatef(pos[0] - 0.68, pos[0] + 3.2, pos[0] + 0.35);
	glScaled(0.4, 0.3, 0.3);
	glRotatef(90, 1, 0, 0);
	glRotatef(-10, 1, 0, 0);
	glRotatef(25, 0, 1, 0);
	gluCylinder(gluNewQuadric(), 0.2, 0.2, 4, 30, 30);
	glPopMatrix();

	glPushMatrix();
	setColor(SILVER);
	glTranslatef(pos[0] + 0, pos[0] + 1.95, pos[0] + 0.47);
	glScaled(0.4, 0.3, 0.3);
	glRotatef(-90, 1, 0, 0);
	glRotatef(20, 1, 0, 0);
	gluCylinder(gluNewQuadric(), 0.2, 0.2, 0.8, 30, 30);
	glPopMatrix();


	//левая передняя
	glPushMatrix();
	setColor(SILVER);
	glTranslatef(pos[0] - 0.68, pos[0] + 3.2, pos[0] - 0.35);
	glScaled(0.4, 0.3, 0.3);
	glRotatef(90, 1, 0, 0);
	glRotatef(10, 1, 0, 0);
	glRotatef(25, 0, 1, 0);
	gluCylinder(gluNewQuadric(), 0.2, 0.2, 4, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glPushMatrix();
	setColor(SILVER);
	glTranslatef(pos[0] + 0, pos[0] + 1.95, pos[0] - 0.47);
	glScaled(0.4, 0.3, 0.3);
	glRotatef(-90, 1, 0, 0);
	glRotatef(-20, 1, 0, 0);
	gluCylinder(gluNewQuadric(), 0.2, 0.2, 0.8, 30, 30);
	glPopMatrix();


	//Заднее колесо
	glPushMatrix();
	setColor(SUPER_DARK_GRAY);
	glTranslatef(pos[0] -2.6, pos[0] + 2, pos[0] + 0);
	glScaled(0.4, 0.4, 0.4);
	glRotatef(90, 0, 0, 1);
	glutSolidTorus(0.8, 1.2, 20, 50);
	glPopMatrix();

	//Заднняя сошка
	glPushMatrix();
	setColor(SILVER);
	glTranslatef(pos[0] - 2.6, pos[0] + 2, pos[0] - 0.37);
	glScaled(0.4, 0.4, 0.4);
	glRotatef(90, 0, 0, 1);
	gluCylinder(gluNewQuadric(), 0.2, 0.2, 1.8, 30, 30);
	glPopMatrix();

	//правая задняя
	glPushMatrix();
	setColor(SILVER);
	glTranslatef(pos[0] -1.93, pos[0] + 2.50, pos[0] + 0.45);
	glScaled(0.4, 0.3, 0.3);
	glRotatef(90, 1, 0, 0);
	glRotatef(15, 1, 0, 0);
	glRotatef(-45, 0, 1, 0);
	gluCylinder(gluNewQuadric(), 0.2, 0.2, 2.5, 30, 30);
	glPopMatrix();

	glPushMatrix();
	setColor(SILVER);
	glTranslatef(pos[0] - 1.93, pos[0] + 3.15, pos[0] + 0.20);
	glScaled(0.4, 0.3, 0.3);
	glRotatef(90, 1, 0, 0);
	glRotatef(-20, 1, 0, 0);
	gluCylinder(gluNewQuadric(), 0.2, 0.2, 2.5, 30, 30);
	glPopMatrix();

	//левая задняя
	glPushMatrix();
	setColor(SILVER);
	glTranslatef(pos[0] - 1.93, pos[0] + 2.50, pos[0] - 0.45);
	glScaled(0.4, 0.3, 0.3);
	glRotatef(90, 1, 0, 0);
	glRotatef(-15, 1, 0, 0);
	glRotatef(-45, 0, 1, 0);
	gluCylinder(gluNewQuadric(), 0.2, 0.2, 2.5, 30, 30);
	glPopMatrix();

	glPushMatrix();
	setColor(SILVER);
	glTranslatef(pos[0] - 1.93, pos[0] + 3.15, pos[0] - 0.20);
	glScaled(0.4, 0.3, 0.3);
	glRotatef(90, 1, 0, 0);
	glRotatef(20, 1, 0, 0);
	gluCylinder(gluNewQuadric(), 0.2, 0.2, 2.5, 30, 30);
	glPopMatrix();

	//Корпус
	glPushMatrix();
	setColor(RED);
	glTranslatef(pos[0] - 1.4, pos[0] + 2.5, pos[0] + 0);
	glScaled(0.4, 0.58, 0.4);
	glRotatef(-60, 0, 0, 1);
	glutSolidCube(2);
	glPopMatrix();

	glPushMatrix();
	setColor(RED);
	glTranslatef(pos[0] - 1.3, pos[0] + 3.1, pos[0] + 0);
	glScaled(1.3, 0.2, 0.4);
	glRotatef(0, 0, 0, 1);
	glutSolidCube(2);
	glPopMatrix();


	//Передний подкрылок
	glPushMatrix();
	setColor(RED);
	glTranslatef(pos[0] - 1, pos[0] + 2.8, pos[0] + 0);
	glScaled(0.4, 0.35, 0.4);
	glRotatef(-30, 0, 0, 1);
	glutSolidCube(2);
	glPopMatrix();


	//Задний подкрылок
	glPushMatrix();
	setColor(RED);
	glTranslatef(pos[0] - 1.5, pos[0] + 2.8, pos[0] + 0);
	glScaled(0.60, 0.2, 0.4);
	glRotatef(20, 0, 0, 1);
	glutSolidCube(2);
	glPopMatrix();


	//Хвост
	glPushMatrix();
	setColor(RED);
	glTranslatef(pos[0] - 2.6, pos[0] + 3.2, pos[0] + 0);
	glScaled(0.5, 0.2, 0.4);
	glRotatef(50, 0, 0, 1);
	glutSolidCube(2);
	glPopMatrix();

	//Приборка
	glPushMatrix();
	setColor(RED);
	glTranslatef(pos[0] - 0.2, pos[0] + 3.45, pos[0] + 0);
	glScaled(0.7, 0.4, 0.4);
	glRotatef(-30, 0, 0, 1);
	glutSolidCube(2);
	glPopMatrix();

	//сиденье
	glPushMatrix();
	setColor(SUPER_DARK_GRAY);
	glTranslatef(pos[0] - 1.8, pos[0] + 3.2, pos[0] + 0);
	glScaled(0.7, 0.4, 0.4);
	glRotatef(-30, 0, 0, 1);
	gluSphere(gluNewQuadric(), 1, 10, 10);
	glPopMatrix();

	//бензобак
	glPushMatrix();
	setColor(RED);
	glTranslatef(pos[0] - 0.5, pos[0] + 3.5, pos[0] + 0);
	glScaled(0.5, 0.5, 0.4);
	glRotatef(-30, 0, 0, 1);
	gluSphere(gluNewQuadric(), 1, 10, 10);
	glPopMatrix();

	glPushMatrix();
	setColor(BLACK);
	glTranslatef(pos[0] - 0.8, pos[0] + 3.9, pos[0] + 0);
	glScaled(0.5, 0.5, 0.4);
	glRotatef(90, 0, 1, 0);
	glRotatef(45, 1, 0, 0);
	glutSolidTorus(0.1, 0.15, 30, 30);
	glPopMatrix();

	//ручки
	glPushMatrix();
	setColor(BLACK);
	glTranslatef(pos[0] - 0.4, pos[0] + 3.9, pos[0] + 0.75);
	glScaled(0.5, 0.5, 0.4);
	glRotatef(180, 0, 1, 0);
	gluCylinder(gluNewQuadric(), 0.15, 0.15, 3.5, 30, 30);
	glPopMatrix();

	//передняя фара
	glPushMatrix();
	setColor(YELLOW);
	glTranslatef(pos[0]+0.7, pos[0] + 3.55, pos[0] + 0);
	glScaled(0.05, 0.05, 0.4);
	glRotatef(90, 0, 1, 0);
	glutSolidCube(2);
	glPopMatrix();


	//задняя фара
	glPushMatrix();
	setColor(RED);
	glTranslatef(pos[0] -3.26, pos[0] + 3.2, pos[0] + 0);
	glScaled(0.05, 0.05, 0.3);
	glRotatef(90, 0, 1, 0);
	glutSolidCube(2);
	glPopMatrix();

	glPushMatrix();
	setColor(ORANGE);
	glTranslatef(pos[0] - 3.26, pos[0] + 3.2, pos[0] + 0.35);
	glScaled(0.05, 0.05, 0.05);
	glRotatef(90, 0, 1, 0);
	glutSolidCube(2);
	glPopMatrix();

	glPushMatrix();
	setColor(ORANGE);
	glTranslatef(pos[0] - 3.26, pos[0] + 3.2, pos[0] - 0.35);
	glScaled(0.05, 0.05, 0.05);
	glRotatef(90, 0, 1, 0);
	glutSolidCube(2);
	glPopMatrix();

	//Выхлоп
	glPushMatrix();
	setColor(DARK_GRAY);
	glTranslatef(pos[0] - 2.2, pos[0] + 2, pos[0] + 0.5);
	glScaled(5.7, 0.5, 0.4);
	glRotatef(90, 0, 1, 0);
	glutSolidTorus(0.10, 0.2, 30, 30);
	glPopMatrix();

	glPushMatrix();
	setColor(DARK_GRAY);
	glTranslatef(pos[0] - 1.5, pos[0] + 2, pos[0] + 0.4);
	glScaled(1, 0.5, 0.4);
	glRotatef(-45, 0, 1, 0);
	gluCylinder(gluNewQuadric(), 0.2, 0.2, 0.5, 30, 30);
	glPopMatrix();

	glPushMatrix();
	setColor(DARK_GRAY);
	glTranslatef(pos[0] - 2.2, pos[0] + 2, pos[0] - 0.5);
	glScaled(5.7, 0.5, 0.4);
	glRotatef(90, 0, 1, 0);
	glutSolidTorus(0.10, 0.2, 30, 30);
	glPopMatrix();

	glPushMatrix();
	setColor(DARK_GRAY);
	glTranslatef(pos[0] - 1.5, pos[0] + 2, pos[0] - 0.4);
	glScaled(1, 0.5, 0.4);
	glRotatef(45, 0, 1, 0);
	gluCylinder(gluNewQuadric(), 0.2, 0.2, 0.5, 30, 30);
	glPopMatrix();


	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glutSwapBuffers();

}


int main()
{
	int width = 1920, height = 1080;
	
	glutInitWindowSize(width, height);


	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutCreateWindow("Tesla cyberbike");

	glutDisplayFunc(draw);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 20.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(12, 1, 0, 0);
	glRotatef(75, 0, 1, 0);
	glTranslatef(10, -5, -1);
	//glRotatef(-70, 0, 1, 0);

	//glRotatef(45, 0, 1, 0);

	//перед
	glRotatef(-150, 0, 1, 0);
	/*glRotatef(-55, 0, 1, 0);
	glTranslatef(3, 2, 1);
	glRotatef(15, 1, 0, 0);*/

	/*glTranslatef(2, 3, 1);
	glRotatef(-75, 0, 1, 0);
	glRotatef(50, 1, 0, 0);*/

	glEnable(GL_DEPTH_TEST);

	if (useLighting) {
		glEnable(GL_LIGHTING);
	}
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);

	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glShadeModel(GL_SMOOTH);

	glClearColor(0.0, 0.0, 0.0, 1);
	glClearIndex(0);
	glClearDepth(1);

	glutMainLoop();


	return 0;
}