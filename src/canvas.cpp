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

void Canvas::save(const std::string& filename)
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
   x = std::min(x, _canvas.width() - 1);
   y = std::min(y, _canvas.height() - 1);
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
      std::cerr << "Less than 1 point vertex provided" << std::endl;
   } 
   for (auto it = _vertices.begin(); it < _vertices.end(); it += 1) { 
       Vertex vertex = *it;
      _canvas.set(vertex.x, vertex.y, vertex.color);
   }
}

void Canvas::drawLine() 
{
   if (_vertices.size() < 2) {
      std::cerr << "Less than 2 line vertices provided" << std::endl;
   } else if (_vertices.size() % 2 != 0) {
      std::cerr << "Number of line vertices isn't a multiple of 2" << std::endl;
   }
   for (auto it = _vertices.begin(); it < _vertices.end(); it += 2) { 
      Vertex beginVertex = *it;
      Vertex endVertex = *(it + 1);
      int ax = beginVertex.x, ay = beginVertex.y;
      int bx = endVertex.x, by = endVertex.y;
      int H = abs(by - ay), W = abs(bx - ax);
      if (ax > bx) {
         beginVertex = _vertices[1];
         endVertex = _vertices[0];
         ax = beginVertex.x, ay = beginVertex.y;
         bx = endVertex.x, by = endVertex.y;
      }
      int incAmount = 1;
      if (by < ay) {
         incAmount = -1;
      } else if (ax == bx) {
         incAmount = 0;
      }
      bool iteratingX = true;
      int beginIdx = ax, endIdx = bx;
      int incVar = ay;
      if (H > W) {
         iteratingX = false;
         if (ay < by) {
            beginIdx = ay;
            endIdx = by;
            incVar = ax;
         } else {
            beginIdx = by;
            endIdx = ay;
            incVar = bx;
            beginVertex = *(it + 1);
            endVertex = *it;
         }
         int temp = H;
         H = W;
         W = temp;      
      }
      int F = 2 * H - W;
      for (int iterator = beginIdx; iterator <= endIdx; iterator++) {
         unsigned char r = endVertex.color.r * (float) iterator / endIdx + beginVertex.color.r * (1 - (float) iterator / endIdx);
         unsigned char g = endVertex.color.g * (float) iterator / endIdx + beginVertex.color.g * (1 - (float) iterator / endIdx);
         unsigned char b = endVertex.color.b * (float) iterator / endIdx + beginVertex.color.b * (1 - (float) iterator / endIdx);
         int x, y;
         iteratingX? x = iterator : x = incVar;
         iteratingX? y = incVar : y = iterator;
         int idx = (y * _canvas.width()) + x;
         paint(idx, {r, g, b}, "set");
         if (F > 0) {
            incVar += incAmount;
            F += 2 * (H - W);
         } else {
            F += 2 * H;
         }
      }
   }
}

void Canvas::drawTriangle()
{
   if (_vertices.size() < 3) {
      std::cerr << "Less than 3 triangle vertices provided" << std::endl;
   } else if (_vertices.size() % 3 != 0) {
      std::cerr << "Number of triangle vertices isn't a multiple of 3" << std::endl;
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
            paint(idx, {r, g, b}, "set");
         }
      }
   }
}

void Canvas::paint(int idx, const Pixel& color, const std::string& blendMode) {
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

void Canvas::gradient(const Pixel& a, const Pixel& b, const std::string& orientation) {
   _canvas.set(_canvas.width(), _canvas.height(), _canvas.gradient(a, b, orientation).data());
}
