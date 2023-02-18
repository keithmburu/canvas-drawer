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

   // test point primitive
   drawer.begin(POINTS);
   drawer.vertex(50, 50);
   drawer.end();
   drawer.save("../images/test/point.png");

   test_line(drawer, 0, 50, 100, 50, "../images/test/horizontal-line.png");
   test_line(drawer, 50, 0, 50, 100, "../images/test/vertical-line.png");
   test_line(drawer, 0, 0, 100, 100, "../images/test/diagonal-line-1.png"); // slope = 1
   test_line(drawer, 25, 10, 75, 25,  "../images/test/h-lessthan-w-line-1.png"); // slope H < W
   test_line(drawer, 25, 25, 75, 75,  "../images/test/w-lessthan-h-line-1.png"); // slope W < H
   test_line(drawer, 0, 100, 100, 0, "../images/test/diagonal-line-2.png"); // slope H < W
   test_line(drawer, 25, 90, 75, 75,  "../images/test/h-lessthan-w-line-2.png"); // slope H < W
   test_line(drawer, 25, 90, 75, 25,  "../images/test/w-lessthan-h-line-2.png"); // slope W < H

   // test line interpolation
   drawer.background(0, 0, 0);
   drawer.begin(LINES);
   drawer.color(255, 0, 255);
   drawer.vertex(0, 0);
   drawer.color(0, 255, 255);
   drawer.vertex(100, 100);
   drawer.end();
   drawer.save("../images/test/line-color-interpolation.png");

   // test triangle with interpolation
   drawer.background(0, 0, 0);
   drawer.begin(TRIANGLES);
   drawer.color(255, 0, 255);
   drawer.vertex(10, 0);
   drawer.color(0, 255, 255);
   drawer.vertex(90, 50);
   drawer.color(255, 255, 0);
   drawer.vertex(10, 90);
   drawer.end();
   drawer.save("../images/test/triangle.png");

   // test composite with triangles
   drawer.background(0, 0, 0);
   drawer.begin(TRIANGLES);
   drawer.color(255, 0, 255);
   drawer.vertex(10, 10);
   drawer.vertex(10, 90);
   drawer.vertex(90, 90);
   drawer.color(255, 255, 0);
   drawer.vertex(90, 90);
   drawer.vertex(90, 10);
   drawer.vertex(10, 10);
   drawer.end();
   drawer.save("../images/test/quad.png");

   // test more than one line
   drawer.background(0, 0, 0);
   drawer.color(255, 255, 255);
   drawer.begin(LINES);
   drawer.vertex(0, 0);
   drawer.vertex(99, 99);
   drawer.vertex(0, 99);
   drawer.vertex(99, 0);
   drawer.end();
   drawer.save("../images/test/two-lines.png");

   // test gradient
   drawer.background(255, 255, 255);
   drawer.gradient({0, 255, 0}, {0, 64, 0}, "vertical", 1);
   drawer.color(255, 255, 255);
   drawer.begin(LINES);
   drawer.vertex(0, 0);
   drawer.vertex(99, 99);
   drawer.end();
   drawer.save("../images/test/vertical-gradient.png");

   // test rectangle
   drawer.background(0, 0, 0);
   drawer.rectangle(10, 90, 30, 70);
   drawer.save("../images/test/rectangle.png");

   // test circle
   drawer.background(0, 0, 0);
   drawer.circle(50, 50, 30);
   drawer.save("../images/test/circle.png");

   // test star
   drawer.background(0, 0, 0);
   drawer.star(50, 50, 30);
   drawer.save("../images/test/star.png");

   // test rose
   drawer.background(0, 0, 0);
   drawer.rose(50, 50, 30, 7, 4);
   drawer.save("../images/test/rose.png");

   // test outlined shapes
   drawer.background(0, 0, 0);
   drawer.toggleShapeFill();
   drawer.begin(TRIANGLES);
   drawer.color(255, 0, 255);
   drawer.vertex(10, 10);
   drawer.vertex(10, 90);
   drawer.vertex(90, 90);
   drawer.color(255, 255, 0);
   drawer.vertex(90, 90);
   drawer.vertex(90, 10);
   drawer.vertex(10, 10);
   drawer.end();
   drawer.save("../images/test/outlined-quad.png");

   drawer.background(0, 0, 0);
   drawer.begin(TRIANGLES);
   drawer.color(255, 0, 255);
   drawer.vertex(10, 0);
   drawer.color(0, 255, 255);
   drawer.vertex(90, 50);
   drawer.color(255, 255, 0);
   drawer.vertex(10, 90);
   drawer.end();
   drawer.save("../images/test/outlined-triangle.png");

   drawer.background(0, 0, 0);
   drawer.circle(50, 50, 30);
   drawer.save("../images/test/outlined-circle.png");

   Canvas drawer1(1000, 1000);
   drawer1.background(0, 0, 0);
   drawer1.color(255, 255, 255);

   // test snowflake
   // drawer1.toggleShapeFill();
   drawer1.snowflake(150, 300, 700, 1);
   drawer1.save("../images/test/snowflake-1.png");
   drawer1.snowflake(150, 300, 700, 2);
   drawer1.save("../images/test/snowflake-2.png");
   drawer1.snowflake(150, 300, 700, 4);
   drawer1.save("../images/test/snowflake-4.png");

   // drawer1.toggleShapeFill();


   // test maurer rose
   drawer1.background(0, 0, 0);
   drawer1.color(200, 110, 0);
   drawer1.maurerRose(500, 500, 400, 3, 47);
   drawer1.save("../images/test/maurer-rose.png");
   
   return 0;
}
