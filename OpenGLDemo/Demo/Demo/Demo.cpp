// Demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <GL/glut.h>              // glut.h includes gl.h and glu.h already
#include <windows.h>
#include <queue>
#include <vector>
#include <algorithm>
#include <math.h>
#include "Camera.h"
#include "Model.h"
#include "SoftModel.h"

using namespace std;

const GLfloat WIDTH = 800, HEIGHT = 800;

Camera cam = {};

class Quads {
public:
	vec3 p1, p2, p3, p4;
	vec3 normal, col;
	bool operator<(const Quads& q) const
	{
		vec3 mid = (p1 + p2 + p3 + p4) / 4;
		vec3 q_mid = (q.p1 + q.p2 + q.p3 + q.p4) / 4;
		return (mid - cam.pos).squared_length() < (q_mid - cam.pos).squared_length();
	}
};

Model model;
SoftModel smodel;

priority_queue<Quads> quadsQueue;

void add_quad(vec3 p1, vec3 p2, vec3 p3, vec3 p4, vec3 normal, vec3 col);
void draw_point(vec2 st, vec3 color);
void draw_origin();
void draw_cube(vec3 mid, float width, vec3 col);
void draw_quad(vec3 p1, vec3 p2, vec3 p3, vec3 p4, vec3 normal, vec3 col);
void draw_quad(Quads q);
void draw_tri(vec3 p1, vec3 p2, vec3 p3);
void draw_polygon();
void display();
void myinit();
void mouse_handler(int button, int state, int x, int y);
void mouse_action_handler(int x, int y);
void keyboard_action_handler(unsigned char key, int x, int y);
void refresh_timer(int t);

int _tmain(int argc, _TCHAR* argv[])
{
			// Initialize GLUT function callings
	glutInit(&argc, (char **)argv);          

	// Set window size (width, height) in number of pixels  	
	glutInitWindowSize(WIDTH, HEIGHT);   

	// Specify window position, from the left and top of the screen, in numbers of pixels 
	glutInitWindowPosition(600, 100);        

	// Specify a window creation event 
	glutCreateWindow("A Yellow Quad."); 
	glutMouseFunc(mouse_handler);
	glutMotionFunc(mouse_action_handler);

	glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
	glutKeyboardFunc(keyboard_action_handler);

	//Specify the drawing function, called when the window is created or re-drew 
	myinit();		      //Invoke this function for initializaton	

	glutTimerFunc(16, refresh_timer, 1);
	glutDisplayFunc(display);       

	glutMainLoop();       // Enter the event processing loop

	return 0;             // ANSI C requires main() to return an int
}

void draw_origin()
{
	//glColor3f(1.0, 0.0, 0.0);

	float lineLen = 200.f, lineS = 10.f;

	glBegin(GL_LINES);

	glColor3f(1, 0.1, 0.1);

	glVertex3f(lineLen, 0.0, 0.0);
	glVertex3f(-lineLen, 0.0, 0.0);

	glVertex3f(lineLen, 0.0, 0.0);
	glVertex3f(lineLen - lineS, lineS, 0.0);

	glVertex3f(lineLen, 0.0, 0.0);
	glVertex3f(lineLen - lineS, -lineS, 0.0);



	glColor3f(0.1, 1.0, 0.1);
	glVertex3f(0.0, -lineLen, 0.0);
	glVertex3f(0.0, lineLen, 0.0);

	glVertex3f(0.0, lineLen, 0.0);
	glVertex3f(lineS, lineLen - lineS, 0.0);

	glVertex3f(0.0, lineLen, 0.0);
	glVertex3f(-lineS, lineLen - lineS, 0.0);



	glColor3f(0.1, 0.1, 1.f);
	glVertex3f(0.0, 0.0, lineLen);
	glVertex3f(0.0, 0.0, -lineLen);

	glVertex3f(0.0, 0.0, lineLen);
	glVertex3f(lineS, 0.0, lineLen - lineS);

	glVertex3f(0.0, 0.0, lineLen);
	glVertex3f(-lineS, 0.0, lineLen - lineS);

	glEnd();
}

void draw_polygon()
{

	glBegin(GL_QUADS);

	glNormal3f(0.0F, 0.0F, 1.0F);
	glVertex3f(3.0f, 1.0f, 0.0f);
	glVertex3f(7.0f, 1.0f, 0.0f);
	glVertex3f(7.0f, 3.0f, 0.0f);
	glVertex3f(3.0f, 3.0f, 0.0f);

	glEnd();
}

int rotationACC = 0;

