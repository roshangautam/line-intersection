//
//  main.cpp
//  LineIntersection
//
//  Created by Roshan Gautam on 6/1/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
#include "intersection.h"
#include "3dintersection.h"

using namespace std;

void loop();
void findAndPrintPatch(Vector3,Vector3,Vector3);
void printPatchOnSameSide(ThreeDIntersection, ThreeDIntersection, ThreeDIntersection);
int findPointToBeProjected(ThreeDIntersection[]);
HemiCubeFace findProjectionFace(ThreeDIntersection, ThreeDIntersection, ThreeDIntersection);
Vector3 findCommonVertex(HemiCubeFace[]);
void generatePolygons(double, double, double, string);
void generateHemicubePatchCenters(int n);
int main(int argc, const char * argv[]) {
    cout << "Radiosity Engine - Solid Modeling CS6413 !!! \n";
    cout << "Assumptions for 2D:\n";
    cout << "1. Light Source Origin is always (0,0)\n";
    cout << "2. Height of hemi-square = 1 i.e y = 1 \n";
    cout << "3. Left End of hemi-square from origin = -1 i.e x = -1 \n";
    cout << "4. Right End of hemi-square from origin = 1 i.e x = 1 \n";
    loop();
//    for (int i = 0 ; i < 5; i++) {
//        cout << "i:" << i << ":";
//        Vector3::findPointOnALine(Vector3(1,2.5,3.5),Vector3(1,0.8,0.4), i).print();
////        Vector3::findPointOnALine(Vector3(2.5,2,1),Vector3(0.285714,0.714286,1), i).print();
////        Vector3::findPointOnALine(Vector3( 0.777777,1,1),Vector3(0.285714,0.714286,1), i).print();
//        cout << "\n";
//    }
    return 0;
}

void loop() {
    Point point, secondPoint;
    Intersection intersection, secondIntersection;
    Vector3 vertex, vertex1, vertex2;
    ThreeDIntersection threeDIntersection;
    double length = 0.0, width = 0.0, height = 0.0, boxLength = 0.0, boxWidth = 0.0, boxHeight = 0.0;
    char c = '1';
    while(1) {
        cout << "\nOptions:\n";
        cout << "(1)Enter Coordinates for a 2D Point and Hemicube intersection\n";
        cout << "(2)Enter Coordinates for a 2D line segment and Hemicube intersection\n";
        cout << "(3)Enter Coordinates for a Vertex (3D) and Hemicube intersection\n";
        cout << "(4)Enter Coordinates to calculate shadow (patch) of a triangle (3D) on Hemicube surface\n";
        cout << "(5)Generate environment patch vertices\n";
        cout << "(6)Hemicube cells within patch solid angle\n";
        cout << "(Q)Quit\n";
        cout << "[Select]:";
        cin >> c;
        if(cin.get() != '\n') { // Look at the next character
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input stream
            cout << "Invalid Option\n";
        } else {
            switch (c) {
                case '1':
                {
                    cout << "Enter Coordinates of the point!\n";
                    point.read();
                    if (point.getY() > 0 && (point.getX() > 1 || point.getX() < -1) ) {
                        intersection.intersect(&point);
                        cout << "\nCoordinates of intersection are:";
                        intersection.simplePrint();
                    } else {
                        cout << "\nInvalid Coordinates provided.\nx must be greater than 1 and less than -1.\ny must be greater than 1.\nPlease try again.\n";
                    }
                }
                    break;
                case '2':
                {
                    cout << "Enter Coordinates of first point!\n";
                    point.read();
                    cout << "Enter Coordinates of second point!\n";
                    secondPoint.read();
                    intersection.intersect(&point);
                    secondIntersection.intersect(&secondPoint);
                    if(intersection.getSide() == secondIntersection.getSide()) {
                        cout << "\nCoordinates of intersecting segment are:";
                        intersection.getPoint().print();
                        cout << ",";
                        secondIntersection.getPoint().print();
                        cout << " on " << intersection.getHumanReadableSide() << "\n";
                    } else {
                        cout << "\nCoordinates of intersecting segment are:";
                        intersection.print();
                        if(intersection.getSide() == 0) {
                            cout << " AND (-1,1),(1,1) on Top AND ";
                        } else if(intersection.getSide() == 1) {
                            cout << " AND (1,1),(-1,1) on Top AND ";
                        } else {
                            cout << " AND ";
                        }
                        secondIntersection.print();
                    }
                }
                    break;
                case '3':
                {
                    cout << "Enter Coordinates of first vertex:";
                    vertex.read();
                    threeDIntersection.intersect(&vertex);
                }
                    break;
                case '4':
                {
                    cout << "Enter Coordinates of first vertex:";
                    vertex.read();
                    cout << "Enter Coordinates of second vertex:";
                    vertex1.read();
                    cout << "Enter Coordinates of third vertex:";
                    vertex2.read();
                    findAndPrintPatch(vertex, vertex1, vertex2);
                }
                    break;
                case '5':
                {
                    cout << "Enter the height of the room:";
                    cin >> height;
                    cout << "Enter the width of the room:";
                    cin >> width;
                    cout << "Enter the length of the room:";
                    cin >> length;
                    cout << "Enter the height of the box:";
                    cin >> boxHeight;
                    cout << "Enter the width of the box:";
                    cin >> boxWidth;
                    cout << "Enter the length of the box:";
                    cin >> boxLength;
                    generatePolygons(height, width, length, "room.obj");
                    generatePolygons(boxHeight, boxWidth, boxLength, "box.obj");
                }
                    break;
                case '6':
                {
                    generateHemicubePatchCenters(10);
                }
                    break;
                case 'q':
                case 'Q':
                    cout << "\n--- End of Program ---\n";
                    exit(1);
                default:
                    cout << "\nInvalid option\n";
                    break;
            }
        }
    }
}

