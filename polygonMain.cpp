

// An OpenGL Keyboard and Mouse Interaction Program

#include <GL/glut.h>
#include <stdio.h>
#include <vector>
#include <iostream>

using namespace std; 

//my structs
struct Point{
	double x, y;
};

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


//declarations continued
vector<Point> polygonPoints;
vector<LineSeg> lineSegments;

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

void addPointsLines(Point point, LineSeg line)
{
	 		glBegin(GL_LINES);
                        glColor3f(0.0f, 1.0f, 0.0f);
                                glVertex2d(line.p1.x, line.p1.y);
                                glVertex2d(point.x, point.y);
                        glEnd();
                        glFlush();

                        lineSegments.push_back(line);
                        polygonPoints.push_back(point);
}
 

template <class T>
bool lineSegIntersect(Point newPoint, int i)
{
        bool intersect;
        Point p1=lineSegments[i].p1;
        Point p2= lineSegments[i].p2;
        Point p3=lineSegments[lineSegments.size()-1].p2;
        Point p4=newPoint;

        T denom = ((p2.x - p1.x)*(-(p4.y - p3.y))) - ((-(p4.x - p3.x)*(p2.y - p1.y)));
	cout << "denominator value" << denom << endl;
        if(denom==0)
        {
                intersect=false;
        }
        else{
                T scalarA = ((p3.x - p1.x)*(-(p4.y - p3.y))) - ((-(p4.x - p3.x)*(p3.y - p1.y)));
                T scalarB = ((p2.x - p1.x)*(p3.y - p1.y)) - ((p3.x - p1.x)*(p2.y - p1.y));
                scalarA = scalarA / denom;
                scalarB = scalarB / denom;

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


template <class T> 
void drawLines(T x, T y)
{

	x=(double) x;
	y= (double) y;
	Point newPoint;
	LineSeg newLine;
	newPoint.x=x;
	newPoint.y=y;
	int prevPosition=0;
	bool intersect;

//not drawing first point
	if(polygonPoints.size()<1)
       	{	
               	polygonPoints.push_back(newPoint);
		glBegin(GL_POINTS);
                	glVertex2d(newPoint.x, newPoint.y);
      		  glEnd();
        	  glFlush();
        }
	else if(polygonPoints.size()>=1)
	{
		prevPosition=polygonPoints.size()-1;
		newLine.p1=polygonPoints[prevPosition];
		newLine.p2=newPoint;		
	
		if(lineSegments.size() > 2)
		{
			for(int i=0; i<lineSegments.size(); i++)
			{
				if(lineSegIntersect<int>(newPoint, i)==true){
					intersect=true;
					break;
				}
				else{
					intersect=false;
				}
			}
			if(intersect==false)
			{
				addPointsLines(newPoint, newLine);			
			}
		}	
		else{

			addPointsLines(newPoint, newLine);	
		}
	}
			
}


void closePolygon()
{
	int lastPosition=polygonPoints.size()-1;
	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2d(polygonPoints[0].x, polygonPoints[0].y);
		glVertex2d(polygonPoints[lastPosition].x, polygonPoints[lastPosition].y);
	glEnd();	
	glFlush();
}

//draw the points
void drawBox(int x, int y)
{
	typedef GLfloat point[2];
	point p;

	glColor3ub(red, green, blue);

	p[0] = x;
	p[1] = WINDOW_MAX_Y - y;
	
	drawLines(x, WINDOW_MAX_Y-y);
	glBegin(GL_POINTS);
		glVertex2fv(p);
	glEnd();
	glFlush();			
	
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

void tesselatePolygon()
{

}


void mouse(int button, int state, int x, int y)
{

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		drawBox(x, y);
		//drawLines(x,WINDOW_MAX_Y-y);	
		printf("Point Accepted: %d   %d\n", x, y);
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		printf("%d   %d\n", x, y);
		closePolygon();
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

