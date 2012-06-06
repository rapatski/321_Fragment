


void drawPolygon3D(float[][] vertices, int _primitive) {
  beginShape(_primitive);
  for (int i = 0; i < vertices.length; i++) {
    vertex(vertices[i][0], vertices[i][1], vertices[i][2]);
  }
  endShape();
}

