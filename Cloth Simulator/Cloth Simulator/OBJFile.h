#pragma once

// Qt header
#include <QDebug>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>

// Common C++ header
#include <string>
#include <fstream>
#include <deque>
#include <regex>

//	Class Data3D

class Data3D
{
public:
//	parameter
	unsigned int value[3];


public:
//	function
	Data3D(){
		value[0] = 0;
		value[1] = 0;
		value[2] = 0;
	}
	Data3D(unsigned int x, unsigned int y, unsigned int z) {
		value[0] = x;
		value[1] = y;
		value[2] = z;
			
	}

};

class Spring
{
public:
	//	parameter
	unsigned int end1;
	unsigned int end2;

public:
	//	function
	Spring()
	{
		end1 = 0;
		end2 = 0;
	}
	Spring(unsigned int _end1,unsigned int _end2)
	{
		end1 = _end1;
		end2 = _end2;
	}

};

//	Class OBJFile

class OBJFile
{
public:

//	Constructor
	OBJFile();
	OBJFile(const char* fileName, bool _phyEff);


//	Parameter

	//	coordinates
	std::deque<QVector3D> vertices;
	std::deque<QVector3D> verNormals;
	std::deque<QVector3D> verTextures;
	
	//	index here are start from 1
	std::deque<Data3D> faceVerID;
	std::deque<Data3D> faceVerNorID;
	std::deque<Data3D> faceTexID;

	//	other attributes of object
	QVector3D midPoint;
	float scale;
	float maxDis;
	QMatrix4x4 modelMatrix;

	//	obj file mode
	enum OBJFileMode{ Has_Normal_Texture, Has_Texture, Has_Normal, No_Normal_Texture};
	int mode;

	//	store grid size, if the object is a floor
	int gridSize;

	//	physics (only for fallen object)
	std::deque<QVector3D> velocity;
	std::deque<QVector3D> forces;
	std::deque<float> mass;
	std::deque<float> restLen;
	std::deque<Spring> springs;

	//	other attribute for vertex
	std::deque<bool> updated;

	//	vertices of coners for cloth 
	unsigned int corners[4];

	//	flags for which edges will be nailed
	bool leftEdgeFixed;
	bool rightEdgeFixed;
	bool upperEdgeFixed;
	bool lowerEdgeFixed;

	//	add wind on object
	int windType;

//	Function

	//	Initiate
	void Clean();

	//	Reset scale
	void ChangeScale(float _scale);
	
	//	Check if the object valid
	bool IsEmpty();

	//	Create objects
	void CreateFloor(int _size);


	//	Read object file
	void ReadOBJ(const char* fileName, bool _physicsEff);

	//	Update normal
	void updateNor();

	//	Force
	void GetSpring();
	void GetForce(std::shared_ptr<OBJFile> floor , std::shared_ptr<OBJFile> ball, bool doRotate = false);
	void FloorBounce(std::shared_ptr<OBJFile> obstacle);
	void BallBounce(std::shared_ptr<OBJFile> obstacle, bool doRotate= false);
	
	void DragCloth();
	void AddWind();
	
	//	Calculate speed
	void GetSpeed();

};
