/*
 *  ShapeContainer.cpp
 *  reflection
 *
 *  Created by Tiemen Rapati on 28/05/2012.
 *  Copyright 2012 UVA. All rights reserved.
 *
 */

#include "ShapeContainer.h"
#include "cinder/CinderMath.h"

using namespace ci;
using namespace uva;
using  std::list;

using std::cout;

ShapeContainer::ShapeContainer()
{
	// pyramids!
	
}
void ShapeContainer::init(uva::ShapeType _shape, int _numlevels, float _w, float _h)
{
    // init!
    m_shapes.resize(0);
	
	m_type = _shape;
	
	// construct structure based on type
    float w, h, d;
	
	switch (m_type) 
	{
		case PYRAMID:
			
			// structure based on pyramids
			
            w = _w;
            h = _h;
            d = _w; // regular pyramid has square base
			
			//pos = Vec3f(0, _numlevels*_h/2.0f, 0);
			
			for (int k = 1; k <= _numlevels; k+=2)
			{
				// we're at level k (vertical)
				
				// dimension increase linearly
				int currentw = k;
				int currenth = k;
				
				// horizontal plane i x j
				for (int i = 0; i < currentw; i+=2) {
					for (int j = 0; j < currenth; j+=2) {
						
						// we are at i, j on horizontal plane at vertical level k
						// w, d, h are the dimensions of the object -> gridsize
						
                        //h = (k%2 == 0) ? _h * -1 : _h;
                        
                        
                        
						float x = i*w - currentw*w/2;
						float y = k*h - _numlevels*h/2.0f; // y is up;;
						float z = j*d - currenth*d/2;
                        
						
						Shape p = Shape();
						p.init(x, y, z, w, -h, d, false);
						
                        
						m_shapes.push_back( p );
						
					} //j
				} //i
			} //k
			
			break;
			
		case TETRA:
			
			// structure based on tetrahedrons
			
            w = _w;
            d = tan(M_PI/3.0)*(_w/2.0);
            h = d;
			
			for (int k = 1; k <= _numlevels; k+=2)
			{
				// we're at level k (vertical)
				
				
				// horizontal plane i x j
                for (int j = 0; j < k; j++) {
                    for (int i = 0; i < j; i++) {
					
						
						// we are at i, j on horizontal plane at vertical level k
						// w, d, h are the dimensions of the object -> gridsize
						
						float x = j*w - k*w/2 - i*w/2;
						float y = -k*h + _numlevels*h/2.0f; // y is up;
						float z = i*d - k*d/2;
						
                        bool flipped = false; //(k%2 == 0) ? true : false;
                        
						Shape p = Shape();
						p.init(x, y, z, w, h, d, flipped);
						
						m_shapes.push_back( p );
						
					} // i
				} // j
			} // k
			
			break;
		
		default:
			break;
	}
    

	// set random indices
    
    for( list<Shape>::iterator shapeIt = m_shapes.begin(); shapeIt != m_shapes.end(); ++shapeIt ) 
	{
        int index = rand()%m_shapes.size();
        shapeIt->setIndex(index);
    }
    
    m_shapes.sort();
    //random_shuffle(m_shapes.begin(), m_shapes.end());
    
}
    
void ShapeContainer::update(Surface8u _surf, bool _showColour, bool _showHighContrast)
{
	Surface8u::Iter pixelIter = _surf.getIter();

	// update!
	for( list<Shape>::iterator shapeIt = m_shapes.begin(); shapeIt != m_shapes.end(); ++shapeIt) 
	{
		if (!pixelIter.pixel()) {
            if(pixelIter.line())
            {
                pixelIter.pixel(); // reset x to 0
            } else {
                return;
            }

        }

        float rf = pixelIter.r()*.33;
        float gf = pixelIter.g()*.33;
        float bf = pixelIter.b()*.34;
        
        float alpha = 1.0f;
        
        alpha = (_showHighContrast) ? (rf+gf+bf - 127)/128.0f : (rf+gf+bf)/255.0f;
        
        ColorA col = (_showColour) ? ColorA(pixelIter.r()/255.0, pixelIter.g()/255.0, pixelIter.b()/255.0, 0.3f) : ColorA( 1.0f, 1.0f, 1.0f, alpha/2.0f);
        shapeIt->update(col);
			
		

	}
	
}
void ShapeContainer::draw(bool _showDichroics)
{
	// draw
	for( list<Shape>::iterator shapeIt = m_shapes.begin(); shapeIt != m_shapes.end(); ++shapeIt ) 
	{
        switch (m_type) {
            case PYRAMID:
                shapeIt->drawPyramid(_showDichroics);
                break;
            
            case TETRA:
                shapeIt->drawTetra(_showDichroics);
                break;
                
            default:
                break;
        }
		
	}
	
}



