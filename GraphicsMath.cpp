#include <iostream>
#include <vector>
#include "GraphicsMath.h"
using namespace std;

//dot product function
template <typename T>
T GraphicsMath::dotProduct(vector<T> const &a, vector<T> const &b)
{
	T dProd = 0;
	int minSize = 0;

	//check to make sure neither vector is empty
	if (a.empty() || b.empty() || a.size() != b.size())
		return;
	else
	{

		//add to the result the product of the ith element of each vector
		for (int i = 0; i < a.size(); i++)
		{
			dProd = dProd + (a[i] * b[i]);
		}
		return dProd;
	}
}

//are we assuming we are only using 3x1 vectors?
//function to find the cross Product of 2 vectors
template <typename T>
vector<T> GraphicsMath::crossProduct(vector<T> const &a, vector<T> const &b)
{
	vector<T> zComp;
	if (a.empty() || b.empty())
		return;
	else {
		int sizeV1 = a.size();
		int sizeV2 = b.size();
		zComp.push_back((a[1] * b[2]) - (a[2] * b[1]));
		zComp.push_back((a[2] * b[0]) - (a[0] * b[2]));
		zComp.push_back((a[0] * b[1]) - (a[1] * b[0]));

		return zComp;
	}
}

//function to find if line segment intersects
template <typename T>
bool GraphicsMath::lineSegIntersect(vector<T> const &p1, vector<T> const &p2, vector<T> const &p3, vector <T> const &p4)
{
	T scalarA = 0;
	T scalarB = 0;
	T denom = ((p4[0] - p3[0])*(p1[1] - p2[1])) - ((p1[0] - p2[0])*(p4[1] - p3[1]));
	scalarA = ((p3[1] - p4[1])*(p1[0] - p3[0])) + ((p4[0] - p3[0])*(p1[1] - p3[1]));
	scalarB = ((p1[1] - p2[1])*(p1[0] - p3[0])) + ((p2[0] - p1[0])*(p1[1] - p3[1]));
	scalarA = scalarA / denom;
	scalarB = scalarB / denom;

	if (denom == 0)
	{
		return true;
	}
	else if (scalarA > 0 && scalarA < 1 && scalarB>0 && scalarB < 1)
	{
		return true;
	}
	else
		return false;
}

//function to find the area of a triangle
template <typename T>
T GraphicsMath::areaTriangle(vector<T> const &a, vector<T> const &b, vector<T> const &c)
{
	T area = 0;
	area = ((a[0] * b[1]) + (b[0] * c[1]) + (c[0] * a[1]) - (a[0] * c[1]) - (b[0]a[1]) - (c[0]b[1]));
	area = area / 2;
	return area;
}






