#include "canvas.h"
#include <cassert>
#include "image.h"

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
   } else if (_vertices.size() % 2 != 0) {
      cerr << "Number of line vertices isn't a multiple of 2" << endl;
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

void Canvas::drawLineLow(Vertex& beginVertex, Vertex& endVertex) 
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
      int idx = (y * _canvas.width()) + x;
      colorPixel(idx, {r, g, b}, "set");
      if (F > 0) {
         y += dy;
         F += 2 * (H - W);
      } else {
         F += 2 * H;
      }
   }
}

void Canvas::drawLineHigh(Vertex& beginVertex, Vertex& endVertex) 
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
      int idx = (y * _canvas.width()) + x;
      colorPixel(idx, {r, g, b}, "set");
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
   } else if (_vertices.size() % 3 != 0) {
      cerr << "Number of triangle vertices isn't a multiple of 3" << endl;
   }
   for (auto it = _vertices.begin(); it < _vertices.end(); it += 3) { 
       Vertex vertexA = *it;
       Vertex vertexB = *(it + 1);
       Vertex vertexC = *(it + 2);

      for (int idx = 0; idx < _canvas.width() * _canvas.height(); idx++) {
         float alpha = 0.5f, beta = 0.5f, gamma = 0.5f;
         if (alpha <= 1 && beta <= 1 && gamma <= 1) {
            unsigned char r = vertexA.color.r * alpha + vertexB.color.r * beta + vertexC.color.r * gamma;
            unsigned char g = vertexA.color.g * alpha + vertexB.color.g * beta + vertexC.color.g * gamma;
            unsigned char b = vertexA.color.b * alpha + vertexB.color.b * beta + vertexC.color.b * gamma;
            colorPixel(idx, {r, g, b}, "set");
         }
      }
   }
}

void Canvas::colorPixel(int idx, const Pixel& color, const string& blendMode) {
   Pixel origPx = _canvas.get(idx);
   Pixel newPx;
   if (blendMode == "set") {
      newPx = color;
   } else if (blendMode == "add") {
      newPx = {(unsigned char) (color.r + origPx.r), (unsigned char) (color.g + origPx.g), (unsigned char) (color.b + origPx.b)};
   } else if (blendMode == "difference") {
      newPx = {(unsigned char) abs(color.r - origPx.r), (unsigned char) abs(color.g - origPx.g), (unsigned char) abs(color.b - origPx.b)};
   }   
   _canvas.set(idx, newPx);
}

void Canvas::gradient(const Pixel& a, const Pixel& b, const string& orientation) {
   _canvas.set(_canvas.width(), _canvas.height(), _canvas.gradient(a, b, orientation).data());
}
