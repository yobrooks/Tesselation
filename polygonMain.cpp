

// An OpenGL Keyboard and Mouse Interaction Program

#include <GL/glut.h>
#include <stdio.h>
#include <vector>
#include <iostream>

using namespace std; 

//my struct declarations

//struct for a point, holds double values for the x and y coordinates
struct Point{
	double x, y;
};

//struct for a line segment, holds 2 points that come from the Point struct declaration above
struct LineSeg{
	Point p1, p2;
};


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

//boolean to keep track of when the user is done drawing points
//when set to true they cannot draw anymore
bool doneDrawing=false;
//vector declarations
vector<Point> polygonPoints; //vector of points to hold the points in the polygon
vector<LineSeg> lineSegments; //vector of line segments to hold the line segments that make up the polygon


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
	glPointSize(3.0);

	COLORS_DEFINED = 0;

	/* set up viewing window with origin lower left */

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(WORLD_COORDINATE_MIN_X, WORLD_COORDINATE_MAX_X,
		WORLD_COORDINATE_MIN_Y, WORLD_COORDINATE_MAX_Y);
	glMatrixMode(GL_MODELVIEW);
}

 

template <class T>
bool lineSegIntersect(Point newPoint, int i)
{
       //declarations
	bool intersect;
        Point p1=lineSegments[i].p1;
        Point p2= lineSegments[i].p2;
        Point p3=lineSegments[lineSegments.size()-1].p2;
        Point p4=newPoint;

        T denom = ((p2.x - p1.x)*((p4.y - p3.y))) - ((p2.y - p1.y)*(p4.x - p3.x));
        if(denom==0)
        {
                intersect=false;
        }
        else{
                T scalarA = ((p1.y - p3.y)*(p4.x - p3.x)) - ((p1.x - p3.x)*(p4.y - p3.y));
                T scalarB = ((p1.y - p3.y)*(p2.x - p1.x)) - ((p1.x - p3.x)*(p2.y - p1.y));
                scalarA = scalarA / denom;
		scalarB=scalarB/denom;
		cout << "scalar A" << scalarA << endl;
		cout << "scarlar B" << scalarB << endl;

                 if (scalarA > 0 && scalarA < 1 && scalarB>0 && scalarB < 1)
                {
                        intersect=true;
                }
                else
                        intersect=false;
        }
	cout << "line seg intersect function value: " << intersect << endl;
        return intersect;
}

  

//function to connect the last point drawn to the first point drawn
void closePolygon()
{
	int lastPosition=polygonPoints.size()-1;
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2d(polygonPoints[0].x, polygonPoints[0].y);
		glVertex2d(polygonPoints[lastPosition].x, polygonPoints[lastPosition].y);
	glEnd();	
	glFlush();
}

//draw the points
void drawPoint(Point p)
{
	
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
		glVertex2d(p.x, p.y);
	glEnd();
	glFlush();		
	
}

//draw the lines
void drawLine(Point p, Point prevp)
{
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
		glVertex2d(prevp.x, prevp.y);
		glVertex2d(p.x, p.y);
	glEnd();
	glFlush();
}

//function to process what is being drawn
//adds the points and lines and checks for intersections
template <class T>
void processDraw(T x, T y)
{
	//declarations
	x = (double) x;
	y = (double) y;
	Point newPoint;
	Point prevPoint;
	LineSeg newLine;
	newPoint.x = x;
	newPoint.y = y;
	int prevPosition = 0;
	bool intersect = false;
	
	//if there are no points drawn yet, draw a point and add it to the point vector
	if(polygonPoints.size() < 1)
	{
		drawPoint(newPoint);
		polygonPoints.push_back(newPoint);
	}
	
	else if(polygonPoints.size() >= 1)
	{
		prevPosition = polygonPoints.size() -1;
		prevPoint.x = polygonPoints[prevPosition].x;
		prevPoint.y = polygonPoints[prevPosition].y;
		newLine.p1 = prevPoint;
		newLine.p2 = newPoint;
		
		//if(lineSegments.size() < 1)
		//{
			drawPoint(newPoint);
			drawLine(newPoint, prevPoint);
			polygonPoints.push_back(newPoint);
			lineSegments.push_back(newLine);
		//}
	}
}

void clearBox()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}



void mouse(int button, int state, int x, int y)
{

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		if(doneDrawing==false){
			processDraw(x, WINDOW_MAX_Y-y);
			cout << "Size Points: " << polygonPoints.size() << " Line Points: " << lineSegments.size() << endl;
			//printf("Point Accepted: %d   %d\n", x, y);
		}
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		printf("%d   %d\n", x, y);
		closePolygon();
		doneDrawing=true;
	}

	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		printf("%d   %d\n", x, y);
		clearBox();
	}
}


void keyboard(unsigned char key, int x, int y)
{
/*	switch(tolower(key)){

		case q: exit(0);
		case t: tesselate();
			break;
		case i: returnToInitial();
			break;
		case f: filledPolygon();
			break;
		
	}*/
}


int main(int argc, char** argv)
{
	myglutInit(argc, argv); /* Set up Window */
	myInit(); /* set attributes */

			  // Now start the standard OpenGL glut callbacks //

	glutMouseFunc(mouse);  /* Define Mouse Handler */
	glutKeyboardFunc(keyboard); /* Define Keyboard Handler */
	//glutDisplayFunc(display); /* Display callback invoked when window opened */
	glutMainLoop(); /* enter event loop */
}

