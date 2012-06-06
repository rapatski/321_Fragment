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
#include <list>
#include "cinder/Vector.h"
#include "cinder/Surface.h"

namespace uva 
{
    enum ShapeType {
        PYRAMID, TETRA
    };
}

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
	void init(uva::ShapeType _shape, int _numlevels, float _w, float _h);
    void update(ci::Surface8u _surf, bool _showColour, bool _showHighContrast);
    void draw(bool _showDichroics);
    
    uva::ShapeType m_type;
    std::list<Shape> m_shapes;
};