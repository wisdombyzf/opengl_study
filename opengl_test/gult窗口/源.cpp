#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<GL\freeglut.h>
#include<GL\glut.h>
#include <Windows.h>
#include<assimp\Importer.hpp>

using namespace std;
/*
GL_POINTS����ÿһ��������Ϊһ������д�������n�������˵�n��������N����
GL_LINES����ÿһ��������Ϊһ���������߶Σ�����2n��1��2n֮�乲������n���߶Σ��ܹ�����N/2���߶�
GL_LINE_STRIP�����ƴӵ�һ�����㵽���һ����������������һ���߶Σ���n��n+1�����㶨�����߶�n���ܹ�����n��1���߶�
GL_LINE_LOOP�����ƴӵ�һ�����㵽���һ����������������һ���߶Σ�Ȼ�����һ������͵�һ��������������n��n+1�����㶨�����߶�n���ܹ�����n���߶�
GL_TRIANGLES����ÿ��������Ϊһ�������������Σ�����3n��2��3n��1��3n�����˵�n�������Σ��ܹ�����N/3��������
GL_TRIANGLE_STRIP������һ�������������Σ���������n������n��n+1��n+2�����˵�n�������Σ�����ż��n������n+1��n��n+2�����˵�n�������Σ��ܹ�����N-2��������
GL_TRIANGLE_FAN������һ�������������Σ����������ɵ�һ�����㼰�������Ķ���ȷ��������1��n+1��n+2�����˵�n�������Σ��ܹ�����N-2��������
GL_QUADS���������ĸ�������ɵ�һ�鵥�����ı��Ρ�����4n��3��4n��2��4n��1��4n�����˵�n���ı��Ρ��ܹ�����N/4���ı���
GL_QUAD_STRIP������һ���������ı��Ρ�ÿ���ı�������һ�Զ��㼰��������һ�Զ��㹲ͬȷ���ġ�����2n��1��2n��2n+2��2n+1�����˵�n���ı��Σ��ܹ�����N/2-1���ı���
GL_POLYGON������һ��͹����Ρ�����1��n�������������Ρ�
*/
GLuint texture_1;
GLuint texture_2;

int n = 3600;  //Բ���ƴ���  
float PI = 3.1415926f;
float R = 0.8f;  //�뾶  
static 	GLfloat angle = 0.0f;

////////opengl�������
void display(void)
{
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0, 0, 1.0); //������ɫ     
	glLoadIdentity();  //���ص�λ����     
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

/////����
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //������ɫ����Ȼ���       
	// ����͸��Ч����ͼ        
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80.0f, 1.0f, 1.0f, 20.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 12.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// ����̫����Դ������һ�ְ�ɫ�Ĺ�Դ     
	{
		GLfloat sun_light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f }; //��Դ��λ������������ϵԲ�ģ����������ʽ  
		GLfloat sun_light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f }; //RGBAģʽ�Ļ����⣬Ϊ0  
		GLfloat sun_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //RGBAģʽ��������⣬ȫ�׹�  
		GLfloat sun_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };  //RGBAģʽ�µľ���� ��ȫ�׹�  
		glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
		glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);

		//�����ƹ�  
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
	}

	// ����̫���Ĳ��ʲ�����̫��      
	{
		GLfloat sun_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };  //������ʵĻ�������ɫ��Ϊ0  
		GLfloat sun_mat_diffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };  //������ʵ����������ɫ��Ϊ0  
		GLfloat sun_mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };   //������ʵľ��淴�����ɫ��Ϊ0  
		GLfloat sun_mat_emission[] = { 0.8f, 0.0f, 0.0f, 1.0f };   //������ʵķ������ɫ��Ϊƫ��ɫ  
		GLfloat sun_mat_shininess = 0.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, sun_mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, sun_mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, sun_mat_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, sun_mat_shininess);
		glutSolidSphere(3.0, 40, 32);
	}

	// �������Ĳ��ʲ����Ƶ���      
	{
		GLfloat earth_mat_ambient[] = { 0.0f, 0.0f, 1.0f, 1.0f };  //������ʵĻ�������ɫ��ƭ��ɫ  
		GLfloat earth_mat_diffuse[] = { 0.0f, 0.0f, 0.5f, 1.0f };  //������ʵ����������ɫ��ƫ��ɫ  
		GLfloat earth_mat_specular[] = { 1.0f, 0.0f, 0.0f, 1.0f };   //������ʵľ��淴�����ɫ����ɫ  
		GLfloat earth_mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };   //������ʵķ������ɫ��Ϊ0  
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

