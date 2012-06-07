/*
 *  Shape.h
 *  reflection
 *
 *  Created by Tiemen Rapati on 28/05/2012.
 *  Copyright 2012 UVA. All rights reserved.
 *
 */

#include "cinder/Vector.h"
#include "cinder/Color.h"
#include <vector>

class Shape
{
  public:
	ci::Vec3f  pos;
	ci::Vec3f  dim;

    int        m_index;
	int        m_i, m_k, m_j;
    
    std::vector<ci::ColorA> faceColours;
    
	ci::ColorA strokeColour;
    ci::ColorA activeFaceColour;
    ci::ColorA inActiveFaceColour;
    ci::ColorA dichroicColour1;
    ci::ColorA dichroicColour2;
    
    bool flipped;
	
	Shape();
	void init(float _x, float _y, float _z, float _w, float _h, float _d, bool _flipped);
    void setCoordinates(int _i, int _k, int _j);
    void setIndex(int _index);

	void updateFaces(std::vector<ci::ColorA> _faceColours);
    
	void draw();
    void drawPyramid();
	void drawPyramid(float _x, float _y, float _z, float _w, float _h, float _d);
    void drawTetra();
	void drawTetra(float _x, float _y, float _z, float _w, float _h, float _d);
	void drawPolygon3D(ci::Vec3f _vertices[], int _numverts, int _primitive);
};

