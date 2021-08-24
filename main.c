
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

//Window Size
GLsizei wh = 5000, ww = 5000;

//Used in drawSquare, square length
//GLfloat size = 3.0;// gcc -o kruskal  kruskal.c  -lGL -lglut -lGLU  -lm -std=c99

float PI = 3.14285714286;

//Radius of circle
int radius = 30, v1 = 0, v2 = 0, mincost, n;

//No. of nodes in window
int nodeCount = 0;

//Max No. of nodes that can be created
int maxNodes = 26;

//2X2 matrix between the nodes
int adjMatrix[26][26];

//No. of nodes selected to draw a line
int lineNodesCount = 0;

//Source node created
int sourceNodeCreated = 0;

//Destination node created
int destinationNodeCreated = 0;

//Kruskal begin
int kruskalRun = 0;

//instruction display
int instruction = 1;

//Create node struct
struct node
{
	int id;
	int xCoordinate;
	int yCoordinate;
} nodes[26], lineNodes[2], sourceNode, destinationNode;

//return distance between two nodes
//pythagorous theorem
int computeDistance(struct node a, struct node b)
{
	return (int)sqrt(pow(abs(a.xCoordinate - b.xCoordinate), 2) + pow(abs(a.yCoordinate - b.yCoordinate), 2));
}

//Check if two nodes intersect i.e if to create a new node or select a node
int noRepeat(int x, int y)
{
	//if no nodes on screen, dont bother
	if (!nodeCount)
		return 1;

	//create a temp node for comparison on the clicked coordinate
	struct node temp = {nodeCount, x, y};

	for (int i = 0; i < nodeCount; i++)
	{
		//if the nodes intersect, assume a node is clicked
		if (computeDistance(temp, nodes[i]) < 2 * radius)
		{
			//push the clicked node to the stack
			lineNodes[lineNodesCount] = nodes[i];
			return 0;
		}
	}
	//no node was pressed, create a new node at x,y
	return 1;
}

//write Distance between the nodes to screen
void writeDistance(char *text, int x, int y)
{
	if (instruction)
		glColor3f(1.0, 1.0, 1.0);
	else
		glColor3f(1.0, 1.0, 0.0);

	glRasterPos2i(x, y);
	int len = strlen(text);
	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
	}
	glFlush();
}

//write Label to nodes to screen
void writeLabel(char text, int x, int y)
{
	//change the color to yellow
	glColor3f(1.0, 1.0, 0.0);
	glRasterPos2i(x, y);
	// int len = strlen(text);
	//for(int i = 0; i < len; i++)
	//{
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text);
	//}
}

//draw Circle at the given coordinates
void drawCircle(float cx, float cy, float r, float num_segments, char colorCharacter)
{
	//theta- angle in radians of the point
	//x- x coordinate of point
	//y- y coordinate of point
	float theta, x, y;

	//Give the center of the circle a label [A-Z]
	if (colorCharacter == 'N')
		writeLabel((char)65 + nodeCount, cx - 5, cy - 14);

	//change the color to green if colorCharacter = 'N'
	if (colorCharacter == 'N')
		glColor3f(0.0, 1.0, 0.0);

	if (colorCharacter == 'B')
		glColor3f(0.0, 0.0, 1.0);

	if (colorCharacter == 'R')
		glColor3f(1.0, 0.0, 0.0);

	//line loop- connect all the points below

	//float xx,yy;
	glBegin(GL_LINE_LOOP);

	for (int i = 0; i < 200; i++)
	{
		//compute new angle and get the x, y coordinate
		theta = 2.0f * PI * i / num_segments;
		x = r * cosf(theta);
		y = r * sinf(theta);

		//xx = 2 * cosf(theta);
		//yy = 2 * sinf(theta);

		//draw the point
		glVertex2f(x + cx, y + cy);
		//printf("(%f , %f )\n",xx + 3, yy + 3);
	}
	glEnd();

	//flush to output screen
	glFlush();
}

//sets the mid point coordinates to the arguments
void getMidPoint(int *midX, int *midY)
{
	*midX = (lineNodes[0].xCoordinate + lineNodes[1].xCoordinate) / 2;
	*midY = (lineNodes[0].yCoordinate + lineNodes[1].yCoordinate) / 2;
}

//draw lines between two  nodes
void drawLine(char color)
{

	//set color to white
	if (color == 'N' || color == 'R')
		glColor3f(0.3, 0.3, 0.3);

	if (color == 'D')
		glColor3f(1, 0, 0);

	if (color == 'M')
		glColor3f(0.51, 0.0, 0.0);

	//draw lines between selected nodes (stored in lineNodes)
	glBegin(GL_LINES);
	glVertex2f(lineNodes[0].xCoordinate, lineNodes[0].yCoordinate);
	glVertex2f(lineNodes[1].xCoordinate, lineNodes[1].yCoordinate);
	glEnd();
	glFlush();

	//Mid Point of the line segment to display distance
	int midX, midY;
	char distanceString[10];
	//get the mid coordinates
	getMidPoint(&midX, &midY);

	//get nodes id, keys for the adjMatrix
	int id1 = lineNodes[0].id;
	int id2 = lineNodes[1].id;
	//compute distance between the nodes.
	int distance = computeDistance(lineNodes[0], lineNodes[1]);
	//store it in adjMatrix
	adjMatrix[id1][id2] = distance;
	adjMatrix[id2][id1] = distance;

	//Convert distance to string
	sprintf(distanceString, "%d", distance);
	//Display to the window
	//getPerpendicularCoordinates(&midX, &midY);
	writeDistance(distanceString, midX, midY);

	glFlush();

	if (color == 'N')
	{
		printf("\n");
		for (int i = 0; i < nodeCount; i++)
		{
			for (int j = 0; j < nodeCount; j++)
			{
				printf("%d \t", adjMatrix[i][j]);
			}
			printf("\n");
		}
	}
}

