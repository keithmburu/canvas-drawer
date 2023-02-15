/**
 * Implementation of image loading, modification, and saving
 * 
 * @file image.cpp
 * @author Keith Mburu
 * @version 2023-02-09
 */

#include "image.h"

#include <cassert>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../external/include/stb/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../external/include/stb/stb_image.h"

#include <cmath>
#include <algorithm>

namespace agl {


Image::Image() {
   this->_data = NULL;
   this->_width = 0;
   this->_height = 0;
}

Image::Image(int width, int height)  {
   this->_data = (unsigned char*) calloc(width * height * 3, sizeof(unsigned char*));
   this->_width = width;
   this->_height = height;
}

Image::Image(const Image& orig) {
   this->_data = (unsigned char*) malloc(sizeof(unsigned char*) * orig.width() * orig.height() * 3);
   for (int idx = 0; idx < orig.width() * orig.height(); idx++) {
      this->set(idx, orig.get(idx));
   }
   this->_width = orig.width();
   this->_height = orig.height();
}

Image& Image::operator=(const Image& orig) {
   if (&orig == this) {
      return *this;
   }
   free(this->_data);
   this->_data = orig.data();
   this->_width = orig.width();
   this->_height = orig.height();
   return *this;
}

std::ostream& operator<<(std::ostream& os, const Image& image) {
   os << "Width: " << image.width() << "\nHeight: " << image.height();
   int sumRed = 0.0f; int sumGreen = 0.0f; int sumBlue = 0.0f;
   for (int idx = 0; idx < image.width() * image.height(); idx++) {
      Pixel px = image.get(idx);
      sumRed += px.r;
      sumGreen += px.g;
      sumBlue += px.b;
   }
   int avgRed = sumRed / (image.width() * image.height());
   int avgGreen = sumGreen / (image.width() * image.height());
   int avgBlue = sumBlue / (image.width() * image.height());

   os << "\nAverage pixel color: " << avgRed << " " << avgGreen << " " << avgBlue << std::endl;
   return os;
}

Image::~Image() {
   std::cout << "~ Freeing memory" << std::endl;
   if (this->_data) {
      free(this->_data);
   }
}

int Image::width() const { 
   return this->_width;
}

int Image::height() const {
   return this->_height;
}

unsigned char* Image::data() const {
   return this->_data;
}

bool Image::load(const std::string& filename, bool flip) {
   std::cout << "Loading " << filename << std::endl;
   int n;
   this->_data = stbi_load(filename.c_str(), &this->_width, &this->_height, &n, 3);
   if (flip) {
      this->set(this->_width, this->_height, (this->flipHorizontal()).data());
   }
   if (!this->_data) {
      return false;
   } else {
      return true;
   }
}

bool Image::save(const std::string& filename, bool flip) const {
   std::cout << "Saving to " << filename << std::endl;
   unsigned char* data = this->_data;
   if (flip) {
      data = (this->flipHorizontal()).data();
   }
   int saved = stbi_write_png(filename.c_str(), this->_width, this->_height, 3, (void*) data, this->_width * 3);
   if (!saved) {
      std::cout << "Write error" << std::endl;
      return false;
   } else {
      return true;
   }
}

Pixel Image::get(int row, int col) const {
   int idx = ((row * this->_width) + col) * 3;
   return Pixel{ this->_data[idx], this->_data[idx + 1], this->_data[idx + 2] };
}

Pixel Image::get(int i) const {
   int idx = i * 3;
   return Pixel{ this->_data[idx], this->_data[idx + 1], this->_data[idx + 2] };
}

void Image::set(int row, int col, const Pixel& color) {
   int idx = ((row * this->_width) + col) * 3;
   this->_data[idx] = color.r;
   this->_data[idx + 1] = color.g;
   this->_data[idx + 2] = color.b;
}

void Image::set(int i, const Pixel& c) {
   int idx = i * 3;
   this->_data[idx] = c.r;
   this->_data[idx + 1] = c.g;
   this->_data[idx + 2] = c.b;
}

void Image::set(int width, int height, unsigned char* data) {
   if (this->_width == width && this->_height == height) {
      memcpy(this->_data, data, width * height * 3);
   } else {
      std::cerr << "set(): Dimensions must match!" << std::endl;
   }
}

Image Image::resize(int w, int h) const {
   std::cout << "Resizing to " << w << " by " << h << std::endl;
   Image result(w, h);
   for (int i2 = 0; i2 < result.height(); i2++) {
      for (int j2 = 0; j2 < result.width(); j2++) {
         int i1 = (int)((float) i2 / (result.height() - 1) * (this->_height - 1));
         int j1 = (int)((float) j2 / (result.width() - 1) * (this->_width - 1));
         Pixel px1 = this->get(i1, j1);
         result.set(i2, j2, px1);
      }
   }
   return result;
}

Image Image::flipHorizontal() const {
   std::cout << "Flipping horizontally" << std::endl;
   Image result(*this);
   for (int i = 0; i < result.height() / 2; i++) {
      for (int j = 0; j < result.width(); j++) {
         int newI = result.height() - i - 1;
         Pixel temp = this->get(newI, j);
         result.set(newI, j, this->get(i, j));
         result.set(i, j, temp);
      }
   }
   return result;
}

Image Image::flipVertical() const {
   std::cout << "Flipping vertically" << std::endl;
   Image result(*this);
   for (int i = 0; i < result.height(); i++) {
      for (int j = 0; j < result.width() / 2; j++) {
         int newJ = result.width() - j - 1;
         Pixel temp = this->get(i, newJ);
         result.set(i, newJ, this->get(i, j));
         result.set(i, j, temp);
      }
   }
   return result;
}

Image Image::rotate90() const {
   std::cout << "Rotating 90 degrees" << std::endl;
   Image result(*this);
   for (int i = 0; i < result.height(); i++) {
      for (int j = 0; j < result.width(); j++) {
         Pixel px1 = this->get(i, j);
         int newI = j;
         int newJ = result.height() - i;
         result.set(newI, newJ, px1);
      }
   }
   return result;
}

Image Image::subimage(int startx, int starty, int w, int h) const {
   std::cout << "Creating " << w << " by " << h << " subimage starting from (" << startx << ", " << starty << ")" << std::endl;
   Image sub(w, h);
   for (int i = starty; i < starty + h; i++) {
      for (int j = startx; j < startx + w; j++) { 
         int subIdx = ((i - starty) * sub.width()) + (j - startx);
         sub.set(subIdx, this->get(i, j));
      }
   }
   return sub;
}

void Image::replace(const Image& image, int startx, int starty) {
   std::cout << "Replacing with " << image.width() << " by " << image.height() << " image starting from (" << startx << ", " << starty << ")" << std::endl;
   int height = std::min(image.height(), this->_height - starty);
   int width = std::min(image.width(), this->_width - startx);
   for (int i = starty; i < starty + height; i++) {
      for (int j = startx; j < startx + width; j++) {
         Pixel px2 = image.get(i - starty, j - startx);
         this->set(i, j, px2);
      }
   }
}

Image Image::swirl() const {
   std::cout << "Swirling colors" << std::endl;
   Image result(*this);
   for (int idx = 0; idx < result.width() * result.height(); idx++) {
      Pixel px = this->get(idx);
      unsigned char red = px.r;
      px.r = px.g;
      px.g = px.b;
      px.b = red;
      result.set(idx, px);
   }
   return result;
}

Image Image::add(const Image& other) const {
   std::cout << "Adding" << std::endl;
   Image result(*this);
   for (int idx = 0; idx < result.width() * result.height(); idx++) {
      Pixel px1 = this->get(idx);
      Pixel px2 = other.get(idx);
      px1.r = std::min(px1.r + px2.r, 255);
      px1.g = std::min(px1.g + px2.g, 255);
      px1.b = std::min(px1.b + px2.b, 255);
      result.set(idx, px1);
   }
   return result;
}

Image Image::subtract(const Image& other) const {
   std::cout << "Subtracting" << std::endl;
   Image result(*this);
   for (int idx = 0; idx < result.width() * result.height(); idx++) {
      Pixel px1 = this->get(idx);
      Pixel px2 = other.get(idx);
      px1.r = std::max(px1.r - px2.r, 0);
      px1.g = std::max(px1.g - px2.g, 0);
      px1.b = std::max(px1.b - px2.b, 0);
      result.set(idx, px1);
   }
   return result;
}

Image Image::multiply(const Image& other) const {
   std::cout << "Multiplying" << std::endl;
   Image result(*this);
   for (int idx = 0; idx < result.width() * result.height(); idx++) {
      Pixel px1 = this->get(idx);
      Pixel px2 = other.get(idx);
      px1.r = std::min(px1.r * px2.r, 255);
      px1.g = std::min(px1.g * px2.g, 255);
      px1.b = std::min(px1.b * px2.b, 255);
      result.set(idx, px1);
   }
   return result;
}

Image Image::difference(const Image& other) const {
   std::cout << "Finding difference" << std::endl;
   Image result(*this);
   for (int idx = 0; idx < result.width() * result.height(); idx++) {
      Pixel px1 = this->get(idx);
      Pixel px2 = other.get(idx);
      px1.r = std::abs(px1.r - px2.r);
      px1.g = std::abs(px1.g - px2.g);
      px1.b = std::abs(px1.b - px2.b);
      result.set(idx, px1);
   }
   return result;
}

Image Image::lightest(const Image& other) const {
   std::cout << "Finding lightest" << std::endl;
   Image result(*this);
   for (int idx = 0; idx < result.width() * result.height(); idx++) {
      Pixel px1 = this->get(idx);
      Pixel px2 = other.get(idx);
      px1.r = std::max(px1.r, px2.r);
      px1.g = std::max(px1.g, px2.g);
      px1.b = std::max(px1.b, px2.b);
      result.set(idx, px1);
   }
   return result;
}

Image Image::darkest(const Image& other) const {
   std::cout << "Finding darkest" << std::endl;
   Image result(*this);
   for (int idx = 0; idx < result.width() * result.height(); idx++) {
      Pixel px1 = this->get(idx);
      Pixel px2 = other.get(idx);
      px1.r = std::min(px1.r, px2.r);
      px1.g = std::min(px1.g, px2.g);
      px1.b = std::min(px1.b, px2.b);
      result.set(idx, px1);
   }
   return result;
}

Image Image::gammaCorrect(float gamma) const {
   std::cout << "Gamma correcting with gamma = " << gamma << std::endl;
   Image result(*this);
   for (int idx = 0; idx < result.width() * result.height(); idx++) {
      Pixel px = result.get(idx);
      px.r = pow(px.r / 255.0, 1 / gamma) * 255;
      px.g = pow(px.g / 255.0, 1 / gamma) * 255;
      px.b = pow(px.b / 255.0, 1 / gamma) * 255;
      result.set(idx, px);
   }
   return result;
}

Image Image::alphaBlend(const Image& other, float alpha) const {
   std::cout << "Blending with alpha = " << alpha << std::endl;
   Image result(*this);
   for (int idx = 0; idx < result.width() * result.height(); idx++) {
      Pixel px1 = this->get(idx);
      Pixel px2 = other.get(idx);
      px1.r = px1.r * (1 - alpha) + px2.r * alpha;
      px1.g = px1.g * (1 - alpha) + px2.g * alpha;
      px1.b = px1.b * (1 - alpha) + px2.b * alpha;
      result.set(idx, px1);
   }
   return result;
}

Image Image::invert() const {
   std::cout << "Inverting" << std::endl;
   Image result(*this);
   for (int idx = 0; idx < result.width() * result.height(); idx++) {
      Pixel px = this->get(idx);
      px.r = 255 - px.r;
      px.g = 255 - px.g;
      px.b = 255 - px.b;
      result.set(idx, px);
   }
   return result;
}

Image Image::grayscale() const {
   std::cout << "Making grayscale" << std::endl;
   Image result(*this);
   for (int idx = 0; idx < result.width() * result.height(); idx++) {
      Pixel px = this->get(idx);
      float avg = ((0.3 * px.r) + (0.59 * px.g) + (0.11 * px.b)) / 3;
      px.r = px.g = px.b = avg;
      result.set(idx, px);
   }
   return result;
}

Image Image::colorJitter(int maxSize) const {
   std::cout << "Jittering colors with max size = " << maxSize << std::endl;
   Image result(*this);
   int Rjitter, Gjitter, Bjitter, Rsign, Gsign, Bsign;
   for (int idx = 0; idx < this->_width * this->_height; idx++) {
      Pixel px = this->get(idx);
      Rsign = -1 * (rand() % 2); 
      Rjitter = Rsign * (rand() % maxSize);
      px.r = std::max(std::min(px.r + Rjitter, 255), 0);
      Gsign = -1 * (rand() % 2); 
      Gjitter = Gsign * (rand() % maxSize);
      px.g = std::max(std::min(px.g + Gjitter, 255), 0);
      Bsign = -1 * (rand() % 2); 
      Bjitter = Bsign * (rand() % maxSize);
      px.b = std::max(std::min(px.b + Bjitter, 255), 0);
      result.set(idx, px);
   }
   return result;
}

Image Image::bitmap(int size) const {
   Image result(*this);
   std::cout << "Creating bitmap with size = " << size << std::endl;
   int numPixels = size * size;
   for (int i = 0; i < this->_height; i+=size) {
      for (int j = 0; j < this->_width; j+=size) {
         int sumRed, sumGreen, sumBlue;
         sumRed = sumGreen = sumBlue = 0;
         for (int k = i; k < i + size && k < this->_height; k++) {
            for (int l = j; l < j + size && l < this->_width; l++) {
               Pixel px = this->get(k, l);
               sumRed += px.r;
               sumGreen += px.g;
               sumBlue += px.b;
            }
         } 
         unsigned char avgRed = sumRed / numPixels;
         unsigned char avgGreen = sumGreen / numPixels;
         unsigned char avgBlue = sumBlue / numPixels;
         for (int k = i; k < i + size; k++) {
            for (int l = j; l < j + size; l++) {
               result.set(k, l, {avgRed, avgGreen, avgBlue});
            }
         }
      }
   }
   return result;
}

Image Image::fill(const Pixel& a, const Pixel& b) {
   std::cout << "Filling color " << (int) a.r << " " << (int) a.g << " " << (int) a.b << " with color " << (int) b.r << " " << (int) b.g << " " << (int) b.b << std::endl;
   Image result(*this);
   for (int idx = 0; idx < this->_width * this->_height; idx++) {
      Pixel px = this->get(idx);
      int diffR = abs(a.r - px.r);
      int diffG = abs(a.g - px.g);
      int diffB = abs(a.b - px.b);
      if (diffR < 100 && diffG < 100 && diffB < 100) {
         px.r = b.r;
         px.g = b.g;
         px.b = b.b;
      }
      result.set(idx, px);
   }
   return result;
}

Image Image::blur(int iters) const {
   std::cout << "Blurring" << std::endl;
   Image result(*this);
   int KERNEL_SIZE = 3;
   int offset = KERNEL_SIZE / 2;
   for (int iter = 0; iter < iters; iter++) {
      for (int i = 0; i < this->_height; i++) {
         for (int j = 0; j < this->_width; j++) {
            int sumNeighborsRed, sumNeighborsGreen, sumNeighborsBlue;
            sumNeighborsRed = sumNeighborsGreen = sumNeighborsBlue = 0;
            for (int k = i - offset; k <= i + offset; k++) {
               for (int l = j - offset; l <= j + offset; l++) {
                  Pixel px;
                  if (0 <= k && k < this->_height && 0 <= l && l < this->_width) {
                     px = this->get(k, l);
                  } else {
                     px = this->get(i, j);
                  }
                  sumNeighborsRed += px.r;
                  sumNeighborsGreen += px.g;
                  sumNeighborsBlue += px.b;
               }
            }
            unsigned char avgNeighborsRed = sumNeighborsRed / (KERNEL_SIZE * KERNEL_SIZE); 
            unsigned char avgNeighborsGreen = sumNeighborsGreen / (KERNEL_SIZE * KERNEL_SIZE); 
            unsigned char avgNeighborsBlue = sumNeighborsBlue / (KERNEL_SIZE * KERNEL_SIZE);
            result.set(i, j, {avgNeighborsRed, avgNeighborsGreen, avgNeighborsBlue});
         }
      }
   }
   return result;
}

Image Image::blurGaussian() const {
   std::cout << "Blurring with Gaussian blur" << std::endl;
   Image result(*this);
   int KERNEL_SIZE = 17;
   float STD_DEV = 10.0;
   float kernel[KERNEL_SIZE][KERNEL_SIZE];
   int offset = KERNEL_SIZE / 2;
   float kernelSum = 0.0;
   // creating Gaussian kernel
   for (int k = -offset; k <= offset; k++) {
     for (int l = -offset; l <= offset; l++) {
         float exponent = -(k * k + l * l) / (2 * STD_DEV * STD_DEV);
         kernel[k + offset][l + offset] = (1 / (2 * M_PI * STD_DEV * STD_DEV)) * exp(exponent);
         kernelSum += kernel[k + offset][l + offset];
     }
   }
   // normalizing
   for (int i = 0; i < KERNEL_SIZE; i++) {
      for (int j = 0; j < KERNEL_SIZE; j++) {
         kernel[i][j] /= kernelSum;
      }
   }

   int ITERS = 3;
   for (int iter = 0; iter < ITERS; iter++) {
      for (int i = 0; i < result.height(); i++) {
         for (int j = 0; j < result.width(); j++) {
            float avgNeighborsRed, avgNeighborsGreen, avgNeighborsBlue;
            avgNeighborsRed = avgNeighborsGreen = avgNeighborsBlue = 0;
            for (int k = i - offset; k <= i + offset; k++) {
               for (int l = j - offset; l <= j + offset; l++) {
                  Pixel px;
                  if (0 <= k && k < result.height() && 0 <= l && l < result.width()) {
                     px = result.get(k, l);
                  } else {
                     px = result.get(i, j);
                  }
                  avgNeighborsRed += px.r * kernel[k - (i - offset)][l - (j - offset)];
                  avgNeighborsGreen += px.g * kernel[k - (i - offset)][l - (j - offset)];
                  avgNeighborsBlue += px.b * kernel[k - (i - offset)][l - (j - offset)];
               }
            }
            result.set(i, j, {(unsigned char) avgNeighborsRed, (unsigned char) avgNeighborsGreen, (unsigned char) avgNeighborsBlue});
         }
      }
   }
   return result;
}

Image Image::glow() const {
   std::cout << "Applying glow effect" << std::endl;
   Image extractedWhite(*this);
   Image white(this->_width, this->_height);
   Image result(*this);
   float threshold = 0.7;
   for (int idx = 0; idx < this->_width * this->_height; idx++) {
      Pixel px = this->get(idx);
      unsigned char c = px.r * 0.3f + px.g * 0.59f + px.b * 0.11f;
      float intensity = c / 255.0f;
      if (intensity > threshold) { 
         extractedWhite.set(idx, {255, 255, 255});
      } else {
         extractedWhite.set(idx, {0, 0, 0});
      }
      white.set(idx, {255, 255, 255});
   }
   extractedWhite.save("../demo/white-extractedWhite.png");
   Image whiteBlurred = extractedWhite.blurGaussian();
   whiteBlurred.save("../demo/white-blur.png");
   for (int idx = 0; idx < result.width() * result.height(); idx++) {
      Pixel alphaPx = whiteBlurred.get(idx);
      unsigned char c = alphaPx.r * 0.3f + alphaPx.g * 0.59f + alphaPx.b * 0.11f;
      float alpha = c / 255.0f;
      Pixel px1 = this->get(idx);
      Pixel px2 = white.get(idx);
      px1.r = px1.r * (1 - alpha) + px2.r * alpha;
      px1.g = px1.g * (1 - alpha) + px2.g * alpha;
      px1.b = px1.b * (1 - alpha) + px2.b * alpha;
      result.set(idx, px1);
   }
   return result;
}

Image Image::border(const Pixel& c) const {
   std::cout << "Drawing borders with color " << (int) c.r << " " << (int) c.g << " " << (int) c.b << std::endl;
   Image result(*this);
   // top and bottom
   for (int j = 0; j < result.width(); j++) {
      result.set(0, j, c);
      result.set(result.height() - 1, j, c);
   }
   // left and right
   for (int i = 1; i < result.height() - 1; i++) {
      result.set(i, 0, {255, 255, 255});
      result.set(i, result.width() - 1, {255, 255, 255});
   }
   return result;
}

Image Image::sobel() const {
   std::cout << "Applying sobel operator" << std::endl;
   Image result(*this);
   for (int idx = 0; idx < this->_width * this->_height; idx++) {
      Pixel px = this->get(idx);
      int avg = (px.r + px.g + px.b) / 3;
      px.r = px.g = px.b = avg;
      result.set(idx, px);
   }
   int Gx[3][3] = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};
   int Gy[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
   for (int i = 0; i < this->_height; i++) {
      for (int j = 0; j < this->_width; j++) {
         int sumNeighborsRedX, sumNeighborsGreenX, sumNeighborsBlueX, sumNeighborsRedY, sumNeighborsGreenY, sumNeighborsBlueY;
         sumNeighborsRedX = sumNeighborsGreenX = sumNeighborsBlueX = sumNeighborsRedY = sumNeighborsGreenY = sumNeighborsBlueY = 0;
         for (int k = i - 1; k <= i + 1; k++) {
            if (0 <= k && k < this->_height) {
               for (int l = j - 1; l <= j + 1; l++) {
                  if (0 <= l && l < this->_width) {
                     Pixel px = this->get(k, l);
                     sumNeighborsRedX += px.r * Gx[k - (i - 1)][l - (j - 1)];
                     sumNeighborsGreenX += px.g * Gx[k - (i - 1)][l - (j - 1)];
                     sumNeighborsBlueX += px.b * Gx[k - (i - 1)][l - (j - 1)];
                     sumNeighborsRedY += px.r * Gy[k - (i - 1)][l - (j - 1)];
                     sumNeighborsGreenY += px.g * Gy[k - (i - 1)][l - (j - 1)];
                     sumNeighborsBlueY += px.b * Gy[k - (i - 1)][l - (j - 1)];
                  } 
               }
            }
         }
         unsigned char GRed = std::min((int) sqrt(pow(sumNeighborsRedX, 2) + pow(sumNeighborsRedY, 2)), 255);
         unsigned char GGreen = std::min((int) sqrt(pow(sumNeighborsGreenX, 2) + pow(sumNeighborsGreenY, 2)), 255);
         unsigned char GBlue = std::min((int) sqrt(pow(sumNeighborsBlueX, 2) + pow(sumNeighborsBlueY, 2)), 255);
         result.set(i, j, {GRed, GGreen, GBlue});
      }
   }
   return result;
}

Image Image::glitch() const {
   std::cout << "Applying glitch effect" << std::endl;
   Image result(this->_width, this->_height);
   int offset = this->_width / 10;
   for (int i = 0; i < this->_height; i++) {
      for (int j = 0; j < this->_width; j++) {
         int newJ = j + (pow(-1, i % 2) * offset);
         if (newJ >= 0 && newJ < this->_width) {
            result.set(i, newJ, this->get(i, j));
         }
      }
   }
   return result;
}

Image Image::painterly() const {
   std::cout << "Applying painterly effect" << std::endl;
  return (((this->blur()).sobel()).alphaBlend(*this, 0.2)).brighten(20);
}

Image Image::distort(const std::string& orientation) const {
   std::cout << "Applying " << orientation << " distortion effect" << std::endl;
   Image result(*this);
   if (orientation != "vertical" && orientation != "horizontal") {
      std::cerr << "Invalid orientation argument!" << std::endl;
      exit(1);
   }
   for (int i = 0; i < this->_height; i++) {
      for (int j = 0; j < this->_width; j++) {
         int newI = i; int newJ = j;
         if (orientation == "vertical") {
            newI = (i + (int)(sin(((float) j / this->_width) * 8 * M_PI) * 20) + this->_height) % this->_height;
         } else if (orientation == "horizontal") {
            newJ = (j + (int)(cos(((float) i / this->_height) * 8 * M_PI) * 20) + this->_width) % this->_width;
         } 
         result.set(newI, newJ, this->get(i, j));
      }
   }
   return result;
}

Image Image::gradient(const Pixel& px1, const Pixel& px2, const std::string& orientation) const {
   std::cout << "Applying " << orientation << " color gradient with colors " << (int) px1.r << " " << (int) px1.g << " " << (int) px1.b << " and " << (int) px2.r << " " << (int) px2.g << " " << (int) px2.b << std::endl;
   Image filter(*this);
   if (orientation != "vertical" && orientation != "horizontal") {
      std::cerr << "Invalid orientation argument!" << std::endl;
      exit(1);
   }
   float progress;
   unsigned char r, g, b;
   for (int i = 0; i < this->_height; i++) {
      if (orientation == "vertical") {
         progress = (float) i / this->_height;
      }
      for (int j = 0; j < this->_width; j++) {
         if (orientation == "horizontal") {
            progress = (float) j / this->_width;
         }
         r = (unsigned char) (px1.r * progress + px2.r * (1 - progress));
         g = (unsigned char) (px1.g * progress + px2.g * (1 - progress));
         b = (unsigned char) (px1.b * progress + px2.b * (1 - progress));
         filter.set(i, j, {r, g, b});
      }
   }
   std::cout << filter << std::endl;
   return this->alphaBlend(filter, 0.5);
}

Image Image::sharpen() const {
   std::cout << "Sharpening" << std::endl;
   return this->add(this->subtract(this->blur()));
}

Image Image::brighten(int percentage) const {
   std::cout << "Brightening by " << percentage << " percent" << std::endl;
   Image result(*this);
   float factor = (100.0f + percentage) / 100.0f;
   for (int idx = 0; idx < this->_width * this->_height; idx++) {
      Pixel px = this->get(idx);
      px.r = std::min((int) (px.r * factor), 255);
      px.g = std::min((int) (px.g * factor), 255);
      px.b = std::min((int) (px.b * factor), 255);
      result.set(idx, px);
   }
   return result;
}

Image Image::dim(int percentage) const {
   std::cout << "Dimming by " << percentage << " percent" << std::endl;
   Image result(*this);
   float factor = (100.0f - percentage) / 100.0f;
   for (int idx = 0; idx < this->_width * this->_height; idx++) {
      Pixel px = this->get(idx);
      px.r = (int) (px.r * factor);
      px.g = (int) (px.g  * factor);
      px.b = (int) (px.b  * factor);
      result.set(idx, px);
   }
   return result;
}

Image Image::deepFry() const {
   std::cout << "Applying deep fried effect" << std::endl;
   Image result(*this);
   for (int idx = 0; idx < this->_width * this->_height; idx++) {
      Pixel px = this->get(idx);
      px.r = std::min(px.r * 2, 255);
      px.r = (int) (pow(px.r / 255.0, 15) * 255);
      px.g = std::min(px.g * 2, 255);
      px.g = (int) (pow(px.g / 255.0, 15) * 255);
      px.b = std::min(px.b * 2, 255);
      px.b = (int) (pow(px.b / 255.0, 15) * 255);
      result.set(idx, px);
   }
   return result.sharpen().colorJitter(100);
}

}  // namespace agl
