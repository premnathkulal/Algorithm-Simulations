#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

void dijkstra();

float wh = 1020, ww = 1080;

int maxNodes = 26;
int adjMatrix[26][26];
int dist[26];
int prev[26];
int visit[26];
char distanceString[9999];

int nodeCount;
int lineNodesCount;
int sourceNodeCreated;
int destinationNodeCreated;
int dijkstraRun;

int instflag = 0;
int initflag = 0;
int mainflag = 0;

int pe = 1;
int dispPath = 0;
int dispTable = 0;

struct node
{
	int id;
	int xCoordinate;
	int yCoordinate;

} nodes[5], lineNodes[2], sourceNode;

void delay(int number_of_seconds)
{
	int milli_seconds = 1000 * number_of_seconds;

	clock_t start_time = clock();

	while (clock() < start_time + milli_seconds)
		;
}

//write Distance between the nodes to screen
void writeDistance(char *text, int x, int y)
{

	glRasterPos2i(x, y);
	int len = strlen(text);
	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
	}
	glFlush();
}

void about_us()
{
	glColor3f(1.0, 0.30, 0.0);
	writeDistance("VIVEKANANDA COLLEGE OF ENGINEERING AND TECHNOLOGY PUTTUR", 300, 550);
	glColor3f(0.0, 0.0, 1.0);
	writeDistance("( DEPARTMENT OF COMPUTER SCIENCE AND ENGINEERING )", 350, 520);
	writeDistance("COMPUTER GRAPHICS LABORATORY WITH MINI PROJECT (15CSL68) ", 320, 480);

	glColor3f(1.0, 0.0, 0.0);
	writeDistance("DIJKSTRA'S ALGORITHM SIMULATION", 450, 400);

	glColor3f(1.0, 0.50, 0.50);
	writeDistance("PRESS 1 TO CONTINUE", 750, 180);

	glColor3f(1.0, 0.30, 0.50);
	writeDistance("SUBMITTED BY", 100, 210);
	glColor3f(1.0, 0.2, 0.0);
	writeDistance("PREMNATH (4VP16CS066)", 100, 180);
}

void myInit()
{

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, ww, 0, wh);
	glMatrixMode(GL_MODELVIEW);

	if (mainflag == 0)
	{
		glClearColor(0.88f, 0.88f, 0.98f, 1.0f);
	}
	else if (instflag == 1)
	{
		glClearColor(0.0f, 0.153f, 0.153f, 0.4f);
	}
	else
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	}

	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(5);

	for (int i = 0; i < maxNodes; i++)
	{
		for (int j = 0; j < maxNodes; j++)
		{
			adjMatrix[i][j] = 99999;
		}
	}

	nodeCount = 0;
	lineNodesCount = 0;
	dijkstraRun = 0;
	sourceNodeCreated = 0;
	destinationNodeCreated = 0;
	pe = 0;
}

