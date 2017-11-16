#include "MyLoader.h"
#include <stdexcept>
#include <cmath>
#include "3dsId.h"

using namespace std;

MyLoader::MyLoader() : NowPos(0), FileLength(0), Version(0)
{}
/*
*���ܣ���ȡ�ļ�
*˵�������ļ�ǰ��Ҫ��locale::global�Ա�֤������ȷ�����������ļ���
*      ������Ҫ�Զ����Ʒ�ʽ�򿪣��Ա�֤�����ļ��ܹ���ȷ��ȡ
*/
void MyLoader::OpenFile(const string& FileRoad)
{
	locale::global(locale(""));
	FileReader.open(FileRoad.c_str(), ifstream::binary);
	locale::global(locale("C"));
	if (!FileReader)
		throw std::runtime_error("���ļ�ʧ��");;
}
/*
*���ܣ��ر��ļ���
*˵��������
*/
void MyLoader::CloseFile()
{
	FileReader.close();
}
/*
*���ܣ�����3DS�ļ�
*˵�����������и�����������Ϊʵ�ִ˹��ܣ�ʵ�ּ���ͷ3DS�ļ���������Ϣ
*/
void MyLoader::LoadFile()
{
	Chunk MyChunk;
	ReadChunk(&MyChunk);
	if (PRIMARY != MyChunk.ID)
		throw runtime_error("�ļ���");
	FileLength = MyChunk.Len;

	ReadChunk(&MyChunk);
	ReadGLuint(&Version);
	while (NowPos < FileLength)
	{
		ReadChunk(&MyChunk);
		if (MAINOBJECT == MyChunk.ID)
			LoadModel(MyChunk);
		else
			SkipNByte(MyChunk.Len - 6);
	}

	ComputeNormals();
	//MyModel.BuildList();
}
/*
*���ܣ�����ģ��
*˵��������ģ�͵���������
*/
void MyLoader::LoadModel(const Chunk& MyChunk)
{
	size_t BeginPos(NowPos - 6);
	Chunk TempChunk;
	while (NowPos - BeginPos != MyChunk.Len)
	{
		ReadChunk(&TempChunk);
		switch (TempChunk.ID)
		{
		case OBJECT:
			LoadObject(TempChunk);
			break;

		case MATERIAL:
			LoadMaterial(TempChunk);
			break;

		default:
			SkipNByte(TempChunk.Len - 6);
		}
	}
}
/*
*���ܣ��������ж���
*���ܣ��˴�������ȡ��������
*/
void MyLoader::LoadObject(const Chunk& MyChunk)
{
	Object object;
	object.Name = ReadString();
	MyModel.MyObject.push_back(object);

	Chunk ThisChunk;
	size_t BeginPos(NowPos - 7 - object.Name.size());
	while (NowPos - BeginPos != MyChunk.Len)
	{
		ReadChunk(&ThisChunk);
		if (OBJECT_MESH == ThisChunk.ID)
			LoadMesh(ThisChunk);
		else
			SkipNByte(ThisChunk.Len - 6);
	}
}
/*
*���ܣ������������
*˵�����������������㣬�棬�������ʿ�Ĳ���λ�õ�
*/
void MyLoader::LoadMesh(const Chunk& MyChunk)
{
	Object &object = MyModel.MyObject[MyModel.MyObject.size() - 1];

	size_t BeginPos(NowPos - 6);
	Chunk ThisChunk;
	while (NowPos - BeginPos != MyChunk.Len)
	{
		ReadChunk(&ThisChunk);
		switch (ThisChunk.ID)
		{
		case OBJECT_VERTICES:  //����
			LoadVertex(&object);
			break;

		case OBJECT_FACES:     //��
			LoadFaces(&object);
			break;

		case OBJECT_MATERIAL:  //����
			LoadObjectMaterial(&object);
			break;

		default:              //��������Ҫ�Ŀ�
			SkipNByte(ThisChunk.Len - 6);
		}
	}
}
/*
*���ܣ�����Object�Ĺ����Ĳ����б�
*˵�������صĲ���ֱ�ӵĲ��ʣ����ص��ǲ����ڲ��������е�λ��
*/
void MyLoader::LoadObjectMaterial(Object* object)
{
	string Name = ReadString();
	int Pos(-1);
	for (size_t i = 0; i != MyModel.MyMaterial.size(); ++i)
	{
		if (MyModel.MyMaterial[i].name == Name)
			Pos = i;
	}

	if (Pos == -1)
		throw runtime_error("û���ҵ��ò���");

	GLushort Sum(0); GLushort FacePos(0);
	ReadGLushort(&Sum);
	for (size_t i = 0; i != Sum; ++i)
	{
		ReadGLushort(&FacePos);
		object->Faces[FacePos].MaterialPos = Pos;
	}
}
/*
*���ܣ�����ģ���������в���
*˵����ģ������Ҫ�����в��ʶ��ڴ˼�������ʿ�
*/
void MyLoader::LoadMaterial(const Chunk& MyChunk)
{

	Chunk TempChunk;
	Material material;
	size_t BeginPos(NowPos - 6);

	while (NowPos - BeginPos < MyChunk.Len)
	{
		ReadChunk(&TempChunk);
		switch (TempChunk.ID)
		{
		case MATNAME:                       //��������
			material.name = ReadString();
			break;
		case MAT_AMBIENT:                  //����Ambient
			LoadColor(material.ambientColor);
			break;
		case MAT_DIFFUSE:                  //����Diffuse
			LoadColor(material.diffuseColor);
			break;
		case MAT_SPECULAR:                 //����Specular
			LoadColor(material.specularColor);
			break;
		case MAT_SHININESS:                //����Shininess
			LoadPercent(&material.shininess);
			break;
		case MAT_TRANSPARENCY:             //����͸����
			LoadPercent(&material.transparency);
			break;
		default:
			SkipNByte(TempChunk.Len - 6);
		}
	}
	MyModel.MyMaterial.push_back(material);
}
/*
*���ܣ����ز�����ɫ
*˵����3DS�ļ��Ĳ�����ɫ�洢��������ʽ��һ���ǣ�0~255����������ʽ�洢��
*      ռ��һ���ֽڣ���һ���ǣ�0.0~1.0���ĸ����ʹ洢��ռ���ĸ��ֽ�
*/
void MyLoader::LoadColor(float* color)
{
	Chunk TempChunk;
	ReadChunk(&TempChunk);
	switch (TempChunk.ID)
	{
	case COLOR_F:
		ReadGLfloat(&color[0]);
		ReadGLfloat(&color[1]);
		ReadGLfloat(&color[2]);
		break;
	case COLOR_24:
		GLubyte Byte;
		for (size_t i = 0; i != 3; ++i)
		{
			ReadGLubyte(&Byte);
			color[i] = Byte / 255.0;
		}
		break;
	default:
		SkipNByte(TempChunk.Len - 6);
	}
}
/*
*���ܣ����ذٷ���
*˵�������ʵ�Shininess��͸������һ���ٷ��������ǰٷ���������
*      һ�ֵ��������ֽڣ��洢���ǰٷ����ķ����������֣���һ��
*      ��ռ�����ֽڣ���һ��������
*/
void MyLoader::LoadPercent(GLfloat* Temp)
{
	Chunk TempChunk;
	ReadChunk(&TempChunk);
	switch (TempChunk.ID)
	{
	case INT_PERCENTAGE:    //Int�� �ٷ���
		GLushort Percent;
		ReadGLushort(&Percent);
		*Temp = Percent / 100.0;
		break;
	case FLOAT_PERCENTAGE:  //Float�� �ٷ���
		ReadGLfloat(Temp);
		break;
	default:
		SkipNByte(TempChunk.Len - 6);
	}
}
/*
*���ܣ�����������Ϊһʸ��
*˵��������
*/
Vertex MyLoader::Vectors(const Vertex& lPoint, const Vertex& rPoint)
{
	Vertex Point;
	Point.x = lPoint.x - rPoint.x;
	Point.y = lPoint.y - rPoint.y;
	Point.z = lPoint.z - rPoint.z;
	return Point;
}
/*
*���ܣ�������ʸ���Ĳ��
*˵��������ƽ�淨����
*/
Vertex MyLoader::Cross(const Vertex& lPoint, const Vertex& rPoint)
{
	Vertex Point;
	Point.x = lPoint.y * rPoint.z - lPoint.z * rPoint.y;
	Point.y = lPoint.z * rPoint.x - lPoint.x * rPoint.z;
	Point.z = lPoint.x * rPoint.y - lPoint.y * rPoint.x;
	return Point;
}
/*
*���ܣ���λ��ʸ��
*˵���������������
*/
void MyLoader::Normalize(Vertex* point)
{
	float Magnitude = sqrt(point->x * point->x + point->y * point->y + point->z * point->z);
	if (0 == Magnitude)
		Magnitude = 1;
	point->x /= Magnitude;
	point->y /= Magnitude;
	point->z /= Magnitude;
}
/*
*���ܣ�Ϊ����ƽ����㷨����
*˵��������
*/
void MyLoader::ComputeNormals()
{
	for (size_t i = 0; i != MyModel.MyObject.size(); ++i)
	{
		Object& object = MyModel.MyObject[i];
		for (size_t j = 0; j != MyModel.MyObject[i].Faces.size(); ++j)
		{
			Face& face = object.Faces[j];
			const Vertex &Point1 = object.Vertexs[face.Index[0]];
			const Vertex &Point2 = object.Vertexs[face.Index[1]];
			const Vertex &Point3 = object.Vertexs[face.Index[2]];

			face.Normal = Cross(Vectors(Point1, Point3), Vectors(Point3, Point2));
			Normalize(&face.Normal);
		}
	}
}
/*
*���ܣ�����ģ�Ͷ���
*˵�����˴������÷�ʽ���أ����������ֵ���޸�
*/
const Model& MyLoader::GetModel()
{
	return MyModel;
}
/*
*���ܣ�����ͼ�����е��棨3ds max������������洢�ģ�
*˵���������Զ�����������ʽ�洢���ļ��еģ�ÿ����������һ���棬
*      ����������֮����һ�������ֽڵı߽磬��Ҫ����
*/
void MyLoader::LoadFaces(Object* ThisObject)
{
	GLushort Sum(0);
	ReadGLushort(&Sum);
	Face face; GLushort Temp(0);
	for (size_t i = 0; i != Sum; ++i)
	{
		for (size_t j = 0; j != 4; ++j)
		{
			ReadGLushort(&Temp);
			if (j < 3)
				face.Index[j] = Temp;
		}
		ThisObject->Faces.push_back(face);
	}
}
/*
*���ܣ����ض�����Ϣ
*˵�����˴���Ҫע���3DS max������ϵͳ��OpenGL��ͬ����Ҫ
*      ��Y��Z�ύ�����ҽ�����Z��ȡ��
*/
void MyLoader::LoadVertex(Object* const& ThisObject)
{
	GLushort Sum(0);
	ReadGLushort(&Sum);
	Vertex Point;
	float Num(0); float distence(0);
	for (size_t i = 0; i != Sum; ++i)
	{

		ReadGLfloat(&Point.x);
		ReadGLfloat(&Point.z);
		ReadGLfloat(&Point.y);
		Point.z *= -1;
		ThisObject->Vertexs.push_back(Point);
	}
}

