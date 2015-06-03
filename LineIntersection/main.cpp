//
//  main.cpp
//  LineIntersection
//
//  Created by Roshan Gautam on 6/1/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//

#include <iostream>
#include <string>

#include "point.h"
#include "3dpoint.h"
#include "intersection.h"
#include "3dintersection.h"

using namespace std;

void scanPoint(Point *);
Intersection findIntersection(Point *);
void scan3DPoint(ThreeDPoint *);

int main(int argc, const char * argv[]) {
    Point point1, point2;
    ThreeDPoint threeDPoint;
    Intersection intersection, intersection1;
    char c;
    cout << "Line Intersection Program - Solid Modeling CS6413 !!! \n";
    cout << "Assumptions for 2D:\n";
    cout << "1. Light Source Origin is always (0,0)\n";
    cout << "2. Height of hemi-square = 1 i.e y = 1 \n";
    cout << "3. Left End of hemi-square from origin = -1 i.e x = -1 \n";
    cout << "4. Right End of hemi-square from origin = 1 i.e x = 1 \n\n";
    while(1) {
        cout << "Options:\n";
        cout << "(1)Enter Coordinates for a 2D Point\n";
        cout << "(2)Enter Coordinates for a 2D line segment\n";
        cout << "(3)Enter Coordinates for a 3D Point\n";
        cout << "(Q)Quit\n";
        cin >> c;
        switch (c) {
            case '1':
            {
                cout << "Enter Coordinates for the point!\n";
                scanPoint(&point1);
                intersection = findIntersection(&point1);
                cout << "Coordinates of intersecting segment are:";
                if(intersection.getSide() == "Left") {
                    cout << "(" << intersection.getPoint().getX() << "," << intersection.getPoint().getY() << ") , (-1,1)";
                } else if(intersection.getSide() == "Right") {
                    cout << "(" << intersection.getPoint().getX() << "," << intersection.getPoint().getY() << ") , (1,1)";
                } else if(intersection.getSide() == "Negative Top") {
                    cout << "(-1,1) , (" << intersection.getPoint().getX() << "," << intersection.getPoint().getY() << ")";
                } else if(intersection.getSide() == "Positive Top") {
                    cout << "(" << intersection.getPoint().getX() << "," << intersection.getPoint().getY() << ") , (1,1)";
                }
                cout << " on " << intersection.getSide() << "\n\n";
            }
                break;
            case '2':
            {
                cout << "Enter Coordinates for first point!\n";
                scanPoint(&point1);
                cout << "Enter Coordinates for second point!\n";
                scanPoint(&point2);
                intersection = findIntersection(&point1);
                intersection1 = findIntersection(&point2);
                if((intersection.getSide() == "Negative Top" && intersection1.getSide() == "Positive Top") ||
                   (intersection.getSide() == "Positive Top" && intersection1.getSide() == "Negative Top")) {
                    cout << "Coordinates of intersecting segment are:("
                        << intersection.getPoint().getX()
                        << ","
                        << intersection.getPoint().getY()
                        << ") , ("
                        << intersection1.getPoint().getX()
                        << ","
                        << intersection1.getPoint().getY()
                        << ") on "
                        << intersection.getSide()
                        << " and "
                        << intersection1.getSide()
                        << "\n\n";
                } else {
                    cout << "Coordinates of intersecting segment are:";
                    if(intersection.getSide() == "Left") {
                        cout << "(" << intersection.getPoint().getX() << "," << intersection.getPoint().getY() << ") , (-1,1)";
                    } else if(intersection.getSide() == "Right") {
                        cout << "(" << intersection.getPoint().getX() << "," << intersection.getPoint().getY() << ") , (1,1)";
                    } else if(intersection.getSide() == "Negative Top") {
                        cout << "(-1,1) , (" << intersection.getPoint().getX() << "," << intersection.getPoint().getY() << ")";
                    } else if(intersection.getSide() == "Positive Top") {
                        cout << "(" << intersection.getPoint().getX() << "," << intersection.getPoint().getY() << ") , (1,1)";
                    }
                    cout << " on " << intersection.getSide() << " and ";
                    if(intersection.getSide() == "Left" || intersection.getSide() == "Right") {
                        cout << "(-1,1) , (1,1) on Top AND ";
                    }
                    if(intersection1.getSide() == "Left") {
                        cout << "(" << intersection1.getPoint().getX() << "," << intersection1.getPoint().getY() << ") , (-1,1)";
                    } else if(intersection1.getSide() == "Right") {
                        cout << "(" << intersection1.getPoint().getX() << "," << intersection1.getPoint().getY() << ") , (1,1)";
                    } else if(intersection1.getSide() == "Negative Top") {
                        cout << "(-1,1) , (" << intersection1.getPoint().getX() << "," << intersection1.getPoint().getY() << ")";
                    } else if(intersection1.getSide() == "Positive Top") {
                        cout << "(" << intersection1.getPoint().getX() << "," << intersection1.getPoint().getY() << ") , (1,1)";
                    }
                    cout << " on " << intersection1.getSide() << "\n\n";
                }
            }
                break;
            case '3':
            {
                cout << "Enter Coordinates for 3D point!\n";
                scan3DPoint(&threeDPoint);
                intersection = findIntersection(&point1);
                cout << "Coordinates of intersecting segment are:";
                if(intersection.getSide() == "Left") {
                    cout << "(" << intersection.getPoint().getX() << "," << intersection.getPoint().getY() << ") , (-1,1)";
                } else if(intersection.getSide() == "Right") {
                    cout << "(" << intersection.getPoint().getX() << "," << intersection.getPoint().getY() << ") , (1,1)";
                } else if(intersection.getSide() == "Negative Top") {
                    cout << "(-1,1) , (" << intersection.getPoint().getX() << "," << intersection.getPoint().getY() << ")";
                } else if(intersection.getSide() == "Positive Top") {
                    cout << "(" << intersection.getPoint().getX() << "," << intersection.getPoint().getY() << ") , (1,1)";
                }
                cout << " on " << intersection.getSide() << "\n\n";
            }
                break;
            case 'q':
            case 'Q':
                cout << "Thank You. --- End of Program ---\n";
                exit(1);
            default:
                cout << "Invalid option\n\n";
                break;
        }
    }
    return 0;
}