void refresh_timer(int t)
{
	glutPostRedisplay(); 
	glutTimerFunc(16, refresh_timer, 1);
}

void display()
{
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glClearColor(.1, .1, .1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color buffer

	rotationACC++;
	rotationACC = rotationACC % 360;
	
	// light
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	glShadeModel(GL_SMOOTH);


	GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_diffuse[] = { .1, .1, .5, 1.0 };
	GLfloat light_specular[] = { .2, .2, .2, 1.0 };
	GLfloat light_position[] = { -200, -200, -200, 0.0 };
	GLfloat light_position2[] = { 200, -200, 200, 0.0};

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
	glEnable(GL_LIGHT0);       //开启GL_LIGHT0光源
	glEnable(GL_LIGHT1);       //开启GL_LIGHT0光源

	// draw light pos
	draw_cube({ -light_position[0],-light_position[1],-light_position[2] }, 20.f, { 0.4f, 0.7f, 0.0f });
	draw_cube({ -light_position2[0],-light_position2[1],-light_position2[2] }, 20.f, { 0.4f, 0.7f, 0.0f });
	//cam.pos += vec3(0.1f, 0.1f, 0.f);
	//cam.pos.make_unit_vector();
	//cout << cam.pos << endl;

	//cout << cam.front_dir << " " << cam.pos << endl;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	vec3 aimPos = cam.pos + cam.front_dir;
	gluLookAt(cam.pos.x(), cam.pos.y(), cam.pos.z(), aimPos.x(), aimPos.y(), aimPos.z(), cam.up_dir.x(), cam.up_dir.y(), cam.up_dir.z());
	//glTranslatef(cam.pos.x(), cam.pos.y(), cam.pos.z());
	draw_origin();


	// draw cubes
	//draw_cube({ 0,0,0 }, 10.f, { 0.4f, 0.7f, 0.0f });

	glPushMatrix();
	glTranslated(-70, 80, 0);
	//glutWireCube(30);
	glPopMatrix();

	//draw_tri({ 0,0,0 }, { 14,12,16 }, { 17, -14, 1 });

	glEnable(GL_LIGHTING);     //开启光照系统
	glPushMatrix();
	//glRotatef(3.f * rotationACC, 0, 1, 0);
	model.draw();
	glPopMatrix();
	glDisable(GL_LIGHTING);     //开启光照系统

	// draw models

	//glColor3f(0.0, 0.0, 1.0);
	//draw_polygon();
	//glColor3f(1.0, 1.0, 0.0);
	//glPushMatrix();

	////
	//glTranslatef(3.0, 1.0, 0.0);
	//glScalef(1.0, 1.5, 1.0);
	//gluLookAt(-1, 0, 0, 0, 0, 0, 0, 1, 0);
	//glTranslatef(-3.0, -1.0, 0.0);
	//draw_polygon();

	//while (!quadsQueue.empty())
	//{
	//	Quads q = quadsQueue.top();
	//	quadsQueue.pop();
	//	draw_quad(q);
	//}



	glFlush();                    //Force the processor to draw immediately
}

void myinit()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);  //Set the clear color to black

	model.load_OFFformat_model("Bunny_534v.off");
	smodel.load_OFFformat_model("Bunny_534v.off");
	//smodel.load_OFFformat_model("Bunny.off");
	//model.load_OFFformat_model("Bunny.off");

	// Specify the domain of the viewing window
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, WIDTH / HEIGHT, 0.01, 700.0);
	cam.pos = vec3(1, 1, 1) * 100;

	// The para are: (left, right, bottom, top)
	//gluOrtho2D(-8.0, 8.0, -8.0, 8.0);
}

int cnt = 0;

void draw_point(vec2 endPt1, vec3 color)
{
	glBegin(GL_POINTS);
	glColor3f(color.x(), color.y(), color.z());
	glVertex2i(endPt1.x, endPt1.y);
	glEnd();
}

