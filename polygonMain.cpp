//YASMINE BROOKS
//DR.POUNDS CSC 315
//OCTOBER 2, 2018

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
//for some reason, when I try to dimension the screen as 800x800, drawing on the screen becomes unaligned
//not enough time to explore this problem
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

void display(void)
{

	/* define a point data type */

	typedef GLfloat point[2];
	typedef GLfloat point[2];

	point p; /* A point in 2-D space */
	if (!COLORS_DEFINED) {
		red = 255;
		green = 0;
		blue = 0;
	}

	glColor3ub(red, green, blue);	/* define point */
	p[0] = 100;
	p[1] = 100;			
															
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
	//computer the denominator
        T denom = ((p2.x - p1.x)*((p4.y - p3.y))) - ((p2.y - p1.y)*(p4.x - p3.x));
        if(denom==0) //if the deonminator is 0, then there is no intersection
        {
                intersect=false;
        }
        else{ //if the denominator is not 0, compute the numerators for both scalars
                T scalarA = ((p1.y - p3.y)*(p4.x - p3.x)) - ((p1.x - p3.x)*(p4.y - p3.y));
                T scalarB = ((p1.y - p3.y)*(p2.x - p1.x)) - ((p1.x - p3.x)*(p2.y - p1.y));
          
		//divide the scalars by the denominator
	        scalarA = scalarA / denom;
		scalarB = scalarB / denom;

		//if they are both between 0 and 1, then there is an intersection, else there is not an intersection
                 if (scalarA > 0 && scalarA < 1 && scalarB>0 && scalarB < 1)
                {
                        intersect=true;
                }
                else
                        intersect=false;
        }
        return intersect;
}

//function to fill the polygon after the original outline is drawn and before tesselation
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
	cout << "Closing Polygon" << endl;
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

//function to convert back to original outline of the polygon drawn
void initialOutline()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f, 0.0f, 1.0f);
                for(int i=0; i < polygonPoints.size()-1; i++)
                {
                        drawLine(polygonPoints[i], polygonPoints[i+1]);
                }
	closePolygon();
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
		printf("Point Accepted: %d   %d\n", x, y);
		polygonPoints.push_back(newPoint);
	}
	
	//if there are more than one point
	else if(polygonPoints.size() >= 1)
	{
		//set up a new line and a previous point
		prevPosition = polygonPoints.size() -1;
		prevPoint.x = polygonPoints[prevPosition].x;
		prevPoint.y = polygonPoints[prevPosition].y;
		newLine.p1 = prevPoint;
		newLine.p2 = newPoint;
		
		//if there a no line segments drawn yet, draw the point 
		//and then the line and add the points to the vectors
		//and print the point to the console 
		if(lineSegments.size() < 1)
		{
			drawPoint(newPoint);
			printf("Point Accepted: %d   %d\n", x, y);
			drawLine(newPoint, prevPoint);
			polygonPoints.push_back(newPoint);
			lineSegments.push_back(newLine);
		}
		
		//if there is a line segment drawn
		else if(lineSegments.size() >= 1)
		{
			//check for intersection using a for loop
			//if ends up true, break the loop
			//if it's false at the end of the loop, draw and add point and line
			for(int i=0; i < lineSegments.size(); i++)
			{
				intersect = lineSegIntersect<double>(newPoint, lineSegments[lineSegments.size()-1].p2, i);
				if(intersect == true){
					printf("Point Not Accepted-Intersection: %d   %d\n", x, y);
					break;
				}
			}
			
			if(intersect==false)
			{
				drawPoint(newPoint);
				drawLine(newPoint, prevPoint);
				printf("Point Accepted: %d   %d\n", x, y);
				polygonPoints.push_back(newPoint);
				lineSegments.push_back(newLine);
			}
		}
	}
}

//function to determine if a vertex is CCW
double isVertexCCW(Point p1, Point p2, Point p3)
{

		Point crossP1, crossP2;
		crossP1.x = p1.x - p2.x;
		crossP1. y = p1.y - p2.y;
		crossP2.x = p3.x - p2.x;
		crossP2.y = p3.y - p2.y;

		return (crossP1.x * crossP2.y) - (crossP2.x * crossP1.y);
		
}

//function to determine if a polygon is clockwise or counterclockwise
bool isPolygonCCW()
{
	bool isCCW;
	double result=0;
	Point crossP1, crossP2;
	
	for(int i= 0; i<polygonPoints.size()-1; i++)
	{
		result = result + (polygonPoints[i+1].x-polygonPoints[i].x)*(polygonPoints[i+1].y + polygonPoints[i].y);	
	}
	
	result = result + (polygonPoints[0].x - polygonPoints[polygonPoints.size()-1].x) * (polygonPoints[0].y + polygonPoints[polygonPoints.size()-1].y);

	//if sum is negative, it is CCW
	if(result < 0){
		isCCW = true;
	}
	
	//if sum is positive then it is CW
	else{
		isCCW = false;
	}
	
	return isCCW;
}

//function to flip a polygon from CW to CCW
vector<Point> flipPolygon()
{
	vector<Point> tempFlip = polygonPoints;
	vector<Point> flip;
	flip.push_back(tempFlip.front()); //add first vertex point to the new CCW vector of points; 
						//this way the beginning point stays the same
	
	//until tempFlip has only the first element remaining
	//pop the last element from tempFlip and push it to the back of flip
	while(tempFlip.size() != 1)
	{
		flip.push_back(tempFlip.back());
		tempFlip.pop_back();
	}

	return flip;
}

