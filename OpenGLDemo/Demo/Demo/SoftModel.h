#pragma once
#include "vec3.h"
#include <vector>
#include <GL/glut.h>              // glut.h includes gl.h and glu.h already
#include <string>
#include <fstream>

using namespace std;

class vert
{
public:
	vec3 pos, normal, col;
};

class SoftTriangle
{
public:
	vert verts[3];
	float mat_diffuse[4];
	SoftTriangle(vert verts[3])
	{
		memcpy(this->verts, verts, 3 * sizeof(vert));
	}
	void draw()
	{
		//give_mat_col({1,1,1});

		glBegin(GL_TRIANGLES);

		glNormal3f(verts[0].normal.x(), verts[0].normal.y(), verts[0].normal.z());
		//give_mat_col(verts[0].col);
		glVertex3f(verts[0].pos.x(), verts[0].pos.y(), verts[0].pos.z());

		glNormal3f(verts[1].normal.x(), verts[1].normal.y(), verts[1].normal.z());
		//give_mat_col(verts[1].col);
		glVertex3f(verts[1].pos.x(), verts[1].pos.y(), verts[1].pos.z());

		glNormal3f(verts[2].normal.x(), verts[2].normal.y(), verts[2].normal.z());
		//give_mat_col(verts[2].col);
		glVertex3f(verts[2].pos.x(), verts[2].pos.y(), verts[2].pos.z());


		glEnd();
	}

	void give_mat_col(vec3 col)
	{
		mat_diffuse[0] = col.x();
		mat_diffuse[1] = col.y();
		mat_diffuse[2] = col.z();
		mat_diffuse[3] = 1;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	}
};

class SoftModel
{
public:
	vector<vert> verts;
	vector<int> verts_normal_count;
	vector<int*> index_list;
	vector<SoftTriangle> triangle_list;
	float default_mat_diffuse[4];
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
			vert v = { point, vec3(0,0,0), vec3(0,0,0) };
			v.col = random_unit_vector();
			verts.push_back(v);
			verts_normal_count.push_back(0);
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

			int* index = new int[3];
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
			index_list.push_back(index);


			// deal with normals
			vec3 normal = unit_vector(-cross(verts[index[0]].pos - verts[index[1]].pos, verts[index[0]].pos - verts[index[2]].pos));
			for (size_t i = 0; i < 3; i++)
			{
				verts[index[i]].normal += normal;
				verts_normal_count[index[i]] ++;
			}
		}

		for (size_t i = 0; i < vertexCount; i++)
		{
			verts[i].normal /= verts_normal_count[i];
		}

		for (size_t i = 0; i < triCount; i++) 
		{
			vert* trigroup = new vert[3]{ verts[index_list[i][0]], verts[index_list[i][1]], verts[index_list[i][2]] };
			triangle_list.push_back(trigroup);
		}

		cout << "Loading file " << filename << " success" << endl << endl;
		modelfile.close();
	}
	void draw()
	{
		give_default_mat_col({ 1,1,1 });
		for (size_t i = 0; i < triangle_list.size(); i++)
		{
			triangle_list[i].draw();
		}
	}

	void give_default_mat_col(vec3 col)
	{
		default_mat_diffuse[0] = col.x();
		default_mat_diffuse[1] = col.y();
		default_mat_diffuse[2] = col.z();
		default_mat_diffuse[3] = 1;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, default_mat_diffuse);
	}
};

