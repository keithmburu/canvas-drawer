#include "canvas.h"
#include <cassert>
#include "image.h"
#include <algorithm>

using namespace std;
using namespace agl;

Canvas::Canvas(int w, int h) : _canvas(w, h)
{
   _currentType = UNDEFINED;
}

Canvas::~Canvas()
{
}

void Canvas::save(const string& filename)
{
   _canvas.save(filename);
}

void Canvas::begin(PrimitiveType type)
{
   _currentType = type;
}

void Canvas::end()
{
   if (_currentType == POINTS) {
      drawPoint();
   } else if (_currentType == LINES) {
      drawLine();
   } else if (_currentType == TRIANGLES) {
      drawTriangle();
   }
   _vertices.clear();
}

void Canvas::vertex(int x, int y)
{
   x = min(x, _canvas.width() - 1);
   y = min(y, _canvas.height() - 1);
   _vertices.push_back({x, y, _currentColor});
}

void Canvas::color(unsigned char r, unsigned char g, unsigned char b)
{
   _currentColor = {r, g, b};
}

void Canvas::background(unsigned char r, unsigned char g, unsigned char b)
{
   for (int idx = 0; idx < _canvas.width() * _canvas.height(); idx++) {
      _canvas.set(idx, {r, g, b});
   }
}

void Canvas::drawPoint() 
{
   if (_vertices.size() < 1) {
      cerr << "Less than 1 point vertex provided" << endl;
      exit(1);
   } 
   for (auto it = _vertices.begin(); it < _vertices.end(); it += 1) { 
       Vertex vertex = *it;
      _canvas.set(vertex.x, vertex.y, vertex.color);
   }
}

void Canvas::drawLine() 
{
   if (_vertices.size() < 2) {
      cerr << "Less than 2 line vertices provided" << endl;
      exit(2);
   } else if (_vertices.size() % 2 != 0) {
      cerr << "Number of line vertices isn't a multiple of 2" << endl;
      exit(3);
   }
   for (auto it = _vertices.begin(); it < _vertices.end(); it += 2) { 
      Vertex beginVertex = *it;
      Vertex endVertex = *(it + 1);
      int ax = beginVertex.x, ay = beginVertex.y;
      int bx = endVertex.x, by = endVertex.y;
      int H = by - ay, W = bx - ax;
      if (abs(H) < abs(W)) {
         if (ax <= bx) {
            drawLineLow(beginVertex, endVertex);
         } else {
            drawLineLow(endVertex, beginVertex);
         }
      } else {
         if (ay <= by) {
            drawLineHigh(beginVertex, endVertex);
         } else {
            drawLineHigh(endVertex, beginVertex);
         }
      }
   }
}

void Canvas::drawLineLow(const Vertex& beginVertex, const Vertex& endVertex) 
{
   int ax = beginVertex.x, ay = beginVertex.y;
   int bx = endVertex.x, by = endVertex.y;
   int y = ay, dy = 1;
   int H = by - ay, W = bx - ax;
   if (H < 0) {
      dy = -1;
      H = -H;
   }
   int F = 2 * H - W;
   for (int x = ax; x <= bx; x++) {
      unsigned char r = endVertex.color.r * (float) x / bx + beginVertex.color.r * (1 - (float) x / bx);
      unsigned char g = endVertex.color.g * (float) x / bx + beginVertex.color.g * (1 - (float) x / bx);
      unsigned char b = endVertex.color.b * (float) x / bx + beginVertex.color.b * (1 - (float) x / bx);
      colorPixel(x, y, {r, g, b}, "set");
      if (F > 0) {
         y += dy;
         F += 2 * (H - W);
      } else {
         F += 2 * H;
      }
   }
}

void Canvas::drawLineHigh(const Vertex& beginVertex, const Vertex& endVertex) 
{
   int ax = beginVertex.x, ay = beginVertex.y;
   int bx = endVertex.x, by = endVertex.y;
   int x = ax, dx = 1;
   int H = by - ay, W = bx - ax;
   if (W < 0) {
      dx = -1;
      W = -W;
   }
   int F = 2 * W - H;
   for (int y = ay; y <= by; y++) {
      unsigned char r = endVertex.color.r * (float) y / by + beginVertex.color.r * (1 - (float) y / by);
      unsigned char g = endVertex.color.g * (float) y / by + beginVertex.color.g * (1 - (float) y / by);
      unsigned char b = endVertex.color.b * (float) y / by + beginVertex.color.b * (1 - (float) y / by);
      colorPixel(x, y, {r, g, b}, "set");
      if (F > 0) {
         x += dx;
         F += 2 * (W - H);
      } else {
         F += 2 * W;
      }
   }
}

