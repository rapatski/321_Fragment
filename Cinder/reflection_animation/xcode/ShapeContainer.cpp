/*
 *  ShapeContainer.cpp
 *  reflection
 *
 *  Created by Tiemen Rapati on 28/05/2012.
 *  Copyright 2012 UVA. All rights reserved.
 *
 */


#include "ShapeContainer.h"

#include <algorithm>
#include "cinder/CinderMath.h"

using namespace ci;
using namespace std;
using std::vector;

using std::cout;

ShapeContainer::ShapeContainer()
{
	m_type              = PYRAMID;
    
    // MATERIAL
    m_showColour        = false;
    m_alpha             = 0.5f;
    m_contrast          = 1.0f;
    
    // ANIMATION
    m_aniMode           = STATIC;
    
    // radar
    m_interval          = 360;
    m_frontarea         = 1.f;
    m_backarea          = 5.f;
}

void ShapeContainer::init(ShapeType _shape, int _numlevels, int _incr, float _w, float _h)
{
    // initialise m_shapes
    m_shapes.resize(0);
	m_type = _shape;
	
    float w, h, d;
    float ow, oh, od;
    
	
    // construct structure based on type
	switch (m_type) 
	{
		case PYRAMID:
			
			// structure based on pyramids
			
            w = _w;
            h = _h;
            d = _w; // regular pyramid has square base
			
			//pos = Vec3f(0, _numlevels*_h/2.0f, 0);-
			
			for (int k = 1; k <= _numlevels; k++)
			{
				// we're at level k (vertical)
				
				// dimension increase linearly
				int currentw = k;
				int currenth = k;
				
				// horizontal plane i x j
				for (int i = 0; i < currentw; i+=1) {
					for (int j = 0; j < currenth; j+=1) {
						
						// we are at i, j on horizontal plane at vertical level k
						// w, d, h are the dimensions of the object -> gridsize
						
                        //h = (k%2 == 0) ? _h * -1 : _h;
                        
						float x = i*w - currentw*w/2;
						float y = k*h - _numlevels*h/2.0f; // y is up;
						float z = j*d - currenth*d/2; 
                        
						
						Shape p = Shape();
						p.init(x, y, z, w, -h, d, false);
                        p.setCoordinates(i, k, j);
		
                        Shape q = Shape();
                        q.init(x, y+h, z, w, h, d, false);
                        q.setCoordinates(i, k+1, j);

                               
                        if (k%_incr == 0) {
                            p.setActive();
                            q.setActive();
                        }
                        
                        if((i%2 == 0 && j%2 == 0) || k%2 == 0)
                        {
                            m_shapes.push_back( p );
                            m_shapes.push_back( q );
                            
                        }
                        
						
					} //j
				} //i
			} //k
            
            
            
            ow = w*_numlevels;
            oh = -h*_numlevels;
            od = d*_numlevels;
            
            m_structure.init(-w/2.f, h/2.f, -d/2.f, ow, oh, od, false);
                
            
			
			break;
			
		case TETRA:
			
			// structure based on tetrahedrons
			
            w = _w;
            d = tan(M_PI/3.0)*(_w/2.0);
            h = d;
			
			for (int k = 1; k <= _numlevels; k++)
			{
				// we're at level k (vertical)
				
				
				// horizontal plane i x j
                for (int j = 0; j < k; j++) {
                    for (int i = 0; i < j; i++) {
					
						
						// we are at i, j on horizontal plane at vertical level k
						// w, d, h are the dimensions of the object -> gridsize
                        
                        float c = _w/2.0*tan(3.141593/6.0);
						
						float x = j*w - k*w/2 - i*w/2;
						float y = k*h - _numlevels*h/2.0f; // y is up;
						float z = i*d - k*c;
						
                        //bool flipped = false; //(k%2 == 0) ? true : false;
                        
                        
                        
						Shape p = Shape();
						p.init(x, y, z, w, -h, d, false);
                        p.setCoordinates(i, k, j);
                        
                        Shape q = Shape();
                        q.init(x, y, z, w, h, d, false);
                        q.setCoordinates(i, k, j);
                        
                        if (k%_incr == 0) {
                            p.setActive();
                            q.setActive();
                        }
                        
                        m_shapes.push_back( p );
                        m_shapes.push_back( q );
                        
						
					} // i
				} // j
			} // k
			
            
            ow = w*_numlevels;
            oh = -h*_numlevels;
            od = d*_numlevels;
            
            m_structure.init(0, 0, 0, ow, oh, od, false);
            
            
			break;
		
		default:
			break;
	}
    

	// set random indices
    
    for( vector<Shape>::iterator shapeIt = m_shapes.begin(); shapeIt != m_shapes.end(); ++shapeIt ) 
	{
        int index = rand()%m_shapes.size();
        shapeIt->setIndex(index);
    }
    
    random_shuffle(m_shapes.begin(), m_shapes.end());
    
}


