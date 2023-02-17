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
      void begin(PrimitiveType type);
      void end();

      // Specifiy a vertex at raster position (x,y)
      // x corresponds to the column; y to the row
      Vertex vertex(int x, int y);

      // Specify a color. Color components are in range [0,255]
      void color(unsigned char r, unsigned char g, unsigned char b);

      // Fill the canvas with the given background color
      void background(unsigned char r, unsigned char g, unsigned char b);

      // Change how newly drawn pixel colors mix with existing pixel colors
      // blend modes are "replace", "add", "difference", and "average"
      void changeBlendMode(const std::string& newBlendMode);

      // Enable or disable shape outlining
      void toggleShapeFill();

      // Color the pixels corresponding to one specified vertex each
      void drawPoints();

      // Connect each pair of specified vertices with a rasterized line
      void drawLines();

      // Draw line with downward slope
      void drawLineLow(const Vertex& beginVertex, const Vertex& endVertex);

      // Draw line with upward slope
      void drawLineHigh(const Vertex& beginVertex, const Vertex& endVertex);

      // Draw rasterized triangle for each set of three specified vertices
      void drawTriangles();

      // Order triangle vertices in counter-clockwise order
      void sortCounterClockwise(const std::vector<Vertex>::iterator& it);

      // Draw rectangle as a composite of two rasterized triangles
      void rectangle(int xLeft, int yBottom, int xRight, int yTop);
      
      // Draw circle as a composite of rasterized triangles
      void circle(int centerX, int centerY, int radius);

      // Draw star as a composite of four rasterized triangles
      void star(int centerX, int centerY, int radius);

      // Draw rose sinusoid using cosine function
      // a determines the size, 1 <= n <= 7 and 1 <= d <= 9 determine the shape
      void rose(int centerX, int centerY, int a, int n, int d);

      // Draw Koch snowflake with its base at xStart, yStart
      // Size determined by width and complexity determined by recursionDepth
      void snowflake(int xStart, int yStart, int width, int recursionDepth);

      // Draw next triangle in the snowflake
      // vertex3Degrees determines the angle that vertex 1 makes with vertex 3
      void snowflakeHelper(int nextV1X, int nextV1Y, int nextV2X, int nextV2Y, int recursionDepth, int vertex3Degrees);

      // Compute where a point is in relation to a line
      int implicitEqn(int x, int y, const Vertex& a, const Vertex& b);

      // Blend in new pixel value
      void colorPixel(int x, int y, const Pixel& color);

      // Apply a vertical or horizontal gradient to the canvas with two colors
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


