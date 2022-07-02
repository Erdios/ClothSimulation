#pragma once

// Qt header
#include <QtWidgets/QMainWindow>
#include "ui_ClothSimulator.h"
#include <QPushButton>
#include <QFileDialog>
#include <QDebug>
#include <QWheelEvent>
#include <QCheckBox>
#include <QSignalMapper>
#include <QComboBox>

// My header
#include "OBJFile.h"
#include "OpenGLWidget.h"


class ClothSimulator : public QMainWindow
{
    Q_OBJECT

public:
//- Constructor
    ClothSimulator(QWidget *parent = Q_NULLPTR);

//- Parameter
    
//  Widgets

    //  Open gl widget
    OpenGLWidget* openglwidget = new OpenGLWidget(this);

    //  Button
    QPushButton* openOBJFile =new QPushButton("Open OBJ File" , this);
    QPushButton* createFloorOBJ = new QPushButton("Create Floor", this);
    QPushButton* createBallOBJ = new QPushButton("Create Ball", this);
    QPushButton* clearOBJ = new QPushButton("Clear Objects", this);

    // Check box
    QCheckBox* fixedEdge1 = new QCheckBox("Left Edge", this);
    QCheckBox* fixedEdge2 = new QCheckBox("Right Edge", this);
    QCheckBox* fixedEdge3 = new QCheckBox("Upper Edge", this);
    QCheckBox* fixedEdge4 = new QCheckBox("Lower Edge", this);
    QCheckBox* ballRotate = new QCheckBox("Rotate Ball", this);
    
    
    //  combo box
    QComboBox* windType = new QComboBox(this);


protected:
//- Variables
    QPoint clickPos;

//- Functions override
    void resizeEvent(QResizeEvent* e) override;
    void wheelEvent(QWheelEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void keyPressEvent(QKeyEvent* e) override;

private slots:
    void loadOBJFile();
    void createFloor();
    void createBall();
    void clearObjects();
    void chooseEdge();
    void createWind();
    void rotateBall();
};
