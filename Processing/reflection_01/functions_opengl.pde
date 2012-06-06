
void enable_vsync() {

  // opengl
  pgl = (PGraphicsOpenGL) g; //processing graphics object
  gl = pgl.beginGL(); //begin opengl

  // vsync
  int _bit = 1;
  gl.setSwapInterval(_bit); //set vertical sync on

  pgl.endGL(); //end opengl
}


void enableBlending(int mode) {

  pgl = (PGraphicsOpenGL) g;
  gl = pgl.gl; 

  pgl.beginGL();
  // This fixes the overlap issue
  gl.glDisable(GL.GL_DEPTH_TEST);
  // Turn on the blend mode
  gl.glEnable(GL.GL_BLEND);
  // Define the blend mode
  if (mode == MULTIPLY) {
    //gl.glBlendFunc(GL.GL_SRC_ALPHA, GL.GL_ONE_MINUS_SRC_ALPHA);
    gl.glBlendFunc(GL.GL_ZERO, GL.GL_ONE_MINUS_SRC_ALPHA);
  } 
  else {
    gl.glBlendFunc(GL.GL_SRC_ALPHA, GL.GL_ONE);
  }
  pgl.endGL();
}


