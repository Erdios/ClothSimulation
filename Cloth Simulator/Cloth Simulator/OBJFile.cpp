#include "OBJFile.h"

//- Constructor
OBJFile::OBJFile()
{

    //  Initial the attributes of object
    
    scale = 1;
    maxDis = 0;
    mode = No_Normal_Texture;
    modelMatrix = QMatrix4x4();
    modelMatrix.setToIdentity();
    gridSize = 0;

    corners[0] = 0;
    corners[1] = 0;
    corners[2] = 0;
    corners[3] = 0;

    leftEdgeFixed = false;
    rightEdgeFixed = false;
    upperEdgeFixed = false;
    lowerEdgeFixed = false;
    

    windType = 0;
}

OBJFile::OBJFile(const char* fileName, bool _phyEff)
{

    //  Initial the attributes of object

    
    scale = 1;
    maxDis = 0;
    mode = No_Normal_Texture;
    modelMatrix = QMatrix4x4();
    modelMatrix.setToIdentity();
    gridSize = 0;

    corners[0] = 0;
    corners[1] = 0;
    corners[2] = 0;
    corners[3] = 0;

    leftEdgeFixed = false;
    rightEdgeFixed = false;
    upperEdgeFixed = false;
    lowerEdgeFixed = false;

    windType = 0;

    //  Load object
    ReadOBJ(fileName, _phyEff);

}



//- Functions

