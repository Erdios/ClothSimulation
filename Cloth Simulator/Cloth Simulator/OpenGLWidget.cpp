#include "OpenGLWidget.h"

//- Constructor

OpenGLWidget::OpenGLWidget(QWidget* parent):QOpenGLWidget(parent)
{
	//	For animation
	connect(timer, &QTimer::timeout,this,&OpenGLWidget::checkTime);

	
	//	Rotation
	camera = Camera();
	axis = QPoint(0,0);
	angle = 0;
	rm.setToIdentity();
	
}


//-	Function

//	Generate object
void OpenGLWidget::readOBJFile(char* fileName) 
{//	readOBJFile()
	
	//	Read object file
	fallenOBJ = std::make_shared<OBJFile>(fileName,true);

}//	readOBJFile()

void OpenGLWidget::createFloor(int _size)
{
	//	create new floor object
	floor = std::make_shared<OBJFile>();
	floor->CreateFloor(_size);
	
	
}

void OpenGLWidget::createBall()
//	create a ball on floor
{//	createBall()

	
	ball = std::make_shared<OBJFile>("sphere20x20.obj",false);
	
}//	createBall()

void OpenGLWidget::cleanFloor()
// clean the obstacle model data
{//	cleanBall()


	//	check if obstacle is empty
	if (floor == nullptr)
		return;

	//	clean the obstacle if the obstacle object is not empty
	floor->Clean();



}//	cleanBall()

void OpenGLWidget::cleanBall()
// clean the obstacle model data
{//	cleanBall()

	//	check if obstacle is empty
	if (ball == nullptr)
		return;

	//	clean the obstacle if the obstacle object is not empty
	ball->Clean();
	
}//	cleanBall()

void OpenGLWidget::cleanFallenOBJ()
// clean the fallen object model data
{//	cleanObstacle()

	//	check if fallen object is empty
	if (fallenOBJ == nullptr)
		return;

	//	clean the obstacle if the fallen object object is not empty
	fallenOBJ->Clean();

}//	cleanObstacle()


//	Physics effect
void OpenGLWidget::chooseEdge(int edgeSide) 
//	choose which edge will be fixed

{//	chooseEdge()

	if (fallenOBJ != nullptr) {

		if(edgeSide == 1)
		{
			fallenOBJ->leftEdgeFixed = true;
		}
		if(edgeSide == 2)
		{
			fallenOBJ->rightEdgeFixed = true;
		}
		if (edgeSide == 3)
		{
			fallenOBJ->upperEdgeFixed = true;
		}
		if (edgeSide == 4)
		{
			fallenOBJ->lowerEdgeFixed = true;
		}
		if (edgeSide == -1)
		{
			fallenOBJ->leftEdgeFixed = false;
		}
		if (edgeSide == -2)
		{
			fallenOBJ->rightEdgeFixed = false;
		}
		if (edgeSide == -3)
		{
			fallenOBJ->upperEdgeFixed = false;
		}
		if (edgeSide == -4)
		{
			fallenOBJ->lowerEdgeFixed = false;
		}
	}
	
}//	chooseEdge()

void OpenGLWidget::addWind(int windType)
//	add wind on cloth

{//	addWind()

	if (fallenOBJ != nullptr)
	{

		fallenOBJ->windType = windType;
	}

}//	addWind()
 
 //	Render by Qt GL
void OpenGLWidget::initializeGL()
//	override the initializeGL() function for QOpenGL
{//	initializeGL()
	f = this->context()->functions();
    // run and link shader program
    m_program = new QOpenGLShaderProgram(this);

    // there are two shader. one vertex shader and one fragment shader.
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);

    // link the two shader
    if (!m_program->link()) {
		return;
    }
	
	
    // create link to the attributes and uniform of the shaders
    m_posAttr = m_program->attributeLocation("posAttr");
    Q_ASSERT(m_posAttr != -1);
    m_colAttr = m_program->attributeLocation("colAttr");
    Q_ASSERT(m_colAttr != -1);
	m_norAttr = m_program->attributeLocation("normalAttr");
	

    m_matrixUniform = m_program->uniformLocation("matrix");
    Q_ASSERT(m_matrixUniform != -1);

	
    // initial the background
    
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    f->glClearColor(0.94f, 1.f, 1.f, 1.0f);


}//	initializeGL()