void Canvas::drawTriangle()
{
   if (_vertices.size() < 3) {
      cerr << "Less than 3 triangle vertices provided" << endl;
      exit(4);
   } else if (_vertices.size() % 3 != 0) {
      cerr << "Number of triangle vertices isn't a multiple of 3" << endl;
      exit(5);
   }
   for (auto it = _vertices.begin(); it < _vertices.end(); it += 3) { 
       Vertex vertexA = *it;
       Vertex vertexB = *(it + 1);
       Vertex vertexC = *(it + 2);
      // create bounding box
      int xMin = min({vertexA.x, vertexB.x, vertexC.x});
      int xMax = max({vertexA.x, vertexB.x, vertexC.x});
      int yMin = min({vertexA.y, vertexB.y, vertexC.y});
      int yMax = max({vertexA.y, vertexB.y, vertexC.y});
      int fAlpha = implicitEqn(vertexA.x, vertexA.y, vertexB, vertexC);
      int fBeta = implicitEqn(vertexB.x, vertexB.y, vertexA, vertexC);
      int fGamma = implicitEqn(vertexC.x, vertexC.y, vertexA, vertexB);
      for (int y = yMin; y <= yMax; y++) {
         for (int x = xMin; x <= xMax; x++) {
            // convert to barycentric coordinates
            float alpha = (float) implicitEqn(x, y, vertexB, vertexC) / fAlpha;
            float beta = (float) implicitEqn(x, y, vertexA, vertexC) / fBeta;
            float gamma = (float) implicitEqn(x, y, vertexA, vertexB) / fGamma;
            if (alpha >= 0 && beta  >= 0 && gamma >= 0) {
               // Avoid overlap
               if ((alpha > 0 || fAlpha * implicitEqn(-1, -1, vertexB, vertexC) > 0) && (beta > 0 || fBeta * implicitEqn(-1, -1, vertexA, vertexC) > 0) && (gamma > 0 || fGamma * implicitEqn(-1, -1, vertexA, vertexB) > 0)) {
                  unsigned char r = vertexA.color.r * alpha + vertexB.color.r * beta + vertexC.color.r * gamma;
                  unsigned char g = vertexA.color.g * alpha + vertexB.color.g * beta + vertexC.color.g * gamma;
                  unsigned char b = vertexA.color.b * alpha + vertexB.color.b * beta + vertexC.color.b * gamma;
                  colorPixel(x, y, {r, g, b}, "set");
               }
            }
         }
      }
   }
}

void Canvas::rectangle(int xLeft, int xRight, int yBottom, int yTop)
{
   this->vertex(xLeft, yBottom);
   this->vertex(xLeft, yTop);
   this->vertex(xRight, yTop);

   this->vertex(xRight, yBottom);
   this->vertex(xRight, yTop);
   this->vertex(xLeft, yBottom);
   drawTriangle();
}

void Canvas::circle(int centerX, int centerY, int radius)
{
   int NUM_TRIANGLES = 32;
   float theta = 0;
   float dTheta = (float) (2 * M_PI) / NUM_TRIANGLES;
   int firstX, firstY, secondX, secondY;
   for (int i = 0; i < NUM_TRIANGLES; i++) {
      firstX = centerX + (radius * cos(theta));
      firstY = centerY + (radius * sin(theta));
      theta += dTheta;
      secondX = centerX + (radius * cos(theta));
      secondY = centerY + (radius * sin(theta));
      this->vertex(centerX, centerY);
      this->vertex(firstX, firstY);
      this->vertex(secondX, secondY);
   }
   drawTriangle();
}

void Canvas::star(int centerX, int centerY, int radius) {
   this->vertex(centerX - (radius / 2), centerY);
   this->vertex(centerX + (radius / 2), centerY);
   this->vertex(centerX, centerY + radius);

   this->vertex(centerX, centerY - (radius / 2));
   this->vertex(centerX, centerY + (radius / 2));
   this->vertex(centerX + radius, centerY);

   this->vertex(centerX - (radius / 2), centerY);
   this->vertex(centerX + (radius / 2), centerY);
   this->vertex(centerX, centerY - radius);

   this->vertex(centerX, centerY - (radius / 2));
   this->vertex(centerX, centerY + (radius / 2));
   this->vertex(centerX - radius, centerY);
   drawTriangle();
}

void Canvas::rose(int centerX, int centerY, int radius) {
   int x, y;
   float r;
   float a = radius, k = 7.0f / 4;
   for (float theta = 0; theta < 2 * M_PI * 10; theta += 0.017) {
      r = a * cos(k * theta);
      x = centerX + (r * cos(theta));
      y = centerY + (r * sin(theta));
      this->vertex(x, y);
   }
   drawPoint();
}

void Canvas::colorPixel(int x, int y, const Pixel& color, const string& blendMode) {
   Pixel origPx = _canvas.get(y, x);
   Pixel newPx;
   if (blendMode == "set") {
      newPx = color;
   } else if (blendMode == "add") {
      newPx = {(unsigned char) (color.r + origPx.r), (unsigned char) (color.g + origPx.g), (unsigned char) (color.b + origPx.b)};
   } else if (blendMode == "difference") {
      newPx = {(unsigned char) abs(color.r - origPx.r), (unsigned char) abs(color.g - origPx.g), (unsigned char) abs(color.b - origPx.b)};
   }   
   _canvas.set(y, x, newPx);
}

int Canvas::implicitEqn(int x, int y, const Vertex& a, const Vertex& b) {
   int H = b.y - a.y, W = b.x - a.x;
   int f = W * (y - a.y) - H * (x - a.x);
   return f;
}

void Canvas::gradient(const Pixel& a, const Pixel& b, const string& orientation) {
   Image gradientImg = _canvas.gradient(a, b, orientation);
   _canvas.set(gradientImg.width(), gradientImg.height(), gradientImg.data());
}
