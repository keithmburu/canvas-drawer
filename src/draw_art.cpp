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
   
   // //--------------------------- ART 1 ---------------------------
   // drawer.background(0, 0, 0);
   // drawer.color(245, 245, 220);
   // drawer.rectangle(0.1 * WIDTH, 0.3 * HEIGHT, 0.9 * WIDTH, 0.7 * HEIGHT);
   // drawer.color(245, 245, 220);
   // drawer.circle(WIDTH / 2, HEIGHT / 2, HEIGHT / 3);
   // drawer.color(255, 255, 255);
   // for (int i = 1; i < 10; i++) {
   //    drawer.star(0.1 * i * WIDTH, 0.1 * HEIGHT, 0.05 * HEIGHT);
   //    drawer.star(0.1 * i * WIDTH, 0.9 * HEIGHT, 0.05 * HEIGHT);
   // }
   // drawer.color(255, 0, 255);
   // drawer.setLineWidth(2);
   // drawer.rose(WIDTH / 2, HEIGHT / 2, HEIGHT / 4, 7, 4);
   // drawer.glow();
   // drawer.gradient({0, 0, 255}, {0, 255, 255}, "horizontal", 0.1);
   // drawer.save("../images/art/art-1.png");
   // drawer.setLineWidth(1);


   // //--------------------------- ART 2 ---------------------------
   // drawer.background(0, 0, 0);
   // drawer.gradient({0, 0, 0}, {255, 0, 255}, "vertical", 1);
   // drawer.color(128, 0, 128);
   // drawer.circle(WIDTH / 4, HEIGHT / 2, HEIGHT / 3);
   // drawer.circle(WIDTH / 2, HEIGHT / 2, HEIGHT / 3);
   // drawer.circle(3 * WIDTH / 4, HEIGHT / 2, HEIGHT / 3);
   // drawer.color(64, 0, 64);
   // drawer.circle(WIDTH / 4, HEIGHT / 2, HEIGHT / 12);
   // drawer.circle(WIDTH / 2, HEIGHT / 2, HEIGHT / 12);
   // drawer.circle(3 * WIDTH / 4, HEIGHT / 2, HEIGHT / 12);
   // drawer.color(255, 100, 255);
   // int y = 0.5 * HEIGHT;
   // for (int n = 1; n <= 7; n++) {
   //    int d = n + 1;
   //    int x = ((float) n / 8) * WIDTH;
   //    drawer.rose(x, y, 20, n, d);
   // }
   // drawer.glow();
   // drawer.save("../images/art/art-2.png");



   // //--------------------------- ART 3 ---------------------------
   // drawer.background(0, 0, 0);
   // for (int i = 1; i <= 50; i++) {
   //    drawer.color(rand() % 256, rand() % 256, rand() % 256);
   //    int centerX = rand() % WIDTH;
   //    int centerY = rand() % HEIGHT;
   //    int a = rand() % min(WIDTH / 2, HEIGHT / 2);
   //    int n = rand() % 8;
   //    int d = rand() % 100;
   //    drawer.maurerRose(centerX, centerY, a, n, d);
   // }
   // drawer.glow();
   // drawer.save("../images/art/art-3.png");



   // //--------------------------- ART 4 ---------------------------
   // drawer.background(0, 0, 0);
   // for (int i = 1; i <= 100; i++) {
   //    drawer.color(rand() % 256, rand() % 256, rand() % 256);
   //    if (i % 3 == 0) {
   //       drawer.toggleShapeFill();
   //    }
   //    if (i % 2 == 0) {
   //       int xLeft = rand() % WIDTH;
   //       int yBottom = rand() % HEIGHT;
   //       int xRight = xLeft + rand() % (WIDTH - xLeft);
   //       int yTop = yBottom + rand() % (HEIGHT - yBottom);
   //       if (i % 3 == 0) {
   //          drawer.toggleShapeFill();
   //       }
   //       drawer.rectangle(xLeft, yBottom, xRight, yTop);
   //    } else {
   //       int centerX = rand() % WIDTH;
   //       int centerY = rand() % HEIGHT;
   //       int radius = rand() % (min({centerX, WIDTH - centerX, centerY, HEIGHT - centerY}) + 1);
   //       drawer.circle(centerX, centerY, radius);
   //    }
   //    if (i % 3 == 0) {
   //       drawer.toggleShapeFill();
   //    }
   // }
   // drawer.glow();
   // drawer.save("../images/art/art-4.png");



   // //--------------------------- ART 5 ---------------------------
   // drawer.background(0, 0, 0);
   // drawer.setBlendMode("difference");
   // for (int i = 1; i <= 200; i++) {
   //    drawer.color(rand() % 256, rand() % 256, rand() % 256);
   //    int centerX = rand() % WIDTH;
   //    int centerY = rand() % HEIGHT;
   //    int radius = rand() % max(1, WIDTH / 5);
   //    drawer.star(centerX, centerY, radius);
   // }
   // drawer.glow();
   // drawer.save("../images/art/art-5.png");
   // drawer.setBlendMode("replace");




   // //--------------------------- ART 6 ---------------------------
   // drawer.background(0, 0, 0);
   // drawer.color(255, 255, 255);
   // for (int i = 1; i <= 300; i++) {
   //    if (i % 5 <= 3) {
   //       drawer.begin(POINTS);
   //       int x = rand() % WIDTH;
   //       int y = rand() % HEIGHT;
   //       drawer.vertex(x, y);
   //       drawer.end();
   //    } else {
   //       int centerX = rand() % WIDTH;
   //       int centerY = max(20, rand() % HEIGHT);
   //       int radius = rand() % max(1, WIDTH / 75);
   //       drawer.star(centerX, centerY, radius);
   //    }
   // }
   // drawer.glow();
   // drawer.save("../images/art/art-6.png");



   // //--------------------------- ART 7 ---------------------------
   // WIDTH = 4 * WIDTH;
   // HEIGHT = 4 * HEIGHT;
   // Canvas drawer1(WIDTH, HEIGHT);
   // drawer1.color(255, 255, 255);
   // drawer1.background(50, 120, 180);
   // drawer1.setBlendMode("average");
   // drawer1.toggleShapeFill();
   // for (int i = 1; i <= 500; i++) {
   //    if (i % 5 <= 3) {
   //       drawer1.begin(POINTS);
   //       int x = rand() % WIDTH;
   //       int y = rand() % HEIGHT;
   //       drawer1.vertex(x, y);
   //       drawer1.end();
   //    } else {
   //       int xStart = rand() % WIDTH;
   //       int yStart = max(20, rand() % HEIGHT);
   //       int width = rand() % max(1, WIDTH / 20);
   //       drawer1.snowflake(xStart, yStart, width, 4);
   //    }
   // }
   // drawer1.save("../images/art/art-7.png");
   // drawer1.toggleShapeFill();
   // drawer1.setBlendMode("replace");
}

