#include "Delaunay.h"
#include <math.h>

#define MINSINGLE 1.0e-6
#define RAD2DEG(a) (a)*(57.295779513079)

Point::Point(double _x,double _y,double _z)
{
	x=_x;y=_y;z=_z;
}
double Point::GetX()
{
	return x;
}
double Point::GetY()
{
	return y;
}
double Point::GetZ()
{
	return z;
}
void Point::SetXYZ(double _x,double _y,double _z)
{
	x=_x;y=_y;z=_z;
}

double Point::GetLength(Point * other)
{
	double len = 0.0;
	len = sqrt((x - other->GetX()) *(x - other->GetX())
				+(y - other->GetY()) *(y - other->GetY())
				+(z - other->GetZ()) *(z - other->GetZ()));
	return len;
}

Line::Line(Point *stpt,Point *edpt)
{
	st=stpt;
	ed=edpt;
}
TriangleIndex::TriangleIndex(const unsigned p1,const unsigned p2,const unsigned p3)
{
	m_index[0]=p1;m_index[1]=p2;m_index[2]=p3;
}
void TriangleIndex::SetIndex(const unsigned p1,const unsigned p2,const unsigned p3)
{
	m_index[0]=p1;m_index[1]=p2;m_index[2]=p3;
}
unsigned TriangleIndex::GetP1()
{
	return m_index[0];
}
unsigned TriangleIndex::GetP2()
{
	return m_index[1];
}
unsigned TriangleIndex::GetP3()
{
	return m_index[2];
}

Triangle::Triangle(Point *pt1,Point *pt2,Point *pt3):encrypt(false)
{
	p1=pt1;p2=pt2;p3=pt3;
}
Triangle::Triangle():encrypt(false)
{
	oalen=new Line(p1,p2);
	dalen=new Line(p1,p3);
	dolen=new Line(p2,p3);
}
Triangle::~Triangle()
{
	m_index.clear();
	m_pts.clear();
	m_lines.clear();
	m_inpts.clear();
}
void Triangle::GetConstLine(vector<Line *>&m_conline)
{
	m_conline.push_back(oalen);
	m_conline.push_back(dalen);
	m_conline.push_back(dolen);
}
Point *Triangle::GetPoint(size_t i)
{
	return m_pts[i];
}
Line *Triangle::GetLine(size_t i)
{
	return m_lines[i];
}
void Triangle::Circumter(Triangle *tri)
{
	double x1,y1,z1,x2,y2,z2,x3,y3,z3;
	double x_center,y_center,z_center;
	x1=tri->p1->GetX();y1=tri->p1->GetY();z1=tri->p1->GetZ();
	x2=tri->p2->GetX();y2=tri->p2->GetY();z2=tri->p2->GetZ();
	x3=tri->p3->GetX();y3=tri->p3->GetY();z3=tri->p3->GetZ();
	x_center=((y2-y1)*(y3*y3-y1*y1+x3*x3-x1*x1)-(y3-y1)*(y2*y2-y1*y1+x2*x2-x1*x1))/(2*(x3-x1)*(y2-y1)-2*(x2-x1)*(y3-y1));
	y_center=((x2-x1)*(x3*x3-x1*x1+y3*y3-y1*y1)-(x3-x1)*(x2*x2-x1*x1+y2*y2-y1*y1))/(2*(y3-y1)*(x2-x1)-2*(y2-y1)*(x3-x1));
	z_center=0;
	CirCen=new Point(x_center,y_center,z_center);
	radius=sqrt((x1-x_center)*(x1-x_center)+(y1-y_center)*(y1-y_center));
}
bool Triangle::InCell(Point *pt,int i)
{
	double dis;
	Triangle *tri=new Triangle(GetPoint(m_index[i]->GetP1()),GetPoint(m_index[i]->GetP2()),GetPoint(m_index[i]->GetP3()));
	Circumter(tri);
	dis=sqrt((CirCen->GetX()-pt->GetX())*(CirCen->GetX()-pt->GetX())+(CirCen->GetY()-pt->GetY())*(CirCen->GetY()-pt->GetY())+(CirCen->GetZ()-pt->GetZ())*(CirCen->GetZ()-pt->GetZ()));
	if (dis<radius)
	{
		return true;
	}
	return false;
}
void Triangle::test()
{
	m_pts.push_back(new Point(-20,-20,0));
	m_pts.push_back(new Point(20,-20,0));
	m_pts.push_back(new Point(20,20,0));
	m_pts.push_back(new Point(-20,20,0));

	m_index.push_back(new TriangleIndex(0,2,1));
	m_index.push_back(new TriangleIndex(0,2,3));
	
	Input();
	Delaunay2D();
	Output();
}
void Triangle::Output()
{
	vector<int>delid;
	for (size_t i=0;i<m_index.size();++i)
	{
		if(m_index[i]->GetP1()==0||m_index[i]->GetP1()==1||m_index[i]->GetP1()==2||m_index[i]->GetP1()==3
		||m_index[i]->GetP2()==0||m_index[i]->GetP2()==1||m_index[i]->GetP2()==2||m_index[i]->GetP2()==3
		||m_index[i]->GetP3()==0||m_index[i]->GetP3()==1||m_index[i]->GetP3()==2||m_index[i]->GetP3()==3)
		{
			delid.push_back(i);
		}
	}
	for (int i=delid.size()-1;i>=0;--i)
	{
		m_index.erase(m_index.begin()+delid[i]);
	}
}
void Triangle::Input()
{
	ifstream in_put("data0.txt");
	if (!in_put.is_open())
	{
		cerr<<"Error data!try again!"<<endl;
		exit(1);
	}
	string line;
	while(!in_put.eof())
	{
		double x,y,z;
		in_put>>x;
		in_put>>y;
		in_put>>z;
		m_inpts.push_back(new Point(x,y,z));
	}
}
void Triangle::Delaunay2D()
{
	vector<TriangleIndex*>BadTriIndex;
	vector<int>delindex;
	for (size_t i=0;i<m_inpts.size();++i)
	{
		m_pts.push_back(m_inpts[i]);
		for (size_t j=0;j<m_index.size();++j)
		{
			if (InCell(m_inpts[i],j))
			{
				BadTriIndex.push_back(m_index[j]);
				delindex.push_back(j);
			}
		}
		for (int k=delindex.size()-1;k>=0;--k)
		{
			m_index.erase(m_index.begin()+delindex[k]);
		}
		DealBadTri(BadTriIndex);
		delindex.clear();
		BadTriIndex.clear();
	}
}
void Triangle::DealBadTri(vector<TriangleIndex *>BadTriIndex)
{
	for (size_t i=0;i<BadTriIndex.size();++i)
	{
		m_lines.push_back(new Line(GetPoint(BadTriIndex[i]->GetP1()),GetPoint(BadTriIndex[i]->GetP2())));
		m_lines.push_back(new Line(GetPoint(BadTriIndex[i]->GetP1()),GetPoint(BadTriIndex[i]->GetP3())));
		m_lines.push_back(new Line(GetPoint(BadTriIndex[i]->GetP2()),GetPoint(BadTriIndex[i]->GetP3())));
	}
	for (size_t i=0;i<m_lines.size();++i)
	{
		int m=0;
		int n=0;
		int count=0;
		for (size_t j=0;j<m_lines.size();++j)
		{
			if (i==j)
				continue;
			if (((m_lines[i]->GetLineSt()!=m_lines[j]->GetLineSt())&&(m_lines[i]->GetLineSt()!=m_lines[j]->GetLineEd()))
				||((m_lines[i]->GetLineEd()!=m_lines[j]->GetLineSt())&&(m_lines[i]->GetLineEd()!=m_lines[j]->GetLineEd())))
			{
				count+=1;
			}
		}
		if (count==m_lines.size()-1)
		{
			m=i%3;
			n=i/3;
			InputTriIndex(m,n,BadTriIndex);
		}
	}
	m_lines.clear();
}

