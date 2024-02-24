//
// Created by Kasper de Bruin on 19/02/2024.
//

#ifndef IMGUI_HBFLOAT_H
#define IMGUI_HBFLOAT_H

#include <iostream>
#include <stdexcept>
#include <stdexcept>

struct hb_u_float {
  hb_u_float(const float &value) : value(value) {
    if (value < 0) {
      const bool valid = (value >= 0);
      if (!valid) {
        throw std::invalid_argument("Value must be non-negative");
      }
    }
  }

  operator float() const {
    return value;
  }

  operator float &() {
    return value;
  }

  hb_u_float operator=(float v) {
    if (v < 0) {
      v = 0;
    }
    value = v;
    return *this;
  }

  hb_u_float operator>(float v) {
    return value > v;
  }

  hb_u_float operator<(float v) {
    return value < v;
  }

  hb_u_float operator>=(float v) {
    return value >= v;
  }

  hb_u_float operator<=(float v) {
    return value <= v;
  }

  hb_u_float operator==(float v) {
    return value == v;
  }

  hb_u_float operator!=(float v) {
    return value != v;
  }

  float value;
};

#endif //IMGUI_HBFLOAT_H