void init_display()
{
	if (initflag == 1)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		if (instflag == 1)
		{

			glBegin(GL_POLYGON);
			glColor3f(0.0, 0.204, 0.204);
			glVertex2f(0, 0);
			glVertex2f(ww - (ww / 3.8), 0);
			glVertex2f(ww - (ww / 3.8), wh - (wh / 10.5));
			glVertex2f(0, wh - (wh / 10.5));
			glEnd();

			glColor3f(1.0, 0.0, 0.0);

			writeDistance("INSTRUCTIONS", ww / 3.5, wh - (wh / 3.6));
			writeDistance("______________", ww / 3.5, wh - (wh / 3.5));

			glColor3f(1.0, 1.0, 0.0);
			writeDistance("USE MOUSE LEFT BUTTON TO CREATE NODES", ww / 5.2, wh - (wh / 2.9));
			writeDistance("SELECT ANY TWO NODES TO CONNECT THEM", ww / 5.2, wh - (wh / 2.5));
			writeDistance("USE MOUSE RIGHT BUTTON TO SELECT SOURCE NODE", ww / 5.2, wh - (wh / 2.2));
			writeDistance("PRESS ENTER KEY TO START DIJKSTRA ALGORITHM", ww / 5.2, wh - (wh / 1.95));

			glColor3f(1.0, 0.0, 0.0);

			/*glBegin(GL_POLYGON);
				
					glColor3f(0.0, 0.0, 0.0);
					glVertex2f((ww/3.1), wh-(wh/1.55));
					glVertex2f((ww/2.7), wh-(wh/1.55));
					glVertex2f((ww/2.7),wh-(wh/1.7));
					glVertex2f((ww/3.1), wh-(wh/1.7));

				glEnd();*/

			glColor3f(1.0, 1.0, 1.0);
			writeDistance("OK", ww / 3, wh - (wh / 1.6));

			dijkstraRun = 1;
			//printf("%d",dijkstraRun);
		}
		else if (dispTable == 1 || dispPath == 1)
		{
			if (dispPath == 1)
			{

				if (pe == 1)
				{
					dispPath = 0;
					int aa = 400, bb = wh - (wh / 4.8), i, t;
					glColor3f(1.0, 0.0, 0.0);
					writeDistance("PATH", ww / 2 - 400, wh - (wh / 6.5));
					writeDistance("_____", ww / 2 - 400, wh - (wh / 6.3));

					for (i = 0; i < nodeCount; i += 1)
					{

						if (visit[i] == 1 && i != sourceNode.id)
						{

							sprintf(distanceString, "%d to %d : distance = %d path :", sourceNode.id, i, dist[i]);
							writeDistance(distanceString, ww / 2 - 400, bb);

							aa -= 250;

							t = prev[i];
							sprintf(distanceString, "%d", i);
							writeDistance(distanceString, ww / 2 - aa, bb);

							aa -= 10;

							while (t != sourceNode.id)
							{
								sprintf(distanceString, " <--  %d ", t);
								writeDistance(distanceString, ww / 2 - aa, bb);
								t = prev[t];
								aa -= 50;
							}

							sprintf(distanceString, "  <--  %d ", sourceNode.id);
							writeDistance(distanceString, ww / 2 - aa, bb);

							aa = 400;
							bb -= 30;
						}
					}
				}
				else
				{
				}
			}
		}

		glColor3f(1.0, 0.0, 1.0);

		writeDistance("DIJKSTRA'S ALGORITHM SIMULATION", (ww / 3.857), wh - (wh / 17.5));
		writeDistance("RESULTS", ww - (ww / 6.09909), wh - (wh / 3.72392));
		writeDistance("________", ww - (ww / 6.09909), wh - (wh / 3.6));

		glBegin(GL_POLYGON);
		glColor3f(1.0, 1.0, 1.0);
		glVertex2f(ww - (ww / 4.6), wh - (wh / 3));
		glVertex2f(ww - (ww / 20.7), wh - (wh / 3));
		glVertex2f(ww - (ww / 20.7), wh - (wh / 2.55));
		glVertex2f(ww - (ww / 4.6), wh - (wh / 2.55));
		glEnd();

		glBegin(GL_POLYGON);
		glColor3f(1.0, 1.0, 1.0);
		glVertex2f(ww - (ww / 4.6), wh - (wh / 2.4));
		glVertex2f(ww - (ww / 20.7), wh - (wh / 2.4));
		glVertex2f(ww - (ww / 20.7), wh - (wh / 2.1));
		glVertex2f(ww - (ww / 4.6), wh - (wh / 2.1));
		glEnd();
		glColor3f(1.0, 0.0, 1.0);
		writeDistance("DISTANCE TABLE", ww - (ww / 5.1), wh - (wh / 2.7));
		writeDistance("PATH FROM SOURCE", ww - (ww / 4.8), wh - (wh / 2.2));

		//Borders
		glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex2f(ww - (ww / 3.8), 0);
		glVertex2f(ww - (ww / 3.8), wh);
		glEnd();
		glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex2f(0, wh - (wh / 10.5));
		glVertex2f(ww - (ww / 3.8), wh - (wh / 10.5));

		glEnd();

		/*
			glBegin(GL_POLYGON);
				glColor3f(1.0, 1.0, 1.0);
				glVertex2f(ww-(ww/4.82), wh-(wh/1.035));
				glVertex2f(ww-(ww/4.12)+(ww/5.4), wh-(wh/1.035));
				glVertex2f(ww-(ww/4.12)+(ww/5.4), wh-(wh/1.035)+30.9);
				glVertex2f(ww-(ww/4.82), wh-(wh/1.035)+30.9);
			glEnd();*/

		writeDistance("INSTRUCTIONS !!!", ww - (ww / 5.12), wh - (wh / 1.055));

		glBegin(GL_POLYGON);
		glColor3f(1.0, 1.0, 1.0);
		glVertex2f(ww - (ww / 4.82), wh - (wh / 1.138));
		glVertex2f(ww - (ww / 7.3), wh - (wh / 1.138));
		glVertex2f(ww - (ww / 7.3), wh - (wh / 1.138) + 30.9);
		glVertex2f(ww - (ww / 4.82), wh - (wh / 1.138) + 30.9);
		glEnd();
		glBegin(GL_POLYGON);
		glColor3f(1.0, 1.0, 1.0);
		glVertex2f(ww - (ww / 7.8), wh - (wh / 1.138));
		glVertex2f(ww - (ww / 15.5), wh - (wh / 1.138));
		glVertex2f(ww - (ww / 15.5), wh - (wh / 1.138) + 30.9);
		glVertex2f(ww - (ww / 7.8), wh - (wh / 1.138) + 30.9);
		glEnd();
		glColor3f(1.0, 0.0, 1.0);
		writeDistance("RESET", ww - (ww / 5.2), wh - (wh / 1.15));
		writeDistance("EXIT", ww - (ww / 9), wh - (wh / 1.15));
		glFlush();
	}
	else
	{

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		about_us();
		glFlush();
	}
}