void ShapeContainer::setMaterialSettings(std::vector<DisplayMode> _faceModes, bool _showColour, float _alpha, float _contrast)
{
    m_faceModes = _faceModes;
    m_showColour = _showColour;
    m_contrast = _contrast;
    m_alpha = _alpha;
    
}

void ShapeContainer::setAnimationSettings(AnimationMode _aniMode)
{
    m_aniMode = _aniMode;
}

void ShapeContainer::setRadarSettings(int _interval, float _frontarea, float _backarea)
{
    m_interval = _interval;
    m_frontarea = _frontarea;
    m_backarea = _backarea;
}

    
void ShapeContainer::update(Surface8u* _surf, int _tNow)
{
	Surface8u::Iter pixelIter = _surf->getIter();

	// update!
	for( vector<Shape>::iterator shapeIt = m_shapes.begin(); shapeIt != m_shapes.end(); ++shapeIt) 
	{
        
        if (!shapeIt->m_isActive) 
            continue;

        vector<ColorA> faceColours;
        
        for (int i = 0; i < m_faceModes.size(); i++) {
            
            float alpha = 1.0f;
            float rc = 1.0f;
            float gc = 1.0f;
            float bc = 1.0f;
            
            if (m_aniMode == VIDEO || m_aniMode == VIDEO_RADAR)
            {
                rc = pixelIter.r()/255.f;
                gc = pixelIter.g()/255.f;
                bc = pixelIter.b()/255.f;
                
                alpha *= (rc + gc + bc) / 3.f;
            }
            
            if (m_aniMode == RADAR || m_aniMode == VIDEO_RADAR)
            {
                float progress = (_tNow%m_interval)/(float)m_interval;
                
                // to be sure, add twice area size
                float radarY = -max(m_backarea, m_frontarea) + progress*(14.f + max(m_backarea, m_frontarea)*2);
                
                float factor = 0;
                float diff = shapeIt->m_k - radarY;
                
                if (diff > 0)
                {
                    if(diff < m_frontarea) 
                        factor = 1 - diff/m_frontarea;
                }
                if (diff <= 0)
                {
                    if(diff > -m_backarea) 
                        factor = 1 - diff/-m_backarea;
                }
                
                alpha *= factor;
            }
            
            if (!pixelIter.pixel()) {
                if(pixelIter.line())
                {
                    pixelIter.pixel(); // reset x to 0
                } else {
                    return;
                }
                
            }
            
            ColorA col = ColorA(0, 0, 0, 0);
            alpha = (alpha - .5f)*m_contrast + .5f;
            if(alpha < 0) alpha = 0;
            if(alpha > 1) alpha = 1;
            
            alpha *= m_alpha;
            
            
            switch (m_faceModes[i]) {
                case ACTIVE:
                    col = (m_showColour) ? ColorA(rc, gc, bc, alpha) : ColorA( 1.0f, 1.0f, 1.0f, alpha);
                    break;
                case DICHROIC1:
                    col = ColorA(0.3f, 0.8f, 1.0f, .1f);
                    break;
                case DICHROIC2:
                    col = ColorA(1.0f, 0.8f, 0.3f, .1f);
                    break;
                case GLASS:
                    col = ColorA(1.0f, 1.0f, 1.0f, .015f);
                    break;
                case NONE:
                    col = ColorA(1.0f, 1.0f, 1.0f, 0.f);
                    break;
                default:
                    col = ColorA(1.0f, 1.0f, 1.0f, 0.f);
                    break;
            }
            faceColours.push_back(col);
            
        }
        
        shapeIt->updateFaces(faceColours);


	}
	
}
void ShapeContainer::draw()
{
	// draw
	
    switch (m_type) {
        case PYRAMID:
            for( vector<Shape>::iterator shapeIt = m_shapes.begin(); shapeIt != m_shapes.end(); ++shapeIt ) 
                shapeIt->drawPyramid();

            m_structure.drawPyramid();
            break;
        
        case TETRA:
            for( vector<Shape>::iterator shapeIt = m_shapes.begin(); shapeIt != m_shapes.end(); ++shapeIt ) 
                shapeIt->drawTetra();

            m_structure.drawTetra();
            break;
            
        default:
            break;
    }
		
	
    
	
}



