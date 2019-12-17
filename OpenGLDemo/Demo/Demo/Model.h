#pragma once
#include "vec3.h"
#include <vector>
#include <fstream>
#include <string>

using namespace std;

class Triangle
{
public:
	vec3 points[3];
	vec3 normal;
	vec3 col;
	GLfloat mat_diffuse[4];
	Triangle(vec3 points[3])
	{
		memcpy(this->points, points, 9 * sizeof(float));
		normal = unit_vector(-cross(points[0] - points[1], points[0] - points[2]));
		col = random_unit_vector();
		mat_diffuse[0] = col.x();
		mat_diffuse[1] = col.y();
		mat_diffuse[2] = col.z();
		mat_diffuse[3] = 1;
	}
	void draw()
	{
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

		glBegin(GL_TRIANGLES);

		glNormal3f(normal.x(), normal.y(), normal.z());
		glVertex3f(points[0].x(), points[0].y(), points[0].z());
		glVertex3f(points[1].x(), points[1].y(), points[1].z());
		glVertex3f(points[2].x(), points[2].y(), points[2].z());

		glEnd();
	}  
};

class Model
{
public:
	vector<Triangle> triangle_list;
	vector<vec3> verts;
	void load_OFFformat_model(string filename)
	{
		ifstream modelfile; modelfile.open(filename);

		string filetype;
		modelfile >> filetype;
		if (filetype != "OFF")
		{
			cout << "Error file type: " << filetype << endl;
			cout << "Loading file " << filename << " error" << endl << endl;
			return;
		}
		int vertexCount = 0, triCount = 0, uselessInfo = 0;
		modelfile >> vertexCount >> triCount >> uselessInfo;
		for (size_t i = 0; i < vertexCount; i++)
		{
			vec3 point;
			// swap z and y
			modelfile >> point[0] >> point[2] >> point[1];
			verts.push_back(point);
		}
		for (size_t i = 0; i < triCount; i++)
		{
			int quadsCount = 0;
			modelfile >> quadsCount;
			if (quadsCount != 3)
			{
				cout << "Contains strange data: point count: " << quadsCount << "." << endl << endl;
				cout << "Loading file " << filename << " error" << endl << endl;
				return;
			}

			int index[3];
			modelfile >> index[0] >> index[1] >> index[2];
			for (size_t i = 0; i < 3; i++)
			{
				if (index[i] >= vertexCount)
				{
					cout << "Contains strange data: point index: " << index[i] << "." << endl << endl;
					cout << "Loading file " << filename << " error" << endl << endl;
					return;
				}
			}

			vec3 points[] = { verts[index[0]],verts[index[1]],verts[index[2]] };
			triangle_list.push_back(Triangle(points));
		}

		cout << "Loading file " << filename << " success" << endl << endl;
		modelfile.close();
	}
	void draw()
	{
		for (size_t i = 0; i < triangle_list.size(); i++)
		{
			triangle_list[i].draw();
		}
	}
};