//  fill new object with object data
void OBJFile::ReadOBJ(const char* fileName, bool _physicsEff)
// read an object file
{// ReadOBJ()

    //  Open file
    std::fstream OBJFile;
    OBJFile.open(fileName, std::ios::in);

    //  Look through the file data
    std::string line;
    
    //  Variables for reading object data 
    std::size_t sz;
        
        //  for reading line start with 'v'
    QVector3D temp3D;
    std::deque<QVector3D>* verQueue;
        
        //  for reading line start with 'f'
    std::deque<unsigned int> faceid;


    //  look through the OBJFile
    while (!OBJFile.eof())
    {
        getline(OBJFile, line);
        //  Check content
        if (line.c_str()[0] == 'v') {

            
            if (line.c_str()[1] == 'n')
                //  check if it is "vn"
            {
                //  remove 'vn ' from line
                line = line.substr(3);

                //  select the normals queue
                verQueue = &verNormals;
            }
            else if (line.c_str()[1] == 't')
                //  check if it is "vt"
            {
                //  remove 'vt ' from line
                line = line.substr(3);

                //  select the textures queue
                verQueue = &verTextures;
            }
            else
                //  check if it is "v"
            {
                //  remove 'v ' from line
                line = line.substr(2);

                //  select the vertices queue
                verQueue = &vertices;


                //  PHYSICS ATTRIBUTES
                if (_physicsEff == true) {
                    //  create speed variable for this vertex
                    velocity.push_back(QVector3D(0.f, 0.f, 0.f));

                    //  create force variable for this vertex
                    forces.push_back(QVector3D(0.f, 0.f, 0.f));

                    //  updated data
                    updated.push_back(false);

                    //  create mass variable for this vertex
                    mass.push_back(1.f);
                }
            }


            //  get x
            temp3D.setX(std::stof(line, &sz));
            line = line.substr(sz);

            //  get y
            temp3D.setY(std::stof(line, &sz));
            line = line.substr(sz);

            //  get z
            temp3D.setZ(std::stof(line, &sz));
            line = line.substr(sz);

            //  add new 3d data to corresponding queue
            verQueue->push_back(temp3D);

            
        }
        else if (line.c_str()[0] == 'f') {

            //  Remove the first 2 char in the string
            line = line.substr(2);

            //  Store the length of the current string line
            int length = line.length();

            //  Location of previous divider '/' and ' '
            int preDivider = 0;

            //  Clean the queue for storing data from a line for face data
            faceid.clear();

            //  Read face data from a line
            for (int i = 0; i < length; i++)
            {
                if (line.c_str()[i] == '/' || line.c_str()[i] == ' ')
                {
                    faceid.push_back(stoi(line.substr(preDivider, i)));

                    preDivider = i + 1;
                    
                }

                else if (i == length - 1)
                {

                    faceid.push_back(stoi(line.substr(preDivider)));
                    
                }

            }

            unsigned int dataType = faceid.size() / 3;

            //  add new face to queue
            faceVerID.push_back(Data3D(faceid[0 * dataType], faceid[1 * dataType], faceid[2 * dataType]));
            

            if(dataType > 1)
                faceTexID.push_back(Data3D(faceid[0 * dataType + 1], faceid[1 * dataType + 1], faceid[2 * dataType + 1]));
            
            if (dataType > 2)
                faceVerNorID.push_back(Data3D(faceid[0 * dataType + 2], faceid[1 * dataType + 2], faceid[2 * dataType + 2]));
            


            //  add new spring
            if (_physicsEff == false)
                continue;

            int check1 = 1, check2 = 1, check3 = 1;

            for (int count = 0; count < springs.size(); count++)
            {
                if (faceid[0 * dataType] == springs[count].end2 && faceid[1 * dataType] == springs[count].end1)
                {

                    check1 = 0;
                }
                else if (faceid[1 * dataType] == springs[count].end2 && faceid[2 * dataType] == springs[count].end1)
                {
                    check2 = 0;

                }
                else if (faceid[2 * dataType] == springs[count].end2 && faceid[0 * dataType] == springs[count].end1)
                {
                    check3 = 0;

                }


            }

            
            if (check1 == 1)
            {
                springs.push_back(Spring(faceid[0 * dataType], faceid[1 * dataType]));
            }

            if (check2 == 1)
            {

                springs.push_back(Spring(faceid[1 * dataType], faceid[2 * dataType]));
            }
            if (check3 == 1)
            {

                springs.push_back(Spring(faceid[2 * dataType], faceid[0 * dataType]));
            }
            
        }


    }

    //  Check the mode of this OBJFile
    if(faceVerNorID.size() > 0)
    {
        
        if (mode == Has_Texture)
            mode = Has_Normal_Texture;
        else
            mode = Has_Normal;

    }
    else if (faceTexID.size() > 0)
    {

        if (mode == Has_Normal)
            mode = Has_Normal_Texture;
        else
            mode = Has_Texture;

    }

    //  find the middle points
    for (QVector3D v : vertices)
    {
        midPoint += v;

    }
    if(vertices.size() > 0)
        midPoint /= vertices.size();

    
    //  Find the furthest distance from the mid point to the vertex on object
    for (unsigned int count =0 ; count< vertices.size();count++ )
    {
        float distance = (vertices[count] - midPoint).length();
        if (maxDis < distance)
        {
            maxDis = distance;

        }
    }

    for (unsigned int count = 0; count < vertices.size(); count++)
    {
        float distance = (vertices[count] - midPoint).length();
        if (maxDis == distance)
        {
            if (vertices[count].x() < 0 && vertices[count].z()>0)
            {
                corners[0] = count+1;
            }
            else if (vertices[count].x() > 0 && vertices[count].z() > 0)
            {
                corners[1] = count + 1;
            }
            else if (vertices[count].x() < 0 && vertices[count].z() < 0)
            {
                corners[2] = count + 1;
            }
            else if (vertices[count].x() > 0 && vertices[count].z() < 0)
            {
                corners[3] = count + 1;
            }

        }
    }
    

    //  Recalculate the scale
    if (maxDis > 0)
    {
        scale /= maxDis;
        
    }



    //  Store the rest length of every edge
    for (int i = 0; i < springs.size(); i++)
    {

        restLen.push_back((vertices[springs[i].end1 - 1] - vertices[springs[i].end2 -1]).length());

    }



    // close file
    OBJFile.close();

}// ReadOBJ()

void OBJFile::CreateFloor(int _size)
//  Generate floor object

