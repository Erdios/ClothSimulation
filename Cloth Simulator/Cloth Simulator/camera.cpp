//	My header
#include "camera.h"

//	Qt header
#include <QQuaternion>
#include <QMatrix4x4>

//-	Constructors
Camera::Camera():eyePos(QVector3D(0,0,0)), viewCenter(QVector3D(0, 0, -1)), up(QVector3D(0, 1, 0))
{
	

}


//-	Functions
void Camera::rotateCamera(float _angle, QVector3D _rotateAxis)
{//	rotateCamera()

	//	view direction
	QVector3D viewDir = -(viewCenter - eyePos).normalized();

	//	rotate teh view direction
	viewDir = QQuaternion::fromAxisAndAngle(_rotateAxis, _angle) * viewDir;
	


	//	calculate new view center
	eyePos = viewCenter + viewDir;


}//rotateCamera()

void Camera::translateCamera(float shift, int _translateType)
{
	//	view direction
	QVector3D viewDir = (viewCenter - eyePos).normalized();

	if (_translateType == NearFar)
	{	//	Move the camera from near to far of on the contrary

		//	Move Eye position
		eyePos += viewDir * shift;

		//	Move view center position
		viewCenter += viewDir * shift;
	}
	else if (_translateType == LeftRight)
	{	//	Move the camera from left to right of on the contrary


		QVector3D pitch = QVector3D::crossProduct(viewDir,up).normalized();
		
		//	Move Eye position
		eyePos += pitch * shift;

		//	Move view center position
		viewCenter += pitch * shift;

	}
	else if (_translateType == UpDown)
	{	//	Move the camera from up to down of on the contrary

		//	Move Eye position
		eyePos += up * shift;

		//	Move view center position
		viewCenter += up * shift;

	}
}

void Camera::rotateAroundYaw(float _angle)
{//	rotateAroundYaw()

	//	rotate the camera by up (pitch) axis
	//	up no need to change, because the horizontal rotation will always around (0,1,0)


	up.normalized();
	rotateCamera(_angle, up);
	

}//	rotateAroundYaw()

void Camera::rotateAroundPitch(float _angle)
{//	rotateAroundPitch()

	//	Calculate new pitch for the camera
	QVector3D pitch;

	pitchAngle += _angle;

	if (pitchAngle >= 85 || pitchAngle <= -85)
	{
		
		pitchAngle -= _angle;
		return;
	}

	pitch = QVector3D::crossProduct((viewCenter - eyePos).normalized(), up).normalized();

	//	Rotate by the right axis or so called Pitch
	rotateCamera(_angle, pitch);

}//	rotateAroundPitch()

