// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

namespace ArduinoJson {
namespace Polyfills {

template <typename T>
class Moving {
 public:
  Moving(T& ref) : _ref(ref) {}

  operator T&() const {
    return _ref;
  }

 private:
  T& _ref;

  Moving<T>& operator=(const Moving<T>&);
};

template <typename T>
Moving<T> move(T& ref) {
  return Moving<T>(ref);
}
}
}
