#include <iostream>
#include "canvas.h"
#include <cmath>

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
   
   drawer.color(255, 0, 0);
   drawer.rectangle(0.1 * WIDTH, 0.9 * WIDTH, 0.3 * HEIGHT, 0.7 * HEIGHT);

   drawer.color(0, 255, 0);
   drawer.circle(WIDTH / 2, HEIGHT / 2, HEIGHT / 3);

   drawer.color(255, 255, 255);
   for (int i = 1; i < 10; i++) {
      drawer.star(0.1 * i * WIDTH, 0.1 * HEIGHT, 0.05 * HEIGHT);
      drawer.star(0.1 * i * WIDTH, 0.9 * HEIGHT, 0.05 * HEIGHT);
   }

   drawer.rose(WIDTH / 2, HEIGHT / 2, HEIGHT / 4, 7, 4);

   drawer.save("../images/art/art-1.png");


   //--------------------------- ART 2 ---------------------------
   drawer.background(0.3 * 255, 0.59 * 255, 0.11 * 255);
   
   drawer.color(200, 200, 150);

   drawer.circle(WIDTH / 4, HEIGHT / 2, HEIGHT / 3);
   drawer.circle(WIDTH / 2, HEIGHT / 2, HEIGHT / 3);
   drawer.circle(3 * WIDTH / 4, HEIGHT / 2, HEIGHT / 3);

   drawer.color(150, 255, 200);
   int x, y;
   for (int n = 1; n <= 7; n++) {
      for (int d = 1; d <= 9; d++) {
         x = ((float) n / 8) * WIDTH;
         y = ((float) d / 10) * HEIGHT;
         std::cout << x << " " << y << std::endl;
         drawer.rose(x, y, 15, n, d);
      }
   }

   drawer.save("../images/art/art-2.png");



   //--------------------------- ART 3 ---------------------------
   drawer.background(0, 0, 0);

   for (int i = 1; i <= 100; i++) {
      drawer.color(rand() % 255, rand() % 255, rand() % 255);
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
         drawer.vertex(x, y);
      } else if (i % 3 == 2) {
         drawer.begin(TRIANGLES);
         x = rand() % WIDTH;
         y = rand() % HEIGHT;
         drawer.vertex(x, y);
         x = rand() % WIDTH;
         y = rand() % HEIGHT;
         drawer.vertex(x, y);
         x = rand() % WIDTH;
         y = rand() % HEIGHT;
         drawer.vertex(x, y);
      }
      drawer.end();
   }
   
   drawer.save("../images/art/art-3.png");
}