{// CreateFloor()

    //  store the grid size of the floor
    gridSize = _size * 5;
    
    //  Store vertices
    for (int i = 0; i < _size; i+= 1)
    {//  Per row
        
        for (int j = 0; j < _size; j+= 1) 
        {// Per col

            //  Add a vertex into vertices queue
            vertices.push_back(QVector3D(i*5, 0, j*5));

            //  Add a vertex id to face vertices id queue
            if (i != _size - 1 && j != _size - 1)
            //  Stop when reach the edges
            {
                //  current vertex index in vertices queue: i*_size + j + 1 (because start from 1)
                //  CCW
                faceVerID.push_back(Data3D(i * _size + j + 1, (i + 1) * _size + j + 1, (i + 1) * _size + j + 2));
                faceVerID.push_back(Data3D(i * _size + j + 1, (i + 1) * _size + j + 2, i * _size + j + 2));
                
            }


        }// Per col

    }//  Per row

    //  find the middle points
    for (QVector3D v : vertices)
    {
        midPoint += v;

    }
    if (vertices.size() > 0)
        midPoint /= vertices.size();



    //  Find the furthest distance from the mid point to the vertex on object
    for (QVector3D v : vertices)
    {
        float distance = (v - midPoint).length();
        if (maxDis < distance)
            maxDis = distance;
    }


    //  Recalculate the scale
    if (maxDis > 0)
        scale /= maxDis;

}// CreateFloor()

//  clean object
void OBJFile::Clean()
//  Clean all the queue in this object
{// Clean()

    //	coordinates
    if(!vertices.empty())
        vertices.clear();
    
    if (!verNormals.empty())
        verNormals.clear();
    
    if (!verTextures.empty())
        verTextures.clear();

    //	index here are start from 1
    if(!faceVerID.empty())
        faceVerID.clear();

    if(!faceVerNorID.empty())
        faceVerNorID.clear();
    
    if(!faceTexID.empty())
        faceTexID.clear();

    //	other attributes of object
    midPoint = QVector3D(0.f,0.f,0.f);
    scale = 1;
    maxDis = 0;
    modelMatrix.setToIdentity();

    //	obj file mode
    mode = No_Normal_Texture;

    //	store grid size, if the object is a floor
    gridSize = 0;

    //	physics (only for fallen object)
    if(!velocity.empty())
        velocity.clear();

    if(!forces.empty())
        forces.clear();

    if(!mass.empty())
        mass.clear();
    
    if(!restLen.empty())
        restLen.clear();
    
    if(!springs.empty())
        springs.clear();

    //	other attribute for vertex
    if(!updated.empty())
        updated.clear();


}// Clean()

//  check if object is empty or not
bool OBJFile::IsEmpty()
//  Check if this object is empty
{// Empty()

    //  Check if there is any normal
    if ((mode == Has_Normal || mode == Has_Normal_Texture) && (verNormals.empty() || faceVerNorID.empty()))
    {
        return true;
    }
    

    //  Check if there is any texture
    if ((mode == Has_Texture || mode == Has_Normal_Texture) && (verTextures.empty() || faceTexID.empty()))
    {
        return true;
    }

    //  Check if there is any vertex
    if (vertices.empty()  || faceVerID.empty()  )
    {
        return true;

    }
    else
    {
        return false;
    }

}// Empty()

//  update scale of the object
void OBJFile::ChangeScale(float _scale) 
//  Resize the size of the object
{   //  resizeObject()
    
    //  Check if maxDis is 0;
    if (maxDis < 0)
        return;

    
    //  Recalculate new scale after receiving user input scale
    scale = _scale/maxDis;

    
    if (scale <= 0)
    {
        scale = 1 / maxDis;
        
    }
    
}   //  resizeObject()


