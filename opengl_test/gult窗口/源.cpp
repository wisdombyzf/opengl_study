#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<GL\freeglut.h>
#include<GL\glut.h>
#include <Windows.h>
#include<assimp\Importer.hpp>

using namespace std;
/*
GL_POINTS：把每一个顶点作为一个点进行处理，顶点n即定义了点n，共绘制N个点
GL_LINES：把每一个顶点作为一个独立的线段，顶点2n－1和2n之间共定义了n条线段，总共绘制N/2条线段
GL_LINE_STRIP：绘制从第一个顶点到最后一个顶点依次相连的一组线段，第n和n+1个顶点定义了线段n，总共绘制n－1条线段
GL_LINE_LOOP：绘制从第一个顶点到最后一个顶点依次相连的一组线段，然后最后一个顶点和第一个顶点相连，第n和n+1个顶点定义了线段n，总共绘制n条线段
GL_TRIANGLES：把每个顶点作为一个独立的三角形，顶点3n－2、3n－1和3n定义了第n个三角形，总共绘制N/3个三角形
GL_TRIANGLE_STRIP：绘制一组相连的三角形，对于奇数n，顶点n、n+1和n+2定义了第n个三角形；对于偶数n，顶点n+1、n和n+2定义了第n个三角形，总共绘制N-2个三角形
GL_TRIANGLE_FAN：绘制一组相连的三角形，三角形是由第一个顶点及其后给定的顶点确定，顶点1、n+1和n+2定义了第n个三角形，总共绘制N-2个三角形
GL_QUADS：绘制由四个顶点组成的一组单独的四边形。顶点4n－3、4n－2、4n－1和4n定义了第n个四边形。总共绘制N/4个四边形
GL_QUAD_STRIP：绘制一组相连的四边形。每个四边形是由一对顶点及其后给定的一对顶点共同确定的。顶点2n－1、2n、2n+2和2n+1定义了第n个四边形，总共绘制N/2-1个四边形
GL_POLYGON：绘制一个凸多边形。顶点1到n定义了这个多边形。
*/
GLuint texture_1;
GLuint texture_2;

int n = 3600;  //圆绘制次数  
float PI = 3.1415926f;
float R = 0.8f;  //半径  
static 	GLfloat angle = 0.0f;

////////opengl相机设置
void display(void)
{
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0, 0, 1.0); //画笔蓝色     
	glLoadIdentity();  //加载单位矩阵     
	gluLookAt(0.0, 5.0, 0.0, 0, 0, 0, 1.0, 0.0, 0);
	glutWireTeapot(1.3);
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 4, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}
/////////

/////光照
void myIdle(void)
{
	angle += 1.0f;
	if (angle >= 360.0f)
	{
		angle = 0.0f;
	}
	//my_light();
}

