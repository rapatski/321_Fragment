import java.util.ArrayList;

import processing.opengl.*;
import javax.media.opengl.*;

import peasy.*;
import traer.physics.*;



PGraphicsOpenGL pgl;
GL gl;

PeasyCam cam;

PFont font;

// --


// --

String PROJECT_NAME = "REFLECTION TESTS";

// --

Pyramid[] pyramids;


void setup() 
{
  //size(1200, 600, OPENGL);
  size(screen.width, screen.height/6*5, OPENGL);

  hint(DISABLE_OPENGL_2X_SMOOTH);
  hint(ENABLE_OPENGL_4X_SMOOTH);
  //hint(DISABLE_DEPTH_TEST);

  // perspective
  float fov = PI/3.0;
  float nearClip = 1;
  float farClip = 3000;

  float aspect = width/float(height);
  perspective(fov, aspect, nearClip, farClip);

  cam = new PeasyCam(this, 300);

  font = loadFont("knockout-48.vlw");

  // --

  int numlevels = 8;
  
  float w = 100;
  float h = 120;
  float d = 100;
  
  pyramids = new Pyramid[0];

  for (int k = 1; k <= numlevels; k++)
  {
    // we're at level k (vertical)

    // dimension increase linearly
    int currentw = k;
    int currenth = k;

    // horizontal plane i x j
    for (int i = 0; i < currentw; i++) {
      for (int j = 0; j < currenth; j++) {

        // we are at i, j on horizontal plane at vertical level k
        // w, d, h are the dimensions of the object -> gridsize

        float x = i*w - currentw*w/2;
        float y = -k*h; // y is up;
        float z = j*d - currenth*d/2;
        
        Pyramid p = new Pyramid();
        p.init(x, y, z, w, h, d);
        
        pyramids = (Pyramid[]) append(pyramids, p);
        
      }
    }
  }
  
  println(pyramids.length);
  
  println();
  println("pos: "+pyramids[0].pos.x +", "+ pyramids[0].pos.y+", "+ pyramids[0].pos.z);
  println("dim: "+pyramids[0].dim.x +", "+ pyramids[0].dim.y+", "+ pyramids[0].dim.z);
}


void draw() 
{ 
  // set
  enable_vsync();

  // update


  // background
  background(0, 0, 0);
  cam.beginHUD();

  noStroke();
  beginShape();
  fill(0, 0, 0);
  vertex(0, 0);
  fill(0, 0, 0);
  vertex(width, 0);
  fill(100);
  vertex(width, height);
  fill(100);
  vertex(0, height);
  endShape(CLOSE);

  cam.endHUD();

  // --

  enableBlending(ADD);

  // -- 

for(int i = 0; i < pyramids.length; i++)
  {
    Pyramid p = pyramids[i];
    
    p.display();
  }

  // -- 

  cam.beginHUD();
  textFont(font, 20);
  fill(255);
  text(PROJECT_NAME, 30, 35);
  cam.endHUD();
}


void keyReleased() 
{
  if (key == ' ') saveFrame();
}


void mouseReleased()
{
}