//  update normal
void OBJFile::updateNor()
{// updateNor()

    QVector3D faceNormal;



    for (int i = 0; i < verNormals.size(); i++)
        verNormals[i] = QVector3D(0, 0, 0);


    //  extend normal queue if it is not including normals for every vertex
    if (verNormals.size() < vertices.size())
    {

        for (int i = verNormals.size()-1;i<vertices.size();i++)
        {
            verNormals.push_back(QVector3D(0, 0, 0));
        }

        for (int i = 0; i < faceVerNorID.size(); i++)
        {
            faceVerNorID[i].value[0] = faceVerID[i].value[0];
            faceVerNorID[i].value[1] = faceVerID[i].value[1];
            faceVerNorID[i].value[2] = faceVerID[i].value[2];

        }

    }

    for (int i = 0; i < faceVerID.size(); i++)
    {// per face

        //  calculate normal for every face
        QVector3D edgeDir1 = vertices[faceVerID[i].value[1] - 1] - vertices[faceVerID[i].value[0] - 1];
        QVector3D edgeDir2 = vertices[faceVerID[i].value[2] - 1] - vertices[faceVerID[i].value[1] - 1];

        faceNormal = QVector3D::crossProduct(edgeDir1,edgeDir2);


        //  add the normal to the normals of vertices on this face
        verNormals[faceVerID[i].value[0] - 1] += faceNormal;
        verNormals[faceVerID[i].value[1] - 1] += faceNormal;
        verNormals[faceVerID[i].value[2] - 1] += faceNormal;

    }// per face

    for (int i = 0; i < verNormals.size(); i++)
        verNormals[i] = verNormals[i].normalized();

}// updateNor()

//  Physics effect
void OBJFile::GetForce(std::shared_ptr<OBJFile> floor, std::shared_ptr<OBJFile> ball, bool doRotate)
//  Calculate all force for each vertex
{// GetForce()

    
    //  Initialize the force with gravity
    for (int i = 0; i < forces.size(); i++)
    {
        forces[i] = QVector3D(0, -9.8f, 0);

    }

    //  Apply spring force
    GetSpring();

    //  Add wind on the cloth
    AddWind();
    
    //  Bounce force
    if (floor != nullptr && !floor->IsEmpty())
        FloorBounce(floor);
    if (ball != nullptr && !ball->IsEmpty())
        BallBounce(ball, doRotate);

    //  Drag cloth
    DragCloth();

}// GetForce()

void OBJFile::GetSpring()
//  calculate spring force for the end point of every spring (edge)

{// Spring()

    
    
    for (int i = 0; i < springs.size(); i++)
    {// every face

        //  variables for spring model
        float ks = 12000.f, kd = 200.f;

        QVector3D force;
        QVector3D springDir;
        float springLen;


        //  get current spring direction and length
        springDir = vertices[springs[i].end2 - 1]  - vertices[springs[i].end1 - 1];
        springLen = springDir.length();

        //  calculate spring force
        force = ks * (springLen - restLen[i]) * (springDir / springLen);

        //  calculate damping force
        force += kd * QVector3D::dotProduct((velocity[springs[i].end2 - 1] - velocity[springs[i].end1 - 1]), (springDir / springLen)) * (springDir / springLen);
        
        
        forces[springs[i].end1-1] += force;
        forces[springs[i].end2-1] += -force;

    }// every face
}// Spring()

void OBJFile::FloorBounce(std::shared_ptr<OBJFile> obstacle)
//  calculate force for bouncing on the floor