void MyLoader::ReadChunk(Chunk* MyChunk)
{
	ReadGLushort(&MyChunk->ID);
	ReadGLuint(&MyChunk->Len);
}

void MyLoader::ReadGLubyte(GLubyte* Ubyte)
{
	FileReader.read(reinterpret_cast< char* >(Ubyte), sizeof(GLubyte));
	NowPos += sizeof(GLubyte);
}

void MyLoader::ReadGLushort(GLushort* Ushort)
{
	FileReader.read(reinterpret_cast< char* >(Ushort), sizeof(GLushort));
	NowPos += sizeof(GLushort);
}

void MyLoader::ReadGLuint(GLuint* Uint)
{
	FileReader.read(reinterpret_cast< char* >(Uint), sizeof(GLuint));
	NowPos += sizeof(GLuint);
}

void MyLoader::ReadGLfloat(GLfloat* Float)
{
	FileReader.read(reinterpret_cast< char* >(Float), sizeof(GLfloat));
	NowPos += sizeof(GLfloat);
}

std::string MyLoader::ReadString()
{
	char alpha;	string TempWord;
	while (FileReader.get(alpha), alpha != 0)
		TempWord += alpha;
	NowPos += TempWord.size() + 1;
	return TempWord;
}

void MyLoader::SkipNByte(const size_t& Num)
{
	FileReader.seekg(Num, ifstream::cur);
	NowPos += Num;
}