void generatePolygons(double width, double height, double length, string filename) {
    // WE ASSUME THE BOTTOM LEFT CORNER OF THE ROOM IS (0,0,0)
    ofstream objectFile;
    objectFile.open (filename);
    double factor = (4 * (( width * height) + (length * height) + (width * length))) / POLY_COUNT;
    double DELTA = floor(sqrt(factor) * 100) / 100;
    if (objectFile) {
        cout << "File Generated. Now generating polygons\n";
        // FRONT AND BACK
        int i = 0; //polycount
        for (float z = 0; z <= length; z += length) {
            for (float y = 0; y < height; y += DELTA) {
                for (float x =0; x < width; x+= DELTA) {
                    objectFile << x << " " << y << " " << z << "\n";
                    objectFile << x + DELTA << " " << y << " " << z << "\n";
                    objectFile << x << " " << y + DELTA << " " << z << "\n";
                    i++;
                    objectFile << x + DELTA << " " << y + DELTA << " " << z << "\n";
                    objectFile << x << " " << y + DELTA << " " << z << "\n";
                    objectFile << x + DELTA << " " << y << " " << z << "\n";
                    i++;
                }
            }
        }
        // LEFT AND RIGHT
        for (float x = 0; x <= width; x += width) {
            for (float y = 0; y < height; y += DELTA) {
                for (float z =0; z < length; z+= DELTA) {
                    objectFile << x << " " << y << " " << z << "\n";
                    objectFile << x << " " << y + DELTA << " " << z << "\n";
                    objectFile << x << " " << y << " " << z + DELTA << "\n";
                    i++;
                    objectFile << x << " " << y + DELTA << " " << z + DELTA << "\n";
                    objectFile << x << " " << y << " " << z + DELTA << "\n";
                    objectFile << x << " " << y + DELTA << " " << z << "\n";
                    i++;
                }
            }
        }
        // TOP AND BOTTTOM
        for (float y = 0; y <= height; y += height) {
            for (float x = 0; x < width; x += DELTA) {
                for (float z =0; z < length; z+= DELTA) {
                    objectFile << x << " " << y << " " << z << "\n";
                    objectFile << x << " " << y << " " << z + DELTA << "\n";
                    objectFile << x + DELTA  << " " << y << " " << z << "\n";
                    i++;
                    objectFile << x + DELTA << " " << y << " " << z + DELTA << "\n";
                    objectFile << x + DELTA << " " << y << " " << z << "\n";
                    objectFile << x << " " << y << " " << z + DELTA << "\n";
                    i++;
                }
            }
        }
        cout << "Polygon count:" << i << " for " << filename << "\n";
        objectFile.close();
        cout << "Done Generating Polygons\n";
    } else {
        cout << "Error Generating object file. Try again\n";
    }

}

