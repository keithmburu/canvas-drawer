/**
 * Class that loads images, performs various operations on them, and 
 * saves them
 * 
 * @file image.h
 * @author Keith Mburu
 * @version 2023-02-09
 */

#ifndef AGL_IMAGE_H_
#define AGL_IMAGE_H_

#include <iostream>
#include <string>
#include <vector>

namespace agl {

/**
 * @brief Holder for a RGB color
 */
struct Pixel {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

/**
 * @brief Implements loading, modifying, and saving RGB images
 */
class Image {
 public:
  Image();
  Image(int width, int height);
  Image(const Image& orig);
  Image& operator=(const Image& orig);
  friend std::ostream& operator<<(std::ostream& os, const Image& image);

  virtual ~Image();

  /** 
   * @brief Load the given filename 
   * @param filename The file to load, relative to the running directory
   * @param flip Whether the file should flipped vertally when loaded
   * 
   * @verbinclude sprites.cpp
   */
  bool load(const std::string& filename, bool flip = false);

  /** 
   * @brief Save the image to the given filename (.png)
   * @param filename The file to load, relative to the running directory
   * @param flip Whether the file should flipped vertally before being saved
   */
  bool save(const std::string& filename, bool flip = false) const;

  /** @brief Return the image width in pixels
   */
  int width() const;

  /** @brief Return the image height in pixels
   */
  int height() const;

  /** 
   * @brief Return the RGB data
   *
   * Data will have size width * height * 4 (RGB)
   */
  unsigned char* data() const;

  /**
   * @brief Replace image RGB data
   * @param width The new image width
   * @param height The new image height
   *
   * This call will replace the old data with the new data. Data should 
   * match the size width * height * 3
   */
  void set(int width, int height, unsigned char* data);

  /**
   * @brief Get the pixel at index (row, col)
   * @param row The row (value between 0 and height)
   * @param col The col (value between 0 and width)
   *
   * Pixel colors are unsigned char, e.g. in range 0 to 255
   */ 
  Pixel get(int row, int col) const;

  /**
   * @brief Set the pixel RGBA color at index (row, col)
   * @param row The row (value between 0 and height)
   * @param col The col (value between 0 and width)
   *
   * Pixel colors are unsigned char, e.g. in range 0 to 255
   */ 
  void set(int row, int col, const Pixel& color);

  /**
 * @brief Set the pixel RGB color at index i
 * @param i The index (value between 0 and width * height)
 *
 * Pixel colors are unsigned char, e.g. in range 0 to 255
 */
  Pixel get(int i) const;

  /**
 * @brief Set the pixel RGB color at index i
 * @param i The index (value between 0 and width * height)
 *
 * Pixel colors are unsigned char, e.g. in range 0 to 255
 */
  void set(int i, const Pixel& c);


  // resize the image
  Image resize(int width, int height) const;

  // flip around the horizontal midline
  Image flipHorizontal() const;

  // flip around the vertical midline
  Image flipVertical() const;

  // rotate the Image 90 degrees
  Image rotate90() const;

  // Return a sub-Image having the given top,left coordinate and (width, height)
  Image subimage(int x, int y, int w, int h) const;

  // Replace the portion starting at (row, col) with the given image
  // Clamps the image if it doesn't fit on this image
  void replace(const Image& image, int x, int y);

  // swirl the colors 
  Image swirl() const;

  // Apply the following calculation to the pixels in 
  // our image and the given image:
  //    result.pixel = this.pixel + other.pixel
  // Assumes that the two images are the same size
  Image add(const Image& other) const;

  // Apply the following calculation to the pixels in 
  // our image and the given image:
  //    result.pixel = this.pixel - other.pixel
  // Assumes that the two images are the same size
  Image subtract(const Image& other) const;

  // Apply the following calculation to the pixels in 
  // our image and the given image:
  //    result.pixel = this.pixel * other.pixel
  // Assumes that the two images are the same size
  Image multiply(const Image& other) const;

  // Apply the following calculation to the pixels in 
  // our image and the given image:
  //    result.pixel = abs(this.pixel - other.pixel)
  // Assumes that the two images are the same size
  Image difference(const Image& other) const;

  // Apply the following calculation to the pixels in 
  // our image and the given image:
  //    result.pixel = max(this.pixel, other.pixel)
  // Assumes that the two images are the same size
  Image lightest(const Image& other) const;

  // Apply the following calculation to the pixels in 
  // our image and the given image:
  //    result.pixel = min(this.pixel, other.pixel)
  // Assumes that the two images are the same size
  Image darkest(const Image& other) const;

  // Apply gamma correction
  Image gammaCorrect(float gamma) const;

  // Apply the following calculation to the pixels in 
  // our image and the given image:
  //    this.pixels = this.pixels * (1-alpha) + other.pixel * alpha
  // Assumes that the two images are the same size
  Image alphaBlend(const Image& other, float amount) const;

  // Replace each pixel value "x" with 255-x
  Image invert() const;

  // Convert the image to grayscale
  Image grayscale() const;

  // Randomly tweak pixel values with tweak value < maxSize
  Image colorJitter(int maxSize) const;

  // return a bitmap version of this image
  Image bitmap(int size) const;

  // Fill pixels of certain color with another color
  Image fill(const Pixel& a, const Pixel& b);

  // Apply simple box blur to image 
  Image blur(int iters = 1) const;

  // Apply gaussian blur to image 
  Image blurGaussian() const;

  // Apply glowing texture to image
  Image glow() const;

  // Color the pixels at the edges of image
  Image border(const Pixel& c) const;

  // Accentuate edges in image
  Image sobel() const;

  // Generate corrupted version of image
  Image glitch() const;

  // Make image look more like a painting 
  Image painterly() const;

  // Displace pixels based on sine and cosine
  Image distort(const std::string& orientation) const;

  // Apply color gradient
  Image gradient(const Pixel& px1, const Pixel& px2, const std::string& orientation) const;

  // Emphasize edges in image using unsharp mask filtering
  Image sharpen() const;

  // Increase all pixel values by fixed percentage
  Image brighten(int percentage) const;

  // Decrease all pixel values by fixed percentage
  Image dim(int percentage) const;

  // Apply excessive saturation, sharpening, and grainy texture
  Image deepFry() const;

 private:
   // char array for storing pixel data
   unsigned char* _data;
   // number of pixels in the image's x dimension
   int _width;
   // number of pixels in the image's y dimension
   int _height;
};
}  // namespace agl
#endif  // AGL_IMAGE_H_