{// FloorBounce()
    
    //  check if there is an obstacle
    if (obstacle == nullptr)
        return;
   
    //  create temporary vertex variable for obstacle
    QVector3D v1 = obstacle->scale * obstacle->vertices[0];
    v1 = obstacle->modelMatrix * v1;

    
    for (int i = 0; i < velocity.size(); i++)
    {
        //  create temporary vertex variable for fallen object
        QVector3D v2 = scale * vertices[i];
        v2 = modelMatrix * v2;

        //  check if the vertex is not on or under the floor
        if (v1.y() + 0.1 < v2.y() || v1.y() - 0.1  > v2.y())
        {
            continue;
        }


        //  the floor is on the origin on the x-z plane

        //  check if the vertex is within the square of the floor
        if (-obstacle->gridSize * obstacle->scale * 0.5 - 0.01 > v2.x() || obstacle->gridSize * obstacle->scale * 0.5 + 0.01 < v2.x())
        {
            continue;

        }

        if (-obstacle->gridSize * obstacle->scale * 0.5 - 0.01 > v2.z() || obstacle->gridSize * obstacle->scale * 0.5 + 0.01 < v2.z())
        {
            continue;

        }


        //  if the vertex is on the eadg of the floor
        float right = obstacle->gridSize * obstacle->scale * 0.5;
        float left = -obstacle->gridSize * obstacle->scale * 0.5;
        float up = obstacle->gridSize * obstacle->scale * 0.5;
        float down = -obstacle->gridSize * obstacle->scale * 0.5;

        if (left - 0.01 < v2.x() && left + 0.01 >= v2.x())
        {

            //  get the force direction of the force for vertex on the edge
            QVector3D dir = QVector3D(-1, 0, 0);

            //  get the magnitude for the force
            forces[i] += -forces[i].normalized() * abs(QVector3D::dotProduct(forces[i].normalized(), dir)) * abs(QVector3D::dotProduct(forces[i], dir));
            velocity[i] += -velocity[i].normalized() * abs(QVector3D::dotProduct(velocity[i].normalized(), dir)) * abs(QVector3D::dotProduct(velocity[i], dir));

        }

        if (right - 0.01 <= v2.x() && right + 0.01 > v2.x())
        {

            //  get the force direction of the force for vertex on the edge
            QVector3D dir = QVector3D(1, 0, 0);


            //  get the magnitude for the force
            forces[i] += -forces[i].normalized() * abs(QVector3D::dotProduct(forces[i].normalized(), dir)) * abs(QVector3D::dotProduct(forces[i], dir));
            velocity[i] += -velocity[i].normalized() * abs(QVector3D::dotProduct(velocity[i].normalized(), dir)) * abs(QVector3D::dotProduct(velocity[i], dir));
        }

        if ( up - 0.01 <= v2.z() && up + 0.01 > v2.z())// || down - 0.1 < v2.z() && down + 0.1 > v2.z()))
        {

            //  get the force direction of the force for vertex on the edge
            QVector3D dir = QVector3D(0, 0, 1);


            //  get the magnitude for the force
            forces[i] += -forces[i].normalized() * abs(QVector3D::dotProduct(forces[i].normalized(), dir)) * abs(QVector3D::dotProduct(forces[i], dir));
            velocity[i] += -velocity[i].normalized() * abs(QVector3D::dotProduct(velocity[i].normalized(), dir)) * abs(QVector3D::dotProduct(velocity[i], dir));
        }


        if (down - 0.01 < v2.z() && down + 0.01 >= v2.z())
        {

            //  get the force direction of the force for vertex on the edge
            QVector3D dir = QVector3D(0, 0, -1);


            //  get the magnitude for the force
            forces[i] += -forces[i].normalized() * abs(QVector3D::dotProduct(forces[i].normalized(), dir)) * abs(QVector3D::dotProduct(forces[i], dir));
            velocity[i] += -velocity[i].normalized()* abs(QVector3D::dotProduct(velocity[i].normalized(), dir)) * abs(QVector3D::dotProduct(velocity[i], dir));
        }

        //  change force
        //  get the force direction of the force for vertex on the edge
        
        QVector3D dir(0,0,0);
        
        if(v1.y() + 0.1 >= v2.y() && v1.y() <= v2.y())
            dir = QVector3D(0, 1, 0);
        else if(v1.y() - 0.1 <= v2.y() && v1.y() > v2.y())
            dir = QVector3D(0, -1, 0);

        

        //  get the magnitude for the force
        forces[i] += dir * abs(QVector3D::dotProduct(forces[i], dir));
        

        //forces[i].setY(0);
        velocity[i].setY(0);


        //  friction on the floor
        QVector3D friction = - 10* velocity[i];
        
        forces[i] += friction;
    }


}// FloorBounce()

void OBJFile::BallBounce(std::shared_ptr<OBJFile> obstacle, bool doRotate)
//  calculate the bounce force from the ball

