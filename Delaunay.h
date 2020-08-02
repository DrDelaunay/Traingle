#ifndef  _DELAUNAY_H_
#define  _DELAUNAY_H_
#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
#include <ostream>
#include <string>
#include <algorithm>
#include <list>
using namespace std;

class Point
{
public:
	Point(){}
	Point(double _x,double _y,double _z);
	~Point(){}
public:
	double GetX();
	double GetY();
	double GetZ();
	void SetXYZ(double _x,double _y,double _z);
	double GetLength(Point * other);
private:
	double x;
	double y;
	double z;
};
class Line
{
public:
	Line(){}
	Line(Point *st,Point *ed);
	~Line(){};
public:
	virtual Point *GetLineSt(){return st;}
	virtual Point *GetLineEd(){return ed;}
private:
	Point *st;
	Point *ed;
};
class TriangleIndex
{
public:
	TriangleIndex(const unsigned p1,const unsigned p2,const unsigned p3);
	~TriangleIndex(){}
public:
	void SetIndex(const unsigned p1,const unsigned p2,const unsigned p3);
	unsigned GetP1();
	unsigned GetP2();
	unsigned GetP3();
	unsigned m_index[3];
};
class Triangle
{
public:
	Triangle();
	Triangle(Point *pt1,Point *pt2,Point *pt3);
	~Triangle();
public:
	void GetConstLine(vector<Line *>&m_conline);
	bool InCell(Point *pt,int i);
	void Circumter(Triangle *tri);

	int GetPointSz(){return m_pts.size();}
	int GetLineSz(){return m_lines.size();}
	virtual Point *GetPoint(size_t i);
	virtual Line *GetLine(size_t i);
	void test();
	void Output();
	void Input();
	void Delaunay2D();
	void DealBadTri(vector<TriangleIndex *>BadTriIndex);
	
	//保存数据，可以导入到Spider中观察数据
	void InputTriIndex(int m,int n,vector<TriangleIndex *>BadTriIndex);
	bool WriteFile(const char *_name);
	void DisTri();
	void AddNewPoint();
	bool IsEncrypt(){return encrypt;}
	void SetEncrypt(bool crypt){encrypt=crypt;}

	//新的增加点方法
	void AddNewPointByCenter();
	//计算三角形最小角大小,输入为三角形的索引
	double GetMinAngle(int index);
	bool encrypt;

private:
	Line *oalen,*dalen,*dolen;
	Point *p1,*p2,*p3;
	Point *CirCen;
	double radius;
	vector<Point *> m_pts;
	vector<Line *>m_lines;
	vector<TriangleIndex *>m_index;
	vector<Point *>m_inpts;
};
#endif