void generateHemicubePatchCenters(int n) {
    Point3 top_buffer[10][10];
    float delta;
    delta = 2 / (float)n;
    float x, y, z;
    for ( int i = 0 ; i < HemiFaceNum; i++) { // for five faces of hemicube
        switch (i) {
            case TOP_FACE:
            {
                x = -1;
                y = 1;
                z = -1;
                cout << "TOP FACE\n";
                for (int j = 0; j < n; j++) {
                    for (int k=0; k < n; k++) {
                        top_buffer[j][k] = Point3(((x+delta) + x) / 2, y, ((z+delta) + z)/2);
                        cout << "Cell[" << j << "][" << k << "]:";
                        top_buffer[j][k].print();
                        cout << "\n";
                        x += delta;
                    }
                    x = -1;
                    z += delta;
                }
            }
                break;
            case FRONT_FACE:
            {
                x = -1;
                y = 1;
                z = 1;
                cout << "\nFRONT FACE\n";
                for (int j = 0; j < n/2; j++) {
                    for (int k= 0; k < n; k++) {
                        top_buffer[j][k] = Point3(((x+delta) + x) / 2, ((y-delta) + y) / 2, z);
                        cout << "Cell[" << j << "][" << k << "]:";
                        top_buffer[j][k].print();
                        cout << "\n";
                        x += delta;
                    }
                    x = -1;
                    y -= delta;
                }
            }
                break;
            case BACK_FACE:
                x = -1;
                y = 1;
                z = -1;
                cout << "\nBACK FACE\n";
                for (int j = 0; j < n/2; j++) {
                    for (int k= 0; k < n; k++) {
                        top_buffer[j][k] = Point3(((x+delta) + x) / 2, ((y-delta) + y) / 2, z);
                        cout << "Cell[" << j << "][" << k << "]:";
                        top_buffer[j][k].print();
                        cout << "\n";
                        x += delta;
                    }
                    x = -1;
                    y -= delta;
                }
                break;
            case LEFT_FACE:
                x = -1;
                y = 1;
                z = 1;
                cout << "\nLEFT FACE\n";
                for (int j = 0; j < n/2; j++) {
                    for (int k= 0; k < n; k++) {
                        top_buffer[j][k] = Point3(x, ((y-delta) + y) / 2, ((z - delta) + z) / 2);
                        cout << "Cell[" << j << "][" << k << "]:";
                        top_buffer[j][k].print();
                        cout << "\n";
                        z -= delta;
                    }
                    z = 1;
                    y -= delta;
                }
                break;
            case RIGHT_FACE:
                x = 1;
                y = 1;
                z = 1;
                cout << "\nRIGHT FACE\n";
                for (int j = 0; j < n/2; j++) {
                    for (int k= 0; k < n; k++) {
                        top_buffer[j][k] = Point3(x, ((y-delta) + y) / 2, ((z - delta) + z) / 2);
                        cout << "Cell[" << j << "][" << k << "]:";
                        top_buffer[j][k].print();
                        cout << "\n";
                        z -= delta;
                    }
                    z = 1;
                    y -= delta;
                }
                break;
            default:
                break;
        }
    }
}

