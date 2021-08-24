#include <iostream>
#include <stdlib.h>
#include "math.h"
#include <GL/glut.h>
#include <string.h>

int flag = 0, k = 0;
//Include OpenGL header files, so that we can use OpenGL

typedef struct node
{
	char name[20]; // name of the node
	float data;
	struct node *right;
	struct node *left;
	struct node *parent;
	struct node *next;
} NODE;

NODE *root = NULL;

using namespace std;

void display_list(NODE *head)
{
	if (head)
	{
		cout << "\nList contents:\n";
		NODE *temp = head;
		while (temp)
		{
			cout << "Node : " << temp->name << "  frequency:" << temp->data << endl;
			temp = temp->next;
		}
	}
	else
	{
		cout << "\nEmpty List";
	}
}

void find_smallest(NODE *head, NODE **smallest1, NODE **smallest2)
{
	*smallest1 = NULL;
	*smallest2 = NULL;
	NODE *t = head;
	while (t)
	{
		// Skip the nodes who has parent
		if (t->parent)
		{
			t = t->next;
			continue;
		}

		if (!*smallest1)
		{
			*smallest1 = t;
		}
		else if (!*smallest2)
		{
			*smallest2 = t;
		}
		else if (t->data < (*smallest1)->data)
		{
			if ((*smallest1)->data < (*smallest2)->data)
			{
				*smallest2 = *smallest1;
			}
			*smallest1 = t;
		}
		else if (t->data < (*smallest2)->data)
		{
			*smallest2 = t;
		}
		else
		{
			// Do nothing
		}

		if (*smallest1 && *smallest2)
		{
			if ((*smallest1)->data > (*smallest2)->data)
			{
				NODE *temp = *smallest1;
				*smallest1 = *smallest2;
				*smallest2 = temp;
			}
		}
		t = t->next;
	}
}

void add_element(NODE **head, NODE **e)
{
	if (!*head)
	{
		*head = *e;
	}
	else
	{
		NODE *t = *head;
		while (t->next != NULL)
		{
			t = t->next;
		}
		t->next = *e;
	}
}

void tree_traversal(NODE *root, char code[20])
{
	if (!root)
	{
		return;
	}
	//cout<<"\n NODE : "<<root->name<<"  prob: "<<root->data;
	if (root->left == NULL && root->right == NULL)
	{
		cout << " \nNODE : " << root->name << "  Freq : " << root->data << "  code: " << code;
		return;
	}
	int len = strlen(code);
	if (root->left)
	{
		code[len] = '0';
		code[len + 1] = '\0';
		tree_traversal(root->left, code);
	}
	if (root->right)
	{
		code[len] = '1';
		code[len + 1] = '\0';
		tree_traversal(root->right, code);
	}
}

void drawCircle(float radius, float x1, float y1)
{
	//filled circle
	float x2, y2;
	float angle;

	glColor3f(0.8, 0.498039, 0.196078);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}
	glFlush();
	glEnd();
}

void draw_line1(float x1, float y1, float x2, float y2)
{

	glColor3f(0.0, 0.6, 0.6);
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}

void draw_text(char *text, float x, float y)
{

	GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;

	glRasterPos3f(x, y, 0);
	for (int i = 0; text[i] != '\0'; i++)
	{
		glutBitmapCharacter(font_style, text[i]);
	}
}

