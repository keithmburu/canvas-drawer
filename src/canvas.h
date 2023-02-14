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
      void vertex(int x, int y);

      // Specify a color. Color components are in range [0,255]
      void color(unsigned char r, unsigned char g, unsigned char b);

      // Fill the canvas with the given background color
      void background(unsigned char r, unsigned char g, unsigned char b);

      void drawPoint();

      void drawLine();

      void drawLineLow(Vertex& beginVertex, Vertex& endVertex);

      void drawLineHigh(Vertex& beginVertex, Vertex& endVertex);

      void drawTriangle();

      void colorPixel(int idx, const Pixel& color, const std::string& blendMode);

      void gradient(const Pixel& a, const Pixel& b, const std::string& orientation);

   private:
      Image _canvas;
      PrimitiveType _currentType;
      Pixel _currentColor;
      std::vector<Vertex> _vertices;
   };
}

#endif