void findAndPrintPatch(Vector3 point1, Vector3 point2, Vector3 point3) {
    ThreeDIntersection patchPoint1, patchPoint2, patchPoint3;
    Vector3 givenPoints[3] = {point1, point2, point3 };
    Vector3 projectedPoint;
    Vector3 pointOnEdges[4];
    HemiCubeFace projectionFace;
    
    patchPoint1.intersect(&point1);
    patchPoint1.printWithFace();
    patchPoint2.intersect(&point2);
    patchPoint2.printWithFace();
    patchPoint3.intersect(&point3);
    patchPoint3.printWithFace();
    
    ThreeDIntersection patchPoints[3] = {patchPoint1, patchPoint2, patchPoint3};
    if (DEBUG_MODE) {
        cout << "\nChecking if the points are in same or different planes by comparing where they intersect on hemicube ...... \n";
    }
    if (patchPoint1.getIntersectingFace() == patchPoint2.getIntersectingFace() &&
        patchPoint2.getIntersectingFace() == patchPoint3.getIntersectingFace() &&
        patchPoint3.getIntersectingFace() == patchPoint1.getIntersectingFace()) { // all in the same side of cube
        if (DEBUG_MODE) {
            cout << "Points are on the same plane because they all intersect on the same surface of hemicube:" << patchPoint1.getHumanReadableIntersectingFace() << "\n";
        }
        printPatchOnSameSide(patchPoint1, patchPoint2, patchPoint3);
    } else {
        if (patchPoint1.getIntersectingFace() != patchPoint2.getIntersectingFace() &&
            patchPoint2.getIntersectingFace() != patchPoint3.getIntersectingFace() &&
            patchPoint3.getIntersectingFace() != patchPoint1.getIntersectingFace()) {
            if (DEBUG_MODE) {
                cout << "Points are on three different planes\n";
            }
            // patch is in three different sides of the cube // Need 4 new points: we need to calculate three new points and find another point based on what faces the points fall

            projectedPoint = Vector3::findPointOnALine(Vector3(0,0,0), givenPoints[0], patchPoints[2].getIntersectingFace());
            pointOnEdges[0] = Vector3::findPointOnALine(patchPoints[2].getVector(), projectedPoint, patchPoints[0].getIntersectingFace());

            projectedPoint = Vector3::findPointOnALine(Vector3(0,0,0), givenPoints[1], patchPoints[0].getIntersectingFace());
            pointOnEdges[1] = Vector3::findPointOnALine(patchPoints[0].getVector(), projectedPoint, patchPoints[1].getIntersectingFace());
            
            projectedPoint = Vector3::findPointOnALine(Vector3(0,0,0), givenPoints[2], patchPoints[1].getIntersectingFace());
            pointOnEdges[2] = Vector3::findPointOnALine(patchPoints[1].getVector(), projectedPoint, patchPoints[2].getIntersectingFace());

            
            HemiCubeFace faces[3] = {patchPoints[0].getIntersectingFace(),patchPoints[1].getIntersectingFace(), patchPoints[2].getIntersectingFace()};
            
            pointOnEdges[3] = findCommonVertex(faces);
            
            patchPoints[0].print();
            cout << " ––– " ;
            pointOnEdges[1].print();
            cout << " ––– ";
            pointOnEdges[3].print();
            cout << " ––– ";
            pointOnEdges[0].print();
            cout << " on " << patchPoints[0].getHumanReadableIntersectingFace() << " \n" ;
            
            patchPoints[1].print();
            cout << " ––– ";
            pointOnEdges[1].print();
            cout << " ––– ";
            pointOnEdges[3].print();
            cout << " ––– ";
            pointOnEdges[2].print();
            cout << " on " << patchPoints[1].getHumanReadableIntersectingFace() << " \n" ;
            
            patchPoints[2].print();
            cout << " ––– ";
            pointOnEdges[2].print();
            cout << " ––– ";
            pointOnEdges[3].print();
            cout << " ––– ";
            pointOnEdges[0].print();
            cout << " on " << patchPoints[2].getHumanReadableIntersectingFace() << " \n " ;
        } else {
            if (DEBUG_MODE) {
                cout << "Points are on two different planes\n";
                cout << "Point which will be projected to calculate new points is: ";
            }
            // patch is in two sides of the cube // Need 2 new points
           
            int index = findPointToBeProjected(patchPoints);
            if (DEBUG_MODE) {
                givenPoints[index].print();
                cout << ": On";
            }
            
            if(index >= 0) {
                projectionFace = findProjectionFace(patchPoint1, patchPoint2, patchPoint3);
                if (DEBUG_MODE) {
                    cout << projectionFace << "\n";
                }
                projectedPoint = Vector3::findPointOnALine(Vector3(0,0,0), givenPoints[index], projectionFace);
                if (DEBUG_MODE) {
                    cout << "Projected point which will be used to calculate two new points is:"; projectedPoint.print(); cout<<"\n";
                }
                if (index == 0) {
                    pointOnEdges[1] = Vector3::findPointOnALine(projectedPoint, patchPoints[1].getVector(), patchPoints[index].getIntersectingFace());
                    pointOnEdges[2] = Vector3::findPointOnALine(projectedPoint, patchPoints[2].getVector(), patchPoints[index].getIntersectingFace());
                    
                    cout << "\nCoordinates of the patch are:\n";
                    patchPoints[0].print();
                    cout << " ––– ";
                    pointOnEdges[1].print();
                    cout << " ––– ";
                    pointOnEdges[2].print();
                    cout << " on " << patchPoints[0].getHumanReadableIntersectingFace() << " \n" ;
                    pointOnEdges[1].print();
                    cout << " ––– ";
                    patchPoints[1].print();
                    cout << " ––– ";
                    patchPoints[2].print();
                    cout << " ––– ";
                    pointOnEdges[2].print();
                    cout << " on " << patchPoints[1].getHumanReadableIntersectingFace() << "\n";
                } else if(index == 1) {
                    pointOnEdges[0] = Vector3::findPointOnALine(projectedPoint, patchPoints[0].getVector(), patchPoints[index].getIntersectingFace());
                    pointOnEdges[2] = Vector3::findPointOnALine(projectedPoint, patchPoints[2].getVector(), patchPoints[index].getIntersectingFace());
                    
                    patchPoints[1].print();
                    cout << " ––– ";
                    pointOnEdges[0].print();
                    cout << " ––– ";
                    pointOnEdges[2].print();
                    cout << " on " << patchPoints[1].getHumanReadableIntersectingFace() << " \n" ;
                    pointOnEdges[0].print();
                    cout << " ––– ";
                    patchPoints[0].print();
                    cout << " ––– ";
                    patchPoints[2].print();
                    cout << " ––– ";
                    pointOnEdges[2].print();
                    cout << " on " << patchPoints[2].getHumanReadableIntersectingFace() << "\n";
                } else if(index == 2) {
                    pointOnEdges[0] = Vector3::findPointOnALine(projectedPoint, patchPoints[0].getVector(), patchPoints[index].getIntersectingFace());
                    pointOnEdges[1] = Vector3::findPointOnALine(projectedPoint, patchPoints[1].getVector(), patchPoints[index].getIntersectingFace());

                    patchPoints[2].print();
                    cout << " ––– ";
                    pointOnEdges[0].print();
                    cout << " ––– ";
                    pointOnEdges[1].print();
                    cout << " on " << patchPoints[2].getHumanReadableIntersectingFace() << " \n" ;
                    pointOnEdges[0].print();
                    cout << " ––– ";
                    patchPoints[0].print();
                    cout << " ––– ";
                    patchPoints[1].print();
                    cout << " ––– ";
                    pointOnEdges[1].print();
                    cout << " on " << patchPoints[0].getHumanReadableIntersectingFace() << "\n";
                }
                
            } else {
                cout << "ERROR:Invalid projection point\n";
            }
        }
    }
    
    
}