void drawNode1(NODE *t_root, float x1, float y1, int level)
{
	if (t_root == NULL)
	{
		return;
	}

	float segments = 25;
	float radius = 1.0;
	float left_angle = 245;
	float right_angle = 115;
	float branch_length = 12 - level * 2.5;
	float angle_change = 20;
	// Draw the current circle

	drawCircle(radius, x1, y1);
	//glColor3f(1.0, 1.0, 0.0);
	//draw_text(t_root->name, x1, y1 + 1);

	if (t_root->left)
	{
		// Draw the Left circle
		float angle1 = left_angle - level * angle_change;
		double radian2 = angle1 * 3.14 / 180;
		float m = (double)tan((double)radian2);
		float x2 = x1 + branch_length * sin((double)radian2);
		float y2 = y1 + branch_length * cos((double)radian2);

		drawNode1(t_root->left, x2, y2, level + 1);

		//draw_line1(x1, y1, x2, y2);

		float x3 = (x1 + x2) / 2;
		float y3 = (y1 + y2) / 2;
		char label1[2];
		label1[0] = '0';
		label1[1] = '\0';

		//glColor3f(1.0, 1.0, 0.0);
		//draw_text(label1, x3, y3 + 1);
	}

	if (t_root->right)
	{
		// Draw the Right circle
		float angle2 = right_angle + level * angle_change;
		float radian2 = angle2 * 3.14 / 180;
		float m = (double)tan((double)radian2);
		float x4 = x1 + branch_length * sin((double)radian2);
		float y4 = y1 + branch_length * cos((double)radian2);
		drawNode1(t_root->right, x4, y4, level + 1);
		//draw_line1(x1, y1, x4, y4);

		float x5 = (x1 + x4) / 2;
		float y5 = (y1 + y4) / 2;
		char label2[2];
		label2[0] = '1';
		label2[1] = '\0';
		//glColor3f(1.0, 1.0, 0.0);
		//draw_text(label2, x5, y5 + 1);
	}
	glFlush();
}
void drawNode2(NODE *t_root, float x1, float y1, int level)
{
	if (t_root == NULL)
	{
		return;
	}

	float segments = 25;
	float radius = 1.0;
	float left_angle = 245;
	float right_angle = 115;
	float branch_length = 12 - level * 2.5;
	float angle_change = 20;
	// Draw the current circle

	drawCircle(radius, x1, y1);
	//glColor3f(1.0, 1.0, 0.0);
	//draw_text(t_root->name, x1, y1 + 1);

	if (t_root->left)
	{
		// Draw the Left circle
		float angle1 = left_angle - level * angle_change;
		double radian1 = angle1 * 3.14 / 180;
		float m = (double)tan((double)radian1);
		float x2 = x1 + branch_length * sin((double)radian1);
		float y2 = y1 + branch_length * cos((double)radian1);

		drawNode2(t_root->left, x2, y2, level + 1);
		draw_line1(x1, y1, x2, y2);

		float x3 = (x1 + x2) / 2;
		float y3 = (y1 + y2) / 2;
		char label1[2];
		label1[0] = '0';
		label1[1] = '\0';
		//glColor3f(1.0, 1.0, 0.0);
		//draw_text(label, x3, y3 + 1);
	}

	if (t_root->right)
	{
		// Draw the Right circle
		float angle2 = right_angle + level * angle_change;
		float radian2 = angle2 * 3.14 / 180;
		float m = (double)tan((double)radian2);
		float x4 = x1 + branch_length * sin((double)radian2);
		float y4 = y1 + branch_length * cos((double)radian2);
		drawNode2(t_root->right, x4, y4, level + 1);
		draw_line1(x1, y1, x4, y4);

		float x5 = (x1 + x4) / 2;
		float y5 = (y1 + y4) / 2;
		char label2[2];
		label2[0] = '1';
		label2[1] = '\0';
		//glColor3f(1.0, 1.0, 0.0);
		//draw_text(label, x3, y3 + 1);
	}
	glFlush();
}
void drawNode3(NODE *t_root, float x1, float y1, int level)
{
	if (t_root == NULL)
	{
		return;
	}

	float segments = 25;
	float radius = 1.0;
	float left_angle = 245;
	float right_angle = 115;
	float branch_length = 12 - level * 2.5;
	float angle_change = 20;
	// Draw the current circle

	drawCircle(radius, x1, y1);
	//glColor3f(1.0, 1.0, 0.0);
	glColor3f(0.737255, 0.560784, 0.560784);
	draw_text(t_root->name, x1, y1 + 1);

	if (t_root->left)
	{
		// Draw the Left circle
		float angle1 = left_angle - level * angle_change;
		double radian1 = angle1 * 3.14 / 180;
		float m = (double)tan((double)radian1);
		float x2 = x1 + branch_length * sin((double)radian1);
		float y2 = y1 + branch_length * cos((double)radian1);

		drawNode3(t_root->left, x2, y2, level + 1);
		draw_line1(x1, y1, x2, y2);

		float x3 = (x1 + x2) / 2;
		float y3 = (y1 + y2) / 2;
		char label1[2];
		label1[0] = '0';
		label1[1] = '\0';
		//glColor3f(1.0, 1.0, 0.0);
		//draw_text(label1, x3, y3 + 1);
	}

	if (t_root->right)
	{
		// Draw the Right circle
		float angle2 = right_angle + level * angle_change;
		float radian2 = angle2 * 3.14 / 180;
		float m = (double)tan((double)radian2);
		float x4 = x1 + branch_length * sin((double)radian2);
		float y4 = y1 + branch_length * cos((double)radian2);
		drawNode3(t_root->right, x4, y4, level + 1);
		draw_line1(x1, y1, x4, y4);

		float x5 = (x1 + x4) / 2;
		float y5 = (y1 + y4) / 2;
		char label2[2];
		label2[0] = '1';
		label2[1] = '\0';
		//glColor3f(1.0, 1.0, 0.0);
		//draw_text(label2, x5, y5 + 1);
	}

	glFlush();
}
void drawNode4(NODE *t_root, float x1, float y1, int level)
{
	if (t_root == NULL)
	{
		return;
	}

	float segments = 25;
	float radius = 1.0;
	float left_angle = 245;
	float right_angle = 115;
	float branch_length = 12 - level * 2.5;
	float angle_change = 20;
	// Draw the current circle

	drawCircle(radius, x1, y1);
	//glColor3f(1.0, 1.0, 0.0);
	glColor3f(0.0, 1.0, 0.560784);
	draw_text(t_root->name, x1, y1 + 1);

	if (t_root->left)
	{
		// Draw the Left circle
		float angle1 = left_angle - level * angle_change;
		double radian1 = angle1 * 3.14 / 180;
		float m = (double)tan((double)radian1);
		float x2 = x1 + branch_length * sin((double)radian1);
		float y2 = y1 + branch_length * cos((double)radian1);

		drawNode4(t_root->left, x2, y2, level + 1);
		draw_line1(x1, y1, x2, y2);

		float x3 = (x1 + x2) / 2;
		float y3 = (y1 + y2) / 2;
		char label1[2];
		label1[0] = '0';
		label1[1] = '\0';
		glColor3f(1.0, 1.0, 0.0);
		draw_text(label1, x3, y3 + 1);
	}

	if (t_root->right)
	{
		// Draw the Right circle
		float angle2 = right_angle + level * angle_change;
		float radian2 = angle2 * 3.14 / 180;
		float m = (double)tan((double)radian2);
		float x4 = x1 + branch_length * sin((double)radian2);
		float y4 = y1 + branch_length * cos((double)radian2);
		drawNode4(t_root->right, x4, y4, level + 1);
		draw_line1(x1, y1, x4, y4);

		float x5 = (x1 + x4) / 2;
		float y5 = (y1 + y4) / 2;
		char label2[2];
		label2[0] = '1';
		label2[1] = '\0';
		glColor3f(1.0, 0.0, 0.0);
		draw_text(label2, x5, y5 + 1);
	}

	glFlush();
}