{// BallBounce()
    for (int i = 0; i < velocity.size(); i++)
    {

        //  create temporary vertex variable for fallen object
        QVector3D clothVer = scale * vertices[i];
        clothVer = modelMatrix * clothVer;

        //  get direction and distance from circle center to cloth vertex
        QVector3D dir = obstacle->scale * obstacle->midPoint;
        dir = obstacle->modelMatrix * dir;
        dir = clothVer - dir;
        
        float distance = dir.length();
        dir = dir.normalized();

        //  if the ball rotate, there will be extra friction
        QVector3D friction(0,0,0);

        if(doRotate)
            friction = -50 * QVector3D::crossProduct(dir, QVector3D(0, 1, 0));

        if (distance < obstacle->scale +0.15) 
        {
            forces[i] += dir * abs(QVector3D::dotProduct(dir, forces[i]));
            forces[i] += friction;
            velocity[i]+= dir * abs(QVector3D::dotProduct(dir, velocity[i]));
            
        }

    }
}// BallBounce()

void OBJFile::GetSpeed()
//  Calculate speed

{// GetSpeed()
    
    //  time step
    float dt = 0.001f;


    for (int i = 0; i < velocity.size(); i++)
    {// Per vertex

        velocity[i] += (dt* forces[i] /mass[i]);
        vertices[i] = vertices[i] + velocity[i] * dt;
    }// Per vertex

    updateNor();

}// GetSpeed()

void OBJFile::DragCloth()
//  drag a corner of the cloth

{// DragCloth()
    if (forces.size() <= 0 || velocity.size()<=0)
        return;
    //  nail the selected edge by making their velocity and force to be zero
    
    if (leftEdgeFixed)
    {

        velocity[corners[0] - 1] = QVector3D(0, 0, 0);
        forces[corners[0] - 1] = QVector3D(0, 0, 0);

        velocity[corners[2] - 1] = QVector3D(0, 0, 0);
        forces[corners[2] - 1] = QVector3D(0, 0, 0);
    }
        

    if (rightEdgeFixed)
    {

        velocity[corners[1] - 1] = QVector3D(0, 0, 0);
        forces[corners[1] - 1] = QVector3D(0, 0, 0);

        velocity[corners[3] - 1] = QVector3D(0, 0, 0);
        forces[corners[3] - 1] = QVector3D(0, 0, 0);
    }
        
    
    if (upperEdgeFixed)
    {

        velocity[corners[0] - 1] = QVector3D(0, 0, 0);
        forces[corners[0] - 1] = QVector3D(0, 0, 0);

        velocity[corners[1] - 1] = QVector3D(0, 0, 0);
        forces[corners[1] - 1] = QVector3D(0, 0, 0);

    }
    
    if (lowerEdgeFixed)
    {
        
        velocity[corners[2]-1] = QVector3D(0, 0, 0);
        forces[corners[2]-1] = QVector3D(0, 0, 0);

        velocity[corners[3]-1] = QVector3D(0, 0, 0);
        forces[corners[3]-1] = QVector3D(0, 0, 0);


    }


    
}// DragCloth()

void OBJFile::AddWind()
//  add wind on the cloth
{

    switch (windType)
    {
    case 1:
        for (int i = 0; i<forces.size();i++) 
            forces[i] += QVector3D(0, 20, 0);
            
        break;
    case 2:
        for (int i = 0; i < forces.size(); i++)
            forces[i] += QVector3D(0, -20, 0);
        break;
    case 3:
        for (int i = 0; i < forces.size(); i++)
            forces[i] += QVector3D(-20, 0, 0);
        break;

    case 4:
        for (int i = 0; i < forces.size(); i++)
            forces[i] += QVector3D(20, 0, 0);
        break;
    case 5:
        for (int i = 0; i < forces.size(); i++)
            forces[i] += QVector3D(0, 0, -20);
        break;
    case 6:
        for (int i = 0; i < forces.size(); i++)
            forces[i] += QVector3D(0, 0, 20);
        break;

    }
    

}