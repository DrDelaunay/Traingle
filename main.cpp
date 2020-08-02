#include "Delaunay.h"
int main()
{
	Triangle Delaunay;
	Delaunay.test();
	Delaunay.WriteFile("Init.stl");
	Delaunay.DisTri();
	if (Delaunay.encrypt==true)
	{
		Delaunay.WriteFile("InitNew.stl");
	}
	return 0;
}