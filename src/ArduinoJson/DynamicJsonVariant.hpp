// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

#include "Data/DynamicJsonBuffer.hpp"
#include "Data/ValueSetter.hpp"
#include "JsonVariant.hpp"

namespace ArduinoJson {

class DynamicJsonVariant : public JsonVariant {
  Internals::DynamicJsonBuffer _buffer;

 public:
  DynamicJsonVariant() {}

  DynamicJsonVariant(const DynamicJsonVariant& other) : JsonVariant() {
    clone(&_buffer, other);
  }

  DynamicJsonVariant(const JsonVariant& value) {
    clone(&_buffer, value);
  }

  DynamicJsonVariant& operator=(const DynamicJsonVariant& other) {
    // _buffer.clear();  // TODO
    clone(&_buffer, other);
    return *this;
  }

  DynamicJsonVariant& operator=(const JsonVariant& value) {
    // _buffer.clear();  // TODO
    clone(&_buffer, value);
    return *this;
  }

  Internals::DynamicJsonBuffer& buffer() {
    return _buffer;
  }
};
}