void frontscreen()
{

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(0, 10, -30);

	glColor3f(0, 0, 1);
	draw_text("VIVEKANANDA COLLEGE OF ENGINEERING AND TECHNOLOGY", -18, 0.5);

	glColor3f(0.7, 0, 1);
	draw_text("DEPARTMENT OF COMPUTER SCIENCE AND ENGINEERING", -17, -1.5);

	glColor3f(1, 0.5, 0);
	draw_text("( COMPUTER GRAPHICS LAB WITH MINI PROJECT )", -15, -3);

	glColor3f(1, 0, 0);
	draw_text(" HUFFMAN CODE SIMULATION ", -10, -5);

	draw_text("PRESS ENTER TO START", -9, -20);
	glFlush();
}
void decide()
{
	drawNode1(root, 0, 0, 0);
	drawNode2(root, 0, 0, 0);
	drawNode3(root, 0, 0, 0);
	drawNode4(root, 0, 0, 0);
}

void display(void)
{
	glClearColor(0.1, 0.2, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0, 10, -30);
	//glColor3f(1, 1, 1);
	decide();
	glFlush();
	//glutSwapBuffers();
	//glutPostRedisplay();
}

void mydisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (flag == 0)
		frontscreen();
	if (flag == 1)
		display();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 13:
		if (flag == 0) //Ascii of 'enter' key is 13
			flag = 1;
		break;
	case 27:
		exit(0);
		break;
	}

	if (key == '1')
		k = 1;
	if (key == '2')
		k = 2;
	if (key == '3')
		k = 3;
	if (key == '4')
		k = 4;
	fflush(stdin);
	mydisplay();
}

int main(int argc, char **argv)
{

	NODE *list = NULL;
	float arr[10];
	int n;
	cout << "Enter n: ";
	cin >> n;
	cout << "Enter Frequency values: ";
	for (int i = 0; i < n; i++)
	{
		cin >> arr[i];
		// Create a new node and append it to the list
		NODE *current = new NODE;
		current->data = arr[i];
		current->next = NULL;
		current->left = NULL;
		current->right = NULL;
		current->parent = NULL;
		current->name[0] = 65 + i;
		current->name[1] = '\0';

		// Appending the node to the list
		add_element(&list, &current);
	}

	// Display the nodes entered by the user along with their probabilities
	display_list(list);

	while (1)
	{

		// Find the 2 smallest elements from the list
		NODE *smallest1 = NULL;
		NODE *smallest2 = NULL;
		find_smallest(list, &smallest1, &smallest2);

		if (!smallest2)
		{
			root = smallest1;
			break;
		}

		float small1 = smallest1->data;
		float small2 = smallest2->data;
		NODE *current = new NODE;
		current->data = small1 + small2;
		current->left = smallest1;
		current->right = smallest2;
		current->parent = NULL;
		current->next = NULL;
		strcpy(current->name, smallest1->name);
		strcat(current->name, smallest2->name);

		smallest1->parent = current;
		smallest2->parent = current;

		add_element(&list, &current);
	}

	cout << "\n\nHuffman code :\n\n";
	char code[20];
	code[0] = '\0';
	tree_traversal(root, code);

	// OPENGL Drawing functions
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(1080, 1020);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Huffman Code");
	// Register function pointers to the drawing framework
	glutDisplayFunc(mydisplay);
	//glutIdleFunc(mydisplay);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	cout << endl;
	return 0;
}
