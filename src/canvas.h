/**
 * @file canvas.h
 * @author Keith Mburu
 * @date 2023-02-13
 * 
 */

#ifndef canvas_H_
#define canvas_H_

#include <string>
#include <vector>
#include "image.h"
#include <cmath>

namespace agl
{
   enum PrimitiveType {UNDEFINED, POINTS, LINES, TRIANGLES};
   struct Vertex {
      int x;
      int y;
      Pixel color;
   };

   class Canvas
   {
   public:
      Canvas(int w, int h);
      virtual ~Canvas();

      // Save to file
      void save(const std::string& filename);

      // Draw primitives with a given type (either LINES or TRIANGLES)
      // For example, the following draws a red line followed by a green line
      // begin(LINES);
      //    color(255,0,0);
      //    vertex(0,0);
      //    vertex(100,0);
      //    color(0,255,0);
      //    vertex(0, 0);
      //    vertex(0,100);
      // end();
      void begin(PrimitiveType type);
      void end();

      // Specifiy a vertex at raster position (x,y)
      // x corresponds to the column; y to the row
      Vertex vertex(int x, int y);

      // Specify a color. Color components are in range [0,255]
      void color(unsigned char r, unsigned char g, unsigned char b);

      // Fill the canvas with the given background color
      void background(unsigned char r, unsigned char g, unsigned char b);

      void changeBlendMode(const std::string& newBlendMode);

      void toggleShapeFill();

      void drawPoint();

      void drawLine();

      void drawLineLow(const Vertex& beginVertex, const Vertex& endVertex);

      void drawLineHigh(const Vertex& beginVertex, const Vertex& endVertex);

      void drawTriangle();

      void sortCounterClockwise(const std::vector<Vertex>::iterator& it);

      void rectangle(int xLeft, int yBottom, int xRight, int yTop);
      
      void circle(int centerX, int centerY, int radius);

      void star(int centerX, int centerY, int radius);

      void rose(int centerX, int centerY, int a, int n, int d);

      void snowflake(int xLeft, int yBottom, int width, int recursionDepth);

      void snowflakeHelper(int nextV1X, int nextV1Y, int nextV2X, int nextV2Y, int recursionDepth, int vertex3Degrees);

      int implicitEqn(int x, int y, const Vertex& a, const Vertex& b);

      void colorPixel(int i, int j, const Pixel& color);

      void gradient(const Pixel& a, const Pixel& b, const std::string& orientation, float alpha);

   private:
      Image _canvas;
      PrimitiveType _currentType;
      Pixel _currentColor;
      std::vector<Vertex> _vertices;
      std::string _blendMode;
      bool _fillShapes;
   };
}

#endif