/////����
/* ����load_texture
* ��ȡһ��BMP�ļ���Ϊ����
* ���ʧ�ܣ�����0������ɹ�������������
*/
GLuint texGround;
GLuint texWall;
#define BMP_Header_Length 54  //ͼ���������ڴ���е�ƫ����  
// ����power_of_two�����ж�һ�������ǲ���2����������  
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

	// ���ļ������ʧ�ܣ�����  
	FILE* pFile = fopen(file_name, "rb");
	if (pFile == 0)
		return 0;

	// ��ȡ�ļ���ͼ��Ŀ�Ⱥ͸߶�  
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, 4, 1, pFile);
	fread(&height, 4, 1, pFile);
	fseek(pFile, BMP_Header_Length, SEEK_SET);

	// ����ÿ��������ռ�ֽ����������ݴ����ݼ����������ֽ���  
	{
		GLint line_bytes = width * 3;
		while (line_bytes % 4 != 0)
			++line_bytes;
		total_bytes = line_bytes * height;
	}

	// �����������ֽ��������ڴ�  
	pixels = (GLubyte*)malloc(total_bytes);
	if (pixels == 0)
	{
		fclose(pFile);
		return 0;
	}

	// ��ȡ��������  
	if (fread(pixels, total_bytes, 1, pFile) <= 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// �Ծ;ɰ汾�ļ��ݣ����ͼ��Ŀ�Ⱥ͸߶Ȳ��ǵ������η�������Ҫ��������  
	// ��ͼ���߳�����OpenGL�涨�����ֵ��Ҳ����  
	{
		GLint max;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
		if (!power_of_two(width)
			|| !power_of_two(height)
			|| width > max
			|| height > max)
		{
			const GLint new_width = 256;
			const GLint new_height = 256; // �涨���ź��µĴ�СΪ�߳���������  
			GLint new_line_bytes, new_total_bytes;
			GLubyte* new_pixels = 0;

			// ����ÿ����Ҫ���ֽ��������ֽ���  
			new_line_bytes = new_width * 3;
			while (new_line_bytes % 4 != 0)
				++new_line_bytes;
			new_total_bytes = new_line_bytes * new_height;

			// �����ڴ�  
			new_pixels = (GLubyte*)malloc(new_total_bytes);
			if (new_pixels == 0)
			{
				free(pixels);
				fclose(pFile);
				return 0;
			}

			// ������������  
			gluScaleImage(GL_RGB,
				width, height, GL_UNSIGNED_BYTE, pixels,
				new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

			// �ͷ�ԭ�����������ݣ���pixelsָ���µ��������ݣ�����������width��height  
			free(pixels);
			pixels = new_pixels;
			width = new_width;
			height = new_height;
		}
	}

	// ����һ���µ�������  
	glGenTextures(1, &texture_ID);
	if (texture_ID == 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// ���µ������������������������  
	// �ڰ�ǰ���Ȼ��ԭ���󶨵������ţ��Ա��������лָ�  
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
	glBindTexture(GL_TEXTURE_2D, lastTextureID);  //�ָ�֮ǰ�������  
	free(pixels);
	return texture_ID;
}

void show_texture(void)
{
	// �����Ļ  
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// �����ӽ�  
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75, 1, 1, 21);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(-4, 6, 7, 0, 0, 0, 0, 0, 1);

	glRotatef(angle, 0.0f, 0.0f, 1.0f); //��ת  

	// ���Ƶ����Լ�����  
	glBindTexture(GL_TEXTURE_2D, texGround);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-8.0f, -8.0f, 0.0f);
	glTexCoord2f(0.0f, 3.0f); glVertex3f(-8.0f, 8.0f, 0.0f);
	glTexCoord2f(3.0f, 3.0f); glVertex3f(8.0f, 8.0f, 0.0f);
	glTexCoord2f(3.0f, 0.0f); glVertex3f(8.0f, -8.0f, 0.0f);
	glEnd();
	// ��������  
	glBindTexture(GL_TEXTURE_2D, texWall);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.0f, -3.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-6.0f, -3.0f, 5.0f);
	glTexCoord2f(2.0f, 1.0f); glVertex3f(6.0f, -3.0f, 5.0f);
	glTexCoord2f(2.0f, 0.0f); glVertex3f(6.0f, -3.0f, 0.0f);
	glEnd();

	//��������һ������  
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
		cout << "����ѧ" << endl;
	}
	//���������
	cout << "������������" << endl;
	system("pause");


	//��άͼ�ε�ƽ�ƣ���ת������
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 0, 1);
	glLoadIdentity();  //�ָ�ԭʼ����ϵ
	glRectf(-0.5, -0.5, 0.5, 0.5);
	glFlush();
	cout << "ԭͼ" << endl;
	system("pause");

	glClear(GL_COLOR_BUFFER_BIT);
	glTranslatef(0.5, 0, 0);	//ƽ�Ʋ���
	glRectf(-0.5, -0.5, 0.5, 0.5);
	glFlush();
	cout << "ƽ�ƺ�" << endl;
	system("pause");

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glRotatef(45, 0, 0, 1);		//��ת����
	glRectf(-0.5, -0.5, 0.5, 0.5);
	glFlush();
	cout << "��ת��" << endl;
	system("pause");


	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glScalef(0.5, 0.5, 0.5);	//���Ų���
	glRectf(-0.5, -0.5, 0.5, 0.5);
	glFlush();
	cout << "���ź�" << endl;
	system("pause");
	
	
	glLoadIdentity();  //�ָ�ԭʼ����ϵ

	//������ɫģ��  
	glClear(GL_COLOR_BUFFER_BIT);
	// glShadeModel(GL_FLAT);    //ȡ����ɫ�Ľ��䣬��Ϊ��һ��ɫ  
	glBegin(GL_POLYGON);
	glColor4f(0, 1, 0, 1);
	glVertex2f(-1, 0);
	glColor4f(0, 0, 1, 1);
	glVertex2f(1, 0);
	glColor4f(1, 0, 0, 1);
	glVertex2f(0, 1);
	glEnd();
	glFlush();
	cout << "��ɫ����ģ�����" << endl;
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
	cout << "��һ��ɫģ�����" << endl;
	system("pause");

	//��Բ  
	glClear(GL_COLOR_BUFFER_BIT);
	glColor4f(0, 0, 1, 0);
	glBegin(GL_POLYGON);		//�ö���������Բ
	for (int i = 0; i<n; i++)
	{
		glVertex2f(R*cos(2 * PI*i / n), R*sin(2 * PI*i / n));   //���嶥��  
	}
	glEnd();
	/*
	glFlush:��GL��������е�����͸��Կ������������У��������������أ�
	glFinish:��GL��������е�����͸��Կ������������У��Կ������Щ���Ҳ���ǻ����ˣ��󷵻ء�
	SwapBuffer:��ǰ̨�ͺ�̨�Ļ�����ָ�뽻��һ�¶���Ҳ���ǰ�ǰ̨�����ݱ�ɺ�̨���������
	*/
	glFlush();

	cout << "��Բ�ɹ��������������" << endl;
	system("pause");

	//�������  
	glClear(GL_COLOR_BUFFER_BIT);
	glColor4f(1, 0, 1, 0);

	glBegin(GL_LINE_LOOP);

	//����ABCDE�ֱ�������ǵ�5������  
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

	cout << "������ǳɹ��������������" << endl;
	system("pause");

	//���������ߣ���5������x������ѹ��  
	GLfloat x = -1.0;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 1.0);
	glBegin(GL_LINES);

	glVertex2f(-1.0f, 0.0f);  //�����ĵ�ʮ��  
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
	cout << "���������" << endl;
	system("pause");

	//���õ�Ĵ�С  
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(5);  //���õ�Ĵ�СΪ5������  
	glColor4f(1, 0, 1, 0);  //������ɫΪ��ɫ  
	glBegin(GL_POINTS); //���Ƶ�  
	glVertex2f(0, 0);
	glVertex2f(0.6, 0.6);
	glEnd();
	glFlush();
	cout << "����������" << endl;
	system("pause");
	
	//����ֱ�ߵĿ��
	glClear(GL_COLOR_BUFFER_BIT);
	glLineWidth(3);	//����ֱ�ߵĿ��Ϊ3

	glColor3f(1, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(-1, -1);
	glVertex2f(1,1);
	glEnd();
	glFlush();
	cout << "ֱ���������" << endl;
	system("pause");

	//��������
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_LINE_STIPPLE);		//��������ģʽ
	glLineStipple(3, 0X00FF);		//��һ����������һ��Ƭ�ε����ظ������ڶ������� ��ʾ��ʵ�߿�ͷ......Ϊɶ�ǵ���16λ����������ʾ��������int������bug
	glBegin(GL_LINES);
	glVertex2f(-1,-1);
	glVertex2f(1, 1);
	glEnd();
	glFlush();
	cout << "���߻������" << endl;
	system("pause");
	glDisable(GL_LINE_STIPPLE);	//�ر�����ģʽ

	//����ε�����
	/*
	��������ʱ��˳������ڴ����ϵ��桱Ϊ���棬
	��˳ʱ��˳����ֵ���Ϊ����
	*/
	glColor4f(1, 1, 1, 1);	//������ɫΪ��ɫ
	glClear(GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT, GL_FILL);	//���ö���ε�����Ϊ���ģʽ
	glPolygonMode(GL_BACK, GL_LINE);	//����Ϊ����ģʽ
	glFrontFace(GL_CCW);	//������ʱ��Ϊ���棬ΪϵͳĬ�� 
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
	cout << "����ε�������ʾ���" << endl;
	system("pause");
	//����εķ�����޳�
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);	//������ʱ��Ϊ���棬ΪϵͳĬ�� 
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
	cout << "����εķ����޳����" << endl;
	system("pause");

	//opengl��ɫ����
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0, 1, 1);
	glRectf(-0.5, -0.5, 0.5, 0.5);
	glFlush();
	cout << "��ɫ�������" << endl;
	system("pause");

	//��ɫ�Ļ��ģʽ
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);  //�������ģʽ  
	glDisable(GL_DEPTH_TEST);  //�ر���Ȳ���  
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glColor4f(0, 1, 0, 1);
	glRectf(-0.5, -0.5, 0.8, 0.8);
	glColor4f(0, 0, 1, 1);
	glRectf(0, 0, 1, 1);
	glFinish();
	cout << "��ɫ�Ļ��ģʽ�������" << endl;
	system("pause");
	glEnable(GL_DEPTH_TEST);  //������Ȳ��� 
	glDisable(GL_BLEND);  //�������ģʽ 

}

void KeyBoards(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		break;
	case 'a':
		cout << "������a��"<< endl;
		angle--;
		if ((angle>=360)||(angle <=-360))
		{
			angle = 0;
		}
		show_texture();
		break;
	case 'd':
		cout << "������d��" << endl;
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
	glutInit(&argc, argv);   //��ʼ��GLUT  
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);//����λ��
	glutInitWindowSize(400, 400);//���ڴ�С
	glutCreateWindow("����");
	/*
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	*/
	/*
	glutKeyboardFunc(&KeyBoards);
	glutDisplayFunc(&my_light);   //�ص�����   
	*/
	//glutIdleFunc(&myIdle);


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);    // ��������  
	texture_1 = load_texture("t1.bmp");  //��������  
	texture_1 = load_texture("t2.bmp");
	glutKeyboardFunc(&KeyBoards);
	glutDisplayFunc(&show_texture);   //ע�ắ��   
	glutMainLoop();    //������ʾ�������ڸı�����»���ͼ��
	return 0;
}