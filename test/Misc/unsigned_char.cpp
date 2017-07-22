// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <catch.hpp>

#if defined(__clang__)
#define CONFLICTS_WITH_BUILTIN_OPERATOR
#endif

using namespace ArduinoJson::Internals;

TEST_CASE("unsigned char string") {
  SECTION("JsonBuffer::strdup()") {
    unsigned char value[] = "world";

    DynamicJsonBuffer jsonBuffer;
    const char* dup = jsonBuffer.strdup(value);

    REQUIRE(static_cast<const void*>(value) != static_cast<const void*>(dup));
    REQUIRE(std::string("world") == dup);
  }

  SECTION("parseJson()") {
    SECTION("parseJson(JsonArray&, unsigned char[])") {
      StaticJsonArray<JSON_ARRAY_SIZE(1)> arr;
      unsigned char json[] = "[42]";

      REQUIRE(parseJson(arr, json));
    }

    SECTION("parseJson(JsonObject&, unsigned char[]") {
      StaticJsonObject<JSON_OBJECT_SIZE(1)> obj;
      unsigned char json[] = "{\"a\":42}";

      REQUIRE(parseJson(obj, json));
    }
  }

  SECTION("JsonVariant") {
    SECTION("JsonVariant(unsigned char[])") {
      unsigned char value[] = "42";

      DynamicJsonVariant variant(value);

      REQUIRE(42 == variant.as<int>());
    }

    SECTION("operator= (unsigned char[])") {
      unsigned char value[] = "42";

      DynamicJsonVariant variant(666);
      variant = value;

      REQUIRE(42 == variant.as<int>());
    }

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
    SECTION("operator[] (unsigned char[])") {
      DynamicJsonObject object;
      parseJson(object, "{\"hello\":\"world\"}");

      unsigned char key[] = "hello";

      JsonVariant variant = object;

      REQUIRE(std::string("world") == variant[key]);
    }
#endif

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
    SECTION("operator[](unsigned char[]) const") {
      DynamicJsonObject object;
      parseJson(object, "{\"hello\":\"world\"}");

      unsigned char key[] = "hello";

      const JsonVariant variant = object;

      REQUIRE(std::string("world") == variant[key]);
    }
#endif

    SECTION("operator==(unsigned char[]) const") {
      unsigned char comparand[] = "hello";

      const JsonVariant variant = "hello";

      REQUIRE(comparand == variant);
      REQUIRE(variant == comparand);
      REQUIRE_FALSE(comparand != variant);
      REQUIRE_FALSE(variant != comparand);
    }

    SECTION("operator!=(unsigned char[]) const") {
      unsigned char comparand[] = "hello";

      const JsonVariant variant = "world";

      REQUIRE(comparand != variant);
      REQUIRE(variant != comparand);
      REQUIRE_FALSE(comparand == variant);
      REQUIRE_FALSE(variant == comparand);
    }
  }

  SECTION("JsonObject") {
    DynamicJsonObject obj;

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
    SECTION("operator[](unsigned char[])") {
      unsigned char key[] = "hello";

      obj[key] = "world";

      REQUIRE(std::string("world") == obj["hello"]);
    }
#endif

    SECTION("operator=(unsigned char[])") {  // issue #416
      unsigned char value[] = "world";

      obj["hello"] = value;

      REQUIRE(std::string("world") == obj["hello"]);
    }

    SECTION("set(unsigned char[])") {
      unsigned char value[] = "world";

      obj["hello"].set(value);

      REQUIRE(std::string("world") == obj["hello"]);
    }

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
    SECTION("operator[](unsigned char[]) const") {
      obj["hello"] = "world";
      const JsonObject& cobj = obj;

      unsigned char key[] = "hello";

      REQUIRE(std::string("world") == cobj[key]);
    }
#endif

    SECTION("get(unsigned char[])") {
      obj["hello"] = "world";
      const JsonObject& cobj = obj;

      unsigned char key[] = "hello";

      REQUIRE(std::string("world") == cobj.get<char*>(key));
    }

    SECTION("set(unsigned char[], const char*)") {
      unsigned char key[] = "hello";
      obj.set(key, "world");

      REQUIRE(std::string("world") == obj["hello"]);
    }

    SECTION("set(const char*, unsigned char[])") {
      unsigned char value[] = "world";
      obj.set("hello", value);

      REQUIRE(std::string("world") == obj["hello"]);
    }

    SECTION("set(unsigned char[], unsigned char[])") {
      unsigned char key[] = "world";

      obj.set(key, key);

      REQUIRE(std::string("world") == obj["world"]);
    }

    SECTION("containsKey(unsigned char[]) const") {
      obj["hello"] = "world";
      unsigned char key[] = "hello";

      REQUIRE(true == obj.containsKey(key));
    }

    SECTION("remove(unsigned char[])") {
      obj["hello"] = "world";

      unsigned char key[] = "hello";
      obj.remove(key);

      REQUIRE(0 == obj.size());
    }

    SECTION("is(unsigned char[])") {
      obj["hello"] = 42;
      unsigned char key[] = "hello";

      REQUIRE(true == obj.is<int>(key));
    }

    SECTION("createNestedArray(unsigned char[])") {
      unsigned char key[] = "hello";

      obj.createNestedArray(key);
    }

    SECTION("createNestedObject(unsigned char[])") {
      unsigned char key[] = "hello";

      obj.createNestedObject(key);
    }
  }

  SECTION("JsonArray") {
    DynamicJsonArray arr;

    SECTION("add(unsigned char[])") {
      unsigned char value[] = "world";
      arr.add(value);

      REQUIRE(std::string("world") == arr[0]);
    }

    SECTION("set(size_t, unsigned char[])") {
      arr.add("hello");

      unsigned char value[] = "world";
      arr.set(0, value);

      REQUIRE(std::string("world") == arr[0]);
    }
  }

  SECTION("JsonArraySubscript") {
    DynamicJsonArray arr;
    arr.add("hello");

    SECTION("set(unsigned char[])") {
      unsigned char value[] = "world";
      arr[0].set(value);

      REQUIRE(std::string("world") == arr[0]);
    }

    SECTION("operator=(unsigned char[])") {
      unsigned char value[] = "world";
      arr[0] = value;

      REQUIRE(std::string("world") == arr[0]);
    }
  }
}
