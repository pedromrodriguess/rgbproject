#ifndef __prog_Image_hpp__
#define __prog_Image_hpp__
#include "Color.hpp"
namespace prog
{
  class Image
  {
  private:
    // Deve ser mais isto que o professor quer. O vetor de vetores precisa de uma libraria <vector>. Talvez voces nao possam usar libs para alem das que ja foram included no template.
    // Desta maneira controlas melhor a alocacao de memoria que parece-me ser um ponto que vai ser avaliado.
    Color** pixels;
    int _imageWidth;
    int _imageHeight;
  public:
    Image(int w, int h, const Color &fill = {255, 255, 255});
    ~Image();
    int width() const;
    int height() const;
    Color &at(int x, int y);
    const Color &at(int x, int y) const;
  };
}
#endif
