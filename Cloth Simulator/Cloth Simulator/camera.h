#pragma once
//	Qt header
#include <QVector3D>
#include <QMatrix4x4>

class Camera {

public:

//	Constructor
	Camera();

//	Parameters
	QVector3D eyePos, viewCenter, up;
	float pitchAngle = 0;
	enum translateType {NearFar,UpDown,LeftRight};

//	Functions
	void rotateCamera(float _angle, QVector3D _rotateAxis);
	void translateCamera(float shift, int _translateType);
	void rotateAroundYaw(float _angle);
	void rotateAroundPitch(float _angle);
};