void OpenGLWidget::resizeGL(int w, int h) 
//	override the resizeGL() function for QOpenGL
{//	resizeGL()

    const qreal retinaScale = devicePixelRatio();
    f->glViewport(0, 0, w * retinaScale,h * retinaScale);
    f->glClearColor(0.69f, 0.76f, 0.87f,1.0f);
	


}//	resizeGL()

void OpenGLWidget::paintGL()
//	override the paintGL() function for QOpenGL
{//	paintGL()
	
	// set up timer to count for next frame
	if (nextFrameOn == 0)
	{
		timer->start(1000 * frameTime);
		nextFrameOn = 1;
	}
    //	bind shader
    m_program->bind();

	f->glViewport(0, 0, width() , height() );
	
	//	enable depth test
	f->glEnable(GL_DEPTH_TEST);


	// draw the shapes and movements
	render();

    // release the shader
    m_program->release();

    // frame move forward 1 step
	drawNextFrame();
	


}//	paintGL()

void OpenGLWidget::drawNextFrame() 
//	move to next frame
{//	drawNextFrame()
	
 
 //if(animationOn == 1 &&)

	if ( nextFrameOn == 2) 
	// if is in animation mode and next frame is ready to play
	// do the following
	{
		
		// reset the nextFrameOn flag
		nextFrameOn = 0;

		// move to next frame
		frameNum += 1;
		
	}
	this->update();
	

}//	drawNextFrame()

void OpenGLWidget::drawObject(std::shared_ptr<OBJFile> obj, QMatrix4x4 _m, GLfloat* _color, float _scale) {

	//	check if obj is load
	if (obj == nullptr)
	{
		return;
	}

	//	Set scale
	obj->ChangeScale(_scale);

	//	vertices
	GLfloat vertices[] = {
		 0.0f,  0.f,0.707f,
		-0.5f, 0.f,0.5,
		 0.5f, 0.f,0.5

	};

	//	normal
	GLfloat normals[] = {
		0.0f, 1.0f, 0.0f,0.0f,
		0.0f, 1.0f, 0.0f,0.0f,
		0.0f, 1.0f, 0.0f,0.0f
	};


	//	color for lines
	GLfloat colors[] = {

		0.f, 0.f, 0.f,
		0.f, 0.f, 0.f,
		0.f, 0.f, 0.f
	};

	//	Transformation
	
	//	Move the object center to the origin
	_m.translate(-1 * obj->scale * obj->midPoint);

	m_program->setUniformValue("modelMatrix", _m);

	//	Record the new position after translating
	obj->modelMatrix = _m;

	
	//	get physical effect if this is a fallen object
	if (obj->velocity.size() > 0) {

		obj->GetForce(floor, ball, doRotate);
		

		obj->GetSpeed();
	}
	
	//	initialize the vertex update status
	for (int i =0 ;i<obj->updated.size(); i++)
	{
		obj->updated[i] = false;
	}

	


	//	loop through vertices for all faces
	for (int i = 0; i < obj->faceVerID.size(); i++)
	{//	per face

		for (int j = 0; j < 3; j++)
		{//	per vertices

			if (obj->velocity.size() > 0 && obj->updated[obj->faceVerID[i].value[j] - 1] == false) {
				
				obj->vertices[obj->faceVerID[i].value[j] - 1].setX(obj->vertices[obj->faceVerID[i].value[j] - 1].x());// +(obj->velocity[obj->faceVerID[i].value[j] - 1].x() * 0.001));
				obj->vertices[obj->faceVerID[i].value[j] - 1].setY(obj->vertices[obj->faceVerID[i].value[j] - 1].y());// +(obj->velocity[obj->faceVerID[i].value[j] - 1].y() * 0.001));
				obj->vertices[obj->faceVerID[i].value[j] - 1].setZ(obj->vertices[obj->faceVerID[i].value[j] - 1].z());// +(obj->velocity[obj->faceVerID[i].value[j] - 1].z() * 0.001));

				obj->updated[obj->faceVerID[i].value[j] - 1] = true;
			}
			
			
			vertices[0 + j * 3] = obj->scale * obj->vertices[obj->faceVerID[i].value[j] - 1].x();
			vertices[1 + j * 3] = obj->scale * obj->vertices[obj->faceVerID[i].value[j] - 1].y();
			vertices[2 + j * 3] = obj->scale * obj->vertices[obj->faceVerID[i].value[j] - 1].z();
			


			if (obj->mode == OBJFile::Has_Normal || obj->mode == OBJFile::Has_Normal_Texture)
			//	check if the object file has normals inside
			{
				normals[0 + j * 4] = obj->verNormals[obj->faceVerNorID[i].value[j] - 1].x();
				normals[1 + j * 4] = obj->verNormals[obj->faceVerNorID[i].value[j] - 1].y();
				normals[2 + j * 4] = obj->verNormals[obj->faceVerNorID[i].value[j] - 1].z();
			}

		}//	per vertices

		//	pass vertices attributes to shader
		f->glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices);
		f->glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, _color);
		f->glVertexAttribPointer(m_norAttr, 4, GL_FLOAT, GL_FALSE, 0, normals);



		//	enable attriarray
		f->glEnableVertexAttribArray(m_posAttr);
		f->glEnableVertexAttribArray(m_colAttr);
		f->glEnableVertexAttribArray(m_norAttr);

		

		// triangle primitive
		f->glDrawArrays(GL_TRIANGLES, 0, 3);


		// line primitive
		f->glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);
		f->glDrawArrays(GL_LINE_LOOP, 0, 3);


		//	disable attriarray
		f->glDisableVertexAttribArray(m_colAttr);
		f->glDisableVertexAttribArray(m_posAttr);
		f->glDisableVertexAttribArray(m_norAttr);

		//	light
		m_program->setUniformValue("lightPos", lightPos);
		m_program->setUniformValue("lightColor", lightColor);
		m_program->setUniformValue("viewPos", QVector4D(camera.eyePos, 1.0));
		m_program->setUniformValue("ambientStr", GLfloat(0.1));
		m_program->setUniformValue("specularStr", GLfloat(0.5));


	}//	per face
	


}

