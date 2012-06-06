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

class Shape
{
  public:
    bool operator < (const Shape& rhs)
    {
        return m_index < rhs.m_index;
    }
    
  public:
	ci::Vec3f  pos;
	ci::Vec3f  dim;
    
    int        m_index;
	
	ci::ColorA strokeColour;
    ci::ColorA activeFaceColour;
    ci::ColorA inActiveFaceColour;
    ci::ColorA dichroicColour1;
    ci::ColorA dichroicColour2;
    
    bool flipped;
	
	Shape();
	void init(float _x, float _y, float _z, float _w, float _h, float _d, bool _flipped);
    void setIndex(int _i);
	void update(ci::ColorA _col);
	void draw();
    void drawPyramid( bool _showDichroics);
	void drawPyramid(float _x, float _y, float _z, float _w, float _h, float _d, bool _showDichroics);
    void drawTetra( bool _showDichroics);
	void drawTetra(float _x, float _y, float _z, float _w, float _h, float _d, bool _showDichroics);
	void drawPolygon3D(ci::Vec3f _vertices[], int _numverts, int _primitive);
};

