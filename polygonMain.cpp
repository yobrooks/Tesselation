

// An OpenGL Keyboard and Mouse Interaction Program

#include <GL/glut.h>
#include <stdio.h>
#include <vector>
#include <iostream>

using namespace std; 

//my structs
struct Point{
	GLfloat x, y;
};

struct Polygon{
	int numPoints;
	vector <Point> polygonPoints;
};


//prototypes
void myglutInit(int argc, char ** argv);
void myInit(void);
void display(void);
void drawBox(int x, int y);
void eraseBox(int x, int y); 
void clearBox();
void mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);
void drawLines(int x, int y, Polygon poly);


// These are defined in a global scope
GLubyte red, green, blue;
int COLORS_DEFINED;
// Specity the values to place and size the window on the screen

const int WINDOW_POSITION_X = 100;
const int WINDOW_POSITION_Y = 100;
const int WINDOW_MAX_X = 400;
const int WINDOW_MAX_Y = 400;

// Specify the coordinate ranges for the world coordinates in the 2D Frame

const float WORLD_COORDINATE_MIN_X = 0.0;
const float WORLD_COORDINATE_MAX_X = 400.0;
const float WORLD_COORDINATE_MIN_Y = 0.0;
const float WORLD_COORDINATE_MAX_Y = 400.0;


//declarations continued
Polygon myPolygon;


void myglutInit(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); /* default, not needed */
	glutInitWindowSize(WINDOW_MAX_X, WINDOW_MAX_Y); /* set pixel window */
	glutInitWindowPosition(WINDOW_POSITION_X, WINDOW_POSITION_Y); /* place window top left on display */
	glutCreateWindow("Mouse and Keyboard Interaction"); /* window title */
}


void myInit(void)
{

	/* standard OpenGL attributes */

	glClearColor(1.0, 1.0, 1.0, 1.0); /* white background */
	glColor3f(1.0, 0.0, 0.0); /* draw in red */
	glPointSize(10.0);

	COLORS_DEFINED = 0;

	/* set up viewing window with origin lower left */

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(WORLD_COORDINATE_MIN_X, WORLD_COORDINATE_MAX_X,
		WORLD_COORDINATE_MIN_Y, WORLD_COORDINATE_MAX_Y);
	glMatrixMode(GL_MODELVIEW);
}


void display(void)
{

	/* define a point data type */

	typedef GLfloat point[2];

	point p; /* A point in 2-D space */

	glClear(GL_COLOR_BUFFER_BIT);  /*clear the window */

	if (!COLORS_DEFINED) {
		red = 255;
		green = 0;
		blue = 0;
	}

	glColor3ub(red, green, blue);

	/* define point */

	p[0] = 100;
	p[1] = 100;

	/* plot new point */

	glBegin(GL_POINTS);
	glVertex2fv(p);
	glEnd();


	glFlush(); /* clear buffers */

}

void drawLines(int x, int y, Polygon poly)
{

	poly.numPoints = poly.numPoints+1;
	cout << "Num Points: " << poly.numPoints << endl;
	if(poly.numPoints<1)
       	{	
		cout << "No Points" << endl;
               	poly.polygonPoints[0].x=x;
		poly.polygonPoints[0].y=y;
        }
	else if(poly.numPoints>1)
	{

		glColor3f(0.0f, 1.0f, 0.0f);
               	poly.polygonPoints[poly.numPoints].x=x;
		poly.polygonPoints[poly.numPoints].y=y;

               // cout << "Num Points " << poly.numPoints << " Else Statement" << endl;
		
		typedef GLfloat point [2];
		point p1, p2; 
		p1[0]=x;
		p1[1]=y;

		p2[0]=poly.polygonPoints[poly.numPoints-1].x;
		p2[1]=poly.polygonPoints[poly.numPoints-1].y;

               	glBegin(GL_LINES);
                        glVertex2fv(p1);
                        glVertex2fv(p2);
                glEnd();
                glFlush();

                cout << "Line drawn" << endl;
	}		

	else
	{
		cout << "1 Point" << endl;
		poly.polygonPoints[poly.numPoints].x=x;
		poly.polygonPoints[poly.numPoints].y=y;
	}
}
//draw the points
void drawBox(int x, int y)
{
	typedef GLfloat point[2];
	point p;

	glColor3ub(red, green, blue);

	p[0] = x;
	p[1] = WINDOW_MAX_Y - y;

	glBegin(GL_POINTS);
		glVertex2fv(p);
	glEnd();
	glFlush();			
	
	//drawLines(p);
}


void eraseBox(int x, int y)
{
	typedef GLfloat point[2];
	point p;

	glColor3f(1.0, 1.0, 1.0);

	p[0] = x;
	p[1] = WINDOW_MAX_Y - y;

	glBegin(GL_POINTS);
	glVertex2fv(p);
	glEnd();
	glFlush();
}


void clearBox()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}


void mouse(int button, int state, int x, int y)
{

	cout << "Num Points: " << myPolygon.numPoints << endl;
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		printf("%d   %d\n", x, y);
		drawBox(x, y);
		cout << "Draw Line " << endl;
		drawLines(x, y, myPolygon);
		//cout << "Num Points " << numPoints << endl;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		printf("%d   %d\n", x, y);
		eraseBox(x, y);
	}

	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		printf("%d   %d\n", x, y);
		clearBox();
	}
}


void keyboard(unsigned char key, int x, int y)
{
	if (key == 'q' || key == 'Q') exit(0);
}


int main(int argc, char** argv)
{
	myglutInit(argc, argv); /* Set up Window */
	myInit(); /* set attributes */

			  // Now start the standard OpenGL glut callbacks //

	glutMouseFunc(mouse);  /* Define Mouse Handler */
	glutKeyboardFunc(keyboard); /* Define Keyboard Handler */
	glutDisplayFunc(display); /* Display callback invoked when window opened */
	glutMainLoop(); /* enter event loop */
}

