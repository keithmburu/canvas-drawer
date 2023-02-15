#include <iostream>
#include "canvas.h"

using namespace agl;
using namespace std;

void test_line(Canvas& drawer, int ax, int ay, int bx, int by, const std::string& savename)
{
   drawer.background(0, 0, 0);
   drawer.begin(LINES);
   drawer.vertex(ax, ay);
   drawer.vertex(bx, by);
   drawer.end();
   drawer.save(savename);
}

int main(int argc, char** argv)
{
   Canvas drawer(100, 100);

   drawer.color(255, 255, 255);

   // // test point primitive
   // drawer.begin(POINTS);
   // drawer.vertex(50, 50);
   // drawer.end();
   // drawer.save("point.png");

   // test_line(drawer, 0, 50, 100, 50, "horizontal-line.png");
   // test_line(drawer, 50, 0, 50, 100, "vertical-line.png");
   // test_line(drawer, 0, 0, 100, 100, "diagonal-line-1.png"); // slope = 1
   // test_line(drawer, 25, 10, 75, 25,  "h-lessthan-w-line-1.png"); // slope H < W
   // test_line(drawer, 25, 25, 75, 75,  "w-lessthan-h-line-1.png"); // slope W < H
   // test_line(drawer, 0, 100, 100, 0, "diagonal-line-2.png"); // slope H < W
   // test_line(drawer, 25, 90, 75, 75,  "h-lessthan-w-line-2.png"); // slope H < W
   // test_line(drawer, 25, 90, 75, 25,  "w-lessthan-h-line-2.png"); // slope W < H

   // // test line interpolation
   // drawer.background(0, 0, 0);
   // drawer.begin(LINES);
   // drawer.color(255, 0, 255);
   // drawer.vertex(0, 0);
   // drawer.color(0, 255, 255);
   // drawer.vertex(100, 100);
   // drawer.end();
   // drawer.save("line-color-interpolation.png");

   // // test triangle with interpolation
   // drawer.background(0, 0, 0);
   // drawer.begin(TRIANGLES);
   // drawer.color(255, 0, 255);
   // drawer.vertex(10, 0);
   // drawer.color(0, 255, 255);
   // drawer.vertex(90, 50);
   // drawer.color(255, 255, 0);
   // drawer.vertex(10, 90);
   // drawer.end();
   // drawer.save("triangle.png");

   // // test composite with triangles
   // drawer.background(0, 0, 0);
   // drawer.begin(TRIANGLES);
   // drawer.color(255, 0, 255);
   // drawer.vertex(10, 10);
   // drawer.vertex(10, 90);
   // drawer.vertex(90, 90);
   // drawer.color(255, 255, 0);
   // drawer.vertex(90, 90);
   // drawer.vertex(90, 10);
   // drawer.vertex(10, 10);
   // drawer.end();
   // drawer.save("quad.png");

   // // test more than one line
   // drawer.color(255, 255, 255);
   // drawer.begin(LINES);
   // drawer.vertex(0, 0);
   // drawer.vertex(99, 99);
   // drawer.vertex(0, 99);
   // drawer.vertex(99, 0);
   // drawer.end();
   // drawer.save("two-lines.png");

   // // test gradient
   // drawer.background(255, 255, 255);
   // drawer.gradient({0, 255, 0}, {255, 0, 255}, "vertical");
   // drawer.color(255, 255, 255);
   // drawer.begin(LINES);
   // drawer.vertex(0, 0);
   // drawer.vertex(99, 99);
   // drawer.end();
   // drawer.save("vertical-gradient.png");

   // // test circle
   // drawer.background(0, 0, 0);
   // drawer.begin(TRIANGLES);
   // drawer.color(255, 255, 255);
   // drawer.vertex(50, 50);
   // drawer.vertex(50, 80);
   // drawer.vertex(80, 50);

   // drawer.vertex(50, 50);
   // drawer.vertex(50, 20);
   // drawer.vertex(20, 50);

   // drawer.vertex(50, 50);
   // drawer.vertex(50, 80);
   // drawer.vertex(20, 50);

   // drawer.vertex(50, 50);
   // drawer.vertex(50, 20);
   // drawer.vertex(80, 50);

   // drawer.end();
   // drawer.save("circle.png");

   // // test rectangle
   // drawer.rectangle(10, 90, 30, 70);
   // drawer.save("rectangle.png");

   // // test circle
   // drawer.circle(50, 50, 30);
   // drawer.save("circle.png");

   // // test star
   // drawer.star(50, 50, 30);
   // drawer.save("star.png");

   // test rose
   drawer.rose(50, 50, 30);
   drawer.save("rose.png");
   
   return 0;
}