//pythagorous theorem
int computeDistance(struct node a, struct node b)
{
	int distance;
	distance = (int)sqrt(pow(abs(a.xCoordinate - b.xCoordinate), 2) + pow(abs(a.yCoordinate - b.yCoordinate), 2));
	return distance;
}

//Check if two nodes overlapes or not
int noRepeat(int x, int y)
{

	if (!nodeCount)
		return 1;

	struct node temp = {nodeCount, x, y};

	for (int i = 0; i < nodeCount; i++)
	{

		if (computeDistance(temp, nodes[i]) < 2 * 30)
		{
			lineNodes[lineNodesCount] = nodes[i];
			return 0;
		}
	}

	return 1;
}

//write Label to nodes
void writeLabel(char text, int x, int y)
{
	//change the color to yellow
	glColor3f(0.0, 1.0, 0.0);
	glRasterPos2i(x, y);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text);
}

//draw Circle at the given coordinates
void drawCircle(float cx, float cy, float r, char colorCharacter)
{

	float theta, x, y;
	glLineWidth(5);

	if (colorCharacter == 'N')
		writeLabel((char)65 + nodeCount, cx - 5, cy - 14);

	if (colorCharacter == 'N')
		glColor3f(1.0, 0.8, 0.0);

	if (colorCharacter == 'R')
		glColor3f(1.0, 0.0, 0.0);

	glBegin(GL_LINE_LOOP);

	for (int i = 0; i < 20; i++)
	{
		theta = 2.0f * 3.14 * i / 20;
		x = r * cosf(theta);
		y = r * sinf(theta);
		glVertex2f(x + cx, y + cy);
	}

	glEnd();

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

	glLineWidth(5);
	if (color == 'N' || color == 'R')
		glColor3f(0.3, 0.3, 0.3);

	if (color == 'D')
		glColor3f(0.0, 1.0, 0.0);

	if (color == 'M')
		glColor3f(0.5, 0.0, 0.0);

	glBegin(GL_LINES);
	glVertex2f(lineNodes[0].xCoordinate, lineNodes[0].yCoordinate);
	glVertex2f(lineNodes[1].xCoordinate, lineNodes[1].yCoordinate);
	glEnd();
	glFlush();

	int midX, midY;
	char distanceString[10];

	getMidPoint(&midX, &midY);

	int id1 = lineNodes[0].id;
	int id2 = lineNodes[1].id;

	int distance = computeDistance(lineNodes[0], lineNodes[1]);

	adjMatrix[id1][id2] = distance;
	adjMatrix[id2][id1] = distance;

	sprintf(distanceString, "%d Km", distance);

	glColor3f(0.0, 0.0, 1.0);

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

	y = wh - y;

	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mainflag == 1)
	{

		if (dijkstraRun && (x < ww - (ww / 3.8)))
		{
			if ((x > ww / 3.1) && (x < ww / 2.7) && (y < wh - (wh / 1.7)) && (y > wh - (wh / 1.55)))
			{

				instflag = 0;
				dijkstraRun = 0;
				myInit();
				init_display();
			}
			return;
		}

		if ((x > ww - (ww / 4.82)) && (x < ww - (ww / 7.3)) && (y < wh - (wh / 1.138) + 30.9) && (y > wh - (wh / 1.138)))
		{
			printf("CLEARING\n");
			myInit();
			init_display();
		}

		else if ((x > ww - (ww / 7.8)) && (x < ww - (ww / 15.5)) && (y < wh - (wh / 1.138) + 30.9) && (y > wh - (wh / 1.138)))
		{

			printf("Terminated\n");
			exit(0);
		}
		else if ((x > ww - (ww / 4.82)) && (x < (ww - (ww / 4.12) + (ww / 5.4))) && (y < wh - (wh / 1.035) + 30.9) && (y > wh - (wh / 1.035)))
		{

			//printf("Instructions\n");
			instflag = 1;
			myInit();
			init_display();

			glVertex2f(ww - (ww / 4.6), wh - (wh / 2.4));
			glVertex2f(ww - (ww / 20.7), wh - (wh / 2.4));
			glVertex2f(ww - (ww / 20.7), wh - (wh / 2.1));
			glVertex2f(ww - (ww / 4.6), wh - (wh / 2.1));
		}
		else if ((x > ww - (ww / 4.6)) && (x < ww - (ww / 20.7)) && (y < wh - (wh / 2.4)) && (y > wh - (wh / 2.1)))
		{

			dispPath = 1;
			//myInit();
			init_display();
		}

		else if (!(x < ww - (ww / 3.5)) || !(y < wh - (wh / 6.8)))
		{
			return;
		}

		else if (noRepeat(x, y))
		{

			if (nodeCount >= 26)
			{
				return;
			}

			drawCircle(x, y, 30, 'N');
			lineNodesCount = 0;

			nodes[nodeCount].xCoordinate = x;
			nodes[nodeCount].yCoordinate = y;
			nodes[nodeCount].id = nodeCount;

			nodeCount++;
		}

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

	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && (!sourceNodeCreated || !destinationNodeCreated))
	{

		if (nodeCount)
		{

			if (!noRepeat(x, y))
			{

				if (!sourceNodeCreated)
				{
					sourceNode = lineNodes[0];
					sourceNodeCreated = 1;
					drawCircle(sourceNode.xCoordinate, sourceNode.yCoordinate, 30, 'R');
				}
			}
		}
	}
}

