#include "ClothSimulator.h"


//- Constructor
ClothSimulator::ClothSimulator(QWidget *parent)
    : QMainWindow(parent)
{

//  Widgets

    //  Button
    openOBJFile->resize(200, 30);
    openOBJFile->move(50, 50);
    connect(openOBJFile, &QPushButton::clicked, this, [=]() {loadOBJFile(); chooseEdge(); createWind(); });
    
    createFloorOBJ->resize(200, 30);
    createFloorOBJ->move(50, 50);
    connect(createFloorOBJ, &QPushButton::clicked, this, &ClothSimulator::createFloor);

    createBallOBJ->resize(200, 30);
    createBallOBJ->move(50, 50);
    connect(createBallOBJ, &QPushButton::clicked, this, &ClothSimulator::createBall);
    
    clearOBJ->resize(200, 30);
    clearOBJ->move(50, 50);
    connect(clearOBJ, &QPushButton::clicked, this, &ClothSimulator::clearObjects);


    //  Check box
    QSignalMapper* signalmapper = new QSignalMapper(this);

    connect(fixedEdge1, &QCheckBox::clicked, this, &ClothSimulator::chooseEdge);
    connect(fixedEdge2, &QCheckBox::clicked, this, &ClothSimulator::chooseEdge);
    connect(fixedEdge3, &QCheckBox::clicked, this, &ClothSimulator::chooseEdge);
    connect(fixedEdge4, &QCheckBox::clicked, this, &ClothSimulator::chooseEdge);

    connect(ballRotate, &QCheckBox::clicked, this, &ClothSimulator::rotateBall);

    //  Combo box
    windType->resize(200, 30);
    windType->setEditable(true);
    windType->insertItem(0, "No Wind");
    windType->insertItem(1,"Up Wind");
    windType->insertItem(2, "Down Wind");
    windType->insertItem(3, "Left Wind");
    windType->insertItem(4, "Right Wind");
    windType->insertItem(5, "Front Wind");
    windType->insertItem(6, "Back Wind");

    connect(windType,&QComboBox::currentTextChanged, this, &ClothSimulator::createWind);

    //  Iinitiate OpenGl Widget
    QSurfaceFormat format;
    format.setSamples(16);
    openglwidget->setFormat(format);

    //  Focus policy
    this->setFocusPolicy(Qt::StrongFocus);
}


//- Slots
void ClothSimulator::loadOBJFile()
//  Slot function to read OBJ file
{// loadOBJFile()

    //  Check if the widget created
    if (openglwidget == nullptr)
        return;
    //  Get file path
    QString dir = QFileDialog::getOpenFileName(this, tr("Choose a .obj file"), QDir::currentPath());
    QByteArray ba = dir.toLatin1();
    
    //  Read data
    //  Check data type
    if (stricmp(ba.data() + strlen(ba.data()) - 4, ".obj") != 0)
    {
        qDebug() << "WARNING::\n This is not a .obj file...\n End reading...";
        return;
    }

    //  Read obj file
    openglwidget->readOBJFile(ba.data());

    
}// loadOBJFile()

void ClothSimulator::createFloor()
//  create floor object
{// createFloor()
    //  Check if the widget created
    if (openglwidget == nullptr)
        return;
    

    openglwidget->createFloor(10);

    openglwidget->floorOn = true;
    
    

}// createFloor()

void ClothSimulator::createBall()
//  create floor object
{// createFloor()
    if (openglwidget == nullptr)
        return;

    

    openglwidget->createBall();

    openglwidget->ballOn = true;
    


}// createFloor()

void ClothSimulator::clearObjects()
//  Clear all objects

{// clearObjects()

    //  check if the opengl widget is empty
    if (openglwidget == nullptr)
        return;

    // clean all the objects
    openglwidget->cleanBall();
    openglwidget->cleanFloor();
    openglwidget->cleanFallenOBJ();
    
}// clearObjects()

void ClothSimulator::chooseEdge()
//  Mark which edge is fixed
{// chooseEdge()

    if (openglwidget == nullptr)
        return;

    if (fixedEdge1->isChecked() == true)
    {
        
        openglwidget->chooseEdge(1);
    }
    if (fixedEdge2->isChecked() == true)
    {
        openglwidget->chooseEdge(2);

    }
    if (fixedEdge3->isChecked() == true)
    {
        openglwidget->chooseEdge(3);

    }
    if (fixedEdge4->isChecked() == true)
    {
        openglwidget->chooseEdge(4);

    }
    if (fixedEdge1->isChecked() == false)
    {

        openglwidget->chooseEdge(-1);
    }
    if (fixedEdge2->isChecked() == false)
    {
        openglwidget->chooseEdge(-2);

    }
    if (fixedEdge3->isChecked() == false)
    {
        openglwidget->chooseEdge(-3);

    }
    if (fixedEdge4->isChecked() == false)
    {
        openglwidget->chooseEdge(-4);

    }
}// chooseEdge()

void ClothSimulator::createWind()
//  create wind on cloth
{// createWind()

    if (openglwidget != nullptr)
        openglwidget->addWind(windType->currentIndex());

}// createWind()