void scanPoint(Point *point) {
    float x = 0, y = 0;
    cout << "Enter x:";
    cin >> x;
    cout << "Enter y:";
    cin >> y;
    point->setX(x);
    point->setY(y);
}

void scan3DPoint(ThreeDPoint *point) {
    float x = 0, y = 0, z = 0;
    cout << "Enter x:";
    cin >> x;
    cout << "Enter y:";
    cin >> y;
    cout << "Enter z:";
    cin >> z;
    point->setX(x);
    point->setY(y);
    point->setZ(z);
}

Intersection findIntersection(Point *subject) {
    Intersection intersection;
    float y = 1.0;
    float x = 1.0;
    if(subject->getY() > 0) {
        if(subject->getX() < 0) { // if x is negative
            if (subject->getY() < (-(subject->getX()))) { // left side intersection
                intersection.setSide(LEFT);
                x = -1.0;
                y = x * (subject->getY()/subject->getX());
            } else if(subject->getY() > (-(subject->getX()))) { //top intersection in negative abscissa
                intersection.setSide(NEGATIVE_TOP);
                x = y / (subject->getY()/subject->getX());
            }
        } else { // if x is positive
            if (subject->getY() < subject->getX()) { // right side intersection
                intersection.setSide(RIGHT);
                y = x * (subject->getY()/subject->getX());
            } else if(subject->getY() > subject->getX()) { //top intersection in positive abscissa
                intersection.setSide(POSITIVE_TOP);
                x = y / (subject->getY()/subject->getX());
            }
        }
        Point point;
        point.setX(x);
        point.setY(y);
        intersection.setPoint(point);
    }
    return intersection;
}

ThreeDIntersection findIntersection(ThreeDPoint *subject) {
    ThreeDIntersection intersection;
    float x = 1.0;
    float y = 1.0;
    float z = 1.0;
    if(subject->getY() > 0) {
        if(subject->getY() < (-(subject->getZ())) &&
           subject->getY() < subject->getZ()) {
            if (subject->getX() < 0 &&
                subject->getX() > -1) {
                intersection.setIntersectingPlane(LEFT_PLANE);
                x = -1.0;
                //y = ?
                //z = ?
            } else if(subject->getX() > 0 &&
                      subject->getX() < -1) {
                intersection.setIntersectingPlane(RIGHT_PLANE);
                x = 1.0;
                //y = ?
                //z = ?
            }
        } else if(subject->getY() < subject->getX() &&
                  subject->getY() < (-(subject->getX()))) {
            if (subject->getZ() > 0 &&
                subject->getZ() < 1) {
                intersection.setIntersectingPlane(FRONT);
                z = 1.0;
                //x = ?
                //y = ?
            } else if(subject->getZ() < 0 &&
                      subject->getZ() > -1) {
                intersection.setIntersectingPlane(BACK);
                z = -1.0;
            }
        } else if(subject->getZ() < (-(subject->getX())) &&
                  subject->getZ() < subject->getX() &&
                  (-(subject->getZ())) < subject->getX() &&
                  (-(subject->getZ())) < (-subject->getX())){
            intersection.setIntersectingPlane(TOP);
            y = 1.0;
        }
    }
    return intersection;
}