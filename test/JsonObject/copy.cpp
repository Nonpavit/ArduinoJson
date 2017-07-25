// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <catch.hpp>

static const size_t SIZE = JSON_OBJECT_SIZE(2) + JSON_ARRAY_SIZE(1) + 6;

template <typename TObject>
TObject buildObject() {
  TObject object;
  object["number"] = 42;
  object.createNestedArray("hello").add("world");
  return object;
}

template <typename TObject>
void validateObject(TObject& object) {
  CHECK(object.size() == 2);
  REQUIRE(object["number"] == 42);
  REQUIRE(object["hello"].template is<JsonArray>());
  REQUIRE(object["hello"][0] == std::string("world"));

  REQUIRE(SIZE == object.memoryUsage());
}

TEST_CASE("DynamicJsonObject::operator=()") {
  DynamicJsonObject object;
  object["erase"] = "me";

  SECTION("operator=(const DynamicJsonObject&)") {
    object = buildObject<DynamicJsonObject>();
    validateObject(object);
  }

  SECTION("operator=(const StaticJsonObject<N>&)") {
    object = buildObject<StaticJsonObject<SIZE> >();
    validateObject(object);
  }
}

TEST_CASE("StaticJsonObject::operator=()") {
  StaticJsonObject<SIZE> object;
  object["erase"] = "me";

  SECTION("operator=(const DynamicJsonObject&)") {
    object = buildObject<DynamicJsonObject>();
    validateObject(object);
  }

  SECTION("operator=(const StaticJsonObject<N>&)") {
    object = buildObject<StaticJsonObject<SIZE> >();
    validateObject(object);
  }

  SECTION("operator=(const StaticJsonObject<N+1>&)") {
    object = buildObject<StaticJsonObject<SIZE + 1> >();
    validateObject(object);
  }
}
