#include "Image.hpp"

namespace prog
{
  Image::Image(int w, int h, const Color &fill)
  {
    _imageWidth = w;
    _imageHeight = h;

    pixels = new Color*[w];
    for (int i = 0; i < w; i++) {
        pixels[i] = new Color[h];
    }

    for (int i = 0; i < w; i++) {
      for (int j = 0; j < h; j++) {
        pixels[i][j] = fill;
      }
    }
  }
  Image::~Image()
  {
    for (int i = 0; i < _imageWidth; i++) {
      delete[] pixels[i];
      }
      
    delete[] pixels;
  }

  int Image::width() const
  {
    return _imageWidth;
  }
  int Image::height() const
  {
    return _imageHeight;
  }

  Color& Image::at(int x, int y)
  {
    return pixels[x][y];
  }

  const Color& Image::at(int x, int y) const
  {
    return pixels[x][y];
  }
}
