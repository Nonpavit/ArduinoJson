// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <catch.hpp>

// TODO: replace magic value
static const size_t SIZE = JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(1) + 14;

template <typename TArray>
TArray buildArray() {
  TArray array;
  array.add(42);
  array.createNestedObject()["hello"] = std::string("world");
  return array;
}

template <typename TArray>
void validateArray(TArray& array) {
  CHECK(array.size() == 2);
  REQUIRE(array[0] == 42);
  REQUIRE(array[1].template is<JsonObject>());
  REQUIRE(array[1]["hello"] == std::string("world"));

  REQUIRE(SIZE == array.memoryUsage());
}

TEST_CASE("DynamicJsonArray::operator=()") {
  DynamicJsonArray array;
  array.add(666);
  array.add(666);

  SECTION("operator=(const DynamicJsonArray&)") {
    array = buildArray<DynamicJsonArray>();
    validateArray(array);
  }

  SECTION("operator=(const StaticJsonArray<N>&)") {
    array = buildArray<StaticJsonArray<SIZE> >();
    validateArray(array);
  }
}

TEST_CASE("StaticJsonArray::operator=()") {
  StaticJsonArray<SIZE> array;
  array.add(666);
  array.add(666);

  SECTION("operator=(const DynamicJsonArray&)") {
    array = buildArray<DynamicJsonArray>();
    validateArray(array);
  }

  SECTION("operator=(const StaticJsonArray<N>&)") {
    array = buildArray<StaticJsonArray<SIZE> >();
    validateArray(array);
  }

  SECTION("operator=(const StaticJsonArray<N+1>&)") {
    array = buildArray<StaticJsonArray<SIZE + 1> >();
    validateArray(array);
  }
}