void printPatchOnSameSide(ThreeDIntersection patchPoint1, ThreeDIntersection patchPoint2, ThreeDIntersection patchPoint3) {
    cout << "\nCoordinates of the patch are:\n";
    patchPoint1.print();
    cout << ",";
    patchPoint2.print();
    cout << ",";
    patchPoint3.print();
    cout << " on " << patchPoint1.getHumanReadableIntersectingFace() << "\n";
}

int findPointToBeProjected(ThreeDIntersection patchPoints[]) {
    bool found = false;
    for (int i = 0; i <= 2; i++) {
        found = true;
        for (int j = i+1; j<= (2-i); j++) {
            if (patchPoints[i].getIntersectingFace() == patchPoints[j].getIntersectingFace()) {
                found = false;
                break;
            }
        }
        if (found) {
            return  i;
            break;
        }
    }
    return -1;
}

HemiCubeFace findProjectionFace(ThreeDIntersection patchPoint1, ThreeDIntersection patchPoint2, ThreeDIntersection patchPoint3) {
    bool found = false;
    ThreeDIntersection uniquePoint;
    ThreeDIntersection patchPoints[3];
    patchPoints[0] = patchPoint1;
    patchPoints[1] = patchPoint2;
    patchPoints[2] = patchPoint3;
    HemiCubeFace intersectingFace;
    for (int i = 0; i <= 2; i++) {
        found = true;
        for (int j = i+1; j<= (2-i); j++) {
            if (patchPoints[i].getIntersectingFace() == patchPoints[j].getIntersectingFace()) {
                found = false;
                break;
            }
        }
        if (found) {
            if (i == 2) {
                intersectingFace = patchPoints[i-1].getIntersectingFace();
            }
            intersectingFace = patchPoints[i+1].getIntersectingFace();
            break;
        }
    }
    return intersectingFace;
}

