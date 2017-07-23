// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

#include "../JsonArray.hpp"
#include "../JsonObject.hpp"
#include "../JsonVariant.hpp"
#include "../Polyfills/Move.hpp"
#include "../StringTraits/StringTraits.hpp"
#include "../TypeTraits/EnableIf.hpp"
#include "JsonBuffer.hpp"

namespace ArduinoJson {
namespace Internals {

template <typename TSourceRef, typename Enable = void>
struct ValueSetter {
  template <typename TDestination>
  static bool set(JsonBuffer*, TDestination& destination, TSourceRef source) {
    destination = source;
    return true;
  }
};

template <typename TSourceRef>
struct ValueSetter<TSourceRef, typename TypeTraits::EnableIf<StringTraits<
                                   TSourceRef>::should_duplicate>::type> {
  template <typename TDestination>
  static bool set(JsonBuffer* buffer, TDestination& destination,
                  TSourceRef source) {
    const char* copy = buffer->strdup(source);
    if (!copy) return false;
    destination = copy;
    return true;
  }
};

template <typename TSourceRef>
struct ValueSetter<TSourceRef, typename TypeTraits::EnableIf<!StringTraits<
                                   TSourceRef>::should_duplicate>::type> {
  template <typename TDestination>
  static bool set(JsonBuffer*, TDestination& destination, TSourceRef source) {
    // unsigned char* -> char*
    destination = reinterpret_cast<const char*>(source);
    return true;
  }
};

template <>
struct ValueSetter<const JsonVariant&, void> {
  template <typename TDestination>
  static bool set(JsonBuffer* buffer, TDestination& destination,
                  const JsonVariant& source) {
    return destination.clone(buffer, source);
  }
};

template <typename T>
struct ValueSetter<const Polyfills::Moving<T>&, void> {
  template <typename TDestination>
  static bool set(JsonBuffer*, TDestination& destination,
                  const Polyfills::Moving<T>& source) {
    destination = source;
    return true;
  }
};
}
}
