#include <stdlib.h>
#include <GL/glut.h>
#include <stdbool.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>

using namespace std;
float speed = 0.7;
float speed1 = 0.9;
float speed3 = 0.2;

float block1x = 90.0, block1y = 0, block3x = 90.0, block3y = 0, block2x = 90.0, block2y = 0, heli = 0.0, mnk = 1;
int i = 0, ijk = 0, flg = 1;
static int ran = 0;
static int u = 0, d = 0;

bool isLeftKeyPressed = false;
bool isRightKeyPressed = false;

char str1[20];

void init(void)
{
	block1y = (rand() % 2);
	block2y = (rand() % 8) + 60;
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glShadeModel(GL_SMOOTH);
	glLoadIdentity();
	//glOrtho(0.0, 100.0, 0.0, 100.0, -1.0 , 0.0);
	gluOrtho2D(0.0, 100.0, 0.0, 100.0);
}

//Helicopter
void drawcopter()
{
	glColor3f(1.0, 0.0, 0.0);
	glRectf(10, 49.8, 19.8, 44.8);
	glRectf(2, 46, 10, 48);
	glRectf(2, 46, 4, 51);
	glRectf(14, 49.8, 15.8, 52.2);
	glRectf(7, 53.6, 22.8, 52.2);
}

void renderBitmapString(float x, float y, float z, void *font, char *string)
{
	char *c;
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (((((int)block1x == 10 || (int)block1x == 7 || (int)block1x == 4 || (int)block1x == 1) && (((int)block1y > 40 + (int)heli) && ((int)block1y < 55 + (int)heli)))) || ((((int)block2x == 10 || (int)block2x == 7 || (int)block2x == 4 || (int)block2x == 1) && (((int)block2y > 40 + (int)heli) && ((int)block2y < 55 + (int)heli)))) || ((((int)block3x == 10 || (int)block3x == 7 || (int)block3x == 4 || (int)block3x == 1) && (((int)block3y > 40 + (int)heli) && ((int)block3y < 55 + (int)heli)))))

	{
		glColor3f(1.0, 0.0, 0.0);
		glRectf(0.0, 0.0, 100.0, 100.0);
		glColor3f(1.0, 0.0, 0.0);

		renderBitmapString(10, 70, 0, GLUT_BITMAP_TIMES_ROMAN_24, "\n******GAME OVER******\n");
		renderBitmapString(20, 58, 0, GLUT_BITMAP_TIMES_ROMAN_24, "SCORE:");
		renderBitmapString(70, 58, 0, GLUT_BITMAP_TIMES_ROMAN_24, str1);

		glutSwapBuffers();
		glFlush();
		printf("\n*********GAME OVER***********\n");
		printf("\nStart New Game\n");
		exit(0);
	}

	else if (flg == 1)
	{
		flg = 0;
		glColor3f(0.3, 0.7, 0.2);
		printf("Project Developed By cseminiprojects.com\n");
		printf("\n------------------------------------------------------------------\n");
		printf("Steps to run this cg project\n");
		printf("\n------------------------------------------------------------------\n");
		printf("Step1: Click any mouse key to start\n");
		printf("\nStep2: Click and hold left mouse key to handle the helicopter\n\n");
		drawcopter();
		glutSwapBuffers();
		glFlush();
	}
	else
	{
		glPushMatrix();

		renderBitmapString(20, 3, 0, GLUT_BITMAP_TIMES_ROMAN_24, str1);
		glTranslatef(0.0, heli, 0.0);
		drawcopter();

		if (block1x < -50 && block2x < -50 && block3x < -50)
		{
			block1x = 90;
			block2x = 90;
			block3x = 90;
			block1y = (rand() % 3) + 90;
			block2y = (rand() % 16) + 30;
			block3y = (rand() % 32) + 10;
		}
		else
		{
			block1x -= speed;
			block2x -= speed;
			block3x -= speed;
		}

		glTranslatef(block2x, -heli, 0.0);

		glColor3f(1.0, 0.0, 0.0);

		glRectf(block1x, block1y, block1x + 3, block1y + 5);
		glRectf(block2x, block2y, block2x + 3, block2y + 5);
		glRectf(block3x, block3y, block3x + 3, block3y + 5);

		glPopMatrix();
		glutSwapBuffers();
		glFlush();
	}
}

void Heliup()
{

	heli += 0.3;

	i++;
	glutPostRedisplay();
}

void Helidown()
{
	heli -= 0.1;
	i--;
	glutPostRedisplay();
}

void Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-2.0, 2.0, -2.0 * (GLfloat)h / (GLfloat)w, 2.0 * (GLfloat)h / (GLfloat)w, -10.0, 20.0);
	else
		glOrtho(-2.0 * (GLfloat)w / (GLfloat)h, 2.0 * (GLfloat)w / (GLfloat)h, -2.0, 2.0, -10.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}

/*void mouse(int btn,int state,int x,int y)
{
	if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		glutIdleFunc(Heliup);

	if (btn == GLUT_LEFT_BUTTON && state == GLUT_UP)
		glutIdleFunc(Helidown);
	glutPostRedisplay();
}
*/

void handleSpecialKeypress(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		isLeftKeyPressed = true;
		if (!isRightKeyPressed)
		{
			u = 1;
			glutIdleFunc(Heliup);
		}
		break;
	case GLUT_KEY_DOWN:
		isRightKeyPressed = true;
		if (!isLeftKeyPressed)
		{
			glutIdleFunc(Helidown);
		}
		break;
	}
}

void handleSpecialKeyReleased(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		isLeftKeyPressed = false;
		break;
	case GLUT_KEY_DOWN:
		isRightKeyPressed = false;
		break;
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(200, 800);
	glutCreateWindow("Helicopter Game");
	init();
	glutReshapeFunc(Reshape);
	glutDisplayFunc(display);

	glutSpecialFunc(handleSpecialKeypress);
	glutSpecialUpFunc(handleSpecialKeyReleased);

	glutMainLoop();
}
