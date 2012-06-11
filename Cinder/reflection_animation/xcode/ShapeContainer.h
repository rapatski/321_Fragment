/*
 *  ShapeContainer.h
 *  reflection
 *
 *  Created by Tiemen Rapati on 28/05/2012.
 *  Copyright 2012 UVA. All rights reserved.
 *
 */

#pragma once
#include "Shape.h"
#include <vector>
#include "cinder/Vector.h"
#include "cinder/Surface.h"

#include "constants.h";

class ShapeContainer 
{
  public:
    
    bool SortShapePredicate(const Shape& shape1, const Shape& shape2)
    {
        if(shape1.m_index < shape2.m_index)
        {
            return true; 
        } 
        return false;
    };
    
    ShapeContainer();
	void init(ShapeType _shape, int _numlevels, int _incr, float _w, float _h);
    void setMaterialSettings(std::vector<DisplayMode*> _pfaceModes, bool *_showColour, float *_alpha, float *_contrast);
    void setAnimationSettings(AnimationMode *_aniMode);
    void setRadarSettings(int* _interval, float *_frontarea, float *_backarea);
    void update(ci::Surface8u* _surf, int _tNow);
    void draw();
    
    // STRUCTURE
    ShapeType                   m_type;
    std::vector<Shape>          m_shapes;
    Shape                       m_structure;
    
    // MATERIAL
    std::vector<DisplayMode*>    m_faceModes;
    bool                        *p_showColour;
    float                       *p_alpha;
    float                       *p_contrast;
    
    // ANIMATION
    AnimationMode               *p_aniMode;
    
    // radar
    int                         *p_interval;
    float                       *p_frontarea;
    float                       *p_backarea;
};