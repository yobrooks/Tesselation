

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

//struct for triangle that holds 3 points as the vertices
struct Triangle{
	Point vert1, vert2, vert3;
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
vector<Triangle> tessTriangles; //vector of triangles that will be formulated during tesselation

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

 
//equation for scalar A and B and denominator from : http://www.faqs.org/faqs/graphics/algorithms-faq/
template <class T>
bool lineSegIntersect(Point newPoint, Point lastPoint, int i)
{
       //declarations
	bool intersect;
        Point p1=lineSegments[i].p1;
        Point p2= lineSegments[i].p2;
        Point p3=lastPoint;
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
		scalarB = scalarB / denom;

                 if (scalarA > 0 && scalarA < 1 && scalarB>0 && scalarB < 1)
                {
                        intersect=true;
                }
                else
                        intersect=false;
        }
        return intersect;
}

void fillPolygon()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_POLYGON);
		for(int i=0; i<polygonPoints.size(); i++)
		{
			glVertex2d(polygonPoints[i].x, polygonPoints[i].y);
		}
		glVertex2d(polygonPoints[0].x, polygonPoints[0].y);
	glEnd();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glFlush();
	
} 

void initialOutline()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
		for(int i=0; i < polygonPoints.size(); i++)
		{
			glVertex2d(polygonPoints[i].x, polygonPoints[i].y);
		}
		glVertex2d(polygonPoints[0].x, polygonPoints[0].y);
	glEnd();
	glFlush();
}

//function to connect the last point drawn to the first point drawn
void closePolygon()
{
	int lastPosition=polygonPoints.size()-1;
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2d(polygonPoints[0].x, polygonPoints[0].y);
		glVertex2d(polygonPoints[lastPosition].x, polygonPoints[lastPosition].y);
	glEnd();	
	glFlush();
}

//draw the points
void drawPoint(Point p)
{
	
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_POINTS);
		glVertex2d(p.x, p.y);
	glEnd();
	glFlush();		
	
}

//draw the lines
void drawLine(Point p, Point prevp)
{
	glColor3f(0.0f, 0.0f, 1.0f);
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
	
	//if there are more than one point
	else if(polygonPoints.size() >= 1)
	{
		//declarations
		prevPosition = polygonPoints.size() -1;
		prevPoint.x = polygonPoints[prevPosition].x;
		prevPoint.y = polygonPoints[prevPosition].y;
		newLine.p1 = prevPoint;
		newLine.p2 = newPoint;
		
		//if there a no line segments drawn yet, draw the point 
		//and then the line and add the points to the vectors
		if(lineSegments.size() < 1)
		{
			drawPoint(newPoint);
			drawLine(newPoint, prevPoint);
			polygonPoints.push_back(newPoint);
			lineSegments.push_back(newLine);
		}
		
		else if(lineSegments.size() >= 1)
		{
			//check for intersection using a for loop
			//if ends up true, break the loop
			//if it's false at the end of the loop, draw and add points
			for(int i=0; i < lineSegments.size(); i++)
			{
				intersect = lineSegIntersect<double>(newPoint, lineSegments[lineSegments.size()-1].p2, i);
			//	cout << "Intersect? " << intersect << endl;
				if(intersect == true)
					break;
			}
			
			if(intersect==false)
			{
				drawPoint(newPoint);
				drawLine(newPoint, prevPoint);
				polygonPoints.push_back(newPoint);
				lineSegments.push_back(newLine);
			}
		}
	}
}


double isCCW(Point p1, Point p2, Point p3)
{

		Point crossP1, crossP2;
		crossP1.x = p1.x - p2.x;
		crossP1. y = p1.y - p2.y;
		crossP2.x = p3.x - p2.x;
		crossP2.y = p3.y - p2.y;

		return (crossP1.x * crossP2.y) - (crossP2.x * crossP1.y);
		
}


void tesselate()
{
	vector<Point> tempPoints = polygonPoints; //temporary vector that is a copy of the vector that holds the polygon points
						//will be used to manipulate the point data during tesselation 
	int counter = 0;
	int lineSegIndex = 4;
	double zComp = 0;
	bool intersect = false;
	Triangle myTriangle;
	
	//while there are more than 3 points left in the vector
	while(tempPoints.size() > 3)
	{
		cout << "Start Vertex: " << counter << "Points left: " << tempPoints.size() << endl;
		//needs to be a check somewhere to make sure counter doesn't go over tempPoints.size()
		//if so, reset back to beginning
	
		//assign values and check if point is CCW
		myTriangle.vert1 = tempPoints[counter];
		myTriangle.vert2 = tempPoints[counter+1];
		myTriangle.vert3 = tempPoints[counter+2];
	
		zComp = isCCW(myTriangle.vert1, myTriangle.vert2, myTriangle.vert3);
	
		//if zComp is zero
		if(zComp==0)
		{
			tempPoints.erase(tempPoints.begin()+counter+1);
		}

		//if z component of vertex is negative
		else if(zComp < 0)
		{
			lineSegIndex = lineSegIndex + counter;
			cout << "Vertex " << counter <<" is CCW" << endl;
			//check for intersection 
			for(int i=lineSegIndex; i < lineSegments.size(); i++)
                        {
                                intersect = lineSegIntersect<double>(myTriangle.vert3, myTriangle.vert1, i);
				cout << "Does Vertex " << counter << " intersect?" << intersect << endl;
                                if(intersect == true)
                                        break;
                        }
			
			
			if(intersect == true)
			{
				counter ++;
					
			}
			//if does not intersect
			else if(intersect==false)
			{
				tessTriangles.push_back(myTriangle); //add triangle to the triangle vector
				drawLine(myTriangle.vert1, myTriangle.vert3); //draw diagonal line 
				cout<< tempPoints.size() << " points left" <<endl;
				tempPoints.erase(tempPoints.begin() + counter + 1); //remove middle vertex from tempPoints list
				cout << "Reseting counter to 0" << endl;
				counter = 0; //reseting counter back to zero so it can restart at the beginning vertex
				lineSegIndex = 4; 
			}
			
		/*	cout << "Reseting counter to 0" << endl;
			counter = 0; //set counter back to zero so it can restart at the beginning vertex 
			lineSegIndex = 4; */
		}
		
		//zComp is positive
		else if (zComp > 0)
		{
			cout << "zComp is positive. Incrementing counter. " << endl;
			counter++;
		}

	}

	//if there are 3 points left in the polygon
	if(tempPoints.size() == 3)
	{
		cout << "Three points left. Making final triangle" << endl;
		myTriangle.vert1 = tempPoints[0];
		myTriangle.vert2 = tempPoints[1];
		myTriangle.vert3 = tempPoints[2];
		tessTriangles.push_back(myTriangle); //add final triangle to the triangle vector
	}
}

void mouse(int button, int state, int x, int y)
{

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		if(doneDrawing==false){
			processDraw(x, WINDOW_MAX_Y-y);
			//printf("Point Accepted: %d   %d\n", x, y);
		}
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//printf("%d   %d\n", x, y);
		closePolygon();
		doneDrawing=true;
	}
}


void keyboard(unsigned char key, int x, int y)
{
	switch(tolower(key)){

		case 'q': exit(0);
		case 't': tesselate();
			break;
		case 'i': initialOutline();
			break;
		case 'f': fillPolygon();
			break;	
	}
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

