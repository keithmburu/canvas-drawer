#include "canvas.h"
#include <cassert>
#include "image.h"
#include <algorithm>

using namespace std;
using namespace agl;

Canvas::Canvas(int w, int h) : _canvas(w, h)
{
   _currentType = UNDEFINED;
   _blendMode = "replace";
   _fillShapes = true;
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
      drawPoints();
   } else if (_currentType == LINES) {
      drawLines();
   } else if (_currentType == TRIANGLES) {
      drawTriangles();
   } else if (_currentType == UNDEFINED) {
      cerr << "Undefined primitive type" << endl;
      exit(6);
   }
   _vertices.clear();
}

Vertex Canvas::vertex(int x, int y)
{
   x = max(0, min(x, _canvas.width() - 1));
   y = max(0, min(y, _canvas.height() - 1));
   Vertex v = {x, y, _currentColor};
   _vertices.push_back(v);
   return v;
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

void Canvas::changeBlendMode(const string& newBlendMode) 
{
   _blendMode = newBlendMode;
}

void Canvas::toggleShapeFill() 
{
   _fillShapes = !_fillShapes;
}

void Canvas::drawPoints() 
{
   if (_vertices.size() < 1) {
      cerr << "No point vertices provided" << endl;
      exit(1);
   } 
   for (auto it = _vertices.begin(); it < _vertices.end(); it += 1) { 
      Vertex vertex = *it;
      colorPixel(vertex.x, vertex.y, vertex.color);
   }
}

void Canvas::drawLines() 
{
   if (_vertices.size() < 2) {
      cerr << "Less than 2 line vertices provided" << endl;
      exit(2);
   }
   for (auto it = _vertices.begin(); it < _vertices.end(); it += 2) { 
      if (it == _vertices.end() - 1) {
         cerr << "Number of line vertices isn't a multiple of 2" << endl;
         break;
      }
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
      colorPixel(x, y, {r, g, b});
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
      colorPixel(x, y, {r, g, b});
      if (F > 0) {
         x += dx;
         F += 2 * (W - H);
      } else {
         F += 2 * W;
      }
   }
}

void Canvas::drawTriangles()
{
   if (_vertices.size() < 3) {
      cerr << "Less than 3 triangle vertices provided" << endl;
      exit(4);
   }
   for (auto it = _vertices.begin(); it < _vertices.end(); it += 3) { 
      if (it == _vertices.end() - 2) {
         cerr << "Number of triangle vertices isn't a multiple of 3" << endl;
         break;
      }
      sortCounterClockwise(it);
      Vertex vertexA = *it;
      Vertex vertexB = *(it + 1);
      Vertex vertexC = *(it + 2);
      // create bounding box
      int xMin = min({vertexA.x, vertexB.x, vertexC.x});
      int xMax = max({vertexA.x, vertexB.x, vertexC.x});
      int yMin = min({vertexA.y, vertexB.y, vertexC.y});
      int yMax = max({vertexA.y, vertexB.y, vertexC.y});
      int fAlpha = implicitEqn(vertexA.x, vertexA.y, vertexB, vertexC);
      int fBeta = implicitEqn(vertexB.x, vertexB.y, vertexC, vertexA);
      int fGamma = implicitEqn(vertexC.x, vertexC.y, vertexA, vertexB);
      for (int y = yMin; y <= yMax; y++) {
         for (int x = xMin; x <= xMax; x++) {
            // convert to barycentric coordinates
            float alpha = (float) implicitEqn(x, y, vertexB, vertexC) / fAlpha;
            float beta = (float) implicitEqn(x, y, vertexC, vertexA) / fBeta;
            float gamma = (float) implicitEqn(x, y, vertexA, vertexB) / fGamma;
            bool colorCondition = (alpha >= 0 && beta  >= 0 && gamma >= 0);
            if (!_fillShapes) {
               // pixel must be on an edge
               float thresh = 0.015;
               colorCondition = ((0 <= alpha && alpha <= thresh && beta >= 0 && gamma >= 0) || (alpha >= 0 && 0 <= beta && beta <= thresh && gamma >= 0) || (alpha >= 0 && beta >= 0 && 0 <= gamma && gamma <= thresh));
            }
            if (colorCondition) {
               // Avoid overlap
               int offScreenX = -1, offScreenY = -2.1;
               if ((alpha > 0 || fAlpha * implicitEqn(offScreenX, offScreenY, vertexB, vertexC) > 0) && (beta > 0 || fBeta * implicitEqn(offScreenX, offScreenY, vertexC, vertexA) > 0) && (gamma > 0 || fGamma * implicitEqn(offScreenX, offScreenY, vertexA, vertexB) > 0)) {
                  unsigned char r = vertexA.color.r * alpha + vertexB.color.r * beta + vertexC.color.r * gamma;
                  unsigned char g = vertexA.color.g * alpha + vertexB.color.g * beta + vertexC.color.g * gamma;
                  unsigned char b = vertexA.color.b * alpha + vertexB.color.b * beta + vertexC.color.b * gamma;
                  colorPixel(x, y, {r, g, b});
               }
            }
         }
      }
   }
}

void Canvas::sortCounterClockwise(const vector<Vertex>::iterator& it) {
   Vertex vertexA = *it;
   Vertex vertexB = *(it + 1);
   Vertex vertexC = *(it + 2);
   float centroidX = (float) (vertexA.x + vertexB.x + vertexC.x) / 3;
   float centroidY = (float) (vertexA.y + vertexB.y + vertexC.y) / 3;
   sort(it, it + 3, [centroidX, centroidY](const Vertex& v1, const Vertex& v2) {
      float v1Angle = atan2(v1.y - centroidY, v1.x - centroidX);
      float v2Angle = atan2(v2.y - centroidY, v2.x - centroidX);
      return v1Angle > v2Angle;
   });
}

void Canvas::rectangle(int xLeft, int xRight, int yBottom, int yTop)
{
   cout << "Drawing rectangle from " << xLeft << " " << yBottom << " to " << xRight << " " << yTop << endl;
   this->begin(TRIANGLES);
   this->vertex(xLeft, yBottom);
   this->vertex(xLeft, yTop);
   this->vertex(xRight, yTop);

   this->vertex(xRight, yBottom);
   this->vertex(xRight, yTop);
   this->vertex(xLeft, yBottom);
   this->end();
}

void Canvas::circle(int centerX, int centerY, int radius)
{
   cout << "Drawing circle with center " << centerX << " " << centerY << " and radius " << radius << endl;
   if (radius == 0) {
      this->begin(POINTS);
      this->vertex(centerX, centerY);
      this->end();
   } else {
      this->begin(TRIANGLES);
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
      this->end();
   }
}

void Canvas::star(int centerX, int centerY, int radius) 
{
   cout << "Drawing star with center " << centerX << " " << centerY << " and radius " << radius << endl;
   this->begin(TRIANGLES);
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
   this->end();
}

void Canvas::rose(int centerX, int centerY, int a, int n, int d) 
{
   cout << "Drawing rose with center " << centerX << " " << centerY << ", a = " << a << " and k = " << n << " / " << d << endl;
   this->begin(LINES);
   float r;
   float k = (float) n / d;
   int x, y;
   for (float theta = 0; theta < 2 * M_PI * 10; theta += 0.017) {
      r = a * cos(k * theta);
      x = r * cos(theta) + centerX;
      y = r * sin(theta) + centerY;
      this->vertex(x, y);
   }
   this->end();
}

void Canvas::snowflake(int xStart, int yStart, int width, int recursionDepth) 
{
   cout << "Drawing snowflake starting from " << xStart << " " << yStart << " with width " << width << endl;
   this->begin(TRIANGLES);
   snowflakeHelper(xStart, yStart, xStart + width, yStart, recursionDepth - 1, 60);
   snowflakeHelper(xStart, yStart + 0.577 * width, xStart + width, yStart + 0.577 * width, recursionDepth - 1, 300);
   this->end();
}

void Canvas::snowflakeHelper(int nextV1X, int nextV1Y, int nextV2X, int nextV2Y, int recursionDepth, int vertex3Degrees) 
{
   Vertex v1 = this->vertex(nextV1X, nextV1Y);
   Vertex v2 = this->vertex(nextV2X, nextV2Y);
   int r = sqrt(pow(v2.x - v1.x, 2) + pow(abs(v2.y - v1.y), 2));
   float theta = vertex3Degrees * (M_PI / 180) + atan2(v2.y - v1.y, v2.x - v1.x);
   Vertex v3 = this->vertex(v1.x + (r * cos(theta)), v1.y + (r * sin(theta)));

   if (recursionDepth >= 0) {
      int X[3] = { v1.x, v2.x, v3.x };
      int Y[3] = { v1.y, v2.y, v3.y };

      for (int i = 0; i < 3; i++) {
         int nextV1X = X[i] + (X[(i + 1) % 3] - X[i]) * 0.333;
         int nextV1Y = Y[i] + (Y[(i + 1) % 3] - Y[i]) * 0.333;
         int nextV2X = X[i] + (X[(i + 1) % 3] - X[i]) * 0.666;
         int nextV2Y = Y[i] + (Y[(i + 1) % 3] - Y[i]) * 0.666;
         int v4X = X[i] + (nextV1X - X[i]) * 0.333;
         int v4Y = Y[i] + (nextV1Y - Y[i]) * 0.333;
         int v5X = X[i] + (nextV1X - X[i]) * 0.666;
         int v5Y = Y[i] + (nextV1Y - Y[i]) * 0.666;
         int v6X = nextV2X + (X[(i + 1) % 3] - nextV2X) * 0.333;
         int v6Y = nextV2Y + (Y[(i + 1) % 3] - nextV2Y) * 0.333;
         int v7X = nextV2X + (X[(i + 1) % 3] - nextV2X) * 0.666;
         int v7Y = nextV2Y + (Y[(i + 1) % 3] - nextV2Y) * 0.666;

         if (vertex3Degrees == 300) {
            snowflakeHelper(v4X, v4Y, v5X, v5Y, recursionDepth - 1, 60);
            snowflakeHelper(nextV1X, nextV1Y, nextV2X, nextV2Y, recursionDepth - 1, 60);
            snowflakeHelper(v6X, v6Y, v7X, v7Y, recursionDepth - 1, 60);
         } else {
            snowflakeHelper(v4X, v4Y, v5X, v5Y, recursionDepth - 1, 300);
            snowflakeHelper(nextV1X, nextV1Y, nextV2X, nextV2Y, recursionDepth - 1, 300);
            snowflakeHelper(v6X, v6Y, v7X, v7Y, recursionDepth - 1, 300);
         }
      }
   }
}

void Canvas::colorPixel(int x, int y, const Pixel& color) 
{
   if (0 <= x && x < _canvas.width() && 0 <= y && y < _canvas.height()) {
      Pixel origPx = _canvas.get(y, x);
      Pixel newPx;
      if (_blendMode == "replace") {
         newPx = color;
      } else if (_blendMode == "add") {
         newPx = {(unsigned char) (color.r + origPx.r), (unsigned char) (color.g + origPx.g), (unsigned char) (color.b + origPx.b)};
      } else if (_blendMode == "difference") {
         newPx = {(unsigned char) abs(color.r - origPx.r), (unsigned char) abs(color.g - origPx.g), (unsigned char) abs(color.b - origPx.b)};
      } else if (_blendMode == "average") {
         newPx = {(unsigned char) (0.5 * color.r + 0.5 * origPx.r), (unsigned char) (0.5 * color.g + 0.5 * origPx.g), (unsigned char) (0.5 * color.b + 0.5 * origPx.b)};
      }   
      _canvas.set(y, x, newPx);
   }
}

int Canvas::implicitEqn(int x, int y, const Vertex& a, const Vertex& b) 
{
   int H = b.y - a.y, W = b.x - a.x;
   int f = W * (y - a.y) - H * (x - a.x);
   return f;
}

void Canvas::gradient(const Pixel& a, const Pixel& b, const string& orientation, float alpha) 
{
   Image gradientImg = _canvas.gradient(a, b, orientation, alpha);
   _canvas.set(gradientImg.width(), gradientImg.height(), gradientImg.data());
}