void my_light(void)
{
	glClearColor(0.3, 0.7, 0.5, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //清理颜色和深度缓存       
	// 创建透视效果视图        
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80.0f, 1.0f, 1.0f, 20.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 12.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// 定义太阳光源，它是一种白色的光源     
	{
		GLfloat sun_light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f }; //光源的位置在世界坐标系圆心，齐次坐标形式  
		GLfloat sun_light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f }; //RGBA模式的环境光，为0  
		GLfloat sun_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //RGBA模式的漫反射光，全白光  
		GLfloat sun_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };  //RGBA模式下的镜面光 ，全白光  
		glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
		glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);

		//开启灯光  
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
	}

	// 定义太阳的材质并绘制太阳      
	{
		GLfloat sun_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };  //定义材质的环境光颜色，为0  
		GLfloat sun_mat_diffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };  //定义材质的漫反射光颜色，为0  
		GLfloat sun_mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };   //定义材质的镜面反射光颜色，为0  
		GLfloat sun_mat_emission[] = { 0.8f, 0.0f, 0.0f, 1.0f };   //定义材质的辐射广颜色，为偏红色  
		GLfloat sun_mat_shininess = 0.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, sun_mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, sun_mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, sun_mat_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, sun_mat_shininess);
		glutSolidSphere(3.0, 40, 32);
	}

	// 定义地球的材质并绘制地球      
	{
		GLfloat earth_mat_ambient[] = { 0.0f, 0.0f, 1.0f, 1.0f };  //定义材质的环境光颜色，骗蓝色  
		GLfloat earth_mat_diffuse[] = { 0.0f, 0.0f, 0.5f, 1.0f };  //定义材质的漫反射光颜色，偏蓝色  
		GLfloat earth_mat_specular[] = { 1.0f, 0.0f, 0.0f, 1.0f };   //定义材质的镜面反射光颜色，红色  
		GLfloat earth_mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };   //定义材质的辐射光颜色，为0  
		GLfloat earth_mat_shininess = 30.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, earth_mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, earth_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, earth_mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, earth_mat_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, earth_mat_shininess);
		glRotatef(angle, 0.0f, -1.0f, 0.0f);
		glTranslatef(7.0f, 0.0f, 0.0f);
		glutSolidSphere(3.0, 40, 32);
	}
	Sleep(10);
	glutSwapBuffers();
}

////////