void Triangle::InputTriIndex(int m,int n,vector<TriangleIndex *>BadTriIndex)
{
	int m_ptsz=m_pts.size()-1;
	switch(m)
	{
	case 0:
		{
			m_index.push_back(new TriangleIndex(BadTriIndex[n]->GetP1(),BadTriIndex[n]->GetP2(),m_ptsz));
			break;
		}
	case 1:
		{
			m_index.push_back(new TriangleIndex(BadTriIndex[n]->GetP1(),BadTriIndex[n]->GetP3(),m_ptsz));
			break;
		}
	case 2:
		{
			m_index.push_back(new TriangleIndex(BadTriIndex[n]->GetP2(),BadTriIndex[n]->GetP3(),m_ptsz));
			break;
		}
	default:
		break;
	}
}
bool Triangle::WriteFile(const char *_name)
{
	FILE *fp=fopen(_name,"w");
	if (fp==NULL)
	{
		return false;
	}
	fprintf(fp,"solid %s\n","STL FILE PRODUCED BY SPIDER VERSION 0.8");
	for ( size_t i=0;i<m_index.size();++i)
	{
		fprintf(fp," facet normal % .8E % .8E % .8E\n",1.0,1.0,1.0);
		fprintf(fp,"    outer loop\n");
		fprintf(fp,"     vertex % .8E % .8E % .8E\n",
			GetPoint(m_index[i]->GetP1())->GetX(),
			GetPoint(m_index[i]->GetP1())->GetY(),
			GetPoint(m_index[i]->GetP1())->GetZ());
		fprintf(fp,"	 vertex % .8E % .8E % .8E\n",
			GetPoint(m_index[i]->GetP2())->GetX(),
			GetPoint(m_index[i]->GetP2())->GetY(),
			GetPoint(m_index[i]->GetP2())->GetZ());
		fprintf(fp,"	 vertex % .8E % .8E % .8E\n",
			GetPoint(m_index[i]->GetP3())->GetX(),
			GetPoint(m_index[i]->GetP3())->GetY(),
			GetPoint(m_index[i]->GetP3())->GetZ());
		fprintf(fp,"    endloop\n");
		fprintf(fp," endfacet\n");
	}
	fprintf(fp,"endsolid\n");
	fclose(fp);
	return true;
}
void Triangle::DisTri()
{
	cout<<"是否需要加密现在的非结构网格？（Y or N）"<<endl;
	char ch;
	cin>>ch;
	switch(ch)
	{
	case 'Y':
		{
			//AddNewPoint();
			AddNewPointByCenter();
			//AddNewPointByCenter();
			break;
		}
	case 'y':
		{
			//AddNewPoint();
			AddNewPointByCenter();
			//AddNewPointByCenter();
			break;
		}
	case 'N':
		{
			break;
		}
	default:
		break;
	}
}
void Triangle::AddNewPoint()
{
	double x_min=10000;
	double x_max=-1000;
	double y_min=10000;
	double y_max=-1000;
	for (size_t i=0;i<m_inpts.size();++i)
	{
		Point *m_pts=new Point(m_inpts[i]->GetX(),m_inpts[i]->GetY(),m_inpts[i]->GetZ());
		if (x_min>m_pts->GetX())
			x_min=m_pts->GetX();
		if (x_max<m_pts->GetX())
			x_max=m_pts->GetX();
		if (y_min>m_pts->GetY())
			y_min=m_pts->GetY();
		if (y_max<m_pts->GetY())
			y_max=m_pts->GetY();
	}
	m_inpts.clear();
	int x_num=4;
	int y_num=4;
	double x_len,y_len;
	vector<double >x_pts;
	vector<double >y_pts;
	x_len=(x_max-x_min)/(x_num+1);
	y_len=(y_max-y_min)/(y_num+1);
	for (size_t i=0;i<x_num;i++)
	{
		x_min+=x_len;
		x_pts.push_back(x_min);
	}
	for (size_t i=0;i<y_num;i++)
	{
		y_min+=y_len;
		y_pts.push_back(y_min);
	}
	for (size_t i=0;i<x_pts.size();++i)
	{
		for (size_t j=0;j<y_pts.size();++j)
		{
			m_inpts.push_back(new Point(x_pts[i],y_pts[j],0));
		}
	}
	SetEncrypt(true);
	Delaunay2D();
}