void ClothSimulator::rotateBall()
//  rotate the ball

{// rotateBall()

    if (openglwidget == nullptr)
        return;
    if (ballRotate->isChecked() == true)
        openglwidget->doRotate = true;

    else
        openglwidget->doRotate = false;

}// rotateBall()

//- Override QMainWindow Event
void ClothSimulator::resizeEvent(QResizeEvent* e)
//  Resize the scale for opengl widget
{// resizeEvent()

    if (this->height() < this->width()) {
        //  opengl size
        openglwidget->resize(this->height(), this->height());

        //  Button position
        openOBJFile->move(this->height() + (this->width() - this->height()) / 4 , 50);
        createFloorOBJ->move(this->height() + (this->width() - this->height()) / 4, 80);
        createBallOBJ->move(this->height() + (this->width() - this->height()) / 4, 110);

        //  Check box
        fixedEdge1->move(this->height() + (this->width() - this->height()) / 4, 140);
        fixedEdge2->move(this->height() + (this->width() - this->height()) / 4, 170);
        fixedEdge3->move(this->height() + (this->width() - this->height()) / 4, 200);
        fixedEdge4->move(this->height() + (this->width() - this->height()) / 4, 230);
        ballRotate->move(this->height() + (this->width() - this->height()) / 4, 310);
        
        //  Combo box
        windType->move(this->height() + (this->width() - this->height()) / 4, 260);
    }
    else
    {
        //  opengl size
        openglwidget->resize(this->width(), this->width());
    
        //  Button position
        openOBJFile->move(20, this->width() + (this->height() - this->width()) / 4 );
        createFloorOBJ->move(220, this->width() + (this->height() - this->width()) / 4);
        createBallOBJ->move(420, this->width() + (this->height() - this->width()) / 4);

        //  Check box
        fixedEdge1->move(20, 30 + this->width() + (this->height() - this->width()) / 4);
        fixedEdge2->move(140, 30 + this->width() + (this->height() - this->width()) / 4);
        fixedEdge3->move(260, 30 + this->width() + (this->height() - this->width()) / 4);
        fixedEdge4->move(380, 30 + this->width() + (this->height() - this->width()) / 4);
        ballRotate->move(640, this->width() + (this->height() - this->width()) / 4);

        //Combo box
        windType->move(500, 30 + this->width() + (this->height() - this->width()) / 4);

    }
}// resizeEvent()

void ClothSimulator::wheelEvent(QWheelEvent* e) 
//  move camera far -> close or close -> far

{// wheelEvent()

    //  Check if the current openglwidget object valid
    if ( openglwidget == nullptr )
    {
        return;
    }
    //  When roll up
    if (e->delta() > 0)
    {
        //  move camera
        
        openglwidget->camera.translateCamera(1,Camera::NearFar);
    }
    //  When roll down
    else 
    {
        //  move camera
        
        openglwidget->camera.translateCamera(-1, Camera::NearFar);
    }
    
}//wheelEvent()

void ClothSimulator::mousePressEvent(QMouseEvent* e)
{// mousePressEvent()
    //  Mouse
    QMouseEvent* mouse = dynamic_cast<QMouseEvent*>(e);

    //  Mouse click
    clickPos = mouse->pos();


}// mousePressEvent()

void ClothSimulator::mouseMoveEvent(QMouseEvent* e)
{// mouseMoveEvent()

    //  Mouse
    QMouseEvent* mouse = dynamic_cast<QMouseEvent*>(e);

    //  check if openglwidget created successfully
    if (openglwidget == nullptr)
    {
        return;
    }

    QPoint mouseMoveDirect = mouse->pos() - clickPos;

    clickPos = mouse->pos();

    //  choose rotate angle
    if (mouseMoveDirect.y() > 1)
    {
        openglwidget->camera.rotateAroundPitch(0.5);

    }
    else if (mouseMoveDirect.y() < -1)
    {
        openglwidget->camera.rotateAroundPitch(-0.5);

    }
    else if (mouseMoveDirect.x() > 1) 
    {
        openglwidget->camera.rotateAroundYaw(0.5);
        
    }
    else if(mouseMoveDirect.x() < -1)
    {
        openglwidget->camera.rotateAroundYaw(-0.5);
        
    }

    

}// mouseMoveEvent()

void ClothSimulator::mouseReleaseEvent(QMouseEvent* e)
{

    clickPos = QPoint(0,0);

}

void ClothSimulator::keyPressEvent(QKeyEvent* e)
{

    if (e->key() == Qt::Key_Up)
    {
        qDebug() << "up";
        openglwidget->camera.translateCamera(0.5, Camera::UpDown);

    }
    else if (e->key() == Qt::Key_Down)
    {
        qDebug() << "down";
        openglwidget->camera.translateCamera(-0.5, Camera::UpDown);

    }
    else if (e->key() == Qt::Key_Right)
    {
        qDebug() << "up";
        openglwidget->camera.translateCamera(0.5, Camera::LeftRight);

    }
    else if (e->key() == Qt::Key_Left)
    {
        qDebug() << "down";
        openglwidget->camera.translateCamera(-0.5, Camera::LeftRight);

    }

}