Vector3 findCommonVertex(HemiCubeFace faces[]) {
    if ((faces[0] == TOP_FACE && faces[1] == FRONT_FACE && faces[2] == LEFT_FACE) ||
        (faces[0] == TOP_FACE && faces[1] == LEFT_FACE && faces[2] == FRONT_FACE) ||
        (faces[0] == FRONT_FACE && faces[1] == TOP_FACE && faces[2] == LEFT_FACE) ||
        (faces[0] == FRONT_FACE && faces[1] == LEFT_FACE && faces[2] == TOP_FACE) ||
        (faces[0] == LEFT_FACE && faces[1] == TOP_FACE && faces[2] == FRONT_FACE) ||
        (faces[0] == LEFT_FACE && faces[1] == FRONT_FACE && faces[2] == TOP_FACE)) {
        return Vector3(-1,1,1);
    } else if((faces[0] == TOP_FACE && faces[1] == FRONT_FACE && faces[2] == RIGHT_FACE) ||
              (faces[0] == TOP_FACE && faces[1] == RIGHT_FACE && faces[2] == FRONT_FACE) ||
              (faces[0] == FRONT_FACE && faces[1] == TOP_FACE && faces[2] == RIGHT_FACE) ||
              (faces[0] == FRONT_FACE && faces[1] == RIGHT_FACE && faces[2] == TOP_FACE) ||
              (faces[0] == RIGHT_FACE && faces[1] == TOP_FACE && faces[2] == FRONT_FACE) ||
              (faces[0] == RIGHT_FACE && faces[1] == FRONT_FACE && faces[2] == TOP_FACE)) {
        return Vector3(1,1,1);
    } else if((faces[0] == TOP_FACE && faces[1] == BACK_FACE && faces[2] == LEFT_FACE) ||
              (faces[0] == TOP_FACE && faces[1] == LEFT_FACE && faces[2] == BACK_FACE) ||
              (faces[0] == BACK_FACE && faces[1] == TOP_FACE && faces[2] == LEFT_FACE) ||
              (faces[0] == BACK_FACE && faces[1] == LEFT_FACE && faces[2] == TOP_FACE) ||
              (faces[0] == LEFT_FACE && faces[1] == TOP_FACE && faces[2] == BACK_FACE) ||
              (faces[0] == LEFT_FACE && faces[1] == BACK_FACE && faces[2] == TOP_FACE)) {
        return Vector3(-1,1,-1);
    } else if((faces[0] == TOP_FACE && faces[1] == BACK_FACE && faces[2] == RIGHT_FACE) ||
              (faces[0] == TOP_FACE && faces[1] == RIGHT_FACE && faces[2] == BACK_FACE) ||
              (faces[0] == BACK_FACE && faces[1] == TOP_FACE && faces[2] == RIGHT_FACE) ||
              (faces[0] == BACK_FACE && faces[1] == RIGHT_FACE && faces[2] == TOP_FACE) ||
              (faces[0] == RIGHT_FACE && faces[1] == TOP_FACE && faces[2] == BACK_FACE) ||
              (faces[0] == RIGHT_FACE && faces[1] == BACK_FACE && faces[2] == TOP_FACE)) {
        return Vector3(1,1,-1);
    }
    return Vector3(0,0,0);
}