void Triangle::AddNewPointByCenter()
{
	//插三次值
	for (int c = 0 ; c < 2; c++)
	{
		//根据目前的三角形，获取三角形的中心点作为新加入点
		m_inpts.clear();
		for (int i = 0 ; i < m_index.size(); i++)
		{
			double maxAngle = GetMinAngle(i);
			if (maxAngle < 45)
			{
				double x = (GetPoint(m_index[i]->GetP1())->GetX() + GetPoint(m_index[i]->GetP2())->GetX() + GetPoint(m_index[i]->GetP3())->GetX())/3;
				double y = (GetPoint(m_index[i]->GetP1())->GetY() + GetPoint(m_index[i]->GetP2())->GetY() + GetPoint(m_index[i]->GetP3())->GetY())/3;
				double z = (GetPoint(m_index[i]->GetP1())->GetZ() + GetPoint(m_index[i]->GetP2())->GetZ() + GetPoint(m_index[i]->GetP3())->GetZ())/3;
				m_inpts.push_back(new Point(x,y,z));
			}
		}
		SetEncrypt(true);
		Delaunay2D();
	}
	
}

double Triangle::GetMinAngle(int index)
{
	TriangleIndex * triIndex = m_index[index];
	Point * p0 = GetPoint(triIndex->GetP1());
	Point * p1 = GetPoint(triIndex->GetP2());
	Point * p2 = GetPoint(triIndex->GetP3());

	double len0 = p0->GetLength(p1);
	double len1 = p1->GetLength(p2);
	double len2 = p2->GetLength(p0);

	double minLen = 0.0;
	minLen = (len0 > len1) ? len1 : len0;
	minLen = (minLen > len2) ? len2 : minLen;

	double minAngle =0.0;
	//求的最小边长，可以计算最小角度°
	if (len0 - minLen < MINSINGLE)
	{
		minAngle = (len1 *len1 + len2 *len2 - (len0 * len0))/(2 * len1 *len2);
		minAngle = acos(minAngle);
	}
	else if (len1 - minLen < MINSINGLE)
	{
		minAngle = (len0 *len0 + len2 *len2 - (len1 * len1))/(2 * len0 *len2);
		minAngle = acos(minAngle);
	}
	else if (len2 - minLen < MINSINGLE)
	{
		minAngle = (len1 *len1 + len0 *len0 - (len2 * len2))/(2 * len1 *len0);
		minAngle = acos(minAngle);
	}
	return RAD2DEG(minAngle);
}