void draw_cube(vec3 mid, float width, vec3 col = { 0.0f, 1.0f, 0.0f })
{
	vec3 p1 = { mid.x() + width / 2, mid.y() + width / 2, mid.z() + width / 2 };
	vec3 p2 = { mid.x() + width / 2, mid.y() + width / 2, mid.z() - width / 2 };
	vec3 p3 = { mid.x() - width / 2, mid.y() + width / 2, mid.z() - width / 2 };
	vec3 p4 = { mid.x() - width / 2, mid.y() + width / 2, mid.z() + width / 2 };

	vec3 p5 = { mid.x() + width / 2, mid.y() - width / 2, mid.z() + width / 2 };
	vec3 p6 = { mid.x() + width / 2, mid.y() - width / 2, mid.z() - width / 2 };
	vec3 p7 = { mid.x() - width / 2, mid.y() - width / 2, mid.z() - width / 2 };
	vec3 p8 = { mid.x() - width / 2, mid.y() - width / 2, mid.z() + width / 2 };

	//draw_quad(p1, p2, p4, p3, { 0,1,0 }, { col.x * 0.9f, col.y, col.z });
	//draw_quad(p1, p2, p5, p6, { 1,0,0 }, { col.x, col.y * 0.9f, col.z });
	//draw_quad(p3, p4, p8, p7, { -1,0,0 }, { col.x, col.y, col.z * 0.9f });
	//draw_quad(p1, p4, p8, p5, { 0,0,1 }, { col.x * 0.8f, col.y, col.z });
	//draw_quad(p2, p3, p7, p6, { 0,0,-1 }, { col.x, col.y * 0.8f, col.z });
	//draw_quad(p5, p6, p8, p7, { 0,-1,0 }, { col.x, col.y, col.z * 0.8f });

	draw_quad(p1, p2, p3, p4, { 0,1,0 }, { 0.2, 1, 0 });
	draw_quad(p1, p2, p6, p5, { 1,0,0 }, { 1, 0.2, 0 });
	draw_quad(p3, p4, p8, p7, { -1,0,0 }, { 1, 0, 0.2 });
	draw_quad(p1, p4, p8, p5, { 0,0,1 }, { 0, 0.2, 1 });
	draw_quad(p2, p3, p7, p6, { 0,0,-1 }, { 0.2, 0, 1 });
	draw_quad(p5, p6, p7, p8, { 0,-1,0 }, { 0, 1, 0.2 });
}

void add_quad(vec3 p1, vec3 p2, vec3 p3, vec3 p4, vec3 normal, vec3 col)
{
	quadsQueue.push({ p1 , p2, p3, p4, normal, col });
}

void draw_quad(Quads q)
{
	draw_quad(q.p1, q.p2, q.p3, q.p4, q.normal, q.col);
}

void draw_quad(vec3 p1, vec3 p2, vec3 p3, vec3 p4, vec3 normal, vec3 col)
{

	col *= 0.5f;

	//glEnable(GL_COLOR_MATERIAL);
	//glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glColor3f(col.x(), col.y(), col.z());

	//glColorMaterial(GL_FRONT, GL_SPECULAR);
	//glColor3f(1, 1, 1);
	//glDisable(GL_COLOR_MATERIAL);


	//GLfloat mat_specular[] = { col.x(), col.y(), col.z(), 1.0 };
	//GLfloat mat_shininess[] = { 1, 1, 1, 1.0 };

	//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	//glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glBegin(GL_QUADS);

	glNormal3f(normal.x(), normal.y(), normal.z());
	glVertex3f(p1.x(), p1.y(), p1.z());
	glVertex3f(p2.x(), p2.y(), p2.z());
	glVertex3f(p3.x(), p3.y(), p3.z());
	glVertex3f(p4.x(), p4.y(), p4.z());

	glEnd();
}

bool firstMouse = true;
int lastX = 0, lastY = 0;

void mouse_handler(int button, int state, int x, int y)
{
	if (GLUT_UP == state)
	{
		firstMouse = true;
		//lastX = x, lastY = y;
	}
}

void mouse_action_handler(int x, int y)
{
	if (firstMouse)
	{
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	float xoffset = x - lastX;
	float yoffset = lastY - y; // reversed since y-coordinates go from bottom to top

	lastX = x;
	lastY = y;

	cam.process_movement(xoffset, yoffset);

	//cout << "Mouse Action: " << x << " " << y << endl;
}

void keyboard_action_handler(unsigned char key, int x, int y)
{
	cam.ProcessKeyboard(key);

	display();
	//cout << "Key pressed: " << key << " " << endl;
}

void draw_tri(vec3 p1, vec3 p2, vec3 p3)
{
	vec3 normal = unit_vector(-cross(p1 - p2, p1 - p3));

	glColor3f(1, .1f, 1);

	glBegin(GL_TRIANGLES);

	glNormal3f(normal.x(), normal.y(), normal.z());
	glVertex3f(p1.x(), p1.y(), p1.z());
	glVertex3f(p2.x(), p2.y(), p2.z());
	glVertex3f(p3.x(), p3.y(), p3.z());

	glEnd();
}