void myReshape(GLsizei w, GLsizei h)
{

	/*glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(w <= h){
		glOrtho(-2.0,2.0,-2.0*(GLfloat)h/(GLfloat)w, 2.0*(GLfloat)h/(GLfloat)w,-10.0,10.0);
	}else{
		glOrtho(-2.0*(GLfloat)w/(GLfloat)h, 2.0*(GLfloat)w/(GLfloat)h,-2.0,2.0,-10.0,10.0);
	}
	glMatrixMode(GL_MODELVIEW);
	ww = w;
	wh = h;*/

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);
	ww = w;
	wh = h;
}

/*void reshape (int w, int h) {
    glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
    glMatrixMode (GL_MODELVIEW);
    ww = w;
    wh = h;

}*/

//begin dijkstra
void beginDijkstra(unsigned char key, int x, int y)
{
	//printf("%d",key);
	if ((int)key == 49 && !(initflag == 1))
	{
		initflag = 1;
		mainflag = 1;
		dijkstraRun = 0;
		instflag = 0;
		myInit();
		init_display();
		return;
	}

	if (!sourceNodeCreated)
	{
		printf("Source node not Selected\n");
		return;
	}

	if ((int)key == 13)
	{
		dijkstraRun = 1;
		pe = 1;
		dijkstra();
	}
}

//finding minimum distance
int minimun(int *dist, int *visit)
{
	int u = -1;
	int min = 99999;

	for (int i = 0; i < nodeCount; i++)
	{
		if (visit[i] != 1 && dist[i] < min)
		{
			u = i;
			min = dist[i];
		}
	}
	return u;
}

//Dijkstra's algorithm
void dijkstra()
{

	int u;
	for (int i = 0; i < nodeCount; i++)
	{
		dist[i] = 99999;
		prev[i] = -1;
		visit[i] = -1;
	}

	dist[sourceNode.id] = 0;

	for (int i = 0; i < nodeCount; i++)
	{

		u = minimun(dist, visit);

		if (u == -1)
			break;

		visit[u] = 1;

		for (int j = 0; j < nodeCount; j++)
		{

			if (visit[j] != 1)
			{

				if ((dist[u] + adjMatrix[u][j]) < dist[j])
				{
					dist[j] = dist[u] + adjMatrix[u][j];

					if (prev[j] != -1)
					{
						lineNodes[0] = nodes[prev[j]];
						lineNodes[1] = nodes[j];
						delay(700);
						drawLine('R');

						delay(700);
						lineNodes[0] = nodes[u];
						drawLine('D');
					}
					else
					{
						delay(700);
						lineNodes[1] = nodes[j];
						lineNodes[0] = nodes[u];
						drawLine('D');
					}

					prev[j] = u;
				}
			}
		}
	}

	printf("node(i)\t\tdistance\tprevious\tvisit\n");
	for (int i = 0; i < nodeCount; i++)
	{
		printf("%c\t\t%d\t\t%c\t\t%d\n", 65 + i, dist[i], ((prev[i] != -1) ? 65 + prev[i] : ' '), visit[i]);
	}
}

//main function
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(ww, wh);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow("Dijksta's Algorithm Simulation");
	myInit();
	glutReshapeFunc(myReshape);
	glutMouseFunc(myMouse);
	glutKeyboardFunc(beginDijkstra);
	glutDisplayFunc(init_display);
	glutMainLoop();
	return 0;
}

/*
	GLUT_BITMAP_8_BY_13
	GLUT_BITMAP_9_BY_15
	GLUT_BITMAP_TIMES_ROMAN_10
	GLUT_BITMAP_TIMES_ROMAN_24
	GLUT_BITMAP_HELVETICA_10
	GLUT_BITMAP_HELVETICA_12
	GLUT_BITMAP_HELVETICA_18
*/
