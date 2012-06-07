/*
 *  Shape.cpp
 *  reflection
 *
 *  Created by Tiemen Rapati on 28/05/2012.
 *  Copyright 2012 UVA. All rights reserved.
 *
 */

#include "Shape.h"

#include "cinder/gl/gl.h"


using namespace ci;
using  std::vector;

Shape::Shape()
{
	pos = Vec3f::zero();
	dim = Vec3f::zero();
    
	
}

void Shape::init(float _x, float _y, float _z, float _w, float _h, float _d, bool _flip)
{
	pos = Vec3f(_x, _y, _z);
	dim = Vec3f(_w, _h, _d);
    
    flipped = _flip;
    
    strokeColour = ColorA(1.0f, 1.0f, 1.0f, .10f);
    activeFaceColour = ColorA(1.0f, 1.0f, 1.0f, .05f);
    inActiveFaceColour = ColorA(1.0f, 1.0f, 1.0f, .015f);

}

void Shape::setCoordinates(int _i, int _k, int _j)
{
    m_i = _i;
    m_k = _k;
    m_j = _j;
}

void Shape::setIndex(int _index)
{
    m_index = _index;
}



void Shape::updateFaces(vector<ColorA> _faceColours)
{
    // this updates four faces, determined by the shape drawn.
	faceColours = _faceColours;
}

void Shape::draw()
{
	
}

void Shape::drawPyramid()
{
    drawPyramid(pos.x, pos.y, pos.z, dim.x, dim.y, dim.z);
}

void Shape::drawPyramid(float _x, float _y, float _z, float _w, float _h, float _d)
{
    
    // if not enough colours are passed, return
    if (faceColours.size() < 4)
        return;
	
	// radius is more handy.
	float w = _w/2.0;
	float h = _h/2.0;
	float d = _d/2.0;
	
	// w, d make horizontal plane, h is vertical
	// y is up
	
	// vertices
	Vec3f pos = Vec3f(_x, _y, _z);
	
	Vec3f v0 = Vec3f(
		   -w, -h, -d
		   ) + pos;
	Vec3f v1 = Vec3f(
		   w, -h, -d
		   ) + pos;
	Vec3f v2 = Vec3f(
		   w, -h, d
		   ) + pos;
	Vec3f v3 = Vec3f(
		   -w, -h, d
		   ) + pos;
	Vec3f v4 = Vec3f(
		   0, h, 0
		   ) + pos;
	
    
	// draw pyramid
	
	// sides
		
	Vec3f _left[3] = {
		v0, v4, v3
	};
	
    gl::color(faceColours[0]);
	drawPolygon3D(_left, 3, GL_TRIANGLES);
    gl::color(strokeColour);
    drawPolygon3D(_left, 3, GL_LINE_LOOP);
	
	Vec3f _back[3] = {
		v1, v4, v0
	};
    
    gl::color(faceColours[1]);
	drawPolygon3D(_back, 3, GL_TRIANGLES);
    gl::color(strokeColour);
    drawPolygon3D(_back, 3, GL_LINE_LOOP);
	
	Vec3f _rght[3] = {
		v2, v4, v1
	};
	
    gl::color(faceColours[2]);
	drawPolygon3D(_rght, 3, GL_TRIANGLES);
    gl::color(strokeColour);
    drawPolygon3D(_rght, 3, GL_LINE_LOOP);
	
	Vec3f _frnt[3] = {
		v3, v4, v2
	};
    
	gl::color(faceColours[3]);
	drawPolygon3D(_frnt, 3, GL_TRIANGLES);
    gl::color(strokeColour);
    drawPolygon3D(_frnt, 3, GL_LINE_LOOP);
	
	// base

	Vec3f _base[4] = {
		v0, v1, v2, v3
	};
	
	//drawPolygon3D(_base, 4, GL_QUADS);
    gl::color(strokeColour);
    drawPolygon3D(_base, 4, GL_LINE_LOOP);
}

void Shape::drawTetra()
{
    drawTetra(pos.x, pos.y, pos.z, dim.x, dim.y, dim.z);
}

void Shape::drawTetra(float _x, float _y, float _z, float _w, float _h, float _d)
{
    // if not enough colours are passed, return
    if (faceColours.size() < 4)
        return;
    
	// w, d are on horizontal plane
    // h is on vertical axis.
    
	float w = _w;
    float h = _h; // d is perfect tetra?
    float d = _d; 
    
    float c = _w/2.0*tan(3.141593/6.0);
	
	// w, d make horizontal plane, h is vertical
	// y is up
	
	// position
	Vec3f pos = Vec3f(_x, _y, _z);
	
    // vertices
    Vec3f v0, v1, v2, v3;
    
    if(!flipped)
    {
        v0 = Vec3f(0, 0, 0) + pos;
        v1 = Vec3f(w/2.f, 0, d) + pos;
        v2 = Vec3f(w, 0, 0) + pos;
        
          
    } else {
        v0 = Vec3f(0, 0, d) + pos;
        v1 = Vec3f(w/2.f, 0, 0) + pos;
        v2 = Vec3f(w, 0, d) + pos;
    }
    
    v3 = Vec3f(w/2.f, h, c) + pos;
	
	// draw tetra
	
	// sides
    
	Vec3f _frst[3] = { 
        v0, v3, v2
	};
    gl::color(faceColours[0]);
	drawPolygon3D(_frst, 3, GL_TRIANGLES);
    gl::color(strokeColour);
    drawPolygon3D(_frst, 3, GL_LINE_LOOP);

	Vec3f _scnd[3] = {
		v1, v3, v0
	};
    gl::color(faceColours[1]);
	drawPolygon3D(_scnd, 3, GL_TRIANGLES);
    gl::color(strokeColour);
    drawPolygon3D(_scnd, 3, GL_LINE_LOOP);
	
	Vec3f _thrd[3] = {
		v2, v3, v1
	};
    gl::color(faceColours[2]);	
	drawPolygon3D(_thrd, 3, GL_TRIANGLES);
    gl::color(strokeColour);
    drawPolygon3D(_thrd, 3, GL_LINE_LOOP);
	
	// base
	
	Vec3f _base[3] = {
		v0, v1, v2
	};
	gl::color(faceColours[3]);	
	drawPolygon3D(_base, 3, GL_TRIANGLES);
    gl::color(strokeColour);
    drawPolygon3D(_base, 3, GL_LINE_LOOP);
}


void Shape::drawPolygon3D(Vec3f _vertices[], int _numverts, int _primitive) {
	glBegin(_primitive);
	for (int i = 0; i < _numverts; i++) {
		glVertex3f(_vertices[i].x, _vertices[i].y, _vertices[i].z);
	}
	glEnd();
}
