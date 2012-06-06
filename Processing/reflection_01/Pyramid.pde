class Pyramid
{

  PVector pos;
  PVector dim;

  Pyramid()
  {
    pos = new PVector();
    dim = new PVector();
    
  }
  
  void init(float _x, float _y, float _z, float _w, float _h, float _d)
  {
    pos.set(_x, _y, _z);
    dim.set(_w, _h, _d);
  }
  
  void display()
  {
    pushMatrix();
    
    translate(pos.x, pos.y, pos.z);
    
    color c = color(255, 255, 255, 255);
    drawPyramid(dim.x, dim.y, dim.z, c, true);
    
    popMatrix();
  }
  

  void drawPyramid(float w, float h, float d, color c, boolean solid)
  {

    // radius is more handy.
    w /= 2;
    h /= 2;
    d /= 2;

    // w, h make horizontal plane, d is vertical
    // y is up

    // vertices
    float[] v0 = {
      -w, -h, -d
    };
    float[] v1 = { 
      w, -h, -d
    };
    float[] v2 = { 
      w, -h, d
    };
    float[] v3 = {
      -w, -h, d
    };
    float[] v4 = {
      0, h, 0
    };


    // draw pyramid

    noFill();
    noStroke();

    if (!solid) {
      fill(c, 15);
      stroke(c, 60);
    } 
    else {
      fill(c, 20);
      stroke(c, 10);
    }


    // sides

    float[][] _left = {
      v0, v4, v3
    };
    drawPolygon3D(_left, TRIANGLES);

    float[][] _back = {
      v1, v4, v0
    };
    drawPolygon3D(_back, TRIANGLES);

    float[][] _rght = {
      v2, v4, v1
    };
    drawPolygon3D(_rght, TRIANGLES);

    float[][] _frnt = {
      v3, v4, v2
    };
    drawPolygon3D(_frnt, TRIANGLES);

    // base

    //fill(c);
    float[][] _base = {
      v0, v1, v2, v3
    };
    drawPolygon3D(_base, QUADS);
  }
};

