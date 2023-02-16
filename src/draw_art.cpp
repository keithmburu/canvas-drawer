#include <iostream>
#include "canvas.h"
#include <cmath>
#include <algorithm>

using namespace std;
using namespace agl;

int main(int argc, char** argv)
{
   int WIDTH = 640;
   int HEIGHT = 380;
   Canvas drawer(WIDTH, HEIGHT);
   
   //--------------------------- ART 1 ---------------------------
   drawer.background(255, 255, 255);
   drawer.gradient({0, 0, 255}, {0, 255, 255}, "horizontal", 0.5);
   
   drawer.color(245, 245, 220);
   drawer.rectangle(0.1 * WIDTH, 0.9 * WIDTH, 0.3 * HEIGHT, 0.7 * HEIGHT);

   drawer.color(245, 245, 220);
   drawer.circle(WIDTH / 2, HEIGHT / 2, HEIGHT / 3);

   drawer.color(255, 255, 255);
   for (int i = 1; i < 10; i++) {
      drawer.star(0.1 * i * WIDTH, 0.1 * HEIGHT, 0.05 * HEIGHT);
      drawer.star(0.1 * i * WIDTH, 0.9 * HEIGHT, 0.05 * HEIGHT);
   }

   drawer.color(255, 0, 255);
   drawer.rose(WIDTH / 2, HEIGHT / 2, HEIGHT / 4, 7, 4);

   drawer.save("../images/art/art-1.png");


   //--------------------------- ART 2 ---------------------------
   drawer.background(0, 0, 0);
   drawer.gradient({0, 0, 0}, {255, 0, 255}, "vertical", 1);
   
   drawer.color(128, 0, 128);
   drawer.circle(WIDTH / 4, HEIGHT / 2, HEIGHT / 3);
   drawer.circle(WIDTH / 2, HEIGHT / 2, HEIGHT / 3);
   drawer.circle(3 * WIDTH / 4, HEIGHT / 2, HEIGHT / 3);

   int x, y;
   for (int n = 1; n <= 7; n++) {
      for (int d = 1; d <= 9; d++) {
         x = ((float) n / 8) * WIDTH;
         y = ((float) d / 10) * HEIGHT;
         drawer.rose(x, y, 15, n, d);
      }
   }
   drawer.save("../images/art/art-2.png");



   //--------------------------- ART 3 ---------------------------
   drawer.background(0, 0, 0);

   for (int i = 1; i <= 500; i++) {
      drawer.color(rand() % 256, rand() % 256, rand() % 256);
      if (i % 3 == 0) {
         drawer.begin(POINTS);
         x = rand() % WIDTH;
         y = rand() % HEIGHT;
         drawer.vertex(x, y);
      } else if (i % 3 == 1) {
         drawer.begin(LINES);
         x = rand() % WIDTH;
         y = rand() % HEIGHT;
         drawer.vertex(x, y);
         x = rand() % WIDTH;
         y = rand() % HEIGHT;
         drawer.color(rand() % 256, rand() % 256, rand() % 256);
         drawer.vertex(x, y);
      } else if (i % 3 == 2) {
         drawer.begin(TRIANGLES);
         x = rand() % WIDTH;
         y = rand() % HEIGHT;
         drawer.vertex(x, y);
         x = rand() % WIDTH;
         y = rand() % HEIGHT;
         drawer.color(rand() % 256, rand() % 256, rand() % 256);
         drawer.vertex(x, y);
         x = rand() % WIDTH;
         y = rand() % HEIGHT;
         drawer.color(rand() % 256, rand() % 256, rand() % 256);
         drawer.vertex(x, y);
      }
      drawer.end();
   }
   
   drawer.save("../images/art/art-3.png");



   //--------------------------- ART 4 ---------------------------
   drawer.background(0, 0, 0);

   int xLeft, yBottom, xRight, yTop, centerX, centerY, radius;
   for (int i = 1; i <= 100; i++) {
      drawer.color(rand() % 256, rand() % 256, rand() % 256);
      if (i % 2 == 0) {
         xLeft = rand() % WIDTH;
         yBottom = rand() % HEIGHT;
         xRight = xLeft + rand() % (WIDTH - xLeft);
         yTop = yBottom + rand() % (HEIGHT - yBottom);
         drawer.rectangle(xLeft, yBottom, xRight, yTop);
      } else {
         centerX = rand() % WIDTH;
         centerY = rand() % HEIGHT;
         radius = rand() % (min({centerX, WIDTH - centerX, centerY, HEIGHT - centerY}) + 1);
         drawer.circle(centerX, centerY, radius);
      }
   }
   
   drawer.save("../images/art/art-4.png");



   //--------------------------- ART 5 ---------------------------
   drawer.background(0, 0, 0);

   drawer.changeBlendMode("average");
   int a, n, d;
   for (int i = 1; i <= 100; i++) {
      drawer.color(rand() % 256, rand() % 256, rand() % 256);
      if (i % 2 == 0) {
         centerX = rand() % WIDTH;
         centerY = rand() % HEIGHT;
         radius = rand() % (min({centerX, WIDTH - centerX, centerY, HEIGHT - centerY}) + 1);
         drawer.star(centerX, centerY, radius);
      } else {
         centerX = rand() % WIDTH;
         centerY = rand() % HEIGHT;
         a = rand() % min(WIDTH / 2, HEIGHT / 2);
         n = rand() % 8;
         d = rand() % 10;
         drawer.rose(centerX, centerY, a, n, d);
      }
   }

   drawer.save("../images/art/art-5.png");
   drawer.changeBlendMode("set");
}