void OpenGLWidget::render() 
// render the object
{//	render()

	// set up timer to count for next frame
	if (nextFrameOn == 0)
	{
		timer->start(1000 * frameTime);
		nextFrameOn = 1;
	}

	//	Create matrix
	QMatrix4x4 m;

	//	Projection
	m.setToIdentity();
	
	if (width() > height()) {
		
		m.perspective(90.0f, this->width() / this->height(), 0.1f, 100.0f);
	}
	else {
		
		m.perspective(90.0f, this->height() / this->width(), 0.1f, 100.0f);
	}
	m_program->setUniformValue("projectMatrix", m);



	//	Camera
	m.setToIdentity();
	m.lookAt(camera.eyePos, camera.viewCenter, camera.up);
	m_program->setUniformValue("viewMatrix", m);


	//	Transformation
	m.setToIdentity();
	m.translate(QVector3D(0,5,0));


	//	Draw object
		//	color
	GLfloat colors1[] = {

		0.7f, 0.2f, 0.3f,
		0.7f, 0.2f, 0.3f,
		0.7f, 0.2f, 0.3f
	};


	drawObject(fallenOBJ, m, colors1 , 8);

	//	Transformation
	m.setToIdentity();


	//	Draw object
		//	color
	GLfloat colors2[] = {

		0.23f, 0.70f, 0.52f,
		0.23f, 0.70f, 0.52f,
		0.23f, 0.70f, 0.52f
	};
	
	if (floorOn == true)
		drawObject(floor , m, colors2, 10);



	m.translate(0,2,0);
	if (ballOn == true) {
		
		if (doRotate == true)
			rm.rotate(2,0,1,0);

		m = rm * m;
		drawObject(ball, m, colors2, 2);
	}
}//	render()

void OpenGLWidget::checkTime()
{//	checkTime()

	nextFrameOn = 2;

}//	checkTime()


