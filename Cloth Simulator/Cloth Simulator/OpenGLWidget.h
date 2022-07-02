#pragma once

//	OpenGL Header
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QTimer>
#include <QQuaternion>


//	My Header
#include "OBJFile.h"
#include "camera.h"


//	c++ standard lib
#include <deque>


class OpenGLWidget : public QOpenGLWidget, public QOpenGLFunctions
{
Q_OBJECT
public:
	
//- parameters
	
	//	Shader
	GLint m_posAttr = 0;
	GLint m_colAttr = 0;
	GLint m_norAttr = 0;
	GLint m_matrixUniform = 0;
	QOpenGLFunctions* f;
	QOpenGLShaderProgram* m_program = nullptr;

	const char* vertexShaderSource =
		"#version 330 core\n"
		"in highp vec4 posAttr;\n"
		"in lowp vec4 colAttr;\n"
		"in lowp vec4 normalAttr;\n"
		"out lowp vec4 col;\n"
		"out lowp vec4 normal;\n"
		"out lowp vec4 objPos;\n"
		"uniform highp mat4 projectMatrix;\n"
		"uniform highp mat4 viewMatrix;\n"
		"uniform highp mat4 modelMatrix;\n"
		"void main() {\n"
		"   col = colAttr;\n"
		"	normal = modelMatrix * normalAttr;\n"
		"	objPos =  modelMatrix * posAttr;\n"
		"   gl_Position = projectMatrix * viewMatrix * modelMatrix  * posAttr;\n"
		"}\n";

	const char* fragmentShaderSource =
		"#version 330 core\n"
		"in lowp vec4 col;\n"
		"in lowp vec4 normal;\n"
		"in lowp vec4 objPos;\n"
		"uniform vec4 lightPos;\n"
		"uniform vec4 lightColor;\n"
		"uniform vec4 viewPos;\n"
		"uniform float ambientStr;\n"
		"uniform float specularStr;\n"
		"void main() {\n"
		"	vec4 ambient = ambientStr * lightColor;\n"
		"	vec4 lightDir = lightPos - objPos;\n"
		"	vec4 viewDir = viewPos - objPos;\n"
		"	vec4 diffuse = max( dot( normalize(normal),normalize(lightDir)), 0.0) * lightColor;\n"
		"	vec4 specular = max( dot( normalize(normalize(lightDir) + normalize(viewDir)), normalize(normal)), 0.0) * lightColor;\n"
		"   gl_FragColor = (ambient + diffuse + specular) * col;\n"
		"}\n";

	


	//	transformation
	Camera camera;
	QPoint axis;
	float angle;
	QMatrix4x4 rm;
	

	//	Lighting
	QVector4D lightPos = QVector4D(0, 15, 0, 1);
	QVector4D lightColor = QVector4D(1, 1, 1, 1);

	//	animation
	double frameTime = 0.5;
	int frameNum = 0;
	QTimer *timer = new QTimer(this);

	
	//	flag
	int animationOn = 0;
	int nextFrameOn = 0;
	int ballOn = false;
	int floorOn = false;
	int doRotate = false;

//- constructor
	OpenGLWidget(QWidget* parent = nullptr);

//- function

	//	Render object
	void drawObject(std::shared_ptr<OBJFile> obj, QMatrix4x4 _m, GLfloat* _color, float _scale);
	void drawNextFrame();	
	void render();


	//	Create & clean objects
	void readOBJFile(char* fileName);
	void createFloor(int _size);
	void createBall();

	void cleanBall();
	void cleanFloor();
	void cleanFallenOBJ();

	//	Physics effect
	void chooseEdge(int edgeSide);
	void addWind(int windType);

	
	
protected:
//- Parameter
	std::shared_ptr<OBJFile> ball;
	std::shared_ptr<OBJFile> floor;
	std::shared_ptr<OBJFile> fallenOBJ;
	


//- Function
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

private slots:
	void checkTime();

};

