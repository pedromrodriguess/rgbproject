#include "Color.hpp"

namespace prog {
    Color::Color() {
        _red = 0;
        _green = 0;
        _blue = 0;
    }
    Color::Color(const Color& other) {
        _red = other._red;
        _green = other._green;
        _blue = other._blue;
    }
    Color::Color(rgb_value red, rgb_value green, rgb_value blue) {
        _red = red;
        _green = green;
        _blue = blue;
    }
    rgb_value Color::red() const {
        return _red;
    }
    rgb_value Color::green() const {
        return _green;
    }
    rgb_value Color::blue() const {
        return _blue;
    }

    rgb_value& Color::red()  {
        return _red;
    }
    rgb_value& Color::green()  {
      return _green;
    }
    rgb_value& Color::blue()  {
      return _blue;
    }
}