/////纹理
/* 函数load_texture
* 读取一个BMP文件作为纹理
* 如果失败，返回0，如果成功，返回纹理编号
*/
GLuint texGround;
GLuint texWall;
#define BMP_Header_Length 54  //图像数据在内存块中的偏移量  
// 函数power_of_two用于判断一个整数是不是2的整数次幂  
int power_of_two(int n)
{
	if (n <= 0)
	{
		return 0;
	}
	return (n & (n - 1)) == 0;
}
GLuint load_texture(const char* file_name)
{
	GLint width, height, total_bytes;
	GLubyte* pixels = 0;
	GLuint last_texture_ID = 0, texture_ID = 0;

	// 打开文件，如果失败，返回  
	FILE* pFile = fopen(file_name, "rb");
	if (pFile == 0)
		return 0;

	// 读取文件中图象的宽度和高度  
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, 4, 1, pFile);
	fread(&height, 4, 1, pFile);
	fseek(pFile, BMP_Header_Length, SEEK_SET);

	// 计算每行像素所占字节数，并根据此数据计算总像素字节数  
	{
		GLint line_bytes = width * 3;
		while (line_bytes % 4 != 0)
			++line_bytes;
		total_bytes = line_bytes * height;
	}

	// 根据总像素字节数分配内存  
	pixels = (GLubyte*)malloc(total_bytes);
	if (pixels == 0)
	{
		fclose(pFile);
		return 0;
	}

	// 读取像素数据  
	if (fread(pixels, total_bytes, 1, pFile) <= 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// 对就旧版本的兼容，如果图象的宽度和高度不是的整数次方，则需要进行缩放  
	// 若图像宽高超过了OpenGL规定的最大值，也缩放  
	{
		GLint max;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
		if (!power_of_two(width)
			|| !power_of_two(height)
			|| width > max
			|| height > max)
		{
			const GLint new_width = 256;
			const GLint new_height = 256; // 规定缩放后新的大小为边长的正方形  
			GLint new_line_bytes, new_total_bytes;
			GLubyte* new_pixels = 0;

			// 计算每行需要的字节数和总字节数  
			new_line_bytes = new_width * 3;
			while (new_line_bytes % 4 != 0)
				++new_line_bytes;
			new_total_bytes = new_line_bytes * new_height;

			// 分配内存  
			new_pixels = (GLubyte*)malloc(new_total_bytes);
			if (new_pixels == 0)
			{
				free(pixels);
				fclose(pFile);
				return 0;
			}

			// 进行像素缩放  
			gluScaleImage(GL_RGB,
				width, height, GL_UNSIGNED_BYTE, pixels,
				new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

			// 释放原来的像素数据，把pixels指向新的像素数据，并重新设置width和height  
			free(pixels);
			pixels = new_pixels;
			width = new_width;
			height = new_height;
		}
	}

	// 分配一个新的纹理编号  
	glGenTextures(1, &texture_ID);
	if (texture_ID == 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// 绑定新的纹理，载入纹理并设置纹理参数  
	// 在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复  
	GLint lastTextureID = last_texture_ID;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, lastTextureID);  //恢复之前的纹理绑定  
	free(pixels);
	return texture_ID;
}

void show_texture(void)
{
	// 清除屏幕  
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 设置视角  
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75, 1, 1, 21);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(-4, 6, 7, 0, 0, 0, 0, 0, 1);

	glRotatef(angle, 0.0f, 0.0f, 1.0f); //旋转  

	// 绘制底面以及纹理  
	glBindTexture(GL_TEXTURE_2D, texGround);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-8.0f, -8.0f, 0.0f);
	glTexCoord2f(0.0f, 3.0f); glVertex3f(-8.0f, 8.0f, 0.0f);
	glTexCoord2f(3.0f, 3.0f); glVertex3f(8.0f, 8.0f, 0.0f);
	glTexCoord2f(3.0f, 0.0f); glVertex3f(8.0f, -8.0f, 0.0f);
	glEnd();
	// 绘制立面  
	glBindTexture(GL_TEXTURE_2D, texWall);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.0f, -3.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-6.0f, -3.0f, 5.0f);
	glTexCoord2f(2.0f, 1.0f); glVertex3f(6.0f, -3.0f, 5.0f);
	glTexCoord2f(2.0f, 0.0f); glVertex3f(6.0f, -3.0f, 0.0f);
	glEnd();

	//绘制另外一个立面  
	glBegin(GL_QUADS);
	glTexCoord2f(2.0f, 0.0f); glVertex3f(6.0f, -3.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(6.0f, 9.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(6.0f, 9.0f, 5.0f);
	glTexCoord2f(2.0f, 1.0f); glVertex3f(6.0f, -3.0f, 5.0f);
	glEnd();

	glutSwapBuffers();
}
//////

void myDisplay(void)
{

	glClearColor(1, 1, 0.8, 1);
	while (true)
	{
		my_light();
		cout << "不科学" << endl;
	}
	//相机的设置
	cout << "相机的设置完成" << endl;
	system("pause");


	//二维图形的平移，旋转与缩放
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 0, 1);
	glLoadIdentity();  //恢复原始坐标系
	glRectf(-0.5, -0.5, 0.5, 0.5);
	glFlush();
	cout << "原图" << endl;
	system("pause");

	glClear(GL_COLOR_BUFFER_BIT);
	glTranslatef(0.5, 0, 0);	//平移操作
	glRectf(-0.5, -0.5, 0.5, 0.5);
	glFlush();
	cout << "平移后" << endl;
	system("pause");

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glRotatef(45, 0, 0, 1);		//旋转操作
	glRectf(-0.5, -0.5, 0.5, 0.5);
	glFlush();
	cout << "旋转后" << endl;
	system("pause");


	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glScalef(0.5, 0.5, 0.5);	//缩放操作
	glRectf(-0.5, -0.5, 0.5, 0.5);
	glFlush();
	cout << "缩放后" << endl;
	system("pause");
	
	
	glLoadIdentity();  //恢复原始坐标系

	//设置着色模型  
	glClear(GL_COLOR_BUFFER_BIT);
	// glShadeModel(GL_FLAT);    //取消颜色的渐变，改为单一颜色  
	glBegin(GL_POLYGON);
	glColor4f(0, 1, 0, 1);
	glVertex2f(-1, 0);
	glColor4f(0, 0, 1, 1);
	glVertex2f(1, 0);
	glColor4f(1, 0, 0, 1);
	glVertex2f(0, 1);
	glEnd();
	glFlush();
	cout << "颜色渐变模型完成" << endl;
	system("pause");

	glClear(GL_COLOR_BUFFER_BIT);
	glShadeModel(GL_FLAT);    
	glBegin(GL_POLYGON);
	glColor4f(0, 1, 0, 1);
	glVertex2f(-1, 0);
	glColor4f(0, 0, 1, 1);
	glVertex2f(1, 0);
	glColor4f(1, 0, 0, 1);
	glVertex2f(0, 1);
	glEnd();
	glFlush();
	cout << "单一颜色模型完成" << endl;
	system("pause");

	//画圆  
	glClear(GL_COLOR_BUFFER_BIT);
	glColor4f(0, 0, 1, 0);
	glBegin(GL_POLYGON);		//用多边形来拟合圆
	for (int i = 0; i<n; i++)
	{
		glVertex2f(R*cos(2 * PI*i / n), R*sin(2 * PI*i / n));   //定义顶点  
	}
	glEnd();
	/*
	glFlush:将GL命令队列中的命令发送给显卡并清空命令队列，发送完立即返回；
	glFinish:将GL命令队列中的命令发送给显卡并清空命令队列，显卡完成这些命令（也就是画完了）后返回。
	SwapBuffer:把前台和后台的缓冲区指针交换一下而已也就是把前台的内容变成后台缓冲的内容
	*/
	glFlush();

	cout << "画圆成功，任意键继续…" << endl;
	system("pause");

	//画五角星  
	glClear(GL_COLOR_BUFFER_BIT);
	glColor4f(1, 0, 1, 0);

	glBegin(GL_LINE_LOOP);

	//以下ABCDE分别是五角星的5个顶点  
	GLfloat xA = R*cos(90 * 2 * PI / 360);
	GLfloat yA = R*sin(90 * 2 * PI / 360);

	GLfloat xB = R*cos(306 * 2 * PI / 360);
	GLfloat yB = R*sin(306 * 2 * PI / 360);

	GLfloat xC = R*cos(162 * 2 * PI / 360);
	GLfloat yC = R*sin(162 * 2 * PI / 360);

	GLfloat xD = R*cos(18 * 2 * PI / 360);
	GLfloat yD = R*sin(18 * 2 * PI / 360);

	GLfloat xE = R*cos(234 * 2 * PI / 360);
	GLfloat yE = R*sin(234 * 2 * PI / 360);

	glVertex2f(xA, yA);
	glVertex2f(xB, yB);
	glVertex2f(xC, yC);
	glVertex2f(xD, yD);
	glVertex2f(xE, yE);
	glEnd();
	glFlush();

	cout << "画五角星成功，任意键继续…" << endl;
	system("pause");

	//画正弦曲线，画5个，在x方向上压缩  
	GLfloat x = -1.0;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 1.0);
	glBegin(GL_LINES);

	glVertex2f(-1.0f, 0.0f);  //过中心的十字  
	glVertex2f(1.0f, 0.0f);
	glVertex2f(0.0f, -1.0f);
	glVertex2f(0.0f, 1.0f);

	glEnd();
	glBegin(GL_LINE_STRIP);
	for (float x = -5 * PI; x<5 * PI; x += 0.1f)
	{
		glVertex2f(x / (5 * PI), sin(x));
	}
	glEnd();
	glFlush();
	cout << "正弦线完成" << endl;
	system("pause");

	//设置点的大小  
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(5);  //设置点的大小为5个像素  
	glColor4f(1, 0, 1, 0);  //设置颜色为紫色  
	glBegin(GL_POINTS); //绘制点  
	glVertex2f(0, 0);
	glVertex2f(0.6, 0.6);
	glEnd();
	glFlush();
	cout << "点的设置完成" << endl;
	system("pause");
	
	//设置直线的宽度
	glClear(GL_COLOR_BUFFER_BIT);
	glLineWidth(3);	//设置直线的宽度为3

	glColor3f(1, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(-1, -1);
	glVertex2f(1,1);
	glEnd();
	glFlush();
	cout << "直线设置完成" << endl;
	system("pause");

	//设置虚线
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_LINE_STIPPLE);		//开启虚线模式
	glLineStipple(3, 0X00FF);		//第一个参数设置一个片段的像素个数，第二个参数 表示以实线开头......为啥非得用16位二进制数表示，而且用int好像还有bug
	glBegin(GL_LINES);
	glVertex2f(-1,-1);
	glVertex2f(1, 1);
	glEnd();
	glFlush();
	cout << "虚线绘制完成" << endl;
	system("pause");
	glDisable(GL_LINE_STIPPLE);	//关闭虚线模式

	//多边形的正反
	/*
	顶点以逆时针顺序呈现在窗口上的面”为正面，
	以顺时针顺序呈现的面为反面
	*/
	glColor4f(1, 1, 1, 1);	//设置颜色为白色
	glClear(GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT, GL_FILL);	//设置多边形的正面为填充模式
	glPolygonMode(GL_BACK, GL_LINE);	//反面为线性模式
	glFrontFace(GL_CCW);	//设置逆时针为正面，为系统默认 
	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	glVertex2f(-0.5, 0);
	glVertex2f(-0.5, -0.5);
	glVertex2f(0, -0.5);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(0, 0.5);
	glVertex2f(0.5, 0.5);
	glVertex2f(0.5, 0);
	glVertex2f(0, 0);
	glEnd();
	glFlush();
	cout << "多边形的正反表示完成" << endl;
	system("pause");
	//多边形的反面的剔除
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);	//设置逆时针为正面，为系统默认 
	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	glVertex2f(-0.5, 0);
	glVertex2f(-0.5, -0.5);
	glVertex2f(0, -0.5);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(0, 0.5);
	glVertex2f(0.5, 0.5);
	glVertex2f(0.5, 0);
	glVertex2f(0, 0);
	glEnd();
	glFlush();
	cout << "多边形的反面剔除完成" << endl;
	system("pause");

	//opengl颜色设置
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0, 1, 1);
	glRectf(-0.5, -0.5, 0.5, 0.5);
	glFlush();
	cout << "颜色设置完成" << endl;
	system("pause");

	//颜色的混合模式
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);  //开启混合模式  
	glDisable(GL_DEPTH_TEST);  //关闭深度测试  
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glColor4f(0, 1, 0, 1);
	glRectf(-0.5, -0.5, 0.8, 0.8);
	glColor4f(0, 0, 1, 1);
	glRectf(0, 0, 1, 1);
	glFinish();
	cout << "颜色的混合模式测试完成" << endl;
	system("pause");
	glEnable(GL_DEPTH_TEST);  //开启深度测试 
	glDisable(GL_BLEND);  //开启混合模式 

}

void KeyBoards(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		break;
	case 'a':
		cout << "按下了a键"<< endl;
		angle--;
		if ((angle>=360)||(angle <=-360))
		{
			angle = 0;
		}
		show_texture();
		break;
	case 'd':
		cout << "按下了d键" << endl;
		angle++;
		if ((angle >= 360) || (angle <= -360))
		{
			angle = 0;
		}
		show_texture();
		break;

	default:
		break;
	}

}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);   //初始化GLUT  
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);//窗口位置
	glutInitWindowSize(400, 400);//窗口大小
	glutCreateWindow("测试");
	/*
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	*/
	/*
	glutKeyboardFunc(&KeyBoards);
	glutDisplayFunc(&my_light);   //回调函数   
	*/
	//glutIdleFunc(&myIdle);


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);    // 启用纹理  
	texture_1 = load_texture("t1.bmp");  //加载纹理  
	texture_1 = load_texture("t2.bmp");
	glutKeyboardFunc(&KeyBoards);
	glutDisplayFunc(&show_texture);   //注册函数   
	glutMainLoop();    //持续显示，当窗口改变会重新绘制图形
	return 0;
}