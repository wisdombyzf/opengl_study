#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include<GL\glut.h>
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include "CLoad3DS.h"

GLuint	texture[1];			// Storage For One Texture ( NEW )

GLdouble test;
CLoad3DS *gothicLoader = new(CLoad3DS);
t3DModel gothicModel;
GLint  width = 1200;
GLint  height = 800;
float gothicTrans[10] = {
	10, -12, -45,     //��ʾ����������λ��
	0.1, 0.1, 0.1,      //��ʾxyz�Ŵ���
	0, 0, 0, 0		//��ʾ��ת,ͨ��glRotatef����ʵ��
};

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

void  myInit(GLvoid)										// All Setup For OpenGL Goes Here
{

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 150.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	//����ģ�� ģ�͵��ļ��о����������� 
	//Ȼ��ģ����ͼ װ��Data/pic����  һ��Ҫ��ǰ���ͼ���ļ�������һ������ĵ�ȥCLoad3DS�ļ������
	gothicLoader->Import3DS(&gothicModel, "Data/3ds/GUTEMB_L.3DS");
}

void myDisplay(GLvoid)									
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The View
	glPushMatrix();

	cout << "test��ֵΪ" << test << endl;
	for (size_t i = 0; i < 10; i++)
	{
		cout << gothicTrans[i] << "��";
	}
	changeObject(gothicTrans);
	drawModel(gothicModel, true, false);
	
	//gluLookAt(0,0, 0, 0, 0, 0, 0, 0.0, 0);
	glPopMatrix();
	glFlush();
}

void KeyBoards(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		cout << "������w��" << endl;
		gothicTrans[0]++;
		myDisplay();
		break;
	case 's':
		cout << "������s��" << endl;
		gothicTrans[0]--;
		myDisplay();
	case 'a':
		cout << "������a��" << endl;
		gothicTrans[1]++;
		myDisplay();
		break;
	case 'd':
		cout << "������d��" << endl;
		gothicTrans[1]--;
		myDisplay();
		break;
	case 'q':
		cout << "������q��" << endl;
		gothicTrans[2]++;
		myDisplay();
		break;
	case 'e':
		cout << "������e��" << endl;
		gothicTrans[2]--;
		myDisplay();
		break;
	case '+':
		cout << "������d��" << endl;
		gothicTrans[3]++;
		gothicTrans[4]++;
		gothicTrans[5]++;
		myDisplay();
		break;
	case '-':
		cout << "������d��" << endl;
		gothicTrans[3]--;
		gothicTrans[4]--;
		gothicTrans[5]--;
		myDisplay();
		break;
	case '1':
		cout << "������d��" << endl;
		gothicTrans[7] = 0;
		gothicTrans[8] = 0;
		gothicTrans[9] = 0;
		gothicTrans[6]++;
		gothicTrans[9] = 1;

		myDisplay();
		break;

	case '3':
		cout << "������d��" << endl;
		gothicTrans[7] = 0;
		gothicTrans[8] = 0;
		gothicTrans[9] = 0;
		gothicTrans[6]--;
		gothicTrans[9] = 1;

		myDisplay();
		break;
	default:
		break;
	}

}

void main(int argc, char** argv)
{
	test = 0;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("����");
	glutKeyboardFunc(&KeyBoards);
	glutDisplayFunc(&myDisplay);
	myInit();
	glutMainLoop();
}