//computes the area of the triangle formed during tesselation; equation from Class Notes
//returns a negative number when a triangle is formed during tesselation outside of the original polygon outline
double areaTriangle(Triangle t)
{
	return ((t.vert1.x*t.vert2.y) + (t.vert2.x * t.vert3.y) + (t.vert3.x * t.vert1.y) - (t.vert1.x * t.vert3.y) - (t.vert2.x * t.vert1.y) - (t.vert3.x *t.vert2.y)) * 0.5; 		
}

//function to process tesselating the polygon; 
//Not sure if it works fully; sometimes there are segmentation faults and it draws outside polygon
// it is hard to tell what is causing these problems because they don't happen everytime the tesselation is run
// thus they haven't been fixed
// algorithm modeled after class notes
void tesselate()
{
	vector<Point> tempPoints; //temporary vector that is a copy of the vector that holds the polygon points
						//will be used to manipulate the point data during tesselation 
	int counter = 0;
	int lineSegIndex = 4; //counter to determine which line to start checking for intersections at
	double zComp = 0;
	int numTriangles = 0;
	bool intersect = false;
	Triangle myTriangle;
	
	cout << "/*****************TESSELATION STARTING*****************/" << endl;
	//check if polygon is CCW; if it is not then flip the polygon points
	if(!isPolygonCCW())
	{
		tempPoints = flipPolygon();
	}
		
	else{
		//it is CCW, then copy the points of the polygon to the temporary vector
		tempPoints = polygonPoints;
	}

	//while there are more than 3 points left in the vector
	while(tempPoints.size() > 3)
	{
	
		//assign values and check if vertex is CCW
		myTriangle.vert1 = tempPoints[counter];
		myTriangle.vert2 = tempPoints[counter+1];
		myTriangle.vert3 = tempPoints[counter+2];
	
		zComp = isVertexCCW(myTriangle.vert1, myTriangle.vert2, myTriangle.vert3);
	
		//if zComp is zero, the lines are colinear so delete vertex from list
		if(zComp==0)
		{
			tempPoints.erase(tempPoints.begin()+counter+1);
		}

		//if z component of vertex is negative, it is CCW
		else if(zComp < 0)
		{
			lineSegIndex = lineSegIndex + counter;
			//check for intersection 
			for(int i=lineSegIndex; i < lineSegments.size(); i++)
                        {
                                intersect = lineSegIntersect<double>(myTriangle.vert3, myTriangle.vert1, i);
				//if there is an intersection end the loop
                                if(intersect == true)
                                        break;
                        }
			
			//if there is an intersection then increase the starting vertex
			if(intersect == true)
			{
				counter ++;		
			}

			//if there is not an intersection
			else if(intersect==false)
			{
				tessTriangles.push_back(myTriangle); //add triangle to the triangle vector
				drawLine(myTriangle.vert1, myTriangle.vert3); //draw diagonal line between points
				numTriangles ++; 
				
				//find area of the triangle just formed and output it to console
				cout <<  "Area of Triangle " << numTriangles << ": " << areaTriangle(myTriangle) << endl;
				tempPoints.erase(tempPoints.begin() + counter + 1); //remove middle vertex from tempPoints list
				counter = 0; //reseting counter back to zero so it can restart at the beginning vertex
				lineSegIndex = 4; //reseting starting line segment
			}	
		}
		
		// if zComp is positive increase the starting vertex
		else if (zComp > 0)
		{
			counter++;
		}
	}

	//if there are 3 points left in the polygon
	if(tempPoints.size() == 3)
	{
		numTriangles++;
		myTriangle.vert1 = tempPoints[0];
		myTriangle.vert2 = tempPoints[1];
		myTriangle.vert3 = tempPoints[2];
		cout <<  "Area of Triangle " << numTriangles << ": " << areaTriangle(myTriangle) << endl; //find area
		tessTriangles.push_back(myTriangle); //add final triangle to the triangle vector
	}
}

//function to fill the polygon according to the triangles formed during tesselation
void fillTessPolygon()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f, 0.0f, 1.0f);

	for(int i = 0; i < tessTriangles.size(); i++)
	{
		glBegin(GL_POLYGON);
			glVertex2d(tessTriangles[i].vert1.x, tessTriangles[i].vert1.y);	
			glVertex2d(tessTriangles[i].vert2.x, tessTriangles[i].vert2.y);
			glVertex2d(tessTriangles[i].vert3.x, tessTriangles[i].vert3.y);
		glEnd();
		glFlush();	
	}
}

void mouse(int button, int state, int x, int y)
{
	//if right click on mouse, process the point trying to be made
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		//if this is false, no more lines can be drawn
		if(doneDrawing==false){
			processDraw(x, WINDOW_MAX_Y-y);
		}
	}

	//if left click, close the polygon and make it so there can be no more lines drawn
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		closePolygon();
		doneDrawing=true;
	}
}

//function to control what happens when user presses button on keyboard
void keyboard(unsigned char key, int x, int y)
{
	//switch statement detailing the different options
	switch(tolower(key)){

		case 'q': exit(0);
		case 't': tesselate(); //tesselate the polygon
			break;
		case 'i': initialOutline(); //revert back to the original function
			break;
		case 'f': fillPolygon(); //fill the polygon after initial drawing 
			break;
		case 'p': fillTessPolygon(); //fill the polygon after tesselation occurs
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
	glutDisplayFunc(display); /* Display callback invoked when window opened */
	glutMainLoop(); /* enter event loop */
}

