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
	10, -12, -45,     //表示在世界矩阵的位置
	0.1, 0.1, 0.1,      //表示xyz放大倍数
	0, 0, 0, 0		//表示旋转,通过glRotatef函数实现
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

	//导入模型 模型的文件夹尽量这样设置 
	//然后模型贴图 装在Data/pic里面  一定要跟前面截图的文件夹名字一样，想改得去CLoad3DS文件里面改
	gothicLoader->Import3DS(&gothicModel, "Data/3ds/GUTEMB_L.3DS");
}

void myDisplay(GLvoid)									
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The View
	glPushMatrix();

	cout << "test的值为" << test << endl;
	for (size_t i = 0; i < 10; i++)
	{
		cout << gothicTrans[i] << "；";
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
		cout << "按下了w键" << endl;
		gothicTrans[0]++;
		myDisplay();
		break;
	case 's':
		cout << "按下了s键" << endl;
		gothicTrans[0]--;
		myDisplay();
	case 'a':
		cout << "按下了a键" << endl;
		gothicTrans[1]++;
		myDisplay();
		break;
	case 'd':
		cout << "按下了d键" << endl;
		gothicTrans[1]--;
		myDisplay();
		break;
	case 'q':
		cout << "按下了q键" << endl;
		gothicTrans[2]++;
		myDisplay();
		break;
	case 'e':
		cout << "按下了e键" << endl;
		gothicTrans[2]--;
		myDisplay();
		break;
	case '+':
		cout << "按下了d键" << endl;
		gothicTrans[3]++;
		gothicTrans[4]++;
		gothicTrans[5]++;
		myDisplay();
		break;
	case '-':
		cout << "按下了d键" << endl;
		gothicTrans[3]--;
		gothicTrans[4]--;
		gothicTrans[5]--;
		myDisplay();
		break;
	case '1':
		cout << "按下了d键" << endl;
		gothicTrans[7] = 0;
		gothicTrans[8] = 0;
		gothicTrans[9] = 0;
		gothicTrans[6]++;
		gothicTrans[9] = 1;

		myDisplay();
		break;

	case '3':
		cout << "按下了d键" << endl;
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
	glutCreateWindow("测试");
	glutKeyboardFunc(&KeyBoards);
	glutDisplayFunc(&myDisplay);
	myInit();
	glutMainLoop();
}