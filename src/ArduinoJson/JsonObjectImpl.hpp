// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

#include "JsonArray.hpp"
#include "JsonObject.hpp"
#include "JsonObjectSubscript.hpp"

namespace ArduinoJson {

template <typename TStringRef>
inline JsonArray &JsonObject::createNestedArray_impl(TStringRef key) {
  JsonArray *array = new (_buffer) JsonArray(_buffer);
  if (!array) return JsonArray::invalid();
  set(key, *array);
  return *array;
}

template <typename TStringRef>
inline JsonObject &JsonObject::createNestedObject_impl(TStringRef key) {
  JsonObject *object = new (_buffer) JsonObject(_buffer);
  if (!object) return JsonObject::invalid();
  set(key, *object);
  return *object;
}

template <typename TStringRef, typename TValueRef>
inline bool JsonObject::set_impl(TStringRef key, TValueRef value) {
  iterator it = findKey<TStringRef>(key);
  if (it == end()) {
    it = Internals::List<JsonPair>::add();
    if (it == end()) return false;

    bool key_ok =
        Internals::ValueSetter<TStringRef>::set(_buffer, it->key, key);
    if (!key_ok) return false;
  }
  return Internals::ValueSetter<TValueRef>::set(_buffer, it->value, value);
}
}