//mouse click handler
void myMouse(int btn, int state, int x, int y)
{
	//Clear Instructions first
	if (instruction)
	{
		instruction = 0;
		glClear(GL_COLOR_BUFFER_BIT);
	}

	//dont listen to mouse events if kruskal operation is running
	if (kruskalRun)
	{
		return;
	}

	//fix coordinate system
	y = wh - y;

	//left click: create nodes, join nodes
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		// printf("x = %d, y = %d\n",x, y);

		//if not clicked on the circle
		if (noRepeat(x, y))
		{
			//dont draw if nodeCount > 26;
			if (nodeCount >= 26)
			{
				return;
			}

			//draw new circle
			drawCircle(x, y, radius, 200, 'N');
			lineNodesCount = 0;
			//set the values of the new node
			nodes[nodeCount].xCoordinate = x;
			nodes[nodeCount].yCoordinate = y;
			nodes[nodeCount].id = nodeCount;

			//increase the node count
			nodeCount++;
		}

		//draw line
		else
		{
			if (lineNodesCount == 1)
			{
				drawLine('N');
				lineNodesCount = 0;
			}
			else
			{
				lineNodesCount += 1;
			}
		}
	}
}

//myReshape to Handle change in window size
void myReshape(GLsizei w, GLsizei h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);
	ww = w;
	wh = h;
}

//window initilization
void myInit()
{
	glViewport(0, 0, ww, wh);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, ww, 0, wh);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0, 0.0, 0.0, 1);
	glColor3f(1.0, 1.0, 1.0);
	// glPointSize(0);
	glLineWidth(5);
}

//Function to display instructions
void display_hello()
{
	writeDistance("Kruskal Algorithm Simulation", 300, 600);
	writeDistance("--->  Use Mouse Left Button to Create Nodes.", 300, 550);
	writeDistance("--->  Select any two Nodes to form a Path.", 300, 500);
	writeDistance("--->  Use Mouse Right Button to select Source and Destrination.", 300, 450);
	writeDistance("--->  Press ENTER key to Start the Kruskal's Algorithm.", 300, 400);
}

//display function
void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	display_hello();
	glFlush();
}

//fix adjacency matrix: set the distance among the nodes to highest value (99999)
void fixAdjMatrix()
{
	for (int i = 0; i < maxNodes; i++)
	{
		for (int j = 0; j < maxNodes; j++)
		{
			if (j == i)
			{
				adjMatrix[i][j] = 0;
				continue;
			}
			adjMatrix[i][j] = 99999;
		}
	}
}

void delay(int number_of_seconds)
{
	int milli_seconds = 1000 * number_of_seconds;

	clock_t start_time = clock();

	while (clock() < start_time + milli_seconds)
		;
}

void findMin()
{

	int edgwt = 99999, i, j;
	for (int i = 0; i < nodeCount; i++)
	{
		for (int j = i + 1; j < nodeCount; j++)
		{
			if (adjMatrix[i][j] > 0 && adjMatrix[i][j] < edgwt)
			{
				edgwt = adjMatrix[i][j];
				v1 = i;
				v2 = j;
			}
		}
	}
}

void doUnion(int root[], int v1, int v2)
{
	int temp, i;
	temp = root[v2];
	for (int i = 0; i < nodeCount; i++)
	{
		if (root[i] == temp)
		{
			root[i] = root[v1];
		}
	}
}

void Kruskal()
{
	int i, edgwt;
	int root[nodeCount];
	for (int i = 0; i < nodeCount; i++)
	{
		root[i] = i;
	}

	printf("\n\n\tEdges of minimum cost spanning tree are : ");
	i = 0;
	while (i != nodeCount - 1)
	{ // doubt
		findMin();
		edgwt = adjMatrix[v1][v2];

		adjMatrix[v1][v2] = adjMatrix[v2][v1] = 0;

		if (root[v1] != root[v2])
		{
			printf("(%d,%d)", v1, v2);

			lineNodes[0] = nodes[v1];
			lineNodes[1] = nodes[v2];
			delay(700);
			drawLine('D');
			doUnion(root, v1, v2);
			mincost += edgwt;
			i += 1;
		}
	}
	printf("\n\n\tCOST OF MINIMUM SPANNING TREE IS : %d \n", mincost);
}

//keyboard event listener: when enter key is pressed begin kruskal
void beginKruskal(unsigned char key, int x, int y)
{
	//begin kruskal if enter key is pressed
	if ((int)key == 13 && kruskalRun == 0)
	{
		kruskalRun = 1;
		Kruskal();
	}
}

//main function
int main(int argc, char **argv)
{
	fixAdjMatrix();
	printf("Hello\n\n");
	glutInit(&argc, argv);
	glutInitWindowSize(ww, wh);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow("Kruskal Algorithm");
	myInit();
	glutReshapeFunc(myReshape);
	glutMouseFunc(myMouse);
	glutKeyboardFunc(beginKruskal);
	glutDisplayFunc(myDisplay);
	glutMainLoop();
	return 